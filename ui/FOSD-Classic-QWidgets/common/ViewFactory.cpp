#include "ViewFactory.h"
#include "../apps/connection/views/connectwindow.h"
#include "../apps/connection/views/connectdebug.h"
#include "../apps/plot/views/disp2dwindow.h"
#include "../apps/plot/views/disp2dpara.h"
#include "../apps/visualization/views/disp3dwindow.h"
#include "../apps/visualization/views/docks/disp3dinfodockpanel.h"
#include "../apps/visualization/views/docks/disp3dviewdockpanel.h"
#include "../apps/model/views/disp3dmodeldockpanel.h"
#include "../apps/model/views/disp3dkeypointedit.h"
#include "../apps/model/views/disp3dkeypointselect.h"
#include "../apps/model/views/disp3dcolorpicker.h"
#include "../apps/tracking/views/dispdata.h"
#include "../apps/tracking/views/trackingpointmanager.h"
#include "../apps/tracking/views/trackingpointdatatable.h"
#include "../apps/account/views/login.h"
#include "../apps/account/views/userregister.h"
#include "../apps/account/views/userforget.h"
#include "../apps/account/views/usermanage.h"
#include "../apps/mainwindow/views/oldmainwindow.h"
namespace fosd { namespace ui { namespace classic {
QWidget *createPanelView(PanelId panel, QWidget *parent) {
    switch (panel) {
    case PanelId::Connection: return new ConnectionView(parent);
    case PanelId::ConnectionDebug: return new ConnectionDebugView(parent);
    case PanelId::Plot: return new PlotView(parent);
    case PanelId::PlotSettings: return new PlotSettingsView(parent);
    case PanelId::Visualization: return new VisualizationView(parent);
    case PanelId::InfoPanel: return new InfoPanelView(parent);
    case PanelId::ScenePanel: return new ScenePanelView(parent);
    case PanelId::ModelPanel: return new ModelPanelView(parent);
    case PanelId::KeyPointEdit: return new KeyPointEditView(parent);
    case PanelId::KeyPointSelect: return new KeyPointSelectView(parent);
    case PanelId::ColorPicker: return new ColorPickerView(parent);
    case PanelId::HardwareData: return new HardwareDataView(parent);
    case PanelId::TrackingManager: return new TrackingManagerView(parent);
    case PanelId::TrackingData: return new TrackingDataView(parent);
    case PanelId::Login: return new LoginView(parent);
    case PanelId::Registration: return new RegistrationView(parent);
    case PanelId::Recovery: return new RecoveryView(parent);
    case PanelId::Account: return new AccountView(parent);
    case PanelId::Main: return new MainWindowView(parent);
    }
    return nullptr;
}
} } }
