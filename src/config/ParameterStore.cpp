#include "ParameterStore.h"
#include <QCoreApplication>
#include <QDir>
#include <QFileInfo>
#include <QResource>
#include <mutex>

static void registerParameterResources() {
    Q_INIT_RESOURCE(parameter_defaults);
}

static void initParameterResources() {
    static std::once_flag once;
    std::call_once(once, registerParameterResources);
}

namespace fosd {
namespace config {

ParameterStore::ParameterStore(const QString &path) : m_store(path) {
    initParameterResources();
}

QString ParameterStore::runtimePath() {
    const auto overridePath = qEnvironmentVariable("FOSD_CONFIG_FILE");
    return overridePath.isEmpty()
               ? QDir(QCoreApplication::applicationDirPath()).filePath("config/parameters.ini")
               : overridePath;
}

bool ParameterStore::prepare(QString *error) {
    const QFileInfo target(m_store.filePath());
    const auto canonicalName = QFileInfo(target.canonicalFilePath()).fileName();
    if (target.fileName().compare("core-defaults.ini", Qt::CaseInsensitive) == 0 ||
        canonicalName.compare("core-defaults.ini", Qt::CaseInsensitive) == 0) {
        if (error)
            *error = "The default template is read-only; choose a runtime INI file";
        return false;
    }
    if (QFileInfo::exists(m_store.filePath()))
        return true;
    IniConfigStore defaults(":/fosd-config/core-defaults.ini");
    ConfigData data;
    if (!defaults.readAll(&data, error) || !validateCoreDefaults(data, error))
        return false;
    // All deployed defaults are checked against the authoritative typed schema
    // by tests. Atomic key seeding never overwrites an existing user value.
    return m_store.ensureDefaults(data, error);
}
} // namespace config
} // namespace fosd
