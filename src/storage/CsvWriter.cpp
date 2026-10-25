#include "CsvWriter.h"
#include <limits>

namespace fosd {
namespace storage {
namespace {
QString csvField(const QVariant &value) {
    QString text;
    if (value.isValid() && !value.isNull()) {
        if (value.type() == QVariant::Double)
            text =
                QString::number(value.toDouble(), 'g', std::numeric_limits<double>::max_digits10);
        else
            text = value.toString();
    }
    if (text.contains(',') || text.contains('"') || text.contains('\r') || text.contains('\n')) {
        text.replace("\"", "\"\"");
        text = '"' + text + '"';
    }
    return text;
}
} // namespace
bool CsvWriter::open(const QString &path, QString *error) {
    close();
    if (error)
        error->clear();
    if (!prepareOutputPath(path, error))
        return false;
    m_file.setFileName(path);
    if (!m_file.open(QIODevice::ReadWrite)) {
        if (error)
            *error = m_file.errorString();
        return false;
    }
    const QByteArray header = storageColumnNames().join(',').toUtf8() + '\n';
    if (m_file.size() == 0) {
        if (m_file.write(header) != header.size() || !m_file.flush()) {
            if (error)
                *error = m_file.errorString();
            m_file.resize(0);
            close();
            return false;
        }
    } else if (m_file.readLine() != header) {
        if (error)
            *error = QStringLiteral("Existing CSV header does not match the storage schema");
        close();
        return false;
    }
    if (!m_file.seek(m_file.size() - 1) || m_file.read(1) != "\n") {
        if (error)
            *error = QStringLiteral("Existing CSV ends with an incomplete record");
        close();
        return false;
    }
    if (!m_file.seek(m_file.size())) {
        if (error)
            *error = m_file.errorString();
        close();
        return false;
    }
    return true;
}

bool CsvWriter::write(const QVector<StorageRecord> &records, QString *error) {
    if (!validateRecords(records, error))
        return false;
    if (!isOpen()) {
        if (error)
            *error = QStringLiteral("CSV writer is not open");
        return false;
    }
    QByteArray bytes;
    for (const auto &record : records) {
        QStringList fields;
        for (const auto &value : record.values)
            fields.append(csvField(value));
        bytes += fields.join(',').toUtf8() + '\n';
    }
    const qint64 originalSize = m_file.size();
    if (m_file.write(bytes) == bytes.size() && m_file.flush())
        return true;
    if (error)
        *error = m_file.errorString();
    // Undo this batch's append on a normal seekable local file.
    if (!m_file.resize(originalSize) || !m_file.seek(originalSize))
        if (error)
            *error += QStringLiteral("; unable to roll back the CSV append");
    return false;
}

bool CsvWriter::flush(QString *error) {
    if (error)
        error->clear();
    if (isOpen() && m_file.flush())
        return true;
    if (error)
        *error = isOpen() ? m_file.errorString() : QStringLiteral("CSV writer is not open");
    return false;
}

void CsvWriter::close() {
    m_file.close();
}
} // namespace storage
} // namespace fosd
