#ifndef DISP3SELECT_H
#define DISP3SELECT_H

#include <QDialog>
#include <disp3dmodel.h>

namespace Ui {
class disp3dselect;
}

class disp3dselect : public QDialog
{
    Q_OBJECT

public:
    explicit disp3dselect(QWidget *parent = nullptr);
    ~disp3dselect();

public:
    int inicord;
    int selected_node;
    double x_coordination;
    double x_coordination_deformed;
    double y_coordination;
    double y_coordination_deformed;
    double z_coordination;
    double z_coordination_deformed;
    bool flag;//默认为true,添加节点界面；为false,编辑界面，由flagAnum()函数设置

public:
    void GetDisplayModelPtr(disp3dmodel* ptr){m_disp3dmodel = ptr;}
    void InitSelectPoint();
    void flagAnum(bool,int);//第一个为判断添加和修改的标志，第二个为被覆盖的结点，用于修改关键点

signals:
    void selectNodeInf(int num,double x,double y,double z);                       //添加关键点，传递节点信息
    void selectNodeInf_deformed(int num,double x,double y,double z);              //添加关键点，传递变形后节点信息
    void editNodeInf(int ini,int num,double x,double y,double z);                 //编辑关键点，传递节点信息
    void editNodeInf_deformed(int ini, int num, double x, double y, double z);    //编辑关键点，传递变形后节点信息

private slots:
    void on_pbtn_SelectSet_clicked();
    void on_pbtn_FindPoint_clicked();
    void on_cbox_SelectPoint_currentIndexChanged(const QString &arg1);
    void on_pbtn_SelectCancel_clicked();

protected:
    void mouseMoveEvent(QMouseEvent *e);
    void mousePressEvent(QMouseEvent *e);

private:
    Ui::disp3dselect *ui;

    QPoint p;
    disp3dmodel * m_disp3dmodel = nullptr;
};

#endif // DISP3SELECT_H
