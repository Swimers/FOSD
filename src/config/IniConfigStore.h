#pragma once
#include "IConfigStore.h"
#include <QReadWriteLock>

namespace fosd { namespace config {

// Synchronous, uncached UTF-8 INI storage; only Qt Core is required.
// One instance is safe to share across threads. Cooperating instances/processes
// serialize writes using <file>.lock; readers see complete committed documents.
// All writers must use the same absolute file path and this locking protocol.
class IniConfigStore final : public IConfigStore {
public:
    explicit IniConfigStore(const QString &filePath, int lockTimeoutMs = 1000);
    IniConfigStore(const IniConfigStore &) = delete;
    IniConfigStore &operator=(const IniConfigStore &) = delete;

    QString filePath() const { return m_filePath; }
    // Atomic default seeding: keep existing keys, add missing defaults only.
    bool ensureModule(const QString &module, const ConfigSection &defaults, QString *error = nullptr);
    bool ensureDefaults(const ConfigData &defaults, QString *error = nullptr);
    bool readAll(ConfigData *configuration, QString *error = nullptr) const override;
    // Explicit full replacement: can also repair a malformed existing file.
    bool writeAll(const ConfigData &configuration, QString *error = nullptr) override;
    bool readModule(const QString &module, ConfigSection *values, QString *error = nullptr) const override;
    bool writeModule(const QString &module, const ConfigSection &values, QString *error = nullptr) override;
    bool readValue(const QString &module, const QString &key, QVariant *value,
                   QString *error = nullptr) const override;
    bool writeValue(const QString &module, const QString &key, const QVariant &value,
                    QString *error = nullptr) override;
    bool removeModule(const QString &module, QString *error = nullptr) override;
    bool removeValue(const QString &module, const QString &key, QString *error = nullptr) override;

private:
    enum class Change { ReplaceAll, ReplaceModule, SetValue, RemoveModule, RemoveValue, EnsureModule, EnsureAll };
    bool modify(Change change, const QString &module, const QString &key,
                const ConfigData &replacement, const QVariant &value, QString *error);
    bool load(ConfigData *configuration, bool allowMissing, QString *error) const;
    bool save(const ConfigData &configuration, QString *error) const;
    bool validPath(QString *error) const;

    const QString m_filePath;
    const int m_lockTimeoutMs;
    mutable QReadWriteLock m_lock;
};

} }
