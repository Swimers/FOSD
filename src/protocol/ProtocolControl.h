#pragma once
#include "IFrameParser.h"
#include "communication/ITransportTypes.h"
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

// Create/destroy on the caller thread. Parsing and publishing run on m_thread.
// storage consumes frameParsed for every frame; visualization reads latest().
// Null communication supports feeding complete frames via parseFrame().
class ProtocolControl final : public QObject {
    Q_OBJECT
public:
    explicit ProtocolControl(communication::ConnectControl *communication = nullptr,
                             QObject *parent = nullptr, ParserFactory factory = ParserFactory());
    ~ProtocolControl() override;
    core::FrameRepositoryPtr dataModel() const { return m_repository; }
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
};
} } // namespace fosd::protocol
Q_DECLARE_METATYPE(fosd::protocol::ProtocolError)
