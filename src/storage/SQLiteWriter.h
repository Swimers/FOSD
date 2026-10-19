#pragma once
#include "IStorageWriter.h"
#include <QSqlDatabase>

namespace fosd { namespace storage {
class SQLiteWriter final : public IStorageWriter {
public:
    ~SQLiteWriter() override { close(); }
    StorageFormat format() const override { return StorageFormat::SQLite; }
    bool open(const QString &path, QString *error) override;
    bool write(const QVector<StorageRecord> &records, QString *error) override;
    bool flush(QString *error) override;
    void close() override;
    bool isOpen() const override { return m_database.isOpen(); }
private:
    bool initializeSchema(QString *error);
    QSqlDatabase m_database;
    QString m_connectionName;
};
} }
