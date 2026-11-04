#include "ParameterCodec.h"
namespace fosd { namespace config {
ConfigData defaultCoreParameters() {
ConfigData d;
d["communication.serial"] = parameterSection(communication::SerialConfig());
d["communication.udp"] = parameterSection(communication::UdpConfig());
d["communication.tcp"] = parameterSection(communication::TcpConfig());
d["calculation"] = parameterSection(Calculation::CalculationConfiguration());
d["storage"] = parameterSection(storage::StorageConfiguration());
d["visualization.scalar"] = parameterSection(visualization::ScalarColorSettings());
d["visualization.axes"] = parameterSection(visualization::AxesSettings());
d["visualization.grid"] = parameterSection(visualization::GridSettings());
d["visualization.background"] = parameterSection(visualization::BackgroundSettings());
d["visualization.camera"] = parameterSection(visualization::CameraSettings());
d["visualization.world"] = parameterSection(visualization::WorldSettings());
d["visualization.keyPoint"] = parameterSection(visualization::KeyPointStyle());
d["visualization.picking"] = parameterSection(visualization::PickingSettings());
d["visualization.runtime"] = parameterSection(visualization::VisualizationRuntimeSettings());
d["visualization.selection"] = parameterSection(visualization::SelectionStyleSettings());
return d;
}
} }

namespace fosd { namespace config {
bool validateCoreDefaults(const ConfigData &data, QString *error) {
if (data.size() != 15) { if(error) *error="Invalid default section count"; return false; }
communication::SerialConfig v0; if(!decodeParameter(data.value(ParameterCodec<communication::SerialConfig>::section()), &v0, false, error)) return false;
communication::UdpConfig v1; if(!decodeParameter(data.value(ParameterCodec<communication::UdpConfig>::section()), &v1, false, error)) return false;
communication::TcpConfig v2; if(!decodeParameter(data.value(ParameterCodec<communication::TcpConfig>::section()), &v2, false, error)) return false;
Calculation::CalculationConfiguration v3; if(!decodeParameter(data.value(ParameterCodec<Calculation::CalculationConfiguration>::section()), &v3, false, error)) return false;
storage::StorageConfiguration v4; if(!decodeParameter(data.value(ParameterCodec<storage::StorageConfiguration>::section()), &v4, false, error)) return false;
visualization::ScalarColorSettings v5; if(!decodeParameter(data.value(ParameterCodec<visualization::ScalarColorSettings>::section()), &v5, false, error)) return false;
visualization::AxesSettings v6; if(!decodeParameter(data.value(ParameterCodec<visualization::AxesSettings>::section()), &v6, false, error)) return false;
visualization::GridSettings v7; if(!decodeParameter(data.value(ParameterCodec<visualization::GridSettings>::section()), &v7, false, error)) return false;
visualization::BackgroundSettings v8; if(!decodeParameter(data.value(ParameterCodec<visualization::BackgroundSettings>::section()), &v8, false, error)) return false;
visualization::CameraSettings v9; if(!decodeParameter(data.value(ParameterCodec<visualization::CameraSettings>::section()), &v9, false, error)) return false;
visualization::WorldSettings v10; if(!decodeParameter(data.value(ParameterCodec<visualization::WorldSettings>::section()), &v10, false, error)) return false;
visualization::KeyPointStyle v11; if(!decodeParameter(data.value(ParameterCodec<visualization::KeyPointStyle>::section()), &v11, false, error)) return false;
visualization::PickingSettings v12; if(!decodeParameter(data.value(ParameterCodec<visualization::PickingSettings>::section()), &v12, false, error)) return false;
visualization::VisualizationRuntimeSettings v13; if(!decodeParameter(data.value(ParameterCodec<visualization::VisualizationRuntimeSettings>::section()), &v13, false, error)) return false;
visualization::SelectionStyleSettings v14; if(!decodeParameter(data.value(ParameterCodec<visualization::SelectionStyleSettings>::section()), &v14, false, error)) return false;
return true;
}
} }
