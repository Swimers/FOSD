#pragma once

#include <QHostAddress>
#include <QMetaType>
#include <QSerialPort>
#include <QString>

namespace fosd {
namespace communication {

enum class Channel { Serial, Udp, Tcp };
enum class ConnectionState { Closed, Opening, Ready, Listening, Connected };
enum class TcpRole { Client, Server };

struct Endpoint {
    QHostAddress address;
    quint16 port = 0;

    bool isValid() const { return !address.isNull() && port != 0; }
};

struct SerialConfig {
    QString portName;
    qint32 baudRate = 115200;
    QSerialPort::DataBits dataBits = QSerialPort::Data8;
    QSerialPort::Parity parity = QSerialPort::NoParity;
    QSerialPort::StopBits stopBits = QSerialPort::OneStop;
    QSerialPort::FlowControl flowControl = QSerialPort::NoFlowControl;
};

struct UdpConfig {
    QHostAddress bindAddress = QHostAddress::AnyIPv4;
    quint16 localPort = 0; // 0 asks the OS for an available port.
    Endpoint defaultPeer;
};

struct TcpConfig {
    TcpRole role = TcpRole::Client;
    QString host = QStringLiteral("127.0.0.1"); // Client address or host name.
    QHostAddress listenAddress = QHostAddress::AnyIPv4;
    quint16 port = 0;
};

struct CommunicationError {
    Channel channel = Channel::Serial;
    QString operation;
    QString message;
    int nativeCode = 0;
};

struct DecoderError {
    Channel channel = Channel::Serial;
    QString operation;
    QString message;
    int code = 0;
};

} // namespace communication
} // namespace fosd

Q_DECLARE_METATYPE(fosd::communication::Channel)
Q_DECLARE_METATYPE(fosd::communication::ConnectionState)
Q_DECLARE_METATYPE(fosd::communication::Endpoint)
Q_DECLARE_METATYPE(fosd::communication::SerialConfig)
Q_DECLARE_METATYPE(fosd::communication::UdpConfig)
Q_DECLARE_METATYPE(fosd::communication::TcpConfig)
Q_DECLARE_METATYPE(fosd::communication::CommunicationError)
Q_DECLARE_METATYPE(fosd::communication::DecoderError)

