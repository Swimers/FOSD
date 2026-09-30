#ifndef WAVELENGTHFRAME_H
#define WAVELENGTHFRAME_H

#include <QByteArray>
#include <QString>
#include <QVector>

struct WavelengthRecord
{
    // One-based identifiers, matching the existing CH1_1 display convention.
    quint8 channel;
    quint8 sensor;
    float wavelength;
};

struct WavelengthFrame
{
    quint8 version = 0;
    quint8 dataType = 0;
    quint32 sequence = 0;
    QVector<WavelengthRecord> records;
};

bool isWavelengthFrame(const QByteArray &bytes);
bool parseWavelengthFrame(const QByteArray &bytes, WavelengthFrame &frame,
                          QString *error = nullptr);
bool parseLegacyWavelengthFrame(const QByteArray &bytes, WavelengthFrame &frame,
                                QString *error = nullptr);

#endif
