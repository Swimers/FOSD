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

struct StrainConfig
{
    QString aPara;
    QString bPara;
    QString strainLowPara;
    QString strainTopPara;

    static StrainConfig fromJson(const QJsonObject &object)
    {
        StrainConfig config;
        config.aPara = object.value("aPara").toString(config.aPara);
        config.bPara = object.value("bPara").toString(config.bPara);
        config.strainLowPara = object.value("strainLowPara").toString(config.strainLowPara);
        config.strainTopPara = object.value("strainTopPara").toString(config.strainTopPara);
        return config;
    }

    QJsonObject toJson() const
    {
        QJsonObject object;
        object.insert("aPara", aPara);
        object.insert("bPara", bPara);
        object.insert("strainLowPara", strainLowPara);
        object.insert("strainTopPara", strainTopPara);
        return object;
    }
};

struct CampConfig
{
    QString getHz;
    QString saveTime;

    static CampConfig fromJson(const QJsonObject &object)
    {
        CampConfig config;
        config.getHz = object.value("getHz").toString(config.getHz);
        config.saveTime = object.value("saveTime").toString(config.saveTime);
        return config;
    }

    QJsonObject toJson() const
    {
        QJsonObject object;
        object.insert("getHz", getHz);
        object.insert("saveTime", saveTime);
        return object;
    }
};

struct AppConfig
{
    int version = 1;
    ExampleConfig example;
    LoginConfig login;
    StrainConfig strain;
    CampConfig camp;

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
        config.strain = StrainConfig::fromJson(object.value("strain").toObject());
        config.camp = CampConfig::fromJson(object.value("camp").toObject());
        return config;
    }

    QJsonObject toJson() const
    {
        QJsonObject object;
        object.insert("version", version);
        object.insert("example", example.toJson());
        object.insert("login", login.toJson());
        object.insert("strain", strain.toJson());
        object.insert("camp", camp.toJson());
        return object;
    }
};

#endif // APPCONFIG_H
