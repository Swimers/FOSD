#include "ParameterCodec.h"

namespace fosd {
namespace config {

ParameterArchive::ParameterArchive(const ConfigSection &values, bool read)
    : data(values), reading(read) {
}

bool ParameterArchive::convert(const QVariant &v, QHostAddress &value) {
    const auto text = v.toString();
    value = QHostAddress(text);
    return text.isEmpty() || !value.isNull();
}

bool ParameterArchive::convert(const QVariant &v, QString &value) {
    if (v.type() != QVariant::String)
        return false;
    value = v.toString();
    return true;
}

bool ParameterArchive::convert(const QVariant &v, double &value) {
    bool ok;
    value = v.toString().toDouble(&ok);
    return ok && std::isfinite(value);
}

bool ParameterArchive::convert(const QVariant &v, bool &value) {
    const auto text = v.toString();
    if (text == "true" || text == "1") {
        value = true;
        return true;
    }
    if (text == "false" || text == "0") {
        value = false;
        return true;
    }
    return false;
}

QVariant ParameterArchive::encode(quint16 value) {
    return uint(value);
}

QVariant ParameterArchive::encode(const QHostAddress &v) {
    return v.isNull() ? QString("") : v.toString();
}

QString ParameterArchive::text(double v) {
    return QString::number(v, 'g', 17);
}

QString ParameterCodec<communication::SerialConfig>::section() {
    return "communication.serial";
}

void ParameterCodec<communication::SerialConfig>::fields(ParameterArchive &archive,
                                                         communication::SerialConfig &value) {
    archive.field("portName", value.portName);
    archive.field("baudRate", value.baudRate);
    archive.field("dataBits", value.dataBits);
    archive.field("parity", value.parity);
    archive.field("stopBits", value.stopBits);
    archive.field("flowControl", value.flowControl);
}

QString ParameterCodec<communication::UdpConfig>::section() {
    return "communication.udp";
}

void ParameterCodec<communication::UdpConfig>::fields(ParameterArchive &archive,
                                                      communication::UdpConfig &value) {
    archive.field("bindAddress", value.bindAddress);
    archive.field("localPort", value.localPort);
    archive.field("defaultPeer.address", value.defaultPeer.address);
    archive.field("defaultPeer.port", value.defaultPeer.port);
}

QString ParameterCodec<communication::TcpConfig>::section() {
    return "communication.tcp";
}

void ParameterCodec<communication::TcpConfig>::fields(ParameterArchive &archive,
                                                      communication::TcpConfig &value) {
    archive.field("role", value.role);
    archive.field("host", value.host);
    archive.field("listenAddress", value.listenAddress);
    archive.field("port", value.port);
}

QString ParameterCodec<Calculation::CalculationConfiguration>::section() {
    return "calculation";
}

void ParameterCodec<Calculation::CalculationConfiguration>::fields(
    ParameterArchive &archive, Calculation::CalculationConfiguration &value) {
    archive.field("algorithm", value.algorithm);
    archive.field("strainCalibration.originalWavelength",
                  value.strainCalibration.originalWavelength);
    archive.field("strainCalibration.calibrationFactor", value.strainCalibration.calibrationFactor);
    archive.field("strainCalibration.A", value.strainCalibration.A);
    archive.field("strainCalibration.B", value.strainCalibration.B);
    archive.field("channelCalibration.channelIndices", value.channelCalibration.channelIndices);
    archive.field("channelCalibration.calibrationFactors",
                  value.channelCalibration.calibrationFactors);
}

QString ParameterCodec<storage::StorageConfiguration>::section() {
    return "storage";
}

void ParameterCodec<storage::StorageConfiguration>::fields(ParameterArchive &archive,
                                                           storage::StorageConfiguration &value) {
    archive.field("format", value.format);
    archive.field("path", value.path);
    int flags = int(value.dataTypes);
    archive.field("dataTypes", flags);
    value.dataTypes = storage::DataTypes(QFlag(flags));
    archive.field("pointIds", value.pointIds);
    archive.field("allPoints", value.allPoints);
}

QString ParameterCodec<visualization::ScalarColorSettings>::section() {
    return "visualization.scalar";
}

void ParameterCodec<visualization::ScalarColorSettings>::fields(
    ParameterArchive &archive, visualization::ScalarColorSettings &value) {
    archive.field("enabled", value.enabled);
    archive.field("field", value.field);
    archive.field("mappingMode", value.mappingMode);
    archive.field("valueMode", value.valueMode);
    archive.field("rangeMode", value.rangeMode);
    archive.field("minimum", value.minimum);
    archive.field("maximum", value.maximum);
    archive.field("hueStart", value.hueStart);
    archive.field("hueEnd", value.hueEnd);
}

QString ParameterCodec<visualization::AxesSettings>::section() {
    return "visualization.axes";
}

void ParameterCodec<visualization::AxesSettings>::fields(ParameterArchive &archive,
                                                         visualization::AxesSettings &value) {
    archive.field("visible", value.visible);
    archive.field("labelsVisible", value.labelsVisible);
    archive.field("lengthMm", value.lengthMm);
    archive.field("originMm", value.originMm);
    archive.field("fontSize", value.fontSize);
}

QString ParameterCodec<visualization::GridSettings>::section() {
    return "visualization.grid";
}

void ParameterCodec<visualization::GridSettings>::fields(ParameterArchive &archive,
                                                         visualization::GridSettings &value) {
    archive.field("visible", value.visible);
    archive.field("halfExtentMm", value.halfExtentMm);
    archive.field("divisions", value.divisions);
    archive.field("originMm", value.originMm);
    archive.field("color", value.color);
    archive.field("opacity", value.opacity);
    archive.field("lineWidth", value.lineWidth);
}

QString ParameterCodec<visualization::BackgroundSettings>::section() {
    return "visualization.background";
}

void ParameterCodec<visualization::BackgroundSettings>::fields(
    ParameterArchive &archive, visualization::BackgroundSettings &value) {
    archive.field("gradient", value.gradient);
    archive.field("bottomColor", value.bottomColor);
    archive.field("topColor", value.topColor);
}

QString ParameterCodec<visualization::CameraSettings>::section() {
    return "visualization.camera";
}

void ParameterCodec<visualization::CameraSettings>::fields(ParameterArchive &archive,
                                                           visualization::CameraSettings &value) {
    archive.field("projection", value.projection);
    archive.field("viewAngleDegrees", value.viewAngleDegrees);
    archive.field("parallelScaleMm", value.parallelScaleMm);
    archive.field("locked", value.locked);
}

QString ParameterCodec<visualization::WorldSettings>::section() {
    return "visualization.world";
}

void ParameterCodec<visualization::WorldSettings>::fields(ParameterArchive &archive,
                                                          visualization::WorldSettings &value) {
    archive.field("groundVisible", value.groundVisible);
    archive.field("skyVisible", value.skyVisible);
    archive.field("groundHalfExtentMm", value.groundHalfExtentMm);
    archive.field("groundYmm", value.groundYmm);
    archive.field("skyRadiusMm", value.skyRadiusMm);
    archive.field("centerMm", value.centerMm);
    archive.field("groundColor", value.groundColor);
    archive.field("skyColor", value.skyColor);
    archive.field("groundTexturePath", value.groundTexturePath);
    archive.field("skyTexturePath", value.skyTexturePath);
}

QString ParameterCodec<visualization::KeyPointStyle>::section() {
    return "visualization.keyPoint";
}

void ParameterCodec<visualization::KeyPointStyle>::fields(ParameterArchive &archive,
                                                          visualization::KeyPointStyle &value) {
    archive.field("radiusMm", value.radiusMm);
    archive.field("color", value.color);
    archive.field("labelColor", value.labelColor);
    archive.field("labelOffsetMm", value.labelOffsetMm);
    archive.field("fontSize", value.fontSize);
    archive.field("sphereResolution", value.sphereResolution);
    archive.field("labelsVisible", value.labelsVisible);
}

QString ParameterCodec<visualization::PickingSettings>::section() {
    return "visualization.picking";
}

void ParameterCodec<visualization::PickingSettings>::fields(ParameterArchive &archive,
                                                            visualization::PickingSettings &value) {
    archive.field("enabled", value.enabled);
    archive.field("onlyKeyPoints", value.onlyKeyPoints);
    archive.field("clearOnCameraInteraction", value.clearOnCameraInteraction);
    archive.field("tolerance", value.tolerance);
    archive.field("clickTolerancePixels", value.clickTolerancePixels);
}

QString ParameterCodec<visualization::VisualizationRuntimeSettings>::section() {
    return "visualization.runtime";
}

void ParameterCodec<visualization::VisualizationRuntimeSettings>::fields(
    ParameterArchive &archive, visualization::VisualizationRuntimeSettings &value) {
    archive.field("deformationScale", value.deformationScale);
    archive.field("refreshIntervalMs", value.refreshIntervalMs);
}

QString ParameterCodec<visualization::SelectionStyleSettings>::section() {
    return "visualization.selection";
}

void ParameterCodec<visualization::SelectionStyleSettings>::fields(
    ParameterArchive &archive, visualization::SelectionStyleSettings &value) {
    ParameterCodec<visualization::KeyPointStyle>::fields(archive, value);
}

} // namespace config
} // namespace fosd
