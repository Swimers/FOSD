#include "ParameterCodec.h"

namespace fosd {
namespace config {

namespace {
// Default values need a valid schema but may omit operation prerequisites.
template <class T> bool validateDefaultSection(const ConfigData &data, QString *error) {
    T value;
    return decodeParameter(data.value(ParameterCodec<T>::section()), &value, false, error);
}
} // namespace

ConfigData defaultCoreParameters() {
    ConfigData defaults;
    defaults["communication.serial"] = parameterSection(communication::SerialConfig());
    defaults["communication.udp"] = parameterSection(communication::UdpConfig());
    defaults["communication.tcp"] = parameterSection(communication::TcpConfig());
    defaults["calculation"] = parameterSection(Calculation::CalculationConfiguration());
    defaults["storage"] = parameterSection(storage::StorageConfiguration());
    defaults["visualization.scalar"] = parameterSection(visualization::ScalarColorSettings());
    defaults["visualization.axes"] = parameterSection(visualization::AxesSettings());
    defaults["visualization.grid"] = parameterSection(visualization::GridSettings());
    defaults["visualization.background"] = parameterSection(visualization::BackgroundSettings());
    defaults["visualization.camera"] = parameterSection(visualization::CameraSettings());
    defaults["visualization.world"] = parameterSection(visualization::WorldSettings());
    defaults["visualization.keyPoint"] = parameterSection(visualization::KeyPointStyle());
    defaults["visualization.picking"] = parameterSection(visualization::PickingSettings());
    defaults["visualization.runtime"] =
        parameterSection(visualization::VisualizationRuntimeSettings());
    defaults["visualization.selection"] = parameterSection(visualization::SelectionStyleSettings());
    return defaults;
}

bool validateCoreDefaults(const ConfigData &data, QString *error) {
    if (data.size() != 15) {
        if (error)
            *error = "Invalid default section count";
        return false;
    }

    // Short-circuit at the first invalid section so its diagnostic is retained.
    return validateDefaultSection<communication::SerialConfig>(data, error) &&
           validateDefaultSection<communication::UdpConfig>(data, error) &&
           validateDefaultSection<communication::TcpConfig>(data, error) &&
           validateDefaultSection<Calculation::CalculationConfiguration>(data, error) &&
           validateDefaultSection<storage::StorageConfiguration>(data, error) &&
           validateDefaultSection<visualization::ScalarColorSettings>(data, error) &&
           validateDefaultSection<visualization::AxesSettings>(data, error) &&
           validateDefaultSection<visualization::GridSettings>(data, error) &&
           validateDefaultSection<visualization::BackgroundSettings>(data, error) &&
           validateDefaultSection<visualization::CameraSettings>(data, error) &&
           validateDefaultSection<visualization::WorldSettings>(data, error) &&
           validateDefaultSection<visualization::KeyPointStyle>(data, error) &&
           validateDefaultSection<visualization::PickingSettings>(data, error) &&
           validateDefaultSection<visualization::VisualizationRuntimeSettings>(data, error) &&
           validateDefaultSection<visualization::SelectionStyleSettings>(data, error);
}
} // namespace config
} // namespace fosd
