#include "ViewFactory.h"
#include "../apps/connection/views/connectwindow.h"
#include "../apps/connection/views/channlCalibration.h"
#include "../apps/visualization/views/disp3dwindow.h"
#include "../apps/visualization/views/docks/disp3dinfodockpanel.h"
#include "../apps/visualization/views/docks/disp3dviewdockpanel.h"
#include "../apps/model/views/disp3dmodeldockpanel.h"
#include "../apps/model/views/disp3dkeypointedit.h"
#include "../apps/model/views/disp3dkeypointselect.h"
#include "../apps/tracking/views/dispdata.h"
#include "../apps/mainwindow/views/oldmainwindow.h"
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
