#pragma once
#include <QDateTime>
#include <QMetaType>
#include <QString>
#include <QVector>
#include <memory>

namespace fosd {
namespace core {
// Core remains independent of socket classes and the communication module.
enum class TransportKind { Serial, Udp, Tcp }; //数据传输方法
struct FrameSource {
    TransportKind transport = TransportKind::Serial;
    QString address;
    quint16 port = 0;
}; //数据来源
struct FrameData {
    virtual ~FrameData() = default;
};                                                     //帧数据
using FrameDataPtr = std::shared_ptr<const FrameData>; //帧数据 指针

struct WavelengthRecord {
    quint8 channel = 0;
    quint8 sensor = 0;
    float wavelength = 0;
}; //一条波长记录，包含通道、传感器编号和波长值
struct WavelengthFrame final : FrameData {
    quint8 version = 0;
    quint8 dataType = 0;
    quint16 recordCount = 0;
    quint32 sequence = 0;
    QVector<WavelengthRecord> records;
}; //一帧完整的波长数据，包括版本、类型、记录数量、序号和记录数组。

// Shared instances are constructed const. Readers cannot change the envelope
// or payload; old snapshots remain valid after newer frames arrive.
struct FrameSnapshot {
    QString protocolId;      //协议编号
    FrameSource source;      //数据来源
    QDateTime receivedAtUtc; //接收时间
    quint64 revision = 0;    //修订版本
    FrameDataPtr data;       //帧数据
}; //给协议数据增加协议编号、来源、时间和修订版本等上下文信息。
using FrameSnapshotPtr = std::shared_ptr<const FrameSnapshot>; //帧快照 指针

} // namespace core
} // namespace fosd
Q_DECLARE_METATYPE(fosd::core::FrameSnapshotPtr) //
