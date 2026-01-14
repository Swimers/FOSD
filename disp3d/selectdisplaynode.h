#ifndef SELECTDISPLAYNODE_H
#define SELECTDISPLAYNODE_H

#include <QDialog>
#include "displaymodel.h"

namespace Ui {
class SelectDisplayNode;
}

class SelectDisplayNode : public QDialog
{
    Q_OBJECT

public:
    explicit SelectDisplayNode(QWidget *parent = nullptr);
    ~SelectDisplayNode();
    int selected_node;
    double x_coordination;
    double x_coordination_deformed;
    double y_coordination;
    double y_coordination_deformed;
    double z_coordination;
    double z_coordination_deformed;
    bool flag;//默认为true,添加节点界面；为false,编辑界面，由flagAnum()函数设置
    int inicord;
    void flagAnum(bool,int);//第一个为判断添加和修改的标志，第二个为被覆盖的结点，用于修改关键点
    void GetDisplayModelPtr(displayModel* dd)
    {
        displaymodel = dd;
    }
    void initialize();

    void addNode(int num){
        arg = QString::number(num);
        on_ok_clicked();
    }

signals:
    void seleNodeInf(int num,double x,double y,double z);//添加关键点，传递节点信息
    void seleNodeInf_deformed(int num,double x,double y,double z);//添加关键点，传递变形后节点信息
    void editNodeInf(int ini,int num,double x,double y,double z);//编辑关键点，传递节点信息
    void editNodeInf_deformed(int ini, int num, double x, double y, double z);//编辑关键点，传递变形后节点信息

private slots:
    void on_ok_clicked();//会通过判断flag决定是添加还是编辑
    void on_btnFind_clicked();
    void on_comboBox_currentIndexChanged(const QString &arg1);

private:
    QString arg;
    Ui::SelectDisplayNode *ui;
    displayModel* displaymodel = nullptr;
};

#endif // SELECTDISPLAYNODE_H
