#include "SQLiteWriter.h"
#include <QSqlError>
#include <QSqlQuery>
#include <QUuid>

namespace fosd { namespace storage {
bool SQLiteWriter::open(const QString &path, QString *error) {
    close();
    if (error) error->clear();
    if (!prepareOutputPath(path, error)) return false;
    m_connectionName = QStringLiteral("fosd.storage.%1").arg(QUuid::createUuid().toString());
    m_database = QSqlDatabase::addDatabase(QStringLiteral("QSQLITE"), m_connectionName);
    m_database.setDatabaseName(path);
    m_database.setConnectOptions(QStringLiteral("QSQLITE_BUSY_TIMEOUT=3000"));
    if (!m_database.open()) {
        if (error) *error = m_database.lastError().text();
        close();
        return false;
    }
    if (!initializeSchema(error)) { close(); return false; }
    return true;
}
bool SQLiteWriter::initializeSchema(QString *error) {
    QStringList definitions;
    definitions.append(QStringLiteral("record_id INTEGER PRIMARY KEY AUTOINCREMENT"));
    for (const auto &column : storageSchema())
        definitions.append(column.name + ' ' + column.sqlType);
    QSqlQuery query(m_database);
    if (!query.exec(QStringLiteral("CREATE TABLE IF NOT EXISTS fosd_records (%1)").arg(definitions.join(','))) ||
        !query.exec(QStringLiteral("PRAGMA table_info(fosd_records)"))) {
        if (error) *error = query.lastError().text();
        return false;
    }
    QStringList actual;
    QStringList types;
    bool primaryKeyMatches = true;
    while (query.next()) {
        const bool first = actual.isEmpty();
        primaryKeyMatches = primaryKeyMatches && query.value(5).toInt() == (first ? 1 : 0);
        actual.append(query.value(1).toString()); types.append(query.value(2).toString().toUpper());
    }
    QStringList expected = storageColumnNames(); expected.prepend(QStringLiteral("record_id"));
    QStringList expectedTypes; expectedTypes.append(QStringLiteral("INTEGER"));
    for (const auto &column : storageSchema()) expectedTypes.append(column.sqlType);
    if (actual != expected || types != expectedTypes || !primaryKeyMatches) {
        if (error) *error = QStringLiteral("Existing SQLite table does not match the storage schema");
        return false;
    }
    if (!query.exec(QStringLiteral("CREATE INDEX IF NOT EXISTS fosd_records_source ON fosd_records(session_id,source_revision,data_type)"))) {
        if (error) *error = query.lastError().text();
        return false;
    }
    return true;
}
bool SQLiteWriter::write(const QVector<StorageRecord> &records, QString *error) {
    if (!validateRecords(records, error)) return false;
    if (!isOpen() || !m_database.transaction()) {
        if (error) *error = isOpen() ? m_database.lastError().text() : QStringLiteral("SQLite writer is not open");
        return false;
    }
    QSqlQuery query(m_database);
    QStringList placeholders;
    for (int index = 0; index < ColumnCount; ++index) placeholders.append("?");
    bool success = query.prepare(QStringLiteral("INSERT INTO fosd_records (%1) VALUES (%2)")
                                 .arg(storageColumnNames().join(','), placeholders.join(',')));
    if (success) {
        for (const auto &record : records) {
            for (int index = 0; index < ColumnCount; ++index) query.bindValue(index, record.values[index]);
            if (!query.exec()) { success = false; break; }
        }
    }
    if (!success) {
        if (error) *error = query.lastError().text();
        m_database.rollback();
        return false;
    }
    if (!m_database.commit()) {
        if (error) *error = m_database.lastError().text();
        m_database.rollback();
        return false;
    }
    return true;
}
bool SQLiteWriter::flush(QString *error) {
    if (error) error->clear();
    if (isOpen()) return true; // Each write has already committed its transaction.
    if (error) *error = QStringLiteral("SQLite writer is not open");
    return false;
}
void SQLiteWriter::close() {
    if (m_connectionName.isEmpty()) return;
    m_database.close();
    m_database = QSqlDatabase();
    QSqlDatabase::removeDatabase(m_connectionName);
    m_connectionName.clear();
}
} }
