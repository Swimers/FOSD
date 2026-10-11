#include "Tcp.h"

namespace fosd {
namespace communication {

Tcp::Tcp(QObject *parent)
    : ITransport(Channel::Tcp, parent) {
    connect(&m_server, &QTcpServer::newConnection, this, [this]() {
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
    });
    connect(&m_server, &QTcpServer::acceptError, this,
            [this](QAbstractSocket::SocketError error) {
        reportError(QStringLiteral("accept"), m_server.errorString(), static_cast<int>(error));
    });
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
    connect(socket, &QTcpSocket::connected, this, [this, socket]() {
        if (socket == m_socket)
            setState(ConnectionState::Connected);
    });
    connect(socket, &QTcpSocket::readyRead, this, [this, socket]() {
        if (socket != m_socket)
            return;
        const QByteArray bytes = socket->readAll();
        if (!bytes.isEmpty()) {
            Endpoint peer;
            peer.address = socket->peerAddress();
            peer.port = socket->peerPort();
            emit bytesReceived(bytes, peer);
        }
    });
    connect(socket, &QTcpSocket::disconnected, this, [this, socket]() {
        if (socket != m_socket)
            return;
        m_socket = nullptr;
        socket->deleteLater();
        setState(m_server.isListening() ? ConnectionState::Listening : ConnectionState::Closed);
    });
    connect(socket, QOverload<QAbstractSocket::SocketError>::of(&QTcpSocket::error),
            this, [this, socket](QAbstractSocket::SocketError error) {
        if (socket == m_socket)
            reportError(QStringLiteral("tcp"), socket->errorString(), static_cast<int>(error));
    });
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

