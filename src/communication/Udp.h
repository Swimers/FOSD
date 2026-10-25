#pragma once

#include "ITransport.h"

#include <QUdpSocket>

namespace fosd {
namespace communication {
class Udp final : public ITransport {
    Q_OBJECT
public:
    explicit Udp(QObject *parent = nullptr);

    void open(const UdpConfig &config);
    void close() override;
    void send(const QByteArray &bytes, const Endpoint &peer = Endpoint()) override;

    quint16 localPort() const;

private slots:
    void handleReadyRead();
    void handleSocketError(QAbstractSocket::SocketError error);

private:
    QUdpSocket m_socket;
    Endpoint m_defaultPeer;
};

} // namespace communication
} // namespace fosd
