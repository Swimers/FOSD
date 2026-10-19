#include "StorageTypes.h"
#include <QDir>
#include <QFileInfo>

namespace fosd { namespace storage {
const QVector<StorageColumn> &storageSchema() {
    static const QVector<StorageColumn> schema = {
        {"session_id", "TEXT"}, {"data_type", "TEXT"}, {"stage", "TEXT"},
        {"protocol_id", "TEXT"}, {"source_revision", "TEXT"}, {"received_at_utc", "TEXT"},
        {"transport", "TEXT"}, {"peer_address", "TEXT"}, {"peer_port", "INTEGER"},
        {"device_sequence", "INTEGER"}, {"algorithm_id", "TEXT"},
        {"calculation_revision", "TEXT"}, {"model_revision", "TEXT"}, {"configuration_revision", "TEXT"},
        {"sample_index", "INTEGER"}, {"source_index", "INTEGER"}, {"channel", "INTEGER"},
        {"sensor", "INTEGER"}, {"point_id", "INTEGER"}, {"wavelength", "REAL"},
        {"strain", "REAL"}, {"dx_mm", "REAL"}, {"dy_mm", "REAL"}, {"dz_mm", "REAL"},
        {"x_mm", "REAL"}, {"y_mm", "REAL"}, {"z_mm", "REAL"}
    };
    return schema;
}
QStringList storageColumnNames() {
    QStringList result;
    for (const auto &column : storageSchema())
        result.append(column.name);
    return result;
}
bool validateRecords(const QVector<StorageRecord> &records, QString *error) {
    if (error)
        error->clear();
    for (const auto &record : records)
        if (record.values.size() != ColumnCount) {
            if (error)
                *error = QStringLiteral("Storage record does not match the schema");
            return false;
        }
    return true;
}
bool prepareOutputPath(const QString &path, QString *error) {
    if (path.trimmed().isEmpty() || QFileInfo(path).isDir() ||
        !QDir().mkpath(QFileInfo(path).absolutePath())) {
        if (error)
            *error = QStringLiteral("Cannot create storage directory or invalid file path: %1").arg(path);
        return false;
    }
    return true;
}
} }
