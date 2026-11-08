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
enum class FeatureId { 
    MainWindow, 
    Connection, 
    Visualization, 
    Model, 
    Tracking 
};
FeatureId featureForPanel(PanelId panel);

enum class WindowAction { Minimize, ToggleFullScreen, Close };
// Stable UI protocol: panel + original Designer action/object name. Values are
// local to that panel; no backend object or QWidget crosses this boundary.
struct UiRequest {
    quint64 id = 0;
    PanelId panel = PanelId::Main;
    QString action;
    QVariantMap fields;
};
struct UiResponse {
    quint64 id = 0;
    PanelId panel = PanelId::Main;
    bool success = false;
    QString message;
    QVariantMap fields;
};

} // namespace classic
} // namespace ui
} // namespace fosd

Q_DECLARE_METATYPE(fosd::ui::classic::PanelId)
Q_DECLARE_METATYPE(fosd::ui::classic::FeatureId)
Q_DECLARE_METATYPE(fosd::ui::classic::UiRequest)
Q_DECLARE_METATYPE(fosd::ui::classic::UiResponse)
