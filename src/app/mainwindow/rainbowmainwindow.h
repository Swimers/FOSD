#ifndef RAINBOWMAINWINDOW_H
#define RAINBOWMAINWINDOW_H

#include <QMainWindow>

#include <disp3dwindow.h>
#include <rainbowife.h>

#include <teststorge.h>

#include <QPointer>
#include <atomic>
#include <memory>

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

    std::shared_ptr<std::atomic_bool> m_liveflag;

//    typedef struct NET_SEND_BUFF4 {
//        float strian[12];
//        float temper[12];
//        float free[16];
//    }NET_SEND_BUFF4;
    QTimer* m_testTimer = nullptr;  // 定时器指针
    NET_SEND_BUFF4 m_testData;      // 预定义的测试数据
    NET_SEND_BUFF4 m_testData2;      // 预定义的测试数据

    QPoint p;
};

#endif // RAINBOWMAINWINDOW_H
