#pragma once
#include "CoreParameters.h"
#include "IConfigStore.h"
#include <QSet>
#include <cmath>
#include <limits>
#include <type_traits>

namespace fosd { namespace config {
class ParameterArchive {
public:
    ConfigSection data;
    QString error;
    bool reading = false;
    QSet<QString> used;
    explicit ParameterArchive(const ConfigSection &values = {}, bool read = false) : data(values), reading(read) {}
    template<class T> typename std::enable_if<std::is_integral<T>::value && !std::is_same<T, bool>::value, bool>::type
    convert(const QVariant &v, T &value) {
        bool ok = false;
        const QString text = v.toString();
        if (std::is_unsigned<T>::value) {
            const auto number = text.toULongLong(&ok);
            if (!ok || text.startsWith('-') || number > quint64(std::numeric_limits<T>::max())) return false;
            value = T(number);
        } else {
            const auto number = text.toLongLong(&ok);
            if (!ok || number < qint64(std::numeric_limits<T>::min()) || number > qint64(std::numeric_limits<T>::max())) return false;
            value = T(number);
        }
        return true;
    }
    template<class T> typename std::enable_if<std::is_enum<T>::value, bool>::type convert(const QVariant &v, T &value) {
        int number = 0; if (!convert(v, number)) return false; value = T(number); return true;
    }
    bool convert(const QVariant &v, bool &value) {
        const auto text = v.toString();
        if (text == "true" || text == "1") { value = true; return true; }
        if (text == "false" || text == "0") { value = false; return true; }
        return false;
    }
    bool convert(const QVariant &v, double &value) { bool ok; value = v.toString().toDouble(&ok); return ok && std::isfinite(value); }
    bool convert(const QVariant &v, QString &value) { if (v.type() != QVariant::String) return false; value = v.toString(); return true; }
    bool convert(const QVariant &v, QHostAddress &value) { const auto text = v.toString(); value = QHostAddress(text); return text.isEmpty() || !value.isNull(); }
    template<class T> bool convert(const QVariant &v, std::vector<T> &values) {
        if (!v.canConvert<QStringList>()) return false;
        values.clear();
        for (const auto &text : v.toStringList()) { T value{}; if (!convert(text, value)) return false; values.push_back(value); }
        return true;
    }
    template<class T> bool convert(const QVariant &v, QVector<T> &values) {
        std::vector<T> result; if (!convert(v, result)) return false;
        values = QVector<T>::fromStdVector(result); return true;
    }
    template<class T, size_t N> bool convert(const QVariant &v, std::array<T, N> &values) {
        std::vector<T> result; if (!convert(v, result) || result.size() != N) return false;
        for (size_t i=0; i<N; ++i) values[i] = result[i]; return true;
    }
    template<class T> QVariant encode(const T &value) { return QVariant::fromValue(value); }
    QVariant encode(quint16 value) { return uint(value); }
    template<class T> typename std::enable_if<std::is_enum<T>::value, QVariant>::type enumEncode(const T &value) { return int(value); }
    QVariant encode(const QHostAddress &v) { return v.isNull() ? QString("") : v.toString(); }
    template<class T> QString text(const T &v) { return encode(v).toString(); }
    QString text(double v) { return QString::number(v, 'g', 17); }
    template<class T> QVariant encode(const std::vector<T> &values) { QStringList result; for (const auto &v : values) result.append(text(T(v))); return result; }
    template<class T> QVariant encode(const QVector<T> &values) { return encode(values.toStdVector()); }
    template<class T, size_t N> QVariant encode(const std::array<T, N> &values) { return encode(std::vector<T>(values.begin(), values.end())); }
    template<class T> void field(const QString &key, T &value) {
        used.insert(key);
        if (reading) {
            if (!data.contains(key) || !convert(data.value(key), value)) error = QStringLiteral("Invalid or missing parameter '%1'").arg(key);
        } else put(key, value);
    }
    template<class T> typename std::enable_if<!std::is_enum<T>::value>::type put(const QString &key, const T &value) { data[key] = encode(value); }
    template<class T> typename std::enable_if<std::is_enum<T>::value>::type put(const QString &key, const T &value) { data[key] = int(value); }
};
template<class T> struct ParameterCodec;
template<class T> bool validateParameter(const T &value, bool ready, QString *error);
template<class T> ConfigSection parameterSection(const T &value) { T copy = value; ParameterArchive archive; ParameterCodec<T>::fields(archive, copy); return archive.data; }
template<class T> bool decodeParameter(const ConfigSection &section, T *value, bool ready, QString *error) {
    if (error) error->clear();
    T parsed;
    ParameterArchive archive(section, true); ParameterCodec<T>::fields(archive, parsed);
    if (archive.used.size() != section.size() && archive.error.isEmpty()) archive.error = QStringLiteral("Unknown parameter key");
    if (!archive.error.isEmpty()) { if (error) *error = archive.error; return false; }
    if (!validateParameter(parsed, ready, error)) return false;
    *value = parsed; return true;
}

} }

