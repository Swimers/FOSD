#ifndef CONFIGMANAGER_H
#define CONFIGMANAGER_H

#include <QString>

#include "AppConfig.h"

class ConfigManager
{
public:
    static ConfigManager& instance();

    bool load(const QString &filePath = QString());
    bool save(const QString &filePath = QString()) const;
    bool resetToDefault();

    AppConfig& config();
    const AppConfig& config() const;

    QString configPath() const;
    QString lastError() const;

    static QString defaultConfigPath();

private:
    ConfigManager();
    ConfigManager(const ConfigManager&) = delete;
    ConfigManager& operator=(const ConfigManager&) = delete;

    bool ensureConfigDirectory(const QString &filePath) const;

private:
    AppConfig m_config;
    QString m_configPath;
    mutable QString m_lastError;
};

#endif // CONFIGMANAGER_H
