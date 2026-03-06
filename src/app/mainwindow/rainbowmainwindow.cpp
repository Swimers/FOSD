#include "rainbowmainwindow.h"
#include "ui_rainbowmainwindow.h"

#include <QScreen>
#include <QMouseEvent>
#include <QTimer>

#include <teststorge.h>

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
    m_rainbowIFE = new rainbowIFE();
    m_disp3dwindow = new disp3dwindow();
    m_disp3dwindow->GetRainbowIFEPtr(m_rainbowIFE);
    QVBoxLayout* layout = new QVBoxLayout();
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(m_disp3dwindow);
    ui->centralContentWidget->setLayout(layout);

}

void RainbowMainWindow::DataTest()
{
    //测试存储线程部分代码
    m_testTimer = new QTimer(this);  // 指定父对象，自动管理内存
    m_testTimer->setInterval(25);    // 设置定时器间隔为50毫秒
    m_testTimer->setTimerType(Qt::PreciseTimer);  // 高精度定时器（可选，提升精度）
    m_testData.strian[0] = -9.999999747378752e-06;
    m_testData.strian[1] = 7.999999797903001e-05;
    m_testData.strian[2] = 0.001500000013038516;
    m_testData.strian[3] = 0.0007999999797903001;
    m_testData.strian[4] = -6.500000017695129e-05;
    m_testData.strian[5] = -0.001449999981559813;
    m_testData.strian[6] = 0.00019999999494757503;
    m_testData.strian[7] = 9.999999747378752e-06;
    m_testData.strian[8] = 0.0005512720090337098;
    m_testData.strian[9] = 2.6296088435628917e-06;
    m_testData.strian[10] = -0.0002500000118743628;
    m_testData.strian[11] = -7.755196747893933e-06;
    m_testData.temper[0] = 7.7083330154418945;
    m_testData.temper[1] = 6.697247505187988;
    m_testData.temper[2] = 9.9775583267211914;
    m_testData.temper[3] = 8.631840705871582;
    m_testData.temper[4] = 7.194029808044434;
    m_testData.temper[5] = 5.669642925262451;
    m_testData.temper[6] = -25655.529296875;
    m_testData.temper[7] = -71701.53125;
    m_testData.temper[8] = 7.633928298950195;
    m_testData.temper[9] = 7.8216705322265625;
    m_testData.temper[10] = 12.127976417541504;
    m_testData.temper[11] = 16.012659072875977;
    m_testData.free[0] = 0.0891408696770668;
    m_testData.free[1] = -0.018775571137666702;
    m_testData.free[2] = -8.148399353027344;
    m_testData.free[3] = -101.8877182006836;
    m_testData.free[4] = -318.6768798828125;
    m_testData.free[5] = 0.0598243810236454;
    m_testData.free[6] = -0.036320462822914124;
    m_testData.free[7] = -0.020937006920576096;
    m_testData.free[8] = -0.010799256153404713;
    m_testData.free[9] = -0.004124469123780727;
    m_testData.free[10] = -2.8583545684814453;
    m_testData.free[11] = -25.96009063720703;
    m_testData.free[12] = 96.00071716308594;
    m_testData.free[13] = -0.028599070385098457;
    m_testData.free[14] = -0.05329131335020065;
    m_testData.free[15] = -0.007998616434633732;
    connect(m_testTimer, &QTimer::timeout, this, [this](){m_rainbowIFE->GetDataTest(m_testData);});
    m_testTimer->start();
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
