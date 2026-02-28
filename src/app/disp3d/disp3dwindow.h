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

#include <IFE.h>

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

private slots:
    void on_checkBox_3_clicked(bool checked);
    void on_pbtn_ModelLoad_clicked();
    void on_cbox_AxisAble_clicked(bool checked);
    void on_cbox_GridAble_clicked(bool checked);
    void on_pbtn_ModelClear_clicked();
    void on_pbtn_SwitchView_clicked();
    void on_pbtn_ModelMerge_clicked();
    void on_pbtn_ModelExport_clicked();
    void on_pbtn_KeyPointEdit_clicked();
    void on_hsd_AxisSize_valueChanged(int value);
    void on_hsd_GridSize_sliderReleased();
    void on_pbtn_KeyPointSelect_clicked();
    void on_actionModelDock_triggered();
    void on_actionInforDock_triggered();
    void on_actionViewDock_triggered();
    void on_actionStart_triggered();
    void on_actionStop_triggered();
    void on_actionPause_triggered();
    void on_actionReset_triggered();
    void on_cbox_FPS_currentTextChanged(const QString &arg1);
    void on_pbtn_ScalarSet_clicked();
    void on_pbtn_ScalarClear_clicked();

    void on_cbox_XOZGrid_clicked(bool checked);
    void on_cbox_XOYGrid_clicked(bool checked);
    void on_cbox_ZOYGrid_clicked(bool checked);

    void on_cbox_CareamLockAble_clicked(bool checked);
    void on_cbox_FellowAble_clicked(bool checked);

public slots:
    void RefreshStage();
    void RefreshInfor();
    void GetIFEPtr(IFE * exife){ife = exife;}

public:
    int FPS;

private:
    Ui::disp3dwindow *ui;

    IFE * ife = nullptr;
    bool Flag_ShowGrid;

    disp3dmodel *m_disp3dmodel = nullptr;
    disp3dbackground *m_disp3dbackground = nullptr;
    disp3dinteractor *m_disp3dinteractor = nullptr;
    disp3dfreshmodelthread *m_disp3dthread = nullptr;

    vtkRenderWindow *m_vtkrenderwindow = nullptr;
    vtkRenderWindowInteractor *m_iren = nullptr;

    QTimer *freshTimer = nullptr;
};

#endif // DISP3DWINDOW_H
