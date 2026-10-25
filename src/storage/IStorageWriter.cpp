#include "IStorageWriter.h"
#include "CsvWriter.h"
#include "SQLiteWriter.h"

namespace fosd {
namespace storage {
std::unique_ptr<IStorageWriter> createStorageWriter(StorageFormat format) {
    switch (format) {
        case StorageFormat::Csv:
            return std::unique_ptr<IStorageWriter>(new CsvWriter);
        case StorageFormat::SQLite:
            return std::unique_ptr<IStorageWriter>(new SQLiteWriter);
    }
    return {};
}

} // namespace storage
} // namespace fosd
