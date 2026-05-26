#include "ConfigManager.h"

#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QJsonDocument>
#include <QJsonParseError>
#include <QSaveFile>

ConfigManager::ConfigManager()
    : m_config(AppConfig::defaultConfig()),
      m_configPath(defaultConfigPath())
{
}

ConfigManager& ConfigManager::instance()
{
    static ConfigManager manager;
    return manager;
}

bool ConfigManager::load(const QString &filePath)
{
    m_lastError.clear();
    m_configPath = filePath.isEmpty() ? defaultConfigPath() : filePath;
    m_config = AppConfig::defaultConfig();

    QFile file(m_configPath);
    if (!file.exists()) {
        return save(m_configPath);
    }

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        m_lastError = QString("Open config failed: %1").arg(file.errorString());
        return false;
    }

    const QByteArray data = file.readAll();
    QJsonParseError parseError;
    const QJsonDocument document = QJsonDocument::fromJson(data, &parseError);
    if (parseError.error != QJsonParseError::NoError || !document.isObject()) {
        m_lastError = QString("Parse config failed: %1").arg(parseError.errorString());
        return false;
    }

    m_config = AppConfig::fromJson(document.object());
    return true;
}

bool ConfigManager::save(const QString &filePath) const
{
    m_lastError.clear();
    const QString targetPath = filePath.isEmpty() ? m_configPath : filePath;

    if (!ensureConfigDirectory(targetPath)) {
        return false;
    }

    QSaveFile file(targetPath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        m_lastError = QString("Open config for save failed: %1").arg(file.errorString());
        return false;
    }

    const QJsonDocument document(m_config.toJson());
    file.write(document.toJson(QJsonDocument::Indented));

    if (!file.commit()) {
        m_lastError = QString("Commit config failed: %1").arg(file.errorString());
        return false;
    }

    return true;
}

bool ConfigManager::resetToDefault()
{
    m_config = AppConfig::defaultConfig();
    return save();
}

AppConfig& ConfigManager::config()
{
    return m_config;
}

const AppConfig& ConfigManager::config() const
{
    return m_config;
}

QString ConfigManager::configPath() const
{
    return m_configPath;
}

QString ConfigManager::lastError() const
{
    return m_lastError;
}

QString ConfigManager::defaultConfigPath()
{
    return QStringLiteral("data/config/app_config.json");
}

bool ConfigManager::ensureConfigDirectory(const QString &filePath) const
{
    const QFileInfo fileInfo(filePath);
    const QDir dir = fileInfo.dir();
    if (dir.exists()) {
        return true;
    }

    if (!QDir().mkpath(dir.absolutePath())) {
        m_lastError = QString("Create config directory failed: %1").arg(dir.absolutePath());
        return false;
    }

    return true;
}
