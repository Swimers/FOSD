#ifndef DISP3DMAINWINDOW_H
#define DISP3DMAINWINDOW_H

#include <QMainWindow>
#include "displaymodel.h"

#include "vtkRendererCollection.h"
#include "qtreewidget.h"
#include "vtkCollection.h"
#include "vtkCollectionIterator.h"
#include "corddisplayset.h"
#include "IFE.h"
#include <vtkTextProperty.h>
#include "modelcalculationthread.h"
#include <QLabel>



namespace Ui {
class disp3dMainWindow;
}

class disp3dMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit disp3dMainWindow(const QString& privilege,QWidget *parent = nullptr);
    ~disp3dMainWindow();
    void Init();
    void GetIFEPtr(IFE* externife)
    {
        ife = externife;
    }
    std::tuple<QLabel *,QLabel *,QLabel *,QLabel *> sendLabelptr()
    {return std::make_tuple(label0, label1, label2, label3);}

private:
    int fps; //刷新频率

private slots:
    void on_btnLoadModel_clicked();
    void ModelDockShow();
    void InforDockShow();
    void on_btnChangeView_clicked();
    void on_checkBoxEdge_clicked(bool checked);
    void on_checkBoxAxis_clicked(bool checked);
    void on_btnAddPoint_clicked();
    void on_btkSetSize_clicked();

    void on_actionStart_triggered();
    void on_actionStop_triggered();
    void on_actionReset_triggered();
    void handleTimeOut_WJBGL();
    void on_comboBoxHz_currentIndexChanged(const QString &arg1);

    void on_btkDispAnalysis_clicked();

protected slots:
    void deal_addcord(int,double,double,double);
    void deal_delecord(int);
    void deal_editcord(int,int,double,double,double);


private:
    Ui::disp3dMainWindow *ui;
    displayModel* displaymodel = nullptr;
    QTimer *timer_disp3d = nullptr;
    CordDisplaySet *CordDisplaySet1 = nullptr;
    ModelCalculationThread *calculationthread = nullptr; // 模型计算线程对象
    IFE* ife = nullptr;         //逆有限单元类，所有有关逆有限单元的计算都在该类中
    QLabel *label0;
    QLabel *label1;
    QLabel *label2;
    QLabel *label3;
};

#endif // DISP3DMAINWINDOW_H
