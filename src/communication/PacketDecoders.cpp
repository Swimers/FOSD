#include "PacketDecoders.h"

#include <QtEndian>
#include <utility>

namespace fosd {
namespace communication {
namespace {
QString checkRules(const QVector<PacketRule> &rules) {
    if (rules.isEmpty())
        return QStringLiteral("At least one packet rule is required");
    for (int index = 0; index < rules.size(); ++index) {
        const PacketRule &rule = rules.at(index);
        if (rule.marker.isEmpty() || !rule.frameSize || rule.maxFrameSize < rule.marker.size())
            return QStringLiteral("Invalid packet framing rule at index %1").arg(index);
    }
    return {};
}

bool validSize(const PacketRule &rule, int size) {
    return size >= rule.marker.size() && size <= rule.maxFrameSize;
}

// Retain only bytes which can become a marker on the next feed().
int markerSuffixSize(const QByteArray &bytes, const QVector<PacketRule> &rules) {
    int keep = 0;
    for (const PacketRule &rule : rules) {
        const int limit = qMin(bytes.size(), rule.marker.size() - 1);
        for (int size = limit; size > keep; --size) {
            if (bytes.endsWith(rule.marker.left(size))) {
                keep = size;
                break;
            }
        }
    }
    return keep;
}

} // namespace

namespace Rule {
PacketRule JCS_Data_PacketRule() {
    const int headerSize = 24;
    const int recordSize = 4;
    PacketRule rule;
    rule.marker = QByteArray::fromHex("fafbfafb");
    rule.maxFrameSize = headerSize + 255 * recordSize;
    rule.frameSize = [headerSize, recordSize](const QByteArray &bytes) -> int {
        if (bytes.size() < headerSize)
            return 0;
        const uchar *data = reinterpret_cast<const uchar *>(bytes.constData());
        int sensors = 0;
        for (int channel = 0; channel < 16; ++channel)
            sensors += data[4 + channel];
        if (sensors != data[20])
            return -1;
        return headerSize + int(data[20]) * recordSize;
    };
    return rule;
}

PacketRule JCS_Channel_PacketRule() {
    const int headerSize = 10;
    const int recordSize = 6;
    const int maxChannel = 16;
    PacketRule rule;
    rule.marker = QByteArray::fromHex("fa");
    rule.maxFrameSize = headerSize + 65535 * recordSize;
    rule.frameSize = [headerSize, recordSize, maxChannel](const QByteArray &bytes) -> int {
        if (bytes.size() < 4)
            return 0;

        const uchar *data = reinterpret_cast<const uchar *>(bytes.constData());
        const uchar channel = data[1];
        if (data[0] != 0xFA || data[2] != 0xFA || channel < 1 || channel > maxChannel ||
            data[3] != channel)
            return -1;

        if (bytes.size() < headerSize)
            return 0;
        return headerSize + int(qFromLittleEndian<quint16>(data + 4)) * recordSize;
    };
    return rule;
}

PacketRule MP_PacketRule() {
    const int headerSize = 10;
    const int recordSize = 6;
    PacketRule rule;
    rule.marker = QByteArray::fromHex("5aa5");
    rule.maxFrameSize = headerSize + 65535 * recordSize;
    rule.frameSize = [headerSize, recordSize](const QByteArray &bytes) -> int {
        if (bytes.size() < headerSize)
            return 0;
        const uchar *data = reinterpret_cast<const uchar *>(bytes.constData());
        return headerSize + int(qFromLittleEndian<quint16>(data + 4)) * recordSize;
    };
    return rule;
}

} // namespace Rule

UdpPacketDecoder::UdpPacketDecoder(QVector<PacketRule> rules)
    : IPacketDecoder(Channel::Udp), m_rules(std::move(rules)) {
}

QString UdpPacketDecoder::validateConfiguration() const {
    return checkRules(m_rules);
}

QVector<QByteArray> UdpPacketDecoder::feed(const QByteArray &bytes, const Endpoint &peer) {
    Q_UNUSED(peer)
    if (!initialize())
        return {};
    for (const PacketRule &rule : m_rules) {
        if (!bytes.startsWith(rule.marker))
            continue;
        const int size = rule.frameSize(bytes);
        if (rule.marker == QByteArray::fromHex("fafbfafb") && validSize(rule, size) && bytes.size() >= size)
            return {bytes.left(size)};
        if (validSize(rule, size) && size == bytes.size() &&
            (!rule.validate || rule.validate(bytes)))
            return {bytes};
    }
    reportError(QStringLiteral("decode"),
                QStringLiteral("Invalid UDP packet header, length or validation"));
    return {};
}

TcpPacketDecoder::TcpPacketDecoder(QVector<PacketRule> rules)
    : IPacketDecoder(Channel::Tcp), m_rules(std::move(rules)) {
}

QString TcpPacketDecoder::validateConfiguration() const {
    return checkRules(m_rules);
}

QVector<QByteArray> TcpPacketDecoder::feed(const QByteArray &bytes, const Endpoint &peer) {
    Q_UNUSED(peer)
    if (!initialize())
        return {};
    m_buffer.append(bytes);
    QVector<QByteArray> packets;
    while (!m_buffer.isEmpty()) {
        int start = -1;
        const PacketRule *matched = nullptr;
        for (const PacketRule &rule : m_rules) {
            const int pos = m_buffer.indexOf(rule.marker);
            // Rule order decides ties. Prefer distinct, non-overlapping markers.
            if (pos >= 0 && (start < 0 || pos < start)) {
                start = pos;
                matched = &rule;
            }
        }
        if (!matched) {
            const int keep = markerSuffixSize(m_buffer, m_rules);
            if (m_buffer.size() > keep)
                reportError(QStringLiteral("decode"),
                            QStringLiteral("Discarded bytes without a TCP packet header"));
            m_buffer = m_buffer.right(keep);
            break;
        }
        if (start > 0) {
            reportError(QStringLiteral("decode"),
                        QStringLiteral("Discarded bytes before TCP packet header"));
            m_buffer.remove(0, start);
        }
        const int size = matched->frameSize(m_buffer);
        if (size == 0 && m_buffer.size() < matched->maxFrameSize)
            break;
        if (!validSize(*matched, size)) {
            reportError(QStringLiteral("decode"), QStringLiteral("Invalid TCP frame length"));
            m_buffer.remove(0, 1);
            continue;
        }
        if (m_buffer.size() < size)
            break;
        const QByteArray packet = m_buffer.left(size);
        if (matched->validate && !matched->validate(packet)) {
            reportError(QStringLiteral("decode"), QStringLiteral("TCP packet validation failed"));
            m_buffer.remove(0, 1);
            continue;
        }
        packets.append(packet);
        m_buffer.remove(0, size);
    }
    return packets;
}

void TcpPacketDecoder::reset() {
    m_buffer.clear();
}

SerialPacketDecoder::SerialPacketDecoder(QByteArray terminator, int maxFrameSize)
    : IPacketDecoder(Channel::Serial), m_terminator(std::move(terminator)),
      m_maxFrameSize(maxFrameSize) {
}

QString SerialPacketDecoder::validateConfiguration() const {
    if (m_terminator.isEmpty() || m_maxFrameSize < m_terminator.size())
        return QStringLiteral("Invalid serial framing configuration");
    return {};
}

QVector<QByteArray> SerialPacketDecoder::feed(const QByteArray &bytes, const Endpoint &peer) {
    Q_UNUSED(peer)
    if (!initialize())
        return {};
    m_buffer.append(bytes);
    QVector<QByteArray> packets;
    for (;;) {
        const int pos = m_buffer.indexOf(m_terminator);
        if (pos < 0) {
            if (!m_discarding && m_buffer.size() > m_maxFrameSize) {
                reportError(QStringLiteral("decode"),
                            QStringLiteral("Serial command exceeds maximum frame size"));
                m_discarding = true;
            }
            if (m_discarding)
                m_buffer = m_buffer.right(m_terminator.size() - 1);
            break;
        }
        const int size = pos + m_terminator.size();
        if (!m_discarding && size <= m_maxFrameSize)
            packets.append(m_buffer.left(size));
        else if (!m_discarding)
            reportError(QStringLiteral("decode"),
                        QStringLiteral("Serial command exceeds maximum frame size"));
        m_buffer.remove(0, size);
        m_discarding = false;
    }
    return packets;
}

void SerialPacketDecoder::reset() {
    m_buffer.clear();
    m_discarding = false;
}

std::unique_ptr<IPacketDecoder> createPacketDecoder(Channel channel) {
    switch (channel) {
        case Channel::Udp:
            return std::unique_ptr<IPacketDecoder>(new UdpPacketDecoder);
        case Channel::Tcp:
            return std::unique_ptr<IPacketDecoder>(new TcpPacketDecoder);
        case Channel::Serial:
            return std::unique_ptr<IPacketDecoder>(new SerialPacketDecoder);
    }
    return nullptr;
}

} // namespace communication
} // namespace fosd
