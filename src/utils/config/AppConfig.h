#ifndef APPCONFIG_H
#define APPCONFIG_H

#include <QJsonObject>
#include <QJsonArray>
#include <QString>
#include <QStringList>

inline QStringList stringListFromJsonArray(const QJsonArray &array)
{
    QStringList list;
    for (const QJsonValue &value : array) {
        list.append(value.toString());
    }
    return list;
}

inline QJsonArray stringListToJsonArray(const QStringList &list)
{
    QJsonArray array;
    for (const QString &value : list) {
        array.append(value);
    }
    return array;
}

struct ExampleConfig
{
    bool enabled = true;
    int sampleRate = 100;
    double gain = 1.0;
    QString name = "default";

    static ExampleConfig fromJson(const QJsonObject &object)
    {
        ExampleConfig config;
        config.enabled = object.value("enabled").toBool(config.enabled);
        config.sampleRate = object.value("sampleRate").toInt(config.sampleRate);
        config.gain = object.value("gain").toDouble(config.gain);
        config.name = object.value("name").toString(config.name);
        return config;
    }

    QJsonObject toJson() const
    {
        QJsonObject object;
        object.insert("enabled", enabled);
        object.insert("sampleRate", sampleRate);
        object.insert("gain", gain);
        object.insert("name", name);
        return object;
    }
};

struct LoginConfig
{
    QStringList usernames;
    QStringList passwords;
    QStringList privileges;
    QStringList remembered;
    bool saveName = false;
    bool savePassword = false;
    bool legacyRememberFlagsMigrated = false;

    static LoginConfig fromJson(const QJsonObject &object)
    {
        LoginConfig config;
        config.usernames = stringListFromJsonArray(object.value("usernames").toArray());
        config.passwords = stringListFromJsonArray(object.value("passwords").toArray());
        config.privileges = stringListFromJsonArray(object.value("privileges").toArray());
        config.remembered = stringListFromJsonArray(object.value("remembered").toArray());
        config.saveName = object.value("saveName").toBool(config.saveName);
        config.savePassword = object.value("savePassword").toBool(config.savePassword);
        config.legacyRememberFlagsMigrated = object.value("legacyRememberFlagsMigrated").toBool(config.legacyRememberFlagsMigrated);
        return config;
    }

    QJsonObject toJson() const
    {
        QJsonObject object;
        object.insert("usernames", stringListToJsonArray(usernames));
        object.insert("passwords", stringListToJsonArray(passwords));
        object.insert("privileges", stringListToJsonArray(privileges));
        object.insert("remembered", stringListToJsonArray(remembered));
        object.insert("saveName", saveName);
        object.insert("savePassword", savePassword);
        object.insert("legacyRememberFlagsMigrated", legacyRememberFlagsMigrated);
        return object;
    }
};

struct

struct AppConfig
{
    int version = 1;
    ExampleConfig example;
    LoginConfig login;

    static AppConfig defaultConfig()
    {
        return AppConfig();
    }

    static AppConfig fromJson(const QJsonObject &object)
    {
        AppConfig config = defaultConfig();
        config.version = object.value("version").toInt(config.version);
        config.example = ExampleConfig::fromJson(object.value("example").toObject());
        config.login = LoginConfig::fromJson(object.value("login").toObject());
        return config;
    }

    QJsonObject toJson() const
    {
        QJsonObject object;
        object.insert("version", version);
        object.insert("example", example.toJson());
        object.insert("login", login.toJson());
        return object;
    }
};

#endif // APPCONFIG_H
