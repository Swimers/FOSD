#pragma once
#include "IFrameParser.h"
#include "communication/ConnectControl.h"
#include "core/Datamodel/FrameRepository.h"
#include <QObject>
#include <QThread>
#include <functional>

namespace fosd {
namespace communication { class ConnectControl; }
namespace protocol {
struct ProtocolError {
    communication::Channel channel = communication::Channel::Serial;
    communication::Endpoint peer;
    QString protocolId;
    QString message;
};

using ParserFactory = std::function<FrameParsers()>;

namespace detail {
class ProtocolWorker final : public QObject {
    Q_OBJECT
public:
    ProtocolWorker(ParserFactory factory, std::shared_ptr<core::FrameRepository> repository);
public slots:
    void initialize();
    void parseFrame(fosd::communication::Channel channel, const QByteArray &packet,
                    const fosd::communication::Endpoint &peer);
signals:
    void frameParsed(const fosd::core::FrameSnapshotPtr &snapshot);
    void errorOccurred(const fosd::protocol::ProtocolError &error);
private:
    ParserFactory m_factory;
    FrameParsers m_parsers;
    std::shared_ptr<core::FrameRepository> m_repository;
};
}

// Create/destroy on the caller thread. Owns communication and the shared worker
// thread. Both factories, I/O, decoding, parsing and publishing run on m_thread.
// storage consumes frameParsed for every frame; visualization reads latest().
class ProtocolControl final : public QObject {
    Q_OBJECT
public:
    explicit ProtocolControl(QObject *parent = nullptr, 
                             ParserFactory factory = ParserFactory(),
                             communication::DecoderFactory decoderFactory = communication::DecoderFactory());
    ~ProtocolControl() override;

    core::FrameRepositoryPtr dataModel() const { return m_repository; }
    communication::ConnectControl &communication() const { return *m_communication; }

    void parseFrame(communication::Channel channel, const QByteArray &packet,
                    const communication::Endpoint &peer = communication::Endpoint());
signals:
    void frameParsed(const fosd::core::FrameSnapshotPtr &snapshot);
    void errorOccurred(const fosd::protocol::ProtocolError &error);
    void requestParse(fosd::communication::Channel channel, const QByteArray &packet,
                      const fosd::communication::Endpoint &peer);
private:
    std::shared_ptr<core::FrameRepository> m_repository;
    QThread m_thread;
    detail::ProtocolWorker *m_worker = nullptr;
    
    std::unique_ptr<communication::ConnectControl> m_communication;
};
} } // namespace fosd::protocol
Q_DECLARE_METATYPE(fosd::protocol::ProtocolError)
