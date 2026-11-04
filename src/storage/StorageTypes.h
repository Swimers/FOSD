#pragma once
#include "config/CoreParameters.h"
#include "core/Datamodel/CalibratedData.h"
#include "core/Datamodel/ModelData.h"
#include <QFlags>
#include <QVariant>

namespace fosd {
namespace storage {







struct StorageError {
    QString operation;
    QString message;
    QString path;
    quint64 sourceRevision = 0;
};

enum Column {
    SessionId,
    DataTypeName,
    Stage,
    ProtocolId,
    SourceRevision,
    ReceivedAtUtc,
    Transport,
    PeerAddress,
    PeerPort,
    DeviceSequence,
    AlgorithmId,
    CalculationRevision,
    ModelRevision,
    ConfigurationRevision,
    SampleIndex,
    SourceIndex,
    Channel,
    Sensor,
    PointIdCol,
    WavelengthValue,
    StrainValue,
    DxMm,
    DyMm,
    DzMm,
    XMm,
    YMm,
    ZMm,
    ColumnCount
};
struct StorageColumn {
    QString name;
    QString sqlType;
};
struct StorageRecord {
    QVariantList values;
};
struct StorageBatch {
    DataType type = DataType::Wavelength;
    quint64 sourceRevision = 0;
    QVector<StorageRecord> records;
};

const QVector<StorageColumn> &storageSchema();
QStringList storageColumnNames();
bool validateRecords(const QVector<StorageRecord> &records, QString *error);
bool prepareOutputPath(const QString &path, QString *error);

} // namespace storage
} // namespace fosd
Q_DECLARE_OPERATORS_FOR_FLAGS(fosd::storage::DataTypes)
Q_DECLARE_METATYPE(fosd::storage::StorageConfiguration)
Q_DECLARE_METATYPE(fosd::storage::StorageError)
Q_DECLARE_METATYPE(fosd::storage::DataType)
