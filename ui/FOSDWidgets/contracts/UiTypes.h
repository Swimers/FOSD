#pragma once
#include <QMetaType>
#include <QVariantMap>

namespace fosd { namespace widgets {
enum class PanelId {
    Main, RainbowMain, Connection, ConnectionDebug, Plot, PlotSettings,
    Visualization, ModelPanel, ScenePanel, InfoPanel, KeyPointEdit, KeyPointSelect,
    ColorPicker, HardwareData, TrackingManager, TrackingData,
    Login, Registration, Recovery, Account
};
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
} }
Q_DECLARE_METATYPE(fosd::widgets::PanelId)
Q_DECLARE_METATYPE(fosd::widgets::UiRequest)
Q_DECLARE_METATYPE(fosd::widgets::UiResponse)
