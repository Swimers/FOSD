#include "rainbowmainwindow.h"
#include "ui_rainbowmainwindow.h"

#include <QScreen>
#include <QMouseEvent>

RainbowMainWindow::RainbowMainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::RainbowMainWindow)
{
    ui->setupUi(this);

    // 使用绝对路径设置窗口图标
    setWindowIcon(QIcon(":/icon/Resource/icon/favicon.ico"));
    setWindowFlags(Qt::FramelessWindowHint | windowFlags());

    InitMainwindow();
}

RainbowMainWindow::~RainbowMainWindow()
{
    delete ui;
}

void RainbowMainWindow::InitMainwindow()
{
    // 创建 IFE 实例（准备接入新的IFE）
    m_rainbowIFE = new rainbowIFE();

    // 创建3D窗口
    m_disp3dwindow = new disp3dwindow();

    // 将IFE指针传递给3D窗口
    m_disp3dwindow->GetRainbowIFEPtr(m_rainbowIFE);

    // 将3D窗口添加到中央区域
    QVBoxLayout* layout = new QVBoxLayout();
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(m_disp3dwindow);
    ui->centralContentWidget->setLayout(layout);
}

void RainbowMainWindow::on_pbtn_TurnOff_clicked()
{
    qApp->quit();
}

void RainbowMainWindow::on_pbtn_Minimize_clicked()
{
    showMinimized();
}

void RainbowMainWindow::on_pbtn_Full_clicked()
{
    Flag_WindowFull = !Flag_WindowFull;
    if(Flag_WindowFull){
        showFullScreen();
    }
    if(!Flag_WindowFull){
        showNormal();
        QScreen *screen = QGuiApplication::primaryScreen();
        if (!screen) return;
        QRect screenGeometry = screen->availableGeometry();
        int x = screenGeometry.center().x() - this->width() / 2;
        int y = screenGeometry.center().y() - this->height() / 2;
        this->move(x, y);
    }
}

void RainbowMainWindow::on_pbtn_Maximize_clicked()
{
    close();
}

void RainbowMainWindow::mouseMoveEvent(QMouseEvent *e)
{
    if(!Flag_WindowFull){
        if(e->buttons() & Qt::LeftButton){
            move(e->globalPos() - p);       //移到左上角
        }
    }
}

void RainbowMainWindow::mousePressEvent(QMouseEvent *e)
{
    if(!Flag_WindowFull){
        if(e->button() == Qt::LeftButton){
            p = e->globalPos() - this->frameGeometry().topLeft();   //求坐标差值:当前点击坐标-窗口左上角坐标
        }
    }
}
