#pragma once
#include <QWidget>
#include <QDialog>
#include <QMainWindow>
#include <memory>
#include "../contracts/UiTypes.h"
namespace Ui {
class connectdebug;
class connectwindow;
class disp2dpara;
class disp2dwindow;
class Disp3DColorPicker;
class disp3dkeypointedit;
class disp3dkeypointselect;
class disp3dwindow;
class dispdata;
class trackingpointdatatable;
class trackingpointmanager;
class Disp3DInfoDockPanel;
class Disp3DModelDockPanel;
class Disp3DViewDockPanel;
class OldMainWindow;
class RainbowMainWindow;
class loginwindow;
class UserForget;
class UserManage;
class UserRegister;
}
namespace fosd { namespace widgets {
class ConnectionDebugView : public QWidget {
    Q_OBJECT
public:
    explicit ConnectionDebugView(QWidget *parent = nullptr);
    ~ConnectionDebugView() override;
private:
    std::unique_ptr<Ui::connectdebug> m_ui;
};
class ConnectionView : public QWidget {
    Q_OBJECT
public:
    explicit ConnectionView(QWidget *parent = nullptr);
    ~ConnectionView() override;
private:
    std::unique_ptr<Ui::connectwindow> m_ui;
};
class PlotSettingsView : public QWidget {
    Q_OBJECT
public:
    explicit PlotSettingsView(QWidget *parent = nullptr);
    ~PlotSettingsView() override;
private:
    std::unique_ptr<Ui::disp2dpara> m_ui;
};
class PlotView : public QWidget {
    Q_OBJECT
public:
    explicit PlotView(QWidget *parent = nullptr);
    ~PlotView() override;
private:
    std::unique_ptr<Ui::disp2dwindow> m_ui;
};
class ColorPickerView : public QDialog {
    Q_OBJECT
public:
    explicit ColorPickerView(QWidget *parent = nullptr);
    ~ColorPickerView() override;
private:
    std::unique_ptr<Ui::Disp3DColorPicker> m_ui;
};
class KeyPointEditView : public QWidget {
    Q_OBJECT
public:
    explicit KeyPointEditView(QWidget *parent = nullptr);
    ~KeyPointEditView() override;
private:
    std::unique_ptr<Ui::disp3dkeypointedit> m_ui;
};
class KeyPointSelectView : public QWidget {
    Q_OBJECT
public:
    explicit KeyPointSelectView(QWidget *parent = nullptr);
    ~KeyPointSelectView() override;
private:
    std::unique_ptr<Ui::disp3dkeypointselect> m_ui;
};
class VisualizationView : public QMainWindow {
    Q_OBJECT
public:
    explicit VisualizationView(QWidget *parent = nullptr);
    ~VisualizationView() override;
private:
    std::unique_ptr<Ui::disp3dwindow> m_ui;
};
class HardwareDataView : public QDialog {
    Q_OBJECT
public:
    explicit HardwareDataView(QWidget *parent = nullptr);
    ~HardwareDataView() override;
private:
    std::unique_ptr<Ui::dispdata> m_ui;
};
class TrackingDataView : public QWidget {
    Q_OBJECT
public:
    explicit TrackingDataView(QWidget *parent = nullptr);
    ~TrackingDataView() override;
private:
    std::unique_ptr<Ui::trackingpointdatatable> m_ui;
};
class TrackingManagerView : public QWidget {
    Q_OBJECT
public:
    explicit TrackingManagerView(QWidget *parent = nullptr);
    ~TrackingManagerView() override;
private:
    std::unique_ptr<Ui::trackingpointmanager> m_ui;
};
class InfoPanelView : public QWidget {
    Q_OBJECT
public:
    explicit InfoPanelView(QWidget *parent = nullptr);
    ~InfoPanelView() override;
private:
    std::unique_ptr<Ui::Disp3DInfoDockPanel> m_ui;
};
class ModelPanelView : public QWidget {
    Q_OBJECT
public:
    explicit ModelPanelView(QWidget *parent = nullptr);
    ~ModelPanelView() override;
private:
    std::unique_ptr<Ui::Disp3DModelDockPanel> m_ui;
};
class ScenePanelView : public QWidget {
    Q_OBJECT
public:
    explicit ScenePanelView(QWidget *parent = nullptr);
    ~ScenePanelView() override;
private:
    std::unique_ptr<Ui::Disp3DViewDockPanel> m_ui;
};
class MainWindowView : public QMainWindow {
    Q_OBJECT
public:
    explicit MainWindowView(QWidget *parent = nullptr);
    ~MainWindowView() override;
private:
    std::unique_ptr<Ui::OldMainWindow> m_ui;
};
class RainbowWindowView : public QMainWindow {
    Q_OBJECT
public:
    explicit RainbowWindowView(QWidget *parent = nullptr);
    ~RainbowWindowView() override;
private:
    std::unique_ptr<Ui::RainbowMainWindow> m_ui;
};
class LoginView : public QMainWindow {
    Q_OBJECT
public:
    explicit LoginView(QWidget *parent = nullptr);
    ~LoginView() override;
private:
    std::unique_ptr<Ui::loginwindow> m_ui;
};
class RecoveryView : public QWidget {
    Q_OBJECT
public:
    explicit RecoveryView(QWidget *parent = nullptr);
    ~RecoveryView() override;
private:
    std::unique_ptr<Ui::UserForget> m_ui;
};
class AccountView : public QWidget {
    Q_OBJECT
public:
    explicit AccountView(QWidget *parent = nullptr);
    ~AccountView() override;
private:
    std::unique_ptr<Ui::UserManage> m_ui;
};
class RegistrationView : public QWidget {
    Q_OBJECT
public:
    explicit RegistrationView(QWidget *parent = nullptr);
    ~RegistrationView() override;
private:
    std::unique_ptr<Ui::UserRegister> m_ui;
};
QWidget *createPanelView(PanelId panel, QWidget *parent = nullptr);
} }
