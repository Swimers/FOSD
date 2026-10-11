#include "Tcp.h"

namespace fosd {
namespace communication {

Tcp::Tcp(QObject *parent)
    : ITransport(Channel::Tcp, parent) {
    connect(&m_server, &QTcpServer::newConnection, this, &Tcp::handleNewConnection);
    connect(&m_server, &QTcpServer::acceptError, this, &Tcp::handleAcceptError);
}

void Tcp::handleNewConnection() {
    while (m_server.hasPendingConnections()) {
        QTcpSocket *socket = m_server.nextPendingConnection();
        if (m_socket) {
            socket->disconnectFromHost();
            socket->deleteLater();
            reportError(QStringLiteral("accept"),
                        QStringLiteral("Only one TCP client is supported"));
            continue;
        }
        socket->setParent(this);
        attachSocket(socket);
        setState(ConnectionState::Connected);
    }
}

void Tcp::handleAcceptError(QAbstractSocket::SocketError error) {
    reportError(QStringLiteral("accept"), m_server.errorString(), static_cast<int>(error));
}

void Tcp::open(const TcpConfig &config) {
    close();
    if (config.port == 0) {
        reportError(QStringLiteral("open"), QStringLiteral("TCP port must be nonzero"));
        return;
    }
    m_role = config.role;
    if (m_role == TcpRole::Server) {
        if (config.listenAddress.isNull()) {
            reportError(QStringLiteral("listen"), QStringLiteral("Invalid listen address"));
            return;
        }
        if (!m_server.listen(config.listenAddress, config.port)) {
            reportError(QStringLiteral("listen"), m_server.errorString(),
                        static_cast<int>(m_server.serverError()));
            return;
        }
        setState(ConnectionState::Listening);
        return;
    }

    if (config.host.trimmed().isEmpty()) {
        reportError(QStringLiteral("connect"), QStringLiteral("TCP host is empty"));
        return;
    }
    QTcpSocket *socket = new QTcpSocket(this);
    attachSocket(socket);
    setState(ConnectionState::Opening);
    socket->connectToHost(config.host, config.port);
}

void Tcp::close() {
    const bool wasActive = m_state != ConnectionState::Closed;
    m_server.close();
    discardSocket();
    if (wasActive)
        setState(ConnectionState::Closed);
}

void Tcp::send(const QByteArray &bytes, const Endpoint &peer) {
    Q_UNUSED(peer)
    if (!m_socket || m_socket->state() != QAbstractSocket::ConnectedState) {
        reportError(QStringLiteral("send"), QStringLiteral("TCP peer is not connected"));
        return;
    }
    if (bytes.isEmpty())
        return;
    const qint64 accepted = m_socket->write(bytes);
    if (accepted != bytes.size())
        reportError(QStringLiteral("send"), m_socket->errorString(),
                    static_cast<int>(m_socket->error()));
}

void Tcp::attachSocket(QTcpSocket *socket) {
    m_socket = socket;
    connect(socket, &QTcpSocket::connected, this, &Tcp::handleSocketConnected);
    connect(socket, &QTcpSocket::readyRead, this, &Tcp::handleSocketReadyRead);
    connect(socket, &QTcpSocket::disconnected, this, &Tcp::handleSocketDisconnected);
    connect(socket, QOverload<QAbstractSocket::SocketError>::of(&QTcpSocket::error),
            this, &Tcp::handleSocketError);
}

void Tcp::handleSocketConnected() {
    if (!m_socket)
        return;
    setState(ConnectionState::Connected);
}

void Tcp::handleSocketReadyRead() {
    if (!m_socket)
        return;
    const QByteArray bytes = m_socket->readAll();
    if (bytes.isEmpty())
        return;
    Endpoint peer;
    peer.address = m_socket->peerAddress();
    peer.port = m_socket->peerPort();
    emit bytesReceived(bytes, peer);
}

void Tcp::handleSocketDisconnected() {
    QTcpSocket *socket = m_socket;
    if (!socket)
        return;
    m_socket = nullptr;
    socket->deleteLater();
    setState(m_server.isListening() ? ConnectionState::Listening : ConnectionState::Closed);
}

void Tcp::handleSocketError(QAbstractSocket::SocketError error) {
    if (!m_socket)
        return;
    reportError(QStringLiteral("tcp"), m_socket->errorString(), static_cast<int>(error));
}

void Tcp::discardSocket() {
    if (!m_socket)
        return;
    QTcpSocket *socket = m_socket;
    m_socket = nullptr;
    socket->disconnect(this);
    socket->abort();
    socket->deleteLater();
}

void Tcp::setState(ConnectionState state) {
    if (m_state == state)
        return;
    m_state = state;
    emit stateChanged(state);
}

} // namespace communication
} // namespace fosd

