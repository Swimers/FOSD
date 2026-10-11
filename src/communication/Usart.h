#pragma once

#include "ITransport.h"

#include <QSerialPort>

namespace fosd {
namespace communication {

class Usart final : public ITransport {
    Q_OBJECT
public:
    explicit Usart(QObject *parent = nullptr);

    void open(const SerialConfig &config);
    void close() override;
    void send(const QByteArray &bytes, const Endpoint &peer = Endpoint()) override;

private slots:
    void handleReadyRead();
    void handlePortError(QSerialPort::SerialPortError error);

private:
    QSerialPort m_port;
};

} // namespace communication
} // namespace fosd

