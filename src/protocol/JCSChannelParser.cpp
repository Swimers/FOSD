#include "JCSChannelParser.h"

#include <QPair>
#include <QtEndian>

namespace fosd {
namespace protocol {
namespace {
core::FrameDataPtr fail(QString *error, const QString &message) {
    if (error)
        *error = message;
    return {};
}
} // namespace

QString JCSChannelParser::protocolId() const {
    return QStringLiteral("jcs.channel");
}

bool JCSChannelParser::matches(const QByteArray &packet) const {
    if (packet.size() < 4)
        return false;
    const uchar *data = reinterpret_cast<const uchar *>(packet.constData());
    return data[0] == 0xFA && data[2] == 0xFA && data[1] == data[3] && data[1] < 6;
}

core::FrameDataPtr JCSChannelParser::parse(const QByteArray &packet, QString *error) const {
    if (error)
        error->clear();
    const int headerSize = 8;
    const int xOffset = 8;
    const int yOffset = 708;
    const int sampleSize = 2;
    if (packet.size() < headerSize)
        return fail(error, QStringLiteral("JCS channel frame is shorter than the 8-byte header"));
    if (!matches(packet))
        return fail(error,
                    QStringLiteral("Invalid JCS channel frame magic or channel (expected 0..5)"));

    const uchar *data = reinterpret_cast<const uchar *>(packet.constData());
    // Offsets 4..5 are unknown; the legacy source does not interpret them.
    const quint16 pointCount = qFromLittleEndian<quint16>(data + 6);
    const int requiredSize = yOffset + int(pointCount) * sampleSize;
    if (packet.size() < requiredSize)
        return fail(error, QStringLiteral("Invalid JCS channel frame length: actual=%1, minimum=%2")
                               .arg(packet.size())
                               .arg(requiredSize));

    // Reproduce the legacy X/Y offsets, assuming unsigned little-endian 16-bit
    // samples. FLOATA and MAX_POINTS definitions were not supplied. Do not
    // invent a 350-point limit: the old format only establishes fixed offsets.
    // Keep decoded samples local; this retired protocol publishes no data.
    QVector<QPair<quint16, quint16>> samples;
    samples.reserve(pointCount);
    for (int index = 0; index < pointCount; ++index) {
        const quint16 x = qFromLittleEndian<quint16>(data + xOffset + index * sampleSize);
        const quint16 y = qFromLittleEndian<quint16>(data + yOffset + index * sampleSize);
        samples.append(qMakePair(x, y));
    }
    // Like the old parser, accept trailing padding without interpreting it.
    return {};
}

} // namespace protocol
} // namespace fosd