namespace fosd { namespace config {
bool validateCoreSection(const QString &, const ConfigSection &, bool, QString *);
template<> struct ParameterCodec<communication::SerialConfig> {
 static QString section() { return "communication.serial"; }
 static void fields(ParameterArchive &a, communication::SerialConfig &v) {
 a.field("portName", v.portName);
 a.field("baudRate", v.baudRate);
 a.field("dataBits", v.dataBits);
 a.field("parity", v.parity);
 a.field("stopBits", v.stopBits);
 a.field("flowControl", v.flowControl);
 }
};
template<> inline bool validateParameter(const communication::SerialConfig &v, bool ready, QString *error) { return validateCoreSection(ParameterCodec<communication::SerialConfig>::section(), parameterSection(v), ready, error); }
template<> struct ParameterCodec<communication::UdpConfig> {
 static QString section() { return "communication.udp"; }
 static void fields(ParameterArchive &a, communication::UdpConfig &v) {
 a.field("bindAddress", v.bindAddress);
 a.field("localPort", v.localPort);
 a.field("defaultPeer.address", v.defaultPeer.address);
 a.field("defaultPeer.port", v.defaultPeer.port);
 }
};
template<> inline bool validateParameter(const communication::UdpConfig &v, bool ready, QString *error) { return validateCoreSection(ParameterCodec<communication::UdpConfig>::section(), parameterSection(v), ready, error); }
template<> struct ParameterCodec<communication::TcpConfig> {
 static QString section() { return "communication.tcp"; }
 static void fields(ParameterArchive &a, communication::TcpConfig &v) {
 a.field("role", v.role);
 a.field("host", v.host);
 a.field("listenAddress", v.listenAddress);
 a.field("port", v.port);
 }
};
template<> inline bool validateParameter(const communication::TcpConfig &v, bool ready, QString *error) { return validateCoreSection(ParameterCodec<communication::TcpConfig>::section(), parameterSection(v), ready, error); }
template<> struct ParameterCodec<Calculation::CalculationConfiguration> {
 static QString section() { return "calculation"; }
 static void fields(ParameterArchive &a, Calculation::CalculationConfiguration &v) {
 a.field("algorithm", v.algorithm);
 a.field("strainCalibration.originalWavelength", v.strainCalibration.originalWavelength);
 a.field("strainCalibration.calibrationFactor", v.strainCalibration.calibrationFactor);
 a.field("strainCalibration.A", v.strainCalibration.A);
 a.field("strainCalibration.B", v.strainCalibration.B);
 a.field("channelCalibration.channelIndices", v.channelCalibration.channelIndices);
 a.field("channelCalibration.calibrationFactors", v.channelCalibration.calibrationFactors);
 }
};
template<> inline bool validateParameter(const Calculation::CalculationConfiguration &v, bool ready, QString *error) { return validateCoreSection(ParameterCodec<Calculation::CalculationConfiguration>::section(), parameterSection(v), ready, error); }
template<> struct ParameterCodec<storage::StorageConfiguration> {
 static QString section() { return "storage"; }
 static void fields(ParameterArchive &a, storage::StorageConfiguration &v) {
 a.field("format", v.format);
 a.field("path", v.path);
 int flags = int(v.dataTypes); a.field("dataTypes", flags); v.dataTypes = storage::DataTypes(QFlag(flags));
 a.field("pointIds", v.pointIds);
 a.field("allPoints", v.allPoints);
 }
};
template<> inline bool validateParameter(const storage::StorageConfiguration &v, bool ready, QString *error) { return validateCoreSection(ParameterCodec<storage::StorageConfiguration>::section(), parameterSection(v), ready, error); }
template<> struct ParameterCodec<visualization::ScalarColorSettings> {
 static QString section() { return "visualization.scalar"; }
 static void fields(ParameterArchive &a, visualization::ScalarColorSettings &v) {
 a.field("enabled", v.enabled);
 a.field("field", v.field);
 a.field("mappingMode", v.mappingMode);
 a.field("valueMode", v.valueMode);
 a.field("rangeMode", v.rangeMode);
 a.field("minimum", v.minimum);
 a.field("maximum", v.maximum);
 a.field("hueStart", v.hueStart);
 a.field("hueEnd", v.hueEnd);
 }
};
template<> inline bool validateParameter(const visualization::ScalarColorSettings &v, bool ready, QString *error) { return validateCoreSection(ParameterCodec<visualization::ScalarColorSettings>::section(), parameterSection(v), ready, error); }
template<> struct ParameterCodec<visualization::AxesSettings> {
 static QString section() { return "visualization.axes"; }
 static void fields(ParameterArchive &a, visualization::AxesSettings &v) {
 a.field("visible", v.visible);
 a.field("labelsVisible", v.labelsVisible);
 a.field("lengthMm", v.lengthMm);
 a.field("originMm", v.originMm);
 a.field("fontSize", v.fontSize);
 }
};
template<> inline bool validateParameter(const visualization::AxesSettings &v, bool ready, QString *error) { return validateCoreSection(ParameterCodec<visualization::AxesSettings>::section(), parameterSection(v), ready, error); }
template<> struct ParameterCodec<visualization::GridSettings> {
 static QString section() { return "visualization.grid"; }
 static void fields(ParameterArchive &a, visualization::GridSettings &v) {
 a.field("visible", v.visible);
 a.field("halfExtentMm", v.halfExtentMm);
 a.field("divisions", v.divisions);
 a.field("originMm", v.originMm);
 a.field("color", v.color);
 a.field("opacity", v.opacity);
 a.field("lineWidth", v.lineWidth);
 }
};
template<> inline bool validateParameter(const visualization::GridSettings &v, bool ready, QString *error) { return validateCoreSection(ParameterCodec<visualization::GridSettings>::section(), parameterSection(v), ready, error); }
template<> struct ParameterCodec<visualization::BackgroundSettings> {
 static QString section() { return "visualization.background"; }
 static void fields(ParameterArchive &a, visualization::BackgroundSettings &v) {
 a.field("gradient", v.gradient);
 a.field("bottomColor", v.bottomColor);
 a.field("topColor", v.topColor);
 }
};
template<> inline bool validateParameter(const visualization::BackgroundSettings &v, bool ready, QString *error) { return validateCoreSection(ParameterCodec<visualization::BackgroundSettings>::section(), parameterSection(v), ready, error); }
template<> struct ParameterCodec<visualization::CameraSettings> {
 static QString section() { return "visualization.camera"; }
 static void fields(ParameterArchive &a, visualization::CameraSettings &v) {
 a.field("projection", v.projection);
 a.field("viewAngleDegrees", v.viewAngleDegrees);
 a.field("parallelScaleMm", v.parallelScaleMm);
 a.field("locked", v.locked);
 }
};
template<> inline bool validateParameter(const visualization::CameraSettings &v, bool ready, QString *error) { return validateCoreSection(ParameterCodec<visualization::CameraSettings>::section(), parameterSection(v), ready, error); }
template<> struct ParameterCodec<visualization::WorldSettings> {
 static QString section() { return "visualization.world"; }
 static void fields(ParameterArchive &a, visualization::WorldSettings &v) {
 a.field("groundVisible", v.groundVisible);
 a.field("skyVisible", v.skyVisible);
 a.field("groundHalfExtentMm", v.groundHalfExtentMm);
 a.field("groundYmm", v.groundYmm);
 a.field("skyRadiusMm", v.skyRadiusMm);
 a.field("centerMm", v.centerMm);
 a.field("groundColor", v.groundColor);
 a.field("skyColor", v.skyColor);
 a.field("groundTexturePath", v.groundTexturePath);
 a.field("skyTexturePath", v.skyTexturePath);
 }
};
template<> inline bool validateParameter(const visualization::WorldSettings &v, bool ready, QString *error) { return validateCoreSection(ParameterCodec<visualization::WorldSettings>::section(), parameterSection(v), ready, error); }
template<> struct ParameterCodec<visualization::KeyPointStyle> {
 static QString section() { return "visualization.keyPoint"; }
 static void fields(ParameterArchive &a, visualization::KeyPointStyle &v) {
 a.field("radiusMm", v.radiusMm);
 a.field("color", v.color);
 a.field("labelColor", v.labelColor);
 a.field("labelOffsetMm", v.labelOffsetMm);
 a.field("fontSize", v.fontSize);
 a.field("sphereResolution", v.sphereResolution);
 a.field("labelsVisible", v.labelsVisible);
 }
};
template<> inline bool validateParameter(const visualization::KeyPointStyle &v, bool ready, QString *error) { return validateCoreSection(ParameterCodec<visualization::KeyPointStyle>::section(), parameterSection(v), ready, error); }
template<> struct ParameterCodec<visualization::PickingSettings> {
 static QString section() { return "visualization.picking"; }
 static void fields(ParameterArchive &a, visualization::PickingSettings &v) {
 a.field("enabled", v.enabled);
 a.field("onlyKeyPoints", v.onlyKeyPoints);
 a.field("clearOnCameraInteraction", v.clearOnCameraInteraction);
 a.field("tolerance", v.tolerance);
 a.field("clickTolerancePixels", v.clickTolerancePixels);
 }
};
template<> inline bool validateParameter(const visualization::PickingSettings &v, bool ready, QString *error) { return validateCoreSection(ParameterCodec<visualization::PickingSettings>::section(), parameterSection(v), ready, error); }
ConfigData defaultCoreParameters();
template<> struct ParameterCodec<visualization::VisualizationRuntimeSettings> {
    static QString section() { return "visualization.runtime"; }
    static void fields(ParameterArchive &a, visualization::VisualizationRuntimeSettings &v) { a.field("deformationScale", v.deformationScale); a.field("refreshIntervalMs", v.refreshIntervalMs); }
};
template<> inline bool validateParameter(const visualization::VisualizationRuntimeSettings &v, bool, QString *error) {
    if (std::isfinite(v.deformationScale) && v.refreshIntervalMs > 0 && v.refreshIntervalMs <= 10000) return true;
    if (error) *error = "Invalid visualization scale or refresh interval";
    return false;
}
template<> struct ParameterCodec<visualization::SelectionStyleSettings> {
    static QString section() { return "visualization.selection"; }
    static void fields(ParameterArchive &a, visualization::SelectionStyleSettings &v) { ParameterCodec<visualization::KeyPointStyle>::fields(a, v); }
};
template<> inline bool validateParameter(const visualization::SelectionStyleSettings &v, bool ready, QString *error) {
    return validateParameter(static_cast<const visualization::KeyPointStyle &>(v), ready, error);
}
bool validateCoreDefaults(const ConfigData &data, QString *error);
} }
