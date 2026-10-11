#pragma once
#include <QDateTime>
#include <QMetaType>
#include <QString>
#include <QVector>
#include <memory>

namespace fosd { namespace core {

// Core remains independent of socket classes and the communication module.
enum class TransportKind { Serial, Udp, Tcp };
struct FrameSource {
    TransportKind transport = TransportKind::Serial;
    QString address;
    quint16 port = 0;
};
struct FrameData { virtual ~FrameData() = default; };
using FrameDataPtr = std::shared_ptr<const FrameData>;

struct WavelengthRecord {
    quint8 channel = 0;
    quint8 sensor = 0;
    float wavelength = 0;
};
struct WavelengthFrame final : FrameData {
    quint8 version = 0;
    quint8 dataType = 0;
    quint16 recordCount = 0;
    quint32 sequence = 0;
    QVector<WavelengthRecord> records;
};

// Shared instances are constructed const. Readers cannot change the envelope
// or payload; old snapshots remain valid after newer frames arrive.
struct FrameSnapshot {
    QString protocolId;
    FrameSource source;
    QDateTime receivedAtUtc;
    quint64 revision = 0;
    FrameDataPtr data;
};
using FrameSnapshotPtr = std::shared_ptr<const FrameSnapshot>;

} } // namespace fosd::core
Q_DECLARE_METATYPE(fosd::core::FrameSnapshotPtr)
