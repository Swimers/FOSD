#include "rainbowmainwindow.h"
#include "ui_rainbowmainwindow.h"

#include <QScreen>
#include <QMouseEvent>
#include <QTimer>

#include <teststorge.h>
#include <testb3pocket.h>

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


    //测试存储线程部分代码
    m_testTimer = new QTimer(this);  // 指定父对象，自动管理内存
    m_testTimer->setInterval(25);    // 设置定时器间隔为50毫秒
    m_testTimer->setTimerType(Qt::PreciseTimer);  // 高精度定时器（可选，提升精度）
//    m_testData.flag = 179;
//    m_testData.strian[0] = -9.999999747378752e-06;
//    m_testData.strian[1] = 7.999999797903001e-05;
//    m_testData.strian[2] = 0.001500000013038516;
//    m_testData.strian[3] = 0.0007999999797903001;
//    m_testData.strian[4] = -6.500000017695129e-05;
//    m_testData.strian[5] = -0.001449999981559813;
//    m_testData.strian[6] = 0.00019999999494757503;
//    m_testData.strian[7] = 9.999999747378752e-06;
//    m_testData.strian[8] = 0.0005512720090337098;
//    m_testData.strian[9] = 2.6296088435628917e-06;
//    m_testData.strian[10] = -0.0002500000118743628;
//    m_testData.strian[11] = -7.755196747893933e-06;
//    m_testData.temper[0] = 7.7083330154418945;
//    m_testData.temper[1] = 6.697247505187988;
//    m_testData.temper[2] = 9.9775583267211914;
//    m_testData.temper[3] = 8.631840705871582;
//    m_testData.temper[4] = 7.194029808044434;
//    m_testData.temper[5] = 5.669642925262451;
//    m_testData.temper[6] = -25655.529296875;
//    m_testData.temper[7] = -71701.53125;
//    m_testData.temper[8] = 7.633928298950195;
//    m_testData.temper[9] = 7.8216705322265625;
//    m_testData.temper[10] = 12.127976417541504;
//    m_testData.temper[11] = 16.012659072875977;
//    m_testData.free[0] = 0.0891408696770668;
//    m_testData.free[1] = -0.018775571137666702;
//    m_testData.free[2] = -8.148399353027344;
//    m_testData.free[3] = -101.8877182006836;
//    m_testData.free[4] = -318.6768798828125;
//    m_testData.free[5] = 0.0598243810236454;
//    m_testData.free[6] = -0.036320462822914124;
//    m_testData.free[7] = -0.020937006920576096;
//    m_testData.free[8] = -0.010799256153404713;
//    m_testData.free[9] = -0.004124469123780727;
//    m_testData.free[10] = -2.8583545684814453;
//    m_testData.free[11] = -25.96009063720703;
//    m_testData.free[12] = 96.00071716308594;
//    m_testData.free[13] = -0.028599070385098457;
//    m_testData.free[14] = -0.05329131335020065;
//    m_testData.free[15] = -0.007998616434633732;

    //异常数据测试
//    m_testData.strian[0] = 1.09728e-06;
//    m_testData.strian[1] = -2.33862e-07;
//    m_testData.strian[2] = -9.42611e-05;
//    m_testData.strian[3] = -4.58036e-05;
//    m_testData.strian[4] = 2e-05;
//    m_testData.strian[5] = 7.7664e-05;
//    m_testData.strian[6] = 7.95693e-06;
//    m_testData.strian[7] = 1e-05;
//    m_testData.strian[8] = 5.2634e-05;
//    m_testData.strian[9] = 2.0811e-06;
//    m_testData.strian[10] = -1.02748e-05;
//    m_testData.strian[11] = -1.10789e-06;
//    m_testData.free[0] = 6.74107;
//    m_testData.free[1] = 5.0;
//    m_testData.free[2] = 7.87253;
//    m_testData.free[3] = 5.89552;
//    m_testData.free[4] = 6.67164;
//    m_testData.free[5] = 5.16369;
//    m_testData.free[6] = -25655.5;
//    m_testData.free[7] = -71701.5;
//    m_testData.free[8] = 8.31845;
//    m_testData.free[9] = 6.62528;
//    m_testData.free[10] = 11.6071;
//    m_testData.free[11] = 12.2702;
//    m_testData.free[12] = -0.00205191;
//    m_testData.free[13] = 0.000894042;
//    m_testData.free[14] = 0.489451;
//    m_testData.free[15] = 5.16377;
//    m_testData.temper[0] = 4.07445;
//    m_testData.temper[1] = -0.00033295;
//    m_testData.temper[2] = 0.00411405;
//    m_testData.temper[3] = 0.00129352;
//    m_testData.temper[4] = 0.000452859;
//    m_testData.temper[5] = -0.000351004;
//    m_testData.temper[6] = -0.200937;
//    m_testData.temper[7] = -1.94687;
//    m_testData.temper[8] = 0.609272;
//    m_testData.temper[9] = 0.000575149;
//    m_testData.temper[10] = -0.00242432;
//    m_testData.temper[11] = -0.000431414;
    //正常对比
