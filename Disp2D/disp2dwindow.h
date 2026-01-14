#ifndef DISP2DWINDOW_H
#define DISP2DWINDOW_H

#include <QWidget>
#include <QStandardItemModel>
#include <QTimer>
#include "IFE.h"
#include "pararevisewindow.h"

class IFE;

namespace Ui {
class Disp2DWindow;
}

class Disp2DWindow : public QWidget
{
    Q_OBJECT

public:
    explicit Disp2DWindow(const QString& privilege,QWidget *parent = nullptr);
    ~Disp2DWindow();
    void GetStrainPtr(float* strain_float){ife->strain = strain_float;}
    IFE* SendIFEPtr(){return ife;}

private:
    QStandardItemModel *TwoD_model= new QStandardItemModel();//二维观测界面表格模型
    QTimer *timer=new QTimer(this);//创建定时器
    bool observe_flag=false;    //二维观测界面，观测按钮按下标志位。

private:
    enum columntype {POSITION_ORIGINAL,POSITION_DISP,POSITION_FINAL}; //更新表格的位置参数，

private:
    void initialize_widget();
    void LoadCoordinates(float (&p)[MARKPOINTNUM][3],const columntype num);     //传入一个MARKPOINTNUM行3列的二维数组，将其显示在表格第2~4、5~7、8~10列

private slots:
    void on_Button_ChangeOriginalCoordinates_clicked();
    void on_Button_ChangeBackGround_clicked();
    void on_Button_Observe_clicked();
    void Updatatabel_slot();

    void on_Button_ParaRevise_clicked();

private:
    Ui::Disp2DWindow *ui;
    IFE* ife = nullptr;         //逆有限单元类，所有有关逆有限单元的计算都在该类中
    ParaReviseWindow* parawindow = nullptr;
};

#endif // DISP2DWINDOW_H
