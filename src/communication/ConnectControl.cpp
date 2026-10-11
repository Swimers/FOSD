#include "ConnectControl.h"

#include "ITransport.h"
#include "Tcp.h"
#include "Udp.h"
#include "Serialport.h"

#include <QMetaObject>

namespace fosd {
namespace communication {
namespace detail {

ConnectWorker::ConnectWorker(DecoderFactory decoderFactory)
    : m_decoderFactory(std::move(decoderFactory)) {}

void ConnectWorker::initialize() {
    m_udp = new Udp(this);
    m_tcp = new Tcp(this);
    m_serialport = new Serialport(this);

    wire(Channel::Udp, m_udp);
    wire(Channel::Tcp, m_tcp);
    wire(Channel::Serial, m_serialport);

    if (m_decoderFactory) {
        m_serialportDecoder = m_decoderFactory(Channel::Serial);
        m_udpDecoder = m_decoderFactory(Channel::Udp);
        m_tcpDecoder = m_decoderFactory(Channel::Tcp);
    }
}

void ConnectWorker::wire(Channel channel, ITransport *transport) {
    connect(transport, &ITransport::bytesReceived, 
            this,
            [this, channel](const QByteArray &bytes, const Endpoint &peer) {
        forward(channel, bytes, peer);
    });
    connect(transport, 
            &ITransport::stateChanged, 
            this,
            [this, channel](ConnectionState state) { emit stateChanged(channel, state); });
    connect(transport, 
            &ITransport::errorOccurred, 
            this,
            [this](const CommunicationError &error) { emit errorOccurred(error); });
}

void ConnectWorker::forward(Channel channel, const QByteArray &bytes, const Endpoint &peer) {
    emit rawDataReceived(channel, bytes, peer);
    IPacketDecoder *parser = decoder(channel);
    if (!parser)
        return;
    const QVector<QByteArray> packets = parser->feed(bytes, peer);
    for (const QByteArray &packet : packets)
        emit packetReceived(channel, packet, peer);
}

IPacketDecoder *ConnectWorker::decoder(Channel channel) {
    switch (channel) {
    case Channel::Serial: return m_serialportDecoder.get();
    case Channel::Udp: return m_udpDecoder.get();
    case Channel::Tcp: return m_tcpDecoder.get();
    }
    return nullptr;
}

void ConnectWorker::openSerial(const SerialConfig &config) {
    if (m_serialportDecoder)
        m_serialportDecoder->reset();
    m_serialport->open(config);
}

void ConnectWorker::openUdp(const UdpConfig &config) {
    if (m_udpDecoder)
        m_udpDecoder->reset();
    m_udp->open(config);
}

void ConnectWorker::openTcp(const TcpConfig &config) {
    if (m_tcpDecoder)
        m_tcpDecoder->reset();
    m_tcp->open(config);
}

void ConnectWorker::close(Channel channel) {
    IPacketDecoder *parser = decoder(channel);
    if (parser)
        parser->reset();
    switch (channel) {
    case Channel::Serial: m_serialport->close(); break;
    case Channel::Udp: m_udp->close(); break;
    case Channel::Tcp: m_tcp->close(); break;
    }
}

void ConnectWorker::send(Channel channel, const QByteArray &bytes, const Endpoint &peer) {
    switch (channel) {
    case Channel::Serial: m_serialport->send(bytes, peer); break;
    case Channel::Udp: m_udp->send(bytes, peer); break;
    case Channel::Tcp: m_tcp->send(bytes, peer); break;
    }
}

void ConnectWorker::shutdown() {
    close(Channel::Serial);
    close(Channel::Udp);
    close(Channel::Tcp);
    m_serialportDecoder.reset();
    m_udpDecoder.reset();
    m_tcpDecoder.reset();
}

} // namespace detail

ConnectControl::ConnectControl(QObject *parent, DecoderFactory decoderFactory)
    : QObject(parent), m_worker(new detail::ConnectWorker(std::move(decoderFactory))) {
    qRegisterMetaType<Channel>("fosd::communication::Channel");
    qRegisterMetaType<ConnectionState>("fosd::communication::ConnectionState");
    qRegisterMetaType<Endpoint>("fosd::communication::Endpoint");
    qRegisterMetaType<SerialConfig>("fosd::communication::SerialConfig");
    qRegisterMetaType<UdpConfig>("fosd::communication::UdpConfig");
    qRegisterMetaType<TcpConfig>("fosd::communication::TcpConfig");
    qRegisterMetaType<CommunicationError>("fosd::communication::CommunicationError");

    m_worker->moveToThread(&m_thread);
    connect(&m_thread, &QThread::started, m_worker, &detail::ConnectWorker::initialize);
    connect(&m_thread, &QThread::finished, m_worker, &QObject::deleteLater);

    connect(this, &ConnectControl::requestOpenSerial,
            m_worker, &detail::ConnectWorker::openSerial, Qt::QueuedConnection);
    connect(this, &ConnectControl::requestOpenUdp,
            m_worker, &detail::ConnectWorker::openUdp, Qt::QueuedConnection);
    connect(this, &ConnectControl::requestOpenTcp,
            m_worker, &detail::ConnectWorker::openTcp, Qt::QueuedConnection);
    connect(this, &ConnectControl::requestClose,
            m_worker, &detail::ConnectWorker::close, Qt::QueuedConnection);
    connect(this, &ConnectControl::requestSend,
            m_worker, &detail::ConnectWorker::send, Qt::QueuedConnection);

    connect(m_worker, &detail::ConnectWorker::rawDataReceived,
            this, &ConnectControl::rawDataReceived);
    connect(m_worker, &detail::ConnectWorker::packetReceived,
            this, &ConnectControl::packetReceived);
    connect(m_worker, &detail::ConnectWorker::stateChanged,
            this, &ConnectControl::stateChanged);
    connect(m_worker, &detail::ConnectWorker::errorOccurred,
            this, &ConnectControl::errorOccurred);

    m_thread.start();
}

ConnectControl::~ConnectControl() {
    if (m_thread.isRunning()) {
        QMetaObject::invokeMethod(m_worker, "shutdown", Qt::BlockingQueuedConnection);
        m_thread.quit();
        m_thread.wait();
    }
}

void ConnectControl::openSerial(const SerialConfig &config) {
    emit requestOpenSerial(config);
}

void ConnectControl::openUdp(const UdpConfig &config) {
    emit requestOpenUdp(config);
}

void ConnectControl::openTcp(const TcpConfig &config) {
    emit requestOpenTcp(config);
}

void ConnectControl::close(Channel channel) {
    emit requestClose(channel);
}

void ConnectControl::send(Channel channel, const QByteArray &bytes, const Endpoint &peer) {
    emit requestSend(channel, bytes, peer);
}

} // namespace communication
} // namespace fosd

