#ifndef APPCONFIG_H
#define APPCONFIG_H

#include <QJsonObject>
#include <QString>

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

struct AppConfig
{
    int version = 1;
    ExampleConfig example;

    static AppConfig defaultConfig()
    {
        return AppConfig();
    }

    static AppConfig fromJson(const QJsonObject &object)
    {
        AppConfig config = defaultConfig();
        config.version = object.value("version").toInt(config.version);
        config.example = ExampleConfig::fromJson(object.value("example").toObject());
        return config;
    }

    QJsonObject toJson() const
    {
        QJsonObject object;
        object.insert("version", version);
        object.insert("example", example.toJson());
        return object;
    }
};

#endif // APPCONFIG_H
