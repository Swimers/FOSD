#pragma once
#include "StorageTypes.h"
#include <functional>
#include <memory>

namespace fosd { 
namespace storage {
// Construct, open, write, flush and destroy on the owning calculation thread.
// Implementations append to an existing compatible schema, never truncate it.
class IStorageWriter {
public:
    IStorageWriter() = default;
    IStorageWriter(const IStorageWriter &) = delete;
    IStorageWriter &operator=(const IStorageWriter &) = delete;
    virtual ~IStorageWriter() = default;
    virtual StorageFormat format() const = 0;
    virtual bool open(const QString &path, QString *error) = 0;
    virtual bool write(const QVector<StorageRecord> &records, QString *error) = 0;
    virtual bool flush(QString *error) = 0;
    virtual void close() = 0;
    virtual bool isOpen() const = 0;
    virtual QStringList columns() const { return storageColumnNames(); }
};
using WriterFactory = std::function<std::unique_ptr<IStorageWriter>(StorageFormat)>; // 一种“写入器工厂”的函数类型
std::unique_ptr<IStorageWriter> createStorageWriter(StorageFormat format);  // 一个符合该类型的默认工厂函数。

}
}
