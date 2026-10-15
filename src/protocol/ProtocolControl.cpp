#include "ProtocolControl.h"
#include "communication/ConnectControl.h"
#include <QDateTime>
#include <utility>

namespace fosd { namespace protocol {
namespace {
core::TransportKind transportKind(communication::Channel channel) {
    switch (channel) {
    case communication::Channel::Udp: return core::TransportKind::Udp;
    case communication::Channel::Tcp: return core::TransportKind::Tcp;
    case communication::Channel::Serial: return core::TransportKind::Serial;
    }
    return core::TransportKind::Serial;
}
}
namespace detail {
ProtocolWorker::ProtocolWorker(ParserFactory factory, 
                               std::shared_ptr<core::FrameRepository> repository)
    : m_factory(std::move(factory)), 
      m_repository(std::move(repository)) {}

void ProtocolWorker::initialize() {
    m_parsers = m_factory();
    m_factory = ParserFactory();
}

void ProtocolWorker::parseFrame(communication::Channel channel, const QByteArray &packet,
                                const communication::Endpoint &peer) {
    ProtocolError error;
    error.channel = channel;
    error.peer = peer;
    for (const std::unique_ptr<IFrameParser> &parser : m_parsers) {
        if (!parser || !parser->matches(packet))
            continue;
        error.protocolId = parser->protocolId();
        core::FrameDataPtr data = parser->parse(packet, &error.message);
        if (!data) {
            if (error.message.isEmpty())
                error.message = QStringLiteral("Parser returned no data");
            emit errorOccurred(error);
            return;
        }
        core::FrameSnapshot snapshot;
        snapshot.protocolId = error.protocolId;
        snapshot.source.transport = transportKind(channel);
        snapshot.source.address = peer.address.toString();
        snapshot.source.port = peer.port;
        snapshot.receivedAtUtc = QDateTime::currentDateTimeUtc();
        snapshot.data = std::move(data);
        // publish releases its lock before signals can call consumer code.
        const core::FrameSnapshotPtr published = m_repository->publish(std::move(snapshot));
        emit frameParsed(published);
        return;
    }
    error.message = QStringLiteral("No parser recognizes this frame");
    emit errorOccurred(error);
}

}

ProtocolControl::ProtocolControl(QObject *parent, 
                                 ParserFactory factory,
                                 communication::DecoderFactory decoderFactory)
    : QObject(parent), 
      m_repository(std::make_shared<core::FrameRepository>()),
      m_worker(new detail::ProtocolWorker(factory ? std::move(factory) : ParserFactory(createFrameParsers), m_repository)) {

    qRegisterMetaType<communication::Channel>("fosd::communication::Channel");
    qRegisterMetaType<communication::Endpoint>("fosd::communication::Endpoint");
    qRegisterMetaType<core::FrameSnapshotPtr>("fosd::core::FrameSnapshotPtr");
    qRegisterMetaType<ProtocolError>("fosd::protocol::ProtocolError");

    m_worker->moveToThread(&m_thread);

    connect(&m_thread, &QThread::started, 
            m_worker, &detail::ProtocolWorker::initialize);
    connect(&m_thread, &QThread::finished, 
            m_worker, &QObject::deleteLater);

    connect(this, &ProtocolControl::requestParse,
            m_worker, &detail::ProtocolWorker::parseFrame, Qt::QueuedConnection);
            
    connect(m_worker, &detail::ProtocolWorker::frameParsed, 
            this, &ProtocolControl::frameParsed);
    connect(m_worker, &detail::ProtocolWorker::errorOccurred, 
            this, &ProtocolControl::errorOccurred);
    // Register parser initialization first, then communication initialization.
    // All started slots complete before the thread processes I/O events.
    m_communication.reset(new communication::ConnectControl(m_thread, nullptr, std::move(decoderFactory)));

    connect(m_communication->m_worker.data(), &communication::detail::ConnectWorker::packetReceived,
            m_worker, &detail::ProtocolWorker::parseFrame, Qt::DirectConnection);

    m_thread.start();
}

ProtocolControl::~ProtocolControl() {
    // Stop transports on their thread before stopping parsing and deleting workers.
    m_communication->shutdown();
    m_thread.quit();
    m_thread.wait();
    m_communication.reset();
}

void ProtocolControl::parseFrame(communication::Channel channel, const QByteArray &packet,
                                 const communication::Endpoint &peer) {
    emit requestParse(channel, packet, peer);
}
} } // namespace fosd::protocol
