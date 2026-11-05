#include "UiTypes.h"
namespace fosd {
namespace ui {
namespace classic {
FeatureId featureForPanel(PanelId panel) {
    switch (panel) {
        case PanelId::Main:
            return FeatureId::MainWindow;
        case PanelId::Connection:
        case PanelId::ConnectionDebug:
            return FeatureId::Connection;
        case PanelId::Plot:
        case PanelId::PlotSettings:
            return FeatureId::Plot;
        case PanelId::Visualization:
        case PanelId::ScenePanel:
        case PanelId::InfoPanel:
            return FeatureId::Visualization;
        case PanelId::ModelPanel:
        case PanelId::KeyPointEdit:
        case PanelId::KeyPointSelect:
            return FeatureId::Model;
        case PanelId::HardwareData:
            return FeatureId::Tracking;
    }
    return FeatureId::MainWindow;
}

} // namespace classic
} // namespace ui
} // namespace fosd
