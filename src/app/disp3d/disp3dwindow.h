#ifndef DISP3DWINDOW_H
#define DISP3DWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QWidget>
#include <QVTKOpenGLNativeWidget.h>
#include <vtkRenderWidget.h>

#include <disp3dmodel.h>
#include <disp3dbackground.h>
#include <disp3dinteractor.h>
#include <disp3dkeypointedit.h>
#include <disp3dkeypointselect.h>
#include <disp3dfreshmodelthread.h>

#include <trackingpointdatatable.h>
#include <trackingpointmanager.h>
#include <trackingpoint.h>

#include <dispdata.h>
#include <docks/disp3ddockmanager.h>
#include <docks/disp3dinfodockpanel.h>
#include <docks/disp3dmodeldockpanel.h>
#include <docks/disp3dviewdockpanel.h>

#include <IFE.h>
#include <rainbowife.h>

namespace Ui {
class disp3dwindow;
}

class disp3dwindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit disp3dwindow(QWidget *parent = nullptr);
    ~disp3dwindow();

public:
    void InitPointInfor();
    void InitWindow();
    void InitDockPanels();

private slots:
    void on_checkBox_3_clicked(bool checked);
    void LoadModel();
    void ClearModel();
    void MergeModel();
    void ExportModel();
    void EditKeyPoint();
    void SelectKeyPoint();
    void SetScalarMapping(int index);
    void ClearScalarMapping();
    void SetAxesVisible(bool checked);
    void SetGridVisible(bool checked);
    void SwitchView(int viewIndex);
    void SetAxisSize(int value);
    void SetGridSize(int value);
    void SetXOZGridVisible(bool checked);
    void SetXOYGridVisible(bool checked);
    void SetZOYGridVisible(bool checked);
    void SetCameraLock(bool checked);
    void SetCameraFollow(bool checked);
    void SetFPS(int fps);
    void on_actionStart_triggered();
    void on_actionStop_triggered();
    void on_actionPause_triggered();
    void on_actionReset_triggered();

    void on_actionTrackingPoints_triggered();
    void on_actionTrackingPointData_triggered();
    void on_actionBackData_triggered();

public slots:
    void RefreshStage();
    void RefreshInfor();
    void ApplyDisplayFrame(const DisplayPointUpdateList &pointUpdates,
                           double maxDisplace,
                           const TrackingPointFrameUpdateList &trackingUpdates);
    void GetIFEPtr(IFE * exife){ife = exife;}
    void GetRainbowIFEPtr(rainbowIFE * ptr){m_rainbowIFE = ptr;}

public:
    int FPS;

private:
    Ui::disp3dwindow *ui;

    IFE * ife = nullptr;
    rainbowIFE *m_rainbowIFE =nullptr;

    bool Flag_ShowGrid;

    disp3dmodel *m_disp3dmodel = nullptr;
    disp3dbackground *m_disp3dbackground = nullptr;
    disp3dinteractor *m_disp3dinteractor = nullptr;
    disp3dfreshmodelthread *m_disp3dthread = nullptr;
    Disp3DDockManager *m_dockManager = nullptr;
    Disp3DModelDockPanel *m_modelDockPanel = nullptr;
    Disp3DViewDockPanel *m_viewDockPanel = nullptr;
    Disp3DInfoDockPanel *m_infoDockPanel = nullptr;

    trackingpoint *m_trackingpoint = nullptr;

    vtkRenderWindow *m_vtkrenderwindow = nullptr;
    vtkRenderWindowInteractor *m_iren = nullptr;

    QTimer *freshTimer = nullptr;
};

#endif // DISP3DWINDOW_H
