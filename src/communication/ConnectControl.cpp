#include "ConnectControl.h"
#include "config/ParameterStore.h"
#include <QDebug>

#include "ITransport.h"
#include "PacketDecoders.h"
#include "Tcp.h"
#include "Udp.h"
#include "Serialport.h"

#include <QMetaObject>

namespace fosd {
namespace communication {
namespace detail {
ConnectWorker::ConnectWorker(DecoderFactory decoderFactory)
    : m_decoderFactory(decoderFactory ? std::move(decoderFactory)
                                      : DecoderFactory(createPacketDecoder)) {
}

void ConnectWorker::initialize() {
    m_udp = new Udp(this);
    m_tcp = new Tcp(this);
    m_serialport = new Serialport(this);

    wire(m_udp);
    wire(m_tcp);
    wire(m_serialport);

    m_serialportDecoder = initializeDecoder(Channel::Serial);
    m_udpDecoder = initializeDecoder(Channel::Udp);
    m_tcpDecoder = initializeDecoder(Channel::Tcp);
}

std::unique_ptr<IPacketDecoder> ConnectWorker::initializeDecoder(Channel channel) {
    std::unique_ptr<IPacketDecoder> parser = m_decoderFactory(channel);
    if (parser) {
        connect(parser.get(), &IPacketDecoder::errorOccurred, this,
                &ConnectWorker::decoderErrorOccurred);
        parser->initialize();
    }
    return parser;
}

void ConnectWorker::wire(ITransport *transport) {
    connect(transport, &ITransport::bytesReceived, this, &ConnectWorker::handleBytesReceived);
    connect(transport, &ITransport::stateChanged, this,
            &ConnectWorker::handleTransportStateChanged);
    connect(transport, &ITransport::errorOccurred, this, &ConnectWorker::errorOccurred);
}

void ConnectWorker::handleBytesReceived(const QByteArray &bytes, const Endpoint &peer) {
    ITransport *transport = qobject_cast<ITransport *>(sender());
    if (!transport)
        return;
    forward(transport->channel(), bytes, peer);
}

void ConnectWorker::handleTransportStateChanged(ConnectionState state) {
    ITransport *transport = qobject_cast<ITransport *>(sender());
    if (!transport)
        return;
    // A server returns to Listening after a peer disconnects. Discard partial
    // frames before accepting a new peer, including one using the same endpoint.
    if (state == ConnectionState::Closed || state == ConnectionState::Listening) {
        IPacketDecoder *parser = decoder(transport->channel());
        if (parser)
            parser->reset();
    }
    emit stateChanged(transport->channel(), state);
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
        case Channel::Serial:
            return m_serialportDecoder.get();
        case Channel::Udp:
            return m_udpDecoder.get();
        case Channel::Tcp:
            return m_tcpDecoder.get();
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
        case Channel::Serial:
            m_serialport->close();
            break;
        case Channel::Udp:
            m_udp->close();
            break;
        case Channel::Tcp:
            m_tcp->close();
            break;
    }
}

void ConnectWorker::send(Channel channel, const QByteArray &bytes, const Endpoint &peer) {
    switch (channel) {
        case Channel::Serial:
            m_serialport->send(bytes, peer);
            break;
        case Channel::Udp:
            m_udp->send(bytes, peer);
            break;
        case Channel::Tcp:
            m_tcp->send(bytes, peer);
            break;
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
    : ConnectControl(parent, std::move(decoderFactory), nullptr) {
}

// 委托构造函数，独立线程模式

ConnectControl::ConnectControl(QThread &hostThread, QObject *parent, DecoderFactory decoderFactory)
    : ConnectControl(parent, std::move(decoderFactory), &hostThread) {
}

// 委托构造函数，托管线程模式

// Private constructor for both modes. If hostThread is null, creates and owns a thread.
ConnectControl::ConnectControl(QObject *parent, DecoderFactory decoderFactory, QThread *hostThread)
    : QObject(parent), m_ownedThread(hostThread ? nullptr : new QThread),
      m_thread(hostThread ? hostThread : m_ownedThread.get()),
      m_worker(new detail::ConnectWorker(std::move(decoderFactory))) {
    Q_ASSERT(!m_thread->isRunning());

    qRegisterMetaType<Channel>("fosd::communication::Channel");
    qRegisterMetaType<ConnectionState>("fosd::communication::ConnectionState");
    qRegisterMetaType<Endpoint>("fosd::communication::Endpoint");
    qRegisterMetaType<SerialConfig>("fosd::communication::SerialConfig");
    qRegisterMetaType<UdpConfig>("fosd::communication::UdpConfig");
    qRegisterMetaType<TcpConfig>("fosd::communication::TcpConfig");
    qRegisterMetaType<CommunicationError>("fosd::communication::CommunicationError");
    qRegisterMetaType<DecoderError>("fosd::communication::DecoderError");

    config::ParameterStore parameters;
    QString parameterError;
    SerialConfig serial; UdpConfig udp; TcpConfig tcp;
    if (!parameters.initialize(&serial, &parameterError) || !parameters.initialize(&udp, &parameterError) || !parameters.initialize(&tcp, &parameterError)) {
        CommunicationError error; error.operation = "configuration"; error.message = parameterError;
        QMetaObject::invokeMethod(this, "errorOccurred", Qt::QueuedConnection, Q_ARG(fosd::communication::CommunicationError, error));
    }

    m_worker->moveToThread(m_thread);
    connect(m_thread, &QThread::started, m_worker.data(), &detail::ConnectWorker::initialize);
    connect(m_thread, &QThread::finished, m_worker.data(), &QObject::deleteLater);

    connect(this, &ConnectControl::requestOpenSerial, m_worker, &detail::ConnectWorker::openSerial,
            Qt::QueuedConnection);
    connect(this, &ConnectControl::requestOpenUdp, m_worker, &detail::ConnectWorker::openUdp,
            Qt::QueuedConnection);
    connect(this, &ConnectControl::requestOpenTcp, m_worker, &detail::ConnectWorker::openTcp,
            Qt::QueuedConnection);
    connect(this, &ConnectControl::requestClose, m_worker, &detail::ConnectWorker::close,
            Qt::QueuedConnection);
    connect(this, &ConnectControl::requestSend, m_worker, &detail::ConnectWorker::send,
            Qt::QueuedConnection);

    connect(m_worker, &detail::ConnectWorker::rawDataReceived, this,
            &ConnectControl::rawDataReceived);
    connect(m_worker, &detail::ConnectWorker::packetReceived, this,
            &ConnectControl::packetReceived);
    connect(m_worker, &detail::ConnectWorker::stateChanged, this, &ConnectControl::stateChanged);
    connect(m_worker, &detail::ConnectWorker::errorOccurred, this, &ConnectControl::errorOccurred);
    connect(m_worker, &detail::ConnectWorker::decoderErrorOccurred, this,
            &ConnectControl::decoderErrorOccurred);

    if (m_ownedThread)
        m_thread->start();
}

ConnectControl::~ConnectControl() {
    shutdown();
    if (m_ownedThread) {
        m_thread->quit();
        m_thread->wait();
    }
    // An unstarted hosted thread has no finished signal to delete its worker.
    if (m_worker && !m_thread->isRunning())
        delete m_worker.data();
}

void ConnectControl::shutdown() {
    if (m_worker && m_thread->isRunning()) {
        const Qt::ConnectionType type = QThread::currentThread() == m_thread
                                            ? Qt::DirectConnection
                                            : Qt::BlockingQueuedConnection;
        QMetaObject::invokeMethod(m_worker.data(), "shutdown", type);
    }
}

void ConnectControl::openSerial(const SerialConfig &config) {
    QString message;
    if (!fosd::config::validateParameter(config, true, &message)) {
        CommunicationError error; error.channel = Channel::Serial; error.operation = "open"; error.message = message;
        emit errorOccurred(error); return;
    }
    if (!fosd::config::ParameterStore().write(config, true, &message)) {
        CommunicationError error; error.channel = Channel::Serial; error.operation = "configuration"; error.message = message;
        emit errorOccurred(error); return;
    }
    emit requestOpenSerial(config);
}

void ConnectControl::openUdp(const UdpConfig &config) {
    QString message;
    if (!fosd::config::validateParameter(config, true, &message)) {
        CommunicationError error; error.channel = Channel::Udp; error.operation = "open"; error.message = message;
        emit errorOccurred(error); return;
    }
    if (!fosd::config::ParameterStore().write(config, true, &message)) {
        CommunicationError error; error.channel = Channel::Udp; error.operation = "configuration"; error.message = message;
        emit errorOccurred(error); return;
    }
    emit requestOpenUdp(config);
}

void ConnectControl::openTcp(const TcpConfig &config) {
    QString message;
    if (!fosd::config::validateParameter(config, true, &message)) {
        CommunicationError error; error.channel = Channel::Tcp; error.operation = "open"; error.message = message;
        emit errorOccurred(error); return;
    }
    if (!fosd::config::ParameterStore().write(config, true, &message)) {
        CommunicationError error; error.channel = Channel::Tcp; error.operation = "configuration"; error.message = message;
        emit errorOccurred(error); return;
    }
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
