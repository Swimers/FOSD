#include "Usart.h"

namespace fosd {
namespace communication {

Usart::Usart(QObject *parent)
    : ITransport(Channel::Serial, parent) {
    connect(&m_port, &QSerialPort::readyRead, this, [this]() {
        const QByteArray bytes = m_port.readAll();
        if (!bytes.isEmpty())
            emit bytesReceived(bytes, Endpoint());
    });
    connect(&m_port, QOverload<QSerialPort::SerialPortError>::of(&QSerialPort::error),
            this, [this](QSerialPort::SerialPortError error) {
        if (error == QSerialPort::NoError)
            return;
        reportError(QStringLiteral("serial"), m_port.errorString(), static_cast<int>(error));
        if (error == QSerialPort::ResourceError) {
            m_port.close();
            emit stateChanged(ConnectionState::Closed);
        }
    });
}

void Usart::open(const SerialConfig &config) {
    close();
    if (config.portName.trimmed().isEmpty() || config.baudRate <= 0) {
        reportError(QStringLiteral("open"), QStringLiteral("Invalid serial port name or baud rate"));
        return;
    }

    m_port.setPortName(config.portName);
    m_port.setBaudRate(config.baudRate);
    m_port.setDataBits(config.dataBits);
    m_port.setParity(config.parity);
    m_port.setStopBits(config.stopBits);
    m_port.setFlowControl(config.flowControl);

    if (!m_port.open(QIODevice::ReadWrite)) {
        reportError(QStringLiteral("open"), m_port.errorString(), static_cast<int>(m_port.error()));
        return;
    }
    emit stateChanged(ConnectionState::Connected);
}

void Usart::close() {
    if (!m_port.isOpen())
        return;
    m_port.close();
    emit stateChanged(ConnectionState::Closed);
}

void Usart::send(const QByteArray &bytes, const Endpoint &peer) {
    Q_UNUSED(peer)
    if (!m_port.isOpen()) {
        reportError(QStringLiteral("send"), QStringLiteral("Serial port is closed"));
        return;
    }
    if (bytes.isEmpty())
        return;
    const qint64 accepted = m_port.write(bytes);
    if (accepted != bytes.size())
        reportError(QStringLiteral("send"), m_port.errorString(), static_cast<int>(m_port.error()));
}

} // namespace communication
} // namespace fosd

