#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
<<<<<<< Updated upstream
#include <QButtonGroup>

#include <userregister.h>
#include <usermanage.h>
#include <connectwindow.h>
#include <disp2dwindow.h>
#include <disp3dwindow.h>
=======
#include "demowindow.h"
#include "disp2dwindow.h"
//#include "disp3dwindow.h"
#include <QButtonGroup>
#include "disp3dmainwindow.h"
>>>>>>> Stashed changes


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
<<<<<<< Updated upstream
    void on_pbtn_TurnOff_clicked();
    void on_pbtn_UserManage_clicked();
    void onButtonGroupClicked(int btn_id);
    void switchUiStyle(bool flag);
    void on_pbtn_Maximize_clicked();
    void on_pbtn_Minimize_clicked();
    void on_pbtn_Full_clicked();
=======
    void on_tbtn_Exit_clicked();

    void on_tbtn_userInfo_clicked();

    void on_tbtn_saveConfig_clicked();
>>>>>>> Stashed changes

private:
    void SetLogWindow();        //设置登录界面
    void initializeWidget();    //初始化主界面,建立qstackedWidget和qtoolbutton联系

private:
<<<<<<< Updated upstream
    Ui::MainWindow  * ui;
    UserManage      * m_Manage;
    connectwindow   * m_connectwindow;
    disp2dwindow    * m_disp2dwindow;
    disp3dwindow    * m_disp3dwindow;

    QPoint p;
    QString originalStyleSheets[3];
=======
    Ui::MainWindow *ui;
>>>>>>> Stashed changes
    QString privilege;
    QButtonGroup btnGroup;
    DemoWindow* demo;
    Disp2DWindow* disp2d;
    disp3dMainWindow* disp3d;

};

#endif // MAINWINDOW_H
