#pragma once

#include "CoreParameters.h"
#include "IConfigStore.h"
#include <QSet>
#include <cmath>
#include <limits>
#include <type_traits>

namespace fosd {
namespace config {

// Converts typed fields to INI values and tracks consumed keys during decoding.
class ParameterArchive {
public:
    ConfigSection data;
    QString error;
    bool reading = false;
    QSet<QString> used;

    explicit ParameterArchive(const ConfigSection &values = {}, bool read = false);

    // Reject overflow and preserve numeric precision across INI string conversion.
    template <class T>
    typename std::enable_if<std::is_integral<T>::value && !std::is_same<T, bool>::value, bool>::type
    convert(const QVariant &v, T &value) {
        bool ok = false;
        const QString text = v.toString();
        if (std::is_unsigned<T>::value) {
            const auto number = text.toULongLong(&ok);
            if (!ok || text.startsWith('-') || number > quint64(std::numeric_limits<T>::max()))
                return false;
            value = T(number);
        } else {
            const auto number = text.toLongLong(&ok);
            if (!ok || number < qint64(std::numeric_limits<T>::min()) ||
                number > qint64(std::numeric_limits<T>::max()))
                return false;
            value = T(number);
        }
        return true;
    }

    template <class T>
    typename std::enable_if<std::is_enum<T>::value, bool>::type convert(const QVariant &v,
                                                                        T &value) {
        int number = 0;
        if (!convert(v, number))
            return false;
        value = T(number);
        return true;
    }

    bool convert(const QVariant &v, bool &value);
    bool convert(const QVariant &v, double &value);
    bool convert(const QVariant &v, QString &value);
    bool convert(const QVariant &v, QHostAddress &value);
    template <class T> bool convert(const QVariant &v, std::vector<T> &values) {
        if (!v.canConvert<QStringList>())
            return false;
        values.clear();
        for (const auto &text : v.toStringList()) {
            T value{};
            if (!convert(text, value))
                return false;
            values.push_back(value);
        }
        return true;
    }

    template <class T> bool convert(const QVariant &v, QVector<T> &values) {
        std::vector<T> result;
        if (!convert(v, result))
            return false;
        values = QVector<T>::fromStdVector(result);
        return true;
    }

    template <class T, size_t N> bool convert(const QVariant &v, std::array<T, N> &values) {
        std::vector<T> result;
        if (!convert(v, result) || result.size() != N)
            return false;
        for (size_t i = 0; i < N; ++i)
            values[i] = result[i];
        return true;
    }

    template <class T> QVariant encode(const T &value) {
        return QVariant::fromValue(value);
    }

    QVariant encode(quint16 value);
    template <class T>
    typename std::enable_if<std::is_enum<T>::value, QVariant>::type enumEncode(const T &value) {
        return int(value);
    }

    QVariant encode(const QHostAddress &v);
    template <class T> QString text(const T &v) {
        return encode(v).toString();
    }

    QString text(double v);
    template <class T> QVariant encode(const std::vector<T> &values) {
        QStringList result;
        for (const auto &v : values)
            result.append(text(T(v)));
        return result;
    }

    template <class T> QVariant encode(const QVector<T> &values) {
        return encode(values.toStdVector());
    }

    template <class T, size_t N> QVariant encode(const std::array<T, N> &values) {
        return encode(std::vector<T>(values.begin(), values.end()));
    }

    template <class T> void field(const QString &key, T &value) {
        used.insert(key);
        if (reading) {
            if (!data.contains(key) || !convert(data.value(key), value))
                error = QStringLiteral("Invalid or missing parameter '%1'").arg(key);
        } else
            put(key, value);
    }

    template <class T>
    typename std::enable_if<!std::is_enum<T>::value>::type put(const QString &key, const T &value) {
        data[key] = encode(value);
    }

