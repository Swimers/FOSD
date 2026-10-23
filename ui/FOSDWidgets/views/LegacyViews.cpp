#include "LegacyViews.h"
#include "../resources/UiResources.h"
#include "ui_connectdebug.h"
#include "ui_connectwindow.h"
#include "ui_disp2dpara.h"
#include "ui_disp2dwindow.h"
#include "ui_disp3dcolorpicker.h"
#include "ui_disp3dkeypointedit.h"
#include "ui_disp3dkeypointselect.h"
#include "ui_disp3dwindow.h"
#include "ui_dispdata.h"
#include "ui_trackingpointdatatable.h"
#include "ui_trackingpointmanager.h"
#include "ui_disp3dinfodockpanel.h"
#include "ui_disp3dmodeldockpanel.h"
#include "ui_disp3dviewdockpanel.h"
#include "ui_oldmainwindow.h"
#include "ui_rainbowmainwindow.h"
#include "ui_login.h"
#include "ui_userforget.h"
#include "ui_usermanage.h"
#include "ui_userregister.h"
namespace fosd { namespace widgets {
ConnectionDebugView::ConnectionDebugView(QWidget *parent) : QWidget(parent), m_ui(new Ui::connectdebug) { initializeFosdUiResources(); m_ui->setupUi(this); }
ConnectionDebugView::~ConnectionDebugView() = default;
ConnectionView::ConnectionView(QWidget *parent) : QWidget(parent), m_ui(new Ui::connectwindow) { initializeFosdUiResources(); m_ui->setupUi(this); }
ConnectionView::~ConnectionView() = default;
PlotSettingsView::PlotSettingsView(QWidget *parent) : QWidget(parent), m_ui(new Ui::disp2dpara) { initializeFosdUiResources(); m_ui->setupUi(this); }
PlotSettingsView::~PlotSettingsView() = default;
PlotView::PlotView(QWidget *parent) : QWidget(parent), m_ui(new Ui::disp2dwindow) { initializeFosdUiResources(); m_ui->setupUi(this); }
PlotView::~PlotView() = default;
ColorPickerView::ColorPickerView(QWidget *parent) : QDialog(parent), m_ui(new Ui::Disp3DColorPicker) { initializeFosdUiResources(); m_ui->setupUi(this); }
ColorPickerView::~ColorPickerView() = default;
KeyPointEditView::KeyPointEditView(QWidget *parent) : QWidget(parent), m_ui(new Ui::disp3dkeypointedit) { initializeFosdUiResources(); m_ui->setupUi(this); }
KeyPointEditView::~KeyPointEditView() = default;
KeyPointSelectView::KeyPointSelectView(QWidget *parent) : QWidget(parent), m_ui(new Ui::disp3dkeypointselect) { initializeFosdUiResources(); m_ui->setupUi(this); }
KeyPointSelectView::~KeyPointSelectView() = default;
VisualizationView::VisualizationView(QWidget *parent) : QMainWindow(parent), m_ui(new Ui::disp3dwindow) { initializeFosdUiResources(); m_ui->setupUi(this); }
VisualizationView::~VisualizationView() = default;
HardwareDataView::HardwareDataView(QWidget *parent) : QDialog(parent), m_ui(new Ui::dispdata) { initializeFosdUiResources(); m_ui->setupUi(this); }
HardwareDataView::~HardwareDataView() = default;
TrackingDataView::TrackingDataView(QWidget *parent) : QWidget(parent), m_ui(new Ui::trackingpointdatatable) { initializeFosdUiResources(); m_ui->setupUi(this); }
TrackingDataView::~TrackingDataView() = default;
TrackingManagerView::TrackingManagerView(QWidget *parent) : QWidget(parent), m_ui(new Ui::trackingpointmanager) { initializeFosdUiResources(); m_ui->setupUi(this); }
TrackingManagerView::~TrackingManagerView() = default;
InfoPanelView::InfoPanelView(QWidget *parent) : QWidget(parent), m_ui(new Ui::Disp3DInfoDockPanel) { initializeFosdUiResources(); m_ui->setupUi(this); }
InfoPanelView::~InfoPanelView() = default;
ModelPanelView::ModelPanelView(QWidget *parent) : QWidget(parent), m_ui(new Ui::Disp3DModelDockPanel) { initializeFosdUiResources(); m_ui->setupUi(this); }
ModelPanelView::~ModelPanelView() = default;
ScenePanelView::ScenePanelView(QWidget *parent) : QWidget(parent), m_ui(new Ui::Disp3DViewDockPanel) { initializeFosdUiResources(); m_ui->setupUi(this); }
ScenePanelView::~ScenePanelView() = default;
MainWindowView::MainWindowView(QWidget *parent) : QMainWindow(parent), m_ui(new Ui::OldMainWindow) { initializeFosdUiResources(); m_ui->setupUi(this); }
MainWindowView::~MainWindowView() = default;
RainbowWindowView::RainbowWindowView(QWidget *parent) : QMainWindow(parent), m_ui(new Ui::RainbowMainWindow) { initializeFosdUiResources(); m_ui->setupUi(this); }
RainbowWindowView::~RainbowWindowView() = default;
LoginView::LoginView(QWidget *parent) : QMainWindow(parent), m_ui(new Ui::loginwindow) { initializeFosdUiResources(); m_ui->setupUi(this); }
LoginView::~LoginView() = default;
RecoveryView::RecoveryView(QWidget *parent) : QWidget(parent), m_ui(new Ui::UserForget) { initializeFosdUiResources(); m_ui->setupUi(this); }
RecoveryView::~RecoveryView() = default;
AccountView::AccountView(QWidget *parent) : QWidget(parent), m_ui(new Ui::UserManage) { initializeFosdUiResources(); m_ui->setupUi(this); }
AccountView::~AccountView() = default;
RegistrationView::RegistrationView(QWidget *parent) : QWidget(parent), m_ui(new Ui::UserRegister) { initializeFosdUiResources(); m_ui->setupUi(this); }
RegistrationView::~RegistrationView() = default;
QWidget *createPanelView(PanelId panel, QWidget *parent) {
    switch (panel) {
    case PanelId::ConnectionDebug: return new ConnectionDebugView(parent);
    case PanelId::Connection: return new ConnectionView(parent);
    case PanelId::PlotSettings: return new PlotSettingsView(parent);
    case PanelId::Plot: return new PlotView(parent);
    case PanelId::ColorPicker: return new ColorPickerView(parent);
    case PanelId::KeyPointEdit: return new KeyPointEditView(parent);
    case PanelId::KeyPointSelect: return new KeyPointSelectView(parent);
    case PanelId::Visualization: return new VisualizationView(parent);
    case PanelId::HardwareData: return new HardwareDataView(parent);
    case PanelId::TrackingData: return new TrackingDataView(parent);
    case PanelId::TrackingManager: return new TrackingManagerView(parent);
    case PanelId::InfoPanel: return new InfoPanelView(parent);
    case PanelId::ModelPanel: return new ModelPanelView(parent);
    case PanelId::ScenePanel: return new ScenePanelView(parent);
    case PanelId::Main: return new MainWindowView(parent);
    case PanelId::RainbowMain: return new RainbowWindowView(parent);
    case PanelId::Login: return new LoginView(parent);
    case PanelId::Recovery: return new RecoveryView(parent);
    case PanelId::Account: return new AccountView(parent);
    case PanelId::Registration: return new RegistrationView(parent);
    }
    return nullptr;
}
} }
