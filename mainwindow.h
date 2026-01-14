#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "demowindow.h"
#include "disp2dwindow.h"
//#include "disp3dwindow.h"
#include <QButtonGroup>
#include "disp3dmainwindow.h"


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(const QString& privilege,QWidget *parent = nullptr);
    ~MainWindow();
    void SetPrivilege(QString& privilege){this->privilege = privilege;qDebug()<<"权限等级"<<privilege;}

private slots:
    void on_tbtn_Exit_clicked();

    void on_tbtn_userInfo_clicked();

private:
    void SetLogWindow();        //设置登录界面
    void initializeWidget();    //初始化主界面,建立qstackedWidget和qtoolbutton联系

private:
    Ui::MainWindow *ui;
    QString privilege;
    QButtonGroup btnGroup;
    DemoWindow* demo;
    Disp2DWindow* disp2d;
    disp3dMainWindow* disp3d;

};

#endif // MAINWINDOW_H
