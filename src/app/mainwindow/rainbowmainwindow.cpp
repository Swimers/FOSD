#include "rainbowmainwindow.h"
#include "ui_rainbowmainwindow.h"

#include <QScreen>
#include <QMouseEvent>
#include <QTimer>

#include <QFile>
#include <QTextStream>
#include <QTimer>
#include <QStringList>

#include <teststorge.h>

RainbowMainWindow::RainbowMainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::RainbowMainWindow)
{
    ui->setupUi(this);

    m_liveflag = std::make_shared<std::atomic_bool>(true);
    Flag_WindowFull = false;

    setWindowIcon(QIcon(":/icon/Resource/icon/favicon.ico"));
    setWindowFlags(Qt::FramelessWindowHint | windowFlags());

    InitMainwindow();

    // 静态数据测试
    //m_testInjector = new TestDataInjector(m_rainbowIFE, this);
    //m_testInjector->startStaticInjection(25);
    // 动态注入
    QString filePath = "C:/Users/User/Desktop/蒲城/数据处理/2026-04-18data.txt";
    m_testInjector = new TestDataInjector(m_rainbowIFE, this);
    m_testInjector->startTxtInjection(filePath, 25);
}

RainbowMainWindow::~RainbowMainWindow()
{
    delete ui;
    if(m_liveflag){
        m_liveflag->store(false, std::memory_order_release);
    }
}

void RainbowMainWindow::InitMainwindow()
{
    m_rainbowIFE = new rainbowIFE();
    m_disp3dwindow = new disp3dwindow();

    m_disp3dwindow->GetRainbowIFEPtr(m_rainbowIFE);

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
