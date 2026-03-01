#ifndef RAINBOWMAINWINDOW_H
#define RAINBOWMAINWINDOW_H

#include <QMainWindow>

#include <disp3dwindow.h>
#include <rainbowife.h>

namespace Ui {
class RainbowMainWindow;
}

class RainbowMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit RainbowMainWindow(QWidget *parent = nullptr);
    ~RainbowMainWindow();

public:
    bool Flag_Style;
    bool Flag_WindowFull;

private slots:
    void on_pbtn_TurnOff_clicked();
    void on_pbtn_Minimize_clicked();
    void on_pbtn_Full_clicked();
    void on_pbtn_Maximize_clicked();

private:
    void InitMainwindow();

protected:
    void mouseMoveEvent(QMouseEvent *e);
    void mousePressEvent(QMouseEvent *e);

private:
    Ui::RainbowMainWindow *ui;

    disp3dwindow *m_disp3dwindow;
    rainbowIFE *m_rainbowIFE;

    QPoint p;
};

#endif // RAINBOWMAINWINDOW_H
