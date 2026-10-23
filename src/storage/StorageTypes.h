#pragma once
#include "core/Datamodel/CalibratedData.h"
#include "core/Datamodel/ModelData.h"
#include <QFlags>
#include <QVariant>

namespace fosd { 
namespace storage {

using PointId = core::PointId;

enum class StorageFormat { 
    Csv, 
    SQLite 
};
enum class DataType { 
    Wavelength = 1, 
    CalibratedHardware = 2, 
    PointDisplacement = 4 };
Q_DECLARE_FLAGS(DataTypes, DataType)

struct StorageConfiguration {
    StorageFormat format = StorageFormat::Csv;  // 写入方式
    QString path; // A single append-only CSV or SQLite file.
    DataTypes dataTypes = DataType::Wavelength; // 写入数据类型
    QVector<PointId> pointIds;
    bool allPoints = false;
};
struct StorageError {
    QString operation;
    QString message;
    QString path;
    quint64 sourceRevision = 0;
};

enum Column {
    SessionId, DataTypeName, Stage, ProtocolId, SourceRevision, ReceivedAtUtc,
    Transport, PeerAddress, PeerPort, DeviceSequence, AlgorithmId,
    CalculationRevision, ModelRevision, ConfigurationRevision,
    SampleIndex, SourceIndex, Channel, Sensor, PointIdCol, WavelengthValue, StrainValue,
    DxMm, DyMm, DzMm, XMm, YMm, ZMm, ColumnCount
};
struct StorageColumn { QString name; QString sqlType; };
struct StorageRecord { QVariantList values; };
struct StorageBatch {
    DataType type = DataType::Wavelength;
    quint64 sourceRevision = 0;
    QVector<StorageRecord> records;
};

const QVector<StorageColumn> &storageSchema();
QStringList storageColumnNames();
bool validateRecords(const QVector<StorageRecord> &records, QString *error);
bool prepareOutputPath(const QString &path, QString *error);

} 
}
Q_DECLARE_OPERATORS_FOR_FLAGS(fosd::storage::DataTypes)
Q_DECLARE_METATYPE(fosd::storage::StorageConfiguration)
Q_DECLARE_METATYPE(fosd::storage::StorageError)
Q_DECLARE_METATYPE(fosd::storage::DataType)
