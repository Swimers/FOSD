#ifndef DISP2DWINDOW_H
#define DISP2DWINDOW_H

#include <QWidget>
#include <QStandardItemModel>
#include <QTimer>
#include <QDebug>
#include "IFE.h"
#include "disp2dpara.h"

class IFE;

namespace Ui {
class disp2dwindow;
}

class disp2dwindow : public QWidget
{
    Q_OBJECT

public:
    explicit disp2dwindow(QWidget *parent = nullptr);
    ~disp2dwindow();

    void GetStrainPtr(float* strain_float){ife->strain = strain_float;}
    IFE* SendIFEPtr(){return ife;}

private:
    QStandardItemModel *TwoD_model = new QStandardItemModel();      //二维观测界面表格模型
    QTimer *timer = new QTimer(this);                               //创建定时器
    bool Flag_Observe = false;                                      //二维观测界面，观测按钮按下标志位。

private:
    enum columntype {POSITION_ORIGINAL,POSITION_DISP,POSITION_FINAL}; //更新表格的位置参数，

private:
    void InitWindow();
    void LoadCoordinates(float (&p)[MARKPOINTNUM][3],const columntype num);

private slots:
    void UpdatatabelSlot();
    void on_pbtn_ChangeOriginalCoordinates_clicked();
    void on_pbtn_ParaRevise_clicked();
    void on_pbtn_ChangeBackGround_clicked();
    void on_pbtn_Observe_clicked();

    void FreshData();

private:
    Ui::disp2dwindow *ui;

    IFE* ife = nullptr;                     //逆有限单元类，所有有关逆有限单元的计算都在该类中
    disp2dpara* m_disp2dpara = nullptr;
};

#endif // DISP2DWINDOW_H
