#include "FOSD-Classic-QWidgets/common/ViewFactory.h"
#include "FOSD-Classic-QWidgets/features/connection/views/connectwindow.h"
#include "FOSD-Classic-QWidgets/features/connection/views/channlCalibration.h"
#include "FOSD-Classic-QWidgets/features/visualization/views/disp3dwindow.h"
#include "FOSD-Classic-QWidgets/features/visualization/views/docks/disp3dinfodockpanel.h"
#include "FOSD-Classic-QWidgets/features/visualization/views/docks/disp3dviewdockpanel.h"
#include "FOSD-Classic-QWidgets/features/visualization/views/docks/disp3dmodeldockpanel.h"
#include "FOSD-Classic-QWidgets/features/visualization/views/keypoints/disp3dkeypointedit.h"
#include "FOSD-Classic-QWidgets/features/visualization/views/keypoints/disp3dkeypointselect.h"
#include "FOSD-Classic-QWidgets/features/data/views/dispdata.h"
#include "FOSD-Classic-QWidgets/features/mainwindow/views/oldmainwindow.h"
namespace fosd {
namespace ui {
namespace classic {
QWidget *createPanelView(PanelId panel, QWidget *parent) {
    switch (panel) {
        case PanelId::Connection:
            return new ConnectionView(parent);
        case PanelId::ChannelCalibration:
            return new ChannelCalibrationView(parent);
        case PanelId::Visualization:
            return new VisualizationView(parent);
        case PanelId::InfoPanel:
            return new InfoPanelView(parent);
        case PanelId::ScenePanel:
            return new ScenePanelView(parent);
        case PanelId::ModelPanel:
            return new ModelPanelView(parent);
        case PanelId::KeyPointEdit:
            return new KeyPointEditView(parent);
        case PanelId::KeyPointSelect:
            return new KeyPointSelectView(parent);
        case PanelId::HardwareData:
            return new HardwareDataView(parent);
        case PanelId::Main:
            return new MainWindowView(parent);
    }
    return nullptr;
}
} // namespace classic
} // namespace ui
} // namespace fosd
