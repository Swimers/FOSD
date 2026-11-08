#pragma once
#include <QMetaType>
#include <QVariantMap>

namespace fosd {
namespace ui {
namespace classic {

enum class PanelId {
    Main,
    Connection,
    ChannelCalibration,
    Visualization,
    ModelPanel,
    ScenePanel,
    InfoPanel,
    KeyPointEdit,
    KeyPointSelect,
    HardwareData,
};

enum class WindowAction { Minimize, ToggleFullScreen, Close };
} // namespace classic
} // namespace ui
} // namespace fosd

Q_DECLARE_METATYPE(fosd::ui::classic::PanelId)
