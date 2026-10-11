#pragma once

#include "ITransport.h"

#include <QTcpServer>
#include <QTcpSocket>

namespace fosd {
namespace communication {

class Tcp final : public ITransport {
    Q_OBJECT
public:
    explicit Tcp(QObject *parent = nullptr);

    void open(const TcpConfig &config);
    void close() override;
    void send(const QByteArray &bytes, const Endpoint &peer = Endpoint()) override;

private:
    void attachSocket(QTcpSocket *socket);
    void discardSocket();
    void setState(ConnectionState state);

    QTcpServer m_server;
    QTcpSocket *m_socket = nullptr;
    TcpRole m_role = TcpRole::Client;
    ConnectionState m_state = ConnectionState::Closed;
};

} // namespace communication
} // namespace fosd

