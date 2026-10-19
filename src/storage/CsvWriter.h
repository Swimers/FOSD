#pragma once
#include "IStorageWriter.h"
#include <QFile>

namespace fosd { namespace storage {
class CsvWriter final : public IStorageWriter {
public:
    ~CsvWriter() override { close(); }
    StorageFormat format() const override { return StorageFormat::Csv; }
    bool open(const QString &path, QString *error) override;
    bool write(const QVector<StorageRecord> &records, QString *error) override;
    bool flush(QString *error) override;
    void close() override;
    bool isOpen() const override { return m_file.isOpen(); }
private:
    QFile m_file;
};
} }
