#include "IniConfigStore.h"
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QLockFile>
#include <QSaveFile>
#include <QSettings>
#include <QSet>
#include <QTemporaryDir>
#include <cmath>

namespace fosd { namespace config {
namespace {
bool fail(QString *error, const QString &message) {
    if (error) *error = message;
    return false;
}
void clearError(QString *error) { if (error) error->clear(); }
bool validName(const QString &name, QString *error) {
    if (name.isEmpty() || name != name.trimmed() || name.contains('/') || name.contains('\\') ||
        name.contains('\n') || name.contains('\r') || name.contains(QChar(0)))
        return fail(error, QStringLiteral("INI module/key must be nonempty, trimmed and contain no slashes, newlines or NUL"));
    return true;
}
bool validValue(const QVariant &value, QString *error) {
    switch (value.type()) {
    case QVariant::String: case QVariant::StringList: case QVariant::Bool:
    case QVariant::Int: case QVariant::UInt: case QVariant::LongLong: case QVariant::ULongLong:
        return true;
    case QVariant::Double:
        if (std::isfinite(value.toDouble())) return true;
        break;
    default: break;
    }
    return fail(error, QStringLiteral("INI values must be strings, string lists, booleans or finite numbers"));
}
bool validDocument(const ConfigData &data, QString *error) {
    QSet<QString> modules;
    for (auto section = data.constBegin(); section != data.constEnd(); ++section) {
        if (!validName(section.key(), error)) return false;
        const auto moduleName = section.key().toCaseFolded();
        if (modules.contains(moduleName)) return fail(error, QStringLiteral("INI module names must not differ only by case"));
        modules.insert(moduleName);
        QSet<QString> keys;
        for (auto entry = section.value().constBegin(); entry != section.value().constEnd(); ++entry) {
            if (!validName(entry.key(), error) || !validValue(entry.value(), error)) return false;
            const auto key = entry.key().toCaseFolded();
            if (keys.contains(key)) return fail(error, QStringLiteral("INI keys must not differ only by case"));
            keys.insert(key);
        }
    }
    return true;
}
void configure(QSettings &settings) {
    settings.setIniCodec("UTF-8");
    settings.setFallbacksEnabled(false);
    settings.setAtomicSyncRequired(true);
}
}

IniConfigStore::IniConfigStore(const QString &filePath, int lockTimeoutMs)
    : m_filePath(filePath.trimmed().isEmpty() ? QString() : QFileInfo(filePath).absoluteFilePath()),
      m_lockTimeoutMs(lockTimeoutMs) {}
bool IniConfigStore::validPath(QString *error) const {
    if (m_filePath.isEmpty()) return fail(error, QStringLiteral("Configuration file path must not be empty"));
    if (m_lockTimeoutMs < 0) return fail(error, QStringLiteral("Lock timeout must be nonnegative"));
    return true;
}
bool IniConfigStore::load(ConfigData *configuration, bool allowMissing, QString *error) const {
    if (!validPath(error)) return false;
    if (allowMissing && !QFileInfo::exists(m_filePath)) { configuration->clear(); return true; }
    QFile file(m_filePath);
    if (!file.open(QIODevice::ReadOnly))
        return fail(error, QStringLiteral("Cannot read '%1': %2").arg(m_filePath, file.errorString()));
    const auto bytes = file.readAll();
    if (file.error() != QFileDevice::NoError)
        return fail(error, QStringLiteral("Cannot read '%1': %2").arg(m_filePath, file.errorString()));

    // Decode a private snapshot, not the live target. QSettings caches files
    // across instances; a unique temporary path avoids stale cached contents.
    QTemporaryDir temporary;
    if (!temporary.isValid()) return fail(error, QStringLiteral("Cannot create INI decoding workspace"));
    const auto path = temporary.filePath("snapshot.ini");
    QFile snapshot(path);
    if (!snapshot.open(QIODevice::WriteOnly) || snapshot.write(bytes) != bytes.size())
        return fail(error, QStringLiteral("Cannot stage INI snapshot: %1").arg(snapshot.errorString()));
    snapshot.close();
    QSettings settings(path, QSettings::IniFormat); configure(settings);
    const auto keys = settings.allKeys(); // Forces parsing of all sections.
    if (settings.status() != QSettings::NoError)
        return fail(error, QStringLiteral("Invalid or unreadable INI file '%1' (QSettings status %2)")
                    .arg(m_filePath).arg(int(settings.status())));
    ConfigData data;
    for (const auto &fullKey : keys) {
        const int separator = fullKey.indexOf('/');
        const QString module = separator < 0 ? QStringLiteral("General") : fullKey.left(separator);
        const QString key = separator < 0 ? fullKey : fullKey.mid(separator + 1);
        if (data.value(module).contains(key)) return fail(error, QStringLiteral("Ambiguous INI key '%1'").arg(fullKey));
        QVariant value = settings.value(fullKey);
        // Qt encodes an empty QStringList as @Invalid(). Restore the list
        // explicitly; writing an invalid QVariant through our API is rejected.
        if (!value.isValid()) value = QStringList();
        data[module].insert(key, value);
    }
    if (!validDocument(data, error)) return false;
    *configuration = data;
    return true;
}
bool IniConfigStore::save(const ConfigData &configuration, QString *error) const {
    // QSettings writes only the private staging file. A failed sync/destructor
    // retry therefore cannot change the actual configuration after returning.
    QTemporaryDir temporary;
    if (!temporary.isValid()) return fail(error, QStringLiteral("Cannot create INI encoding workspace"));
    const auto path = temporary.filePath("output.ini");
    {
        QSettings settings(path, QSettings::IniFormat); configure(settings);
        for (auto section = configuration.constBegin(); section != configuration.constEnd(); ++section)
            for (auto entry = section.value().constBegin(); entry != section.value().constEnd(); ++entry) {
                const QString key = section.key() == "General" ? entry.key() : section.key() + '/' + entry.key();
                settings.setValue(key, entry.value());
            }
        settings.sync();
        if (settings.status() != QSettings::NoError)
            return fail(error, QStringLiteral("Cannot encode INI (QSettings status %1)").arg(int(settings.status())));
    }
    QByteArray bytes;
    if (QFileInfo::exists(path)) {
        QFile encoded(path);
        if (!encoded.open(QIODevice::ReadOnly)) return fail(error, encoded.errorString());
        bytes = encoded.readAll();
        if (encoded.error() != QFileDevice::NoError) return fail(error, encoded.errorString());
    } // Empty configuration is represented by an empty file.
    QSaveFile file(m_filePath); file.setDirectWriteFallback(false);
    if (!file.open(QIODevice::WriteOnly)) return fail(error, QStringLiteral("Cannot write '%1': %2").arg(m_filePath, file.errorString()));
    if (file.write(bytes) != bytes.size()) {
        file.cancelWriting(); return fail(error, QStringLiteral("Incomplete INI write: %1").arg(file.errorString()));
    }
    if (!file.commit()) return fail(error, QStringLiteral("Cannot commit '%1': %2").arg(m_filePath, file.errorString()));
    return true;
}
bool IniConfigStore::readAll(ConfigData *configuration, QString *error) const {
    clearError(error);
    if (!configuration) return fail(error, QStringLiteral("Configuration output pointer is null"));
    QReadLocker lock(&m_lock); return load(configuration, false, error);
}
bool IniConfigStore::readModule(const QString &module, ConfigSection *values, QString *error) const {
    clearError(error);
    if (!values) return fail(error, QStringLiteral("Module output pointer is null"));
    if (!validName(module, error)) return false;
    ConfigData data;
    if (!readAll(&data, error)) return false;
    if (!data.contains(module)) return fail(error, QStringLiteral("Module '%1' does not exist").arg(module));
    *values = data.value(module); return true;
}
bool IniConfigStore::readValue(const QString &module, const QString &key, QVariant *value, QString *error) const {
    clearError(error);
    if (!value) return fail(error, QStringLiteral("Value output pointer is null"));
    if (!validName(key, error)) return false;
    ConfigSection values;
    if (!readModule(module, &values, error)) return false;
    if (!values.contains(key)) return fail(error, QStringLiteral("Key '%1' does not exist in module '%2'").arg(key, module));
    *value = values.value(key); return true;
}
bool IniConfigStore::modify(Change change, const QString &module, const QString &key,
                            const ConfigData &replacement, const QVariant &value, QString *error) {
    clearError(error);
    if (!validPath(error)) return false;
    if (change != Change::ReplaceAll && !validName(module, error)) return false;
    if ((change == Change::SetValue || change == Change::RemoveValue) && !validName(key, error)) return false;
    if (change == Change::SetValue && !validValue(value, error)) return false;
    if (!validDocument(replacement, error)) return false;
    QWriteLocker lock(&m_lock);
    const auto directory = QFileInfo(m_filePath).absolutePath();
    if (!QDir().mkpath(directory)) return fail(error, QStringLiteral("Cannot create configuration directory '%1'").arg(directory));
    QLockFile fileLock(m_filePath + ".lock");
    if (!fileLock.tryLock(m_lockTimeoutMs))
        return fail(error, QStringLiteral("Cannot lock '%1' (QLockFile error %2)").arg(m_filePath).arg(int(fileLock.error())));
    ConfigData data;
    const bool creating = change == Change::ReplaceModule || change == Change::SetValue;
    if (change != Change::ReplaceAll && !load(&data, creating, error)) return false;
    if (change == Change::ReplaceAll) data = replacement;
    else if (change == Change::ReplaceModule) {
        const auto values = replacement.value(module);
        if (values.isEmpty()) data.remove(module); else data[module] = values;
    } else if (change == Change::RemoveModule) {
        if (!data.remove(module)) return fail(error, QStringLiteral("Module '%1' does not exist").arg(module));
    } else {
        auto values = data.value(module);
        if (change == Change::SetValue) values.insert(key, value);
        else if (!values.remove(key)) return fail(error, QStringLiteral("Key '%1' does not exist in module '%2'").arg(key, module));
        if (values.isEmpty()) data.remove(module); else data[module] = values;
    }
    return validDocument(data, error) && save(data, error);
}
bool IniConfigStore::writeAll(const ConfigData &configuration, QString *error) {
    return modify(Change::ReplaceAll, {}, {}, configuration, {}, error);
}
bool IniConfigStore::writeModule(const QString &module, const ConfigSection &values, QString *error) {
    return modify(Change::ReplaceModule, module, {}, {{module, values}}, {}, error);
}
bool IniConfigStore::writeValue(const QString &module, const QString &key, const QVariant &value, QString *error) {
    return modify(Change::SetValue, module, key, {}, value, error);
}
bool IniConfigStore::removeModule(const QString &module, QString *error) {
    return modify(Change::RemoveModule, module, {}, {}, {}, error);
}
bool IniConfigStore::removeValue(const QString &module, const QString &key, QString *error) {
    return modify(Change::RemoveValue, module, key, {}, {}, error);
}
} }
