#include "Udp.h"

namespace fosd {
namespace communication {

Udp::Udp(QObject *parent)
    : ITransport(Channel::Udp, parent) {
    connect(&m_socket, &QUdpSocket::readyRead, this, &Udp::handleReadyRead);
    connect(&m_socket, QOverload<QAbstractSocket::SocketError>::of(&QUdpSocket::error),
            this, &Udp::handleSocketError);
}

void Udp::handleReadyRead() {
    while (m_socket.hasPendingDatagrams()) {
        const qint64 pending = m_socket.pendingDatagramSize();
        if (pending < 0) {
            reportError(QStringLiteral("receive"), m_socket.errorString(),
                        static_cast<int>(m_socket.error()));
            break;
        }
        
        QByteArray bytes(static_cast<int>(pending), Qt::Uninitialized);
        Endpoint peer;
        const qint64 received = m_socket.readDatagram(bytes.data(), 
                                                      bytes.size(),
                                                      &peer.address,
                                                      &peer.port);

        if (received < 0) {
            reportError(QStringLiteral("receive"), m_socket.errorString(),
                        static_cast<int>(m_socket.error()));
            break;
        }

        bytes.resize(static_cast<int>(received));
        emit bytesReceived(bytes, peer);
    }
}

void Udp::handleSocketError(QAbstractSocket::SocketError error) {
    reportError(QStringLiteral("udp"), m_socket.errorString(), static_cast<int>(error));
}

void Udp::open(const UdpConfig &config) {
    close();
    if (config.bindAddress.isNull()) {
        reportError(QStringLiteral("open"), QStringLiteral("Invalid UDP bind address"));
        return;
    }
    if (config.defaultPeer.port != 0 && !config.defaultPeer.isValid()) {
        reportError(QStringLiteral("open"), QStringLiteral("Invalid default UDP peer"));
        return;
    }
    m_defaultPeer = config.defaultPeer;
    if (!m_socket.bind(config.bindAddress, config.localPort)) {
        reportError(QStringLiteral("open"), m_socket.errorString(),
                    static_cast<int>(m_socket.error()));
        return;
    }
    emit stateChanged(ConnectionState::Ready);
}

void Udp::close() {
    if (m_socket.state() == QAbstractSocket::UnconnectedState)
        return;
    m_socket.close();
    emit stateChanged(ConnectionState::Closed);
}

void Udp::send(const QByteArray &bytes, const Endpoint &peer) {
    if (m_socket.state() != QAbstractSocket::BoundState) {
        reportError(QStringLiteral("send"), QStringLiteral("UDP socket is not bound"));
        return;
    }
    const Endpoint target = peer.isValid() ? peer : m_defaultPeer;
    if (!target.isValid()) {
        reportError(QStringLiteral("send"), QStringLiteral("No valid UDP destination"));
        return;
    }
    const qint64 sent = m_socket.writeDatagram(bytes, target.address, target.port);
    if (sent != bytes.size())
        reportError(QStringLiteral("send"), m_socket.errorString(),
                    static_cast<int>(m_socket.error()));
}

quint16 Udp::localPort() const {
    return m_socket.localPort();
}

} // namespace communication
} // namespace fosd