//    m_testData.strian[0] = -1e-05;
//    m_testData.strian[1] = 8e-05;
//    m_testData.strian[2] = 0.0015;
//    m_testData.strian[3] = 0.0008;
//    m_testData.strian[4] = -6.5e-05;
//    m_testData.strian[5] = -0.00145;
//    m_testData.strian[6] = -7.95693e-06;
//    m_testData.strian[7] = 1e-05;
//    m_testData.strian[8] = 0.000209151;
//    m_testData.strian[9] = 2.83933e-06;
//    m_testData.strian[10] = -3.32452e-05;
//    m_testData.strian[11] = 3.32366e-06;
//    m_testData.free[0] = -0.00534613;
//    m_testData.free[1] = 0.00027775;
//    m_testData.free[2] = -0.0919774;
//    m_testData.free[3] = 1.59952;
//    m_testData.free[4] = 30.0168;
//    m_testData.free[5] = -0.00198382;
//    m_testData.free[6] = -0.00878275;
//    m_testData.free[7] = 0.000397388;
//    m_testData.free[8] = -0.0107993;
//    m_testData.free[9] = -0.00412447;
//    m_testData.free[10] = -2.85835;
//    m_testData.free[11] = -25.9601;
//    m_testData.free[12] = 96.0007;
//    m_testData.free[13] = -0.0285991;
//    m_testData.free[14] = -0.0532913;
//    m_testData.free[15] = -0.00799862;
//    m_testData.temper[0] = 6.05655;
//    m_testData.temper[1] = 5;
//    m_testData.temper[2] = 7.75583;
//    m_testData.temper[3] = 7.21393;
//    m_testData.temper[4] = 5.59701;
//    m_testData.temper[5] = 5.25298;
//    m_testData.temper[6] = -25655.5;
//    m_testData.temper[7] = -71701.5;
//    m_testData.temper[8] = 7.5744;
//    m_testData.temper[9] = 6.62528;
//    m_testData.temper[10] = 10.5506;
//    m_testData.temper[11] = 14.2964;

    QString b3 = "00 00 00 b3 ac c5 27 b7 ac c5 a7 38 a6 9b c4 3a 17 b7 51 3a 9c 50 88 b8 ed 0d be ba 17 b7 51 39 ac c5 27 37 9a d2 f1 39 b5 db dc 35 6f 12 83 b9 11 0d 10 b7 8a a3 6f 41 20 d1 4a 41 cf 73 85 41 b3 a6 74 41 38 1f 57 41 f0 7c 4a 41 0f 6f c8 c6 c4 0a 8c c7 8a 03 5d 41 3f 3f 5f 41 49 12 ba 41 4a c6 8a c7 a6 62 b9 3d e0 e5 9b bc bc 16 04 c1 f2 19 cf c2 14 bfa 3 c3 78 d2 76 3d 59 13 07 bd 1d 05 b0 bc 5d ef 30 bc 8e 26 87 bb 48 ef 36 c0 44 ae cf c1 5e 00 c0 42 99 48 ea bc fe 47 5a bd a1 0c 03 bc ba 00 00 00";

    testb3pocket *m_testb3pocket = new testb3pocket();

    m_testData = m_testb3pocket->AnalysisB3(b3);

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
