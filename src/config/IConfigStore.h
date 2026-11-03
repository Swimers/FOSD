#pragma once
#include <QMap>
#include <QVariant>
#include <QString>

namespace fosd { namespace config {

using ConfigSection = QVariantMap;
using ConfigData = QMap<QString, ConfigSection>;

// Modules are INI groups. Keys are literal names, not dotted paths. On failure,
// read outputs remain unchanged; error receives a diagnostic if supplied.
class IConfigStore {
public:
    virtual ~IConfigStore() = default;
    virtual bool readAll(ConfigData *configuration, QString *error = nullptr) const = 0;
    virtual bool writeAll(const ConfigData &configuration, QString *error = nullptr) = 0;
    virtual bool readModule(const QString &module, ConfigSection *values, QString *error = nullptr) const = 0;
    // Replace this module only; other modules are preserved. An empty module
    // removes its group because QSettings does not persist empty INI groups.
    virtual bool writeModule(const QString &module, const ConfigSection &values, QString *error = nullptr) = 0;
    virtual bool readValue(const QString &module, const QString &key, QVariant *value,
                           QString *error = nullptr) const = 0;
    // Create missing module/key, preserving all other fields.
    virtual bool writeValue(const QString &module, const QString &key, const QVariant &value,
                            QString *error = nullptr) = 0;
    virtual bool removeModule(const QString &module, QString *error = nullptr) = 0;
    virtual bool removeValue(const QString &module, const QString &key, QString *error = nullptr) = 0;
};

} }
