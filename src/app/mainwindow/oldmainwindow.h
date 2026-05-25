#ifndef OLDMAINWINDOW_H
#define OLDMAINWINDOW_H

#include <QMainWindow>
#include <QButtonGroup>

#include <userregister.h>
#include <usermanage.h>
#include <connectwindow.h>
#include <disp2dwindow.h>
#include <disp3dwindow.h>
#include <rainbowife.h>

namespace Ui {
class OldMainWindow;
}

class OldMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit OldMainWindow(const QString& privilege,QWidget *parent = nullptr);
    ~OldMainWindow();

public:
    bool Flag_Style;
    bool Flag_WindowFull;

private slots:
    void on_pbtn_TurnOff_clicked();
    void on_pbtn_UserManage_clicked();
    void onButtonGroupClicked(int btn_id);
    void switchUiStyle(bool flag);
    void on_pbtn_Maximize_clicked();
    void on_pbtn_Minimize_clicked();
    void on_pbtn_Full_clicked();

private:
    void InitOldMainWindow();

protected:
    void mouseMoveEvent(QMouseEvent *e);
    void mousePressEvent(QMouseEvent *e);

signals:
    void setUiBlacked();
    void setUiWhited();

private:
    Ui::OldMainWindow  * ui;
    UserManage      * m_Manage;
    connectwindow   * m_connectwindow;
    disp2dwindow    * m_disp2dwindow;
    disp3dwindow    * m_disp3dwindow;

    QPoint p;
    QString originalStyleSheets[3];
    QString privilege;
};

#endif // OLDMAINWINDOW_H