    template <class T>
    typename std::enable_if<std::is_enum<T>::value>::type put(const QString &key, const T &value) {
        data[key] = int(value);
    }
};

// Each supported parameter type declares its INI group and field mapping.
template <class T> struct ParameterCodec;
template <class T> bool validateParameter(const T &value, bool ready, QString *error);
// Template implementations stay visible to callers for type instantiation.
template <class T> ConfigSection parameterSection(const T &value) {
    T copy = value;
    ParameterArchive archive;
    ParameterCodec<T>::fields(archive, copy);
    return archive.data;
}

template <class T>
bool decodeParameter(const ConfigSection &section, T *value, bool ready, QString *error) {
    if (error)
        error->clear();
    T parsed;
    ParameterArchive archive(section, true);
    ParameterCodec<T>::fields(archive, parsed);
    if (archive.used.size() != section.size() && archive.error.isEmpty())
        archive.error = QStringLiteral("Unknown parameter key");
    if (!archive.error.isEmpty()) {
        if (error)
            *error = archive.error;
        return false;
    }
    if (!validateParameter(parsed, ready, error))
        return false;
    *value = parsed;
    return true;
}

// ready=true also requires the fields needed to start the corresponding operation.
bool validateCoreSection(const QString &group, const ConfigSection &values, bool ready,
                         QString *error);
template <> struct ParameterCodec<communication::SerialConfig> {
    static QString section();
    static void fields(ParameterArchive &archive, communication::SerialConfig &value);
};

template <>
bool validateParameter(const communication::SerialConfig &v, bool ready, QString *error);

template <> struct ParameterCodec<communication::UdpConfig> {
    static QString section();
    static void fields(ParameterArchive &archive, communication::UdpConfig &value);
};

template <> bool validateParameter(const communication::UdpConfig &v, bool ready, QString *error);

template <> struct ParameterCodec<communication::TcpConfig> {
    static QString section();
    static void fields(ParameterArchive &archive, communication::TcpConfig &value);
};

template <> bool validateParameter(const communication::TcpConfig &v, bool ready, QString *error);

template <> struct ParameterCodec<Calculation::CalculationConfiguration> {
    static QString section();
    static void fields(ParameterArchive &archive, Calculation::CalculationConfiguration &value);
};

template <>
bool validateParameter(const Calculation::CalculationConfiguration &v, bool ready, QString *error);

template <> struct ParameterCodec<storage::StorageConfiguration> {
    static QString section();
    static void fields(ParameterArchive &archive, storage::StorageConfiguration &value);
};

template <>
bool validateParameter(const storage::StorageConfiguration &v, bool ready, QString *error);

template <> struct ParameterCodec<visualization::ScalarColorSettings> {
    static QString section();
    static void fields(ParameterArchive &archive, visualization::ScalarColorSettings &value);
};

template <>
bool validateParameter(const visualization::ScalarColorSettings &v, bool ready, QString *error);

template <> struct ParameterCodec<visualization::AxesSettings> {
    static QString section();
    static void fields(ParameterArchive &archive, visualization::AxesSettings &value);
};

template <>
bool validateParameter(const visualization::AxesSettings &v, bool ready, QString *error);

template <> struct ParameterCodec<visualization::GridSettings> {
    static QString section();
    static void fields(ParameterArchive &archive, visualization::GridSettings &value);
};

template <>
bool validateParameter(const visualization::GridSettings &v, bool ready, QString *error);

template <> struct ParameterCodec<visualization::BackgroundSettings> {
    static QString section();
    static void fields(ParameterArchive &archive, visualization::BackgroundSettings &value);
};

template <>
bool validateParameter(const visualization::BackgroundSettings &v, bool ready, QString *error);

template <> struct ParameterCodec<visualization::CameraSettings> {
    static QString section();
    static void fields(ParameterArchive &archive, visualization::CameraSettings &value);
};

template <>
bool validateParameter(const visualization::CameraSettings &v, bool ready, QString *error);

template <> struct ParameterCodec<visualization::WorldSettings> {
    static QString section();
    static void fields(ParameterArchive &archive, visualization::WorldSettings &value);
};

template <>
bool validateParameter(const visualization::WorldSettings &v, bool ready, QString *error);

template <> struct ParameterCodec<visualization::KeyPointStyle> {
    static QString section();
    static void fields(ParameterArchive &archive, visualization::KeyPointStyle &value);
};

template <>
bool validateParameter(const visualization::KeyPointStyle &v, bool ready, QString *error);

template <> struct ParameterCodec<visualization::PickingSettings> {
    static QString section();
    static void fields(ParameterArchive &archive, visualization::PickingSettings &value);
};

template <>
bool validateParameter(const visualization::PickingSettings &v, bool ready, QString *error);

template <> struct ParameterCodec<visualization::VisualizationRuntimeSettings> {
    static QString section();
    static void fields(ParameterArchive &archive,
                       visualization::VisualizationRuntimeSettings &value);
};

template <>
bool validateParameter(const visualization::VisualizationRuntimeSettings &v, bool, QString *error);

template <> struct ParameterCodec<visualization::SelectionStyleSettings> {
    static QString section();
    static void fields(ParameterArchive &archive, visualization::SelectionStyleSettings &value);
};

template <>
bool validateParameter(const visualization::SelectionStyleSettings &v, bool ready, QString *error);

// Build defaults from the parameter structures; validate the complete shipped template.
ConfigData defaultCoreParameters();
bool validateCoreDefaults(const ConfigData &data, QString *error);
} // namespace config
} // namespace fosd
