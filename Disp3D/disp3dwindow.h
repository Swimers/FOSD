#ifndef DISP3DWINDOW_H
#define DISP3DWINDOW_H

#include <QMainWindow>
#include <QLabel>

#include <disp3dmodel.h>
#include <disp3dpoint.h>
#include <disp3dselect.h>
#include <disp3dfreshmodelthread.h>
#include <disp3dinteractor.h>

#include "IFE.h"

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
<<<<<<< Updated upstream
    void InitPointInfor();

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
    void InitWindow();
    void GetIFEPtr(IFE * exife){ife = exife;}
=======
    void GetIFEPtr(IFE* externife)  {ife = externife;}
    void InitStatusBar();
    void InitWindow();

private slots:
    void ModelDockShow();
    void InforDockShow();
    void TimeOutHandel_Fresh();
    void on_pbtn_LoadModel_clicked();
    void on_pbtn_ChangeView_clicked();
    void on_cbox_DispAxis_clicked(bool checked);
    void on_cbox_DispEdge_clicked(bool checked);
    void on_pbtn_SetSize_clicked();
    void on_pbtn_AddPoint_clicked();
    void on_cbox_FreSeclect_currentIndexChanged(const QString &arg1);
    void on_pbtn_DispAnalysis_clicked();
    void on_actionStart_triggered();
    void on_actionStop_triggered();
    void on_actionReset_triggered();
    void SwitchToB();
    void SwitchToW();
>>>>>>> Stashed changes

protected slots:
    void deal_addcord(int,double,double,double);
    void deal_delcord(int);
    void deal_editcord(int,int,double,double,double);

private:
    Ui::disp3dwindow *ui;

<<<<<<< Updated upstream
    IFE * ife = nullptr;
    bool Flag_ShowGrid;
=======
    IFE * ife = nullptr;         //逆有限单元类，所有有关逆有限单元的计算都在该类中
    QTimer * timer = nullptr;
    disp3dmodel * m_disp3dmodel = nullptr;
    disp3dpoint * m_disp3dpoint = nullptr;
    disp3dfreshmodelthread * m_freshmodelthread = nullptr;
    disp3dinteractor * m_disp3dinteractor;
>>>>>>> Stashed changes

    QLabel *label0;
    QLabel *label1;
    QLabel *label2;
    QLabel *label3;

    int fps;                    //刷新频率
};

#endif // DISP3DWINDOW_H
