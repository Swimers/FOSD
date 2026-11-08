#include "ParameterCodec.h"
#include <QSet>
#include <cmath>

namespace fosd {
namespace config {

bool validateCoreSection(const QString &group, const ConfigSection &values, bool ready,
                         QString *error) {
    if (error)
        error->clear();
    bool ok = true;
    // Values/types/finite numbers and fixed array lengths are checked by codec.
    if (group == "communication.serial") {
        ok = values["baudRate"].toInt() > 0 && values["baudRate"].toInt() <= 4000000 &&
             values["dataBits"].toInt() >= 5 && values["dataBits"].toInt() <= 8 &&
             QSet<int>{0, 2, 3, 4, 5}.contains(values["parity"].toInt()) &&
             QSet<int>{1, 2, 3}.contains(values["stopBits"].toInt()) &&
             QSet<int>{0, 1, 2}.contains(values["flowControl"].toInt()) &&
             (!ready || !values["portName"].toString().trimmed().isEmpty());
    } else if (group == "communication.udp") {
        ok = !values["bindAddress"].toString().isEmpty() &&
             (values["defaultPeer.address"].toString().isEmpty() ==
              (values["defaultPeer.port"].toInt() == 0));
    } else if (group == "communication.tcp") {
        ok = QSet<int>{0, 1}.contains(values["role"].toInt()) &&
             !values["listenAddress"].toString().isEmpty() &&
             (values["role"].toInt() != 0 || !values["host"].toString().trimmed().isEmpty()) &&
             (!ready || values["port"].toInt() > 0);
    } else if (group == "calculation") {
        const auto baseline = values["strainCalibration.originalWavelength"].toStringList();
        const auto indices = values["channelCalibration.channelIndices"].toStringList();
        const auto factors = values["channelCalibration.calibrationFactors"].toStringList();
        const auto offsets = values["channelCalibration.calibrationOffsets"].toStringList();
        ok = (values["algorithm"].toInt() == 0) &&
             values["strainCalibration.calibrationFactor"].toDouble() > 0 &&
             indices.size() == factors.size() &&
             (offsets.isEmpty() || offsets.size() == indices.size());
        for (const auto &s : baseline)
            ok = ok && s.toDouble() > 0;
        for (int i = 0; i < indices.size(); ++i)
            ok = ok && indices[i].toInt() >= 0 && indices[i].toInt() < baseline.size() &&
                 factors[i].toDouble() > 0;
        for (const auto &offset : offsets)
            ok = ok && std::isfinite(offset.toDouble());
        if (ready)
            ok = ok && !baseline.isEmpty() && !indices.isEmpty() &&
                 values["strainCalibration.A"].toDouble() != 0;
    } else if (group == "storage") {
        const int flags = values["dataTypes"].toInt();
        ok = QSet<int>{0, 1}.contains(values["format"].toInt()) && flags > 0 && !(flags & ~7) &&
             (!ready || !values["path"].toString().trimmed().isEmpty());
        const auto ids = values["pointIds"].toStringList();
        QSet<qint64> seen;
        for (const auto &s : ids) {
            const auto id = s.toLongLong();
            ok = ok && id >= 0 && !seen.contains(id);
            seen.insert(id);
        }
        if (flags & 4)
            ok = ok && (values["allPoints"].toBool() ? ids.isEmpty() : !ids.isEmpty());
    } else {
        for (auto it = values.constBegin(); it != values.constEnd(); ++it) {
            if (it.key().contains("Color", Qt::CaseInsensitive) || it.key() == "color")
                for (const auto &x : it.value().toStringList())
                    ok = ok && x.toDouble() >= 0 && x.toDouble() <= 1;
        }
        if (group == "visualization.scalar")
            ok = ok && values["minimum"].toDouble() < values["maximum"].toDouble() &&
                 values["field"].toInt() >= 0 && values["field"].toInt() <= 4 &&
                 QSet<int>{0, 1}.contains(values["mappingMode"].toInt()) &&
                 QSet<int>{0, 1}.contains(values["valueMode"].toInt()) &&
                 QSet<int>{0, 1}.contains(values["rangeMode"].toInt()) &&
                 values["hueStart"].toDouble() >= 0 && values["hueStart"].toDouble() <= 1 &&
                 values["hueEnd"].toDouble() >= 0 && values["hueEnd"].toDouble() <= 1;
        if (values.contains("fontSize"))
            ok = ok && values["fontSize"].toInt() >= 1 && values["fontSize"].toInt() <= 256;
        if (values.contains("lengthMm"))
            ok = ok && values["lengthMm"].toDouble() > 0;
        if (group == "visualization.grid")
            ok = ok && values["halfExtentMm"].toDouble() > 0 && values["divisions"].toInt() >= 1 &&
                 values["divisions"].toInt() <= 500 && values["opacity"].toDouble() >= 0 &&
                 values["opacity"].toDouble() <= 1 && values["lineWidth"].toDouble() > 0 &&
                 values["lineWidth"].toDouble() <= 20;
        if (group == "visualization.camera")
            ok = ok && QSet<int>{0, 1}.contains(values["projection"].toInt()) &&
                 values["viewAngleDegrees"].toDouble() >= 1 &&
                 values["viewAngleDegrees"].toDouble() <= 170 &&
                 values["parallelScaleMm"].toDouble() > 0;
        if (group == "visualization.world")
            ok = ok && values["groundHalfExtentMm"].toDouble() > 0 &&
                 values["skyRadiusMm"].toDouble() > 0;
        if (group == "visualization.keyPoint")
            ok = ok && values["radiusMm"].toDouble() > 0 &&
                 values["sphereResolution"].toInt() >= 4 &&
                 values["sphereResolution"].toInt() <= 128;
        if (group == "visualization.picking")
            ok = ok && values["tolerance"].toDouble() > 0 && values["tolerance"].toDouble() <= 1 &&
                 values["clickTolerancePixels"].toInt() >= 0 &&
                 values["clickTolerancePixels"].toInt() <= 100;
    }
    if (!ok && error)
        *error = QStringLiteral("Invalid core parameters in [%1]%2")
                     .arg(group, ready ? " for operation" : "");
    return ok;
}

// Typed entry points share the section rules above.
template <>
bool validateParameter(const communication::SerialConfig &v, bool ready, QString *error) {
    return validateCoreSection(ParameterCodec<communication::SerialConfig>::section(),
                               parameterSection(v), ready, error);
}

template <> bool validateParameter(const communication::UdpConfig &v, bool ready, QString *error) {
    return validateCoreSection(ParameterCodec<communication::UdpConfig>::section(),
                               parameterSection(v), ready, error);
}

template <> bool validateParameter(const communication::TcpConfig &v, bool ready, QString *error) {
    return validateCoreSection(ParameterCodec<communication::TcpConfig>::section(),
                               parameterSection(v), ready, error);
}

template <>
bool validateParameter(const Calculation::CalculationConfiguration &v, bool ready, QString *error) {
    return validateCoreSection(ParameterCodec<Calculation::CalculationConfiguration>::section(),
                               parameterSection(v), ready, error);
}

template <>
bool validateParameter(const storage::StorageConfiguration &v, bool ready, QString *error) {
    return validateCoreSection(ParameterCodec<storage::StorageConfiguration>::section(),
                               parameterSection(v), ready, error);
}

template <>
bool validateParameter(const visualization::ScalarColorSettings &v, bool ready, QString *error) {
    return validateCoreSection(ParameterCodec<visualization::ScalarColorSettings>::section(),
                               parameterSection(v), ready, error);
}

template <>
bool validateParameter(const visualization::AxesSettings &v, bool ready, QString *error) {
    return validateCoreSection(ParameterCodec<visualization::AxesSettings>::section(),
                               parameterSection(v), ready, error);
}

template <>
bool validateParameter(const visualization::GridSettings &v, bool ready, QString *error) {
    return validateCoreSection(ParameterCodec<visualization::GridSettings>::section(),
                               parameterSection(v), ready, error);
}

template <>
bool validateParameter(const visualization::BackgroundSettings &v, bool ready, QString *error) {
    return validateCoreSection(ParameterCodec<visualization::BackgroundSettings>::section(),
                               parameterSection(v), ready, error);
}

template <>
bool validateParameter(const visualization::CameraSettings &v, bool ready, QString *error) {
    return validateCoreSection(ParameterCodec<visualization::CameraSettings>::section(),
                               parameterSection(v), ready, error);
}

template <>
bool validateParameter(const visualization::WorldSettings &v, bool ready, QString *error) {
    return validateCoreSection(ParameterCodec<visualization::WorldSettings>::section(),
                               parameterSection(v), ready, error);
}

template <>
bool validateParameter(const visualization::KeyPointStyle &v, bool ready, QString *error) {
    return validateCoreSection(ParameterCodec<visualization::KeyPointStyle>::section(),
                               parameterSection(v), ready, error);
}

template <>
bool validateParameter(const visualization::PickingSettings &v, bool ready, QString *error) {
    return validateCoreSection(ParameterCodec<visualization::PickingSettings>::section(),
                               parameterSection(v), ready, error);
}

template <>
bool validateParameter(const visualization::VisualizationRuntimeSettings &v, bool, QString *error) {
    if (std::isfinite(v.deformationScale) && v.refreshIntervalMs > 0 &&
        v.refreshIntervalMs <= 10000)
        return true;
    if (error)
        *error = "Invalid visualization scale or refresh interval";
    return false;
}

template <>
bool validateParameter(const visualization::SelectionStyleSettings &v, bool ready, QString *error) {
    return validateParameter(static_cast<const visualization::KeyPointStyle &>(v), ready, error);
}

} // namespace config
} // namespace fosd
