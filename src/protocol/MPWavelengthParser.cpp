#include "MPWavelengthParser.h"
#include <QtEndian>
#include <cstring>
#include <limits>
#include <utility>

namespace fosd {
namespace protocol {
namespace {
core::FrameDataPtr fail(QString *error, const QString &message) {
    if (error)
        *error = message;
    return {};
}
} // namespace

QString MPWavelengthParser::protocolId() const {
    return QStringLiteral("mp.wavelength");
}

bool MPWavelengthParser::matches(const QByteArray &packet) const {
    return packet.startsWith(QByteArray::fromHex("5aa5"));
}

core::FrameDataPtr MPWavelengthParser::parse(const QByteArray &packet, QString *error) const {
    static_assert(sizeof(float) == 4 && std::numeric_limits<float>::is_iec559,
                  "The wire format requires IEEE 754 binary32 floats");
    if (error)
        error->clear();

    const int headerSize = 10;
    const int recordSize = 6;

    if (packet.size() < headerSize)
        return fail(error, QStringLiteral("Frame is shorter than the 10-byte header"));
    const uchar *data = reinterpret_cast<const uchar *>(packet.constData());
    if (qFromLittleEndian<quint16>(data) != 0xA55A)
        return fail(error, QStringLiteral("Invalid wavelength frame magic"));
    core::WavelengthFrame frame;
    frame.version = data[2];
    frame.dataType = data[3];
    frame.recordCount = qFromLittleEndian<quint16>(data + 4);
    frame.sequence = qFromLittleEndian<quint32>(data + 6);
    const int expected = headerSize + int(frame.recordCount) * recordSize;
    if (packet.size() != expected)
        return fail(error, QStringLiteral("Invalid frame length: actual=%1, expected=%2")
                               .arg(packet.size())
                               .arg(expected));
    // The supplied protocol imposes no version/type/channel/sensor restrictions.
    frame.records.reserve(frame.recordCount);
    for (int index = 0; index < frame.recordCount; ++index) {
        const int offset = headerSize + index * recordSize;
        core::WavelengthRecord record;
        record.channel = data[offset];
        record.sensor = data[offset + 1];
        const quint32 bits = qFromLittleEndian<quint32>(data + offset + 2);
        std::memcpy(&record.wavelength, &bits, sizeof(record.wavelength));
        frame.records.append(record);
    }
    return std::make_shared<const core::WavelengthFrame>(std::move(frame));
}

} // namespace protocol
} // namespace fosd
