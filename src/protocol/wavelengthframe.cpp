#include "wavelengthframe.h"
#include "pocketsconfig.h"

#include <QtEndian>
#include <algorithm>
#include <cmath>
#include <cstring>

namespace {
float readFloat(const uchar *data)
{
    const quint32 bits = qFromLittleEndian<quint32>(data);
    float value;
    static_assert(sizeof(value) == sizeof(bits), "Protocol requires 32-bit floats");
    std::memcpy(&value, &bits, sizeof(value));
    return value;
}

bool fail(QString *error, const QString &message)
{
    if (error)
        *error = message;
    return false;
}
}

bool isWavelengthFrame(const QByteArray &bytes)
{
    return bytes.size() >= 2 && quint8(bytes[0]) == 0x5A && quint8(bytes[1]) == 0xA5;
}

bool parseWavelengthFrame(const QByteArray &bytes, WavelengthFrame &frame, QString *error)
{
    if (error)
        error->clear();
    if (bytes.size() < 10)
        return fail(error, QStringLiteral("Wavelength frame header is truncated"));
    if (!isWavelengthFrame(bytes))
        return fail(error, QStringLiteral("Invalid wavelength frame magic"));

    const auto *data = reinterpret_cast<const uchar *>(bytes.constData());
    const int count = qFromLittleEndian<quint16>(data + 4);
    if (count > 1024 || bytes.size() != 10 + count * 6)
        return fail(error, QStringLiteral("Invalid wavelength record count or frame length"));

    WavelengthFrame parsed;
    // No supported version/dataType values are specified in the supplied protocol.
    parsed.version = data[2];
    parsed.dataType = data[3];
    parsed.sequence = qFromLittleEndian<quint32>(data + 6);
    parsed.records.reserve(count);
    for (int i = 0; i < count; ++i) {
        const uchar *record = data + 10 + i * 6;
        const float value = readFloat(record + 2);
        if (record[0] < 1 || record[0] > MAX_CHANNELS || record[1] < 1 || !std::isfinite(value))
            return fail(error, QStringLiteral("Invalid channel, sensor or wavelength"));
        parsed.records.append({record[0], record[1], value});
    }
    std::sort(parsed.records.begin(), parsed.records.end(),
              [](const WavelengthRecord &a, const WavelengthRecord &b) {
        return a.channel < b.channel || (a.channel == b.channel && a.sensor < b.sensor);
    });
    for (int i = 1; i < parsed.records.size(); ++i) {
        const auto &a = parsed.records[i - 1];
        const auto &b = parsed.records[i];
        if (a.channel == b.channel && a.sensor == b.sensor)
            return fail(error, QStringLiteral("Duplicate channel/sensor record"));
    }
    frame = parsed;
    return true;
}

bool parseLegacyWavelengthFrame(const QByteArray &bytes, WavelengthFrame &frame, QString *error)
{
    if (error)
        error->clear();
    if (bytes.size() < DataAddress || bytes.left(4) != QByteArray::fromHex("fafbfafb"))
        return fail(error, QStringLiteral("Invalid legacy wavelength header"));
    const auto *data = reinterpret_cast<const uchar *>(bytes.constData());
    const int count = data[MAX_CHANNELS + 4];
    int channelTotal = 0;
    for (int ch = 0; ch < MAX_CHANNELS; ++ch)
        channelTotal += data[4 + ch];
    // Retain compatibility with trailing legacy fields.
    if (channelTotal != count || bytes.size() < DataAddress + count * 4)
        return fail(error, QStringLiteral("Invalid legacy sensor count or frame length"));
    WavelengthFrame parsed;
    parsed.records.reserve(count);
    int index = 0;
    for (int ch = 0; ch < MAX_CHANNELS; ++ch) {
        for (int sensor = 0; sensor < data[4 + ch]; ++sensor, ++index) {
            const float value = readFloat(data + DataAddress + index * 4);
            if (!std::isfinite(value))
                return fail(error, QStringLiteral("Invalid legacy wavelength"));
            parsed.records.append({quint8(ch + 1), quint8(sensor + 1), value});
        }
    }
    frame = parsed;
    return true;
}
