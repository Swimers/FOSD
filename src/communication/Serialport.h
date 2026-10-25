#ifndef SERIALPORT_H
#define SERIALPORT_H

#pragma once

#include <QSerialPort>

#include "ITransport.h"

namespace fosd {
namespace communication {
class Serialport final : public ITransport {
    Q_OBJECT
public:
    explicit Serialport(QObject *parent = nullptr);

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

#endif