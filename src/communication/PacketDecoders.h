#pragma once

#include "IPacketDecoder.h"

#include <functional>
#include <memory>

namespace fosd {
namespace communication {
// A rule describes framing only; decoding payload fields belongs to protocol.
// frameSize sees bytes beginning at marker: 0 = incomplete header, -1 = invalid
// header, positive = total frame size (including marker). It must not depend on
// the amount of payload received. validate is optional and sees a complete frame.
struct PacketRule {
    QByteArray marker;
    std::function<int(const QByteArray &)> frameSize;
    std::function<bool(const QByteArray &)> validate;
    int maxFrameSize = 1024 * 1024;
};

namespace Rule {
PacketRule JCS_Data_PacketRule();    //FAFBFAFB header
PacketRule JCS_Channel_PacketRule(); //FAxxFAxx herder
PacketRule MP_PacketRule();          //5AA5 header
} // namespace Rule

// Suitable for ConnectControl's DecoderFactory; instances are channel-local.
std::unique_ptr<IPacketDecoder> createPacketDecoder(Channel channel);

class UdpPacketDecoder final : public IPacketDecoder {
public:
    explicit UdpPacketDecoder(QVector<PacketRule> rules = {Rule::MP_PacketRule(),
                                                           Rule::JCS_Data_PacketRule()});
    QVector<QByteArray> feed(const QByteArray &bytes, const Endpoint &peer) override;

private:
    QString validateConfiguration() const override;
    QVector<PacketRule> m_rules;
};

class TcpPacketDecoder final : public IPacketDecoder {
public:
    explicit TcpPacketDecoder(QVector<PacketRule> rules = {Rule::MP_PacketRule(),
                                                           Rule::JCS_Data_PacketRule()});
    QVector<QByteArray> feed(const QByteArray &bytes, const Endpoint &peer) override;
    void reset() override;

private:
    QString validateConfiguration() const override;
    QVector<PacketRule> m_rules;
    QByteArray m_buffer;
};

class SerialPacketDecoder final : public IPacketDecoder {
public:
    // Output retains the terminator, including empty commands ("\r"). An
    // oversized command is discarded through its terminator, never truncated.
    explicit SerialPacketDecoder(QByteArray terminator = QByteArray("\r"),
                                 int maxFrameSize = 64 * 1024);
    QVector<QByteArray> feed(const QByteArray &bytes, const Endpoint &peer) override;
    void reset() override;

private:
    QString validateConfiguration() const override;
    QByteArray m_terminator;
    int m_maxFrameSize;
    QByteArray m_buffer;
    bool m_discarding = false;
};

} // namespace communication
} // namespace fosd
