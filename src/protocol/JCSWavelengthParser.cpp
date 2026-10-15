#include "JCSWavelengthParser.h"
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
}

QString JCSWavelengthParser::protocolId() const { 
    return QStringLiteral("jcs.wavelength"); 
}

bool JCSWavelengthParser::matches(const QByteArray &packet) const {
    return packet.startsWith(QByteArray::fromHex("fafbfafb"));
}

core::FrameDataPtr JCSWavelengthParser::parse(const QByteArray &packet, QString *error) const {
    static_assert(sizeof(float) == 4 && std::numeric_limits<float>::is_iec559,
                  "The wire format requires IEEE 754 binary32 floats");
    if (error)
        error->clear();

    // Reverse-engineered layout: MAX_CHANNELS=7, DataAddress=12 and
    // FLOATA2.all=float32 are assumptions from FBG逆向解包.md. Confirm
    // against the original global.h/analyzepackets.h or device captures.
    const int channelCount = 7;
    const int countOffset = 4 + channelCount;
    const int headerSize = countOffset + 1;
    const int recordSize = 4;
    if (packet.size() < headerSize)
        return fail(error, QStringLiteral("JCS frame is shorter than the 12-byte header"));
    if (!matches(packet))
        return fail(error, QStringLiteral("Invalid JCS wavelength frame magic"));

    const uchar *data = reinterpret_cast<const uchar *>(packet.constData());
    const int totalSensors = data[countOffset];
    int channelSensors = 0;
    for (int channel = 0; channel < channelCount; ++channel)
        channelSensors += data[4 + channel];
    if (channelSensors != totalSensors)
        return fail(error, QStringLiteral("JCS sensor count mismatch: channels=%1, total=%2")
                    .arg(channelSensors).arg(totalSensors));

    const int requiredSize = headerSize + totalSensors * recordSize;
    // The old parser checks a minimum length; trailing bytes have no decoded
    // meaning here and are retained as compatible padding, not extra records.
    if (packet.size() < requiredSize)
        return fail(error, QStringLiteral("Invalid JCS frame length: actual=%1, minimum=%2")
                    .arg(packet.size()).arg(requiredSize));

    core::WavelengthFrame frame;
    frame.recordCount = static_cast<quint16>(totalSensors);
    // JCS supplies no version, dataType or sequence: keep their default zeros.
    frame.records.reserve(totalSensors);
    int offset = headerSize;
    for (int channel = 0; channel < channelCount; ++channel) {
        const int sensors = data[4 + channel];
        for (int sensor = 0; sensor < sensors; ++sensor) {
            core::WavelengthRecord record;
            // Match the legacy CH1_1 naming; empty channels still occupy IDs.
            record.channel = static_cast<quint8>(channel + 1);
            record.sensor = static_cast<quint8>(sensor + 1);
            const quint32 bits = qFromLittleEndian<quint32>(data + offset);
            std::memcpy(&record.wavelength, &bits, sizeof(record.wavelength));
            frame.records.append(record);
            offset += recordSize;
        }
    }
    return std::make_shared<const core::WavelengthFrame>(std::move(frame));
}

FrameParsers createFrameParsers() {
    FrameParsers parsers;
    parsers.emplace_back(new JCSWavelengthParser);
    return parsers;
}

}
} // namespace fosd::protocol
