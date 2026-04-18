#include "rainbowmainwindow.h"
#include "ui_rainbowmainwindow.h"

#include <QScreen>
#include <QMouseEvent>
#include <QTimer>

#include <teststorge.h>
#include <testb3pocket.h>

static float bigEndianToHostFloat(float value) {
    quint32 tmp;
    memcpy(&tmp, &value, sizeof(float));
    tmp = qFromBigEndian(tmp);
    float result;
    memcpy(&result, &tmp, sizeof(float));
    return result;
}

RainbowMainWindow::RainbowMainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::RainbowMainWindow)
{
    ui->setupUi(this);

    m_liveflag = std::make_shared<std::atomic_bool>(true);

    // 使用绝对路径设置窗口图标
    setWindowIcon(QIcon(":/icon/Resource/icon/favicon.ico"));
    setWindowFlags(Qt::FramelessWindowHint | windowFlags());

    InitMainwindow();
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

    //20260411
    m_testData.strian[0]  =     3.2793e-07  ;
    m_testData.strian[1]  =     2.03982e-05 ;
    m_testData.strian[2]  =     3.24476e-05 ;
    m_testData.strian[3]  =     5.65612e-05 ;
    m_testData.strian[4]  =     2e-05       ;
    m_testData.strian[5]  =     -1.07654e-05;
    m_testData.strian[6]  =     2.80922e-06 ;
    m_testData.strian[7]  =     -8.16807e-05;
    m_testData.strian[8]  =     -0.000197924;
    m_testData.strian[9]  =     2e-05       ;
    m_testData.strian[10] =     -9.02709e-06;
    m_testData.strian[11] =     0.00026     ;
    m_testData.temper[0]  =     13.781      ;
    m_testData.temper[1]  =     15.5462     ;
    m_testData.temper[2]  =     -26911.6    ;
    m_testData.temper[3]  =     -71783.3    ;
    m_testData.temper[4]  =     31.2783     ;
    m_testData.temper[5]  =     27.6018     ;
    m_testData.temper[6]  =     -25655.5    ;
    m_testData.temper[7]  =     -71701.5    ;
    m_testData.temper[8]  =     -58513.5    ;
    m_testData.temper[9]  =     -64028.1    ;
    m_testData.temper[10] =     20.0446     ;
    m_testData.temper[11] =     21.329      ;
    m_testData.free[0]    =     0.0250854   ;
    m_testData.free[1]    =     -0.00346172 ;
    m_testData.free[2]    =     -1.14618    ;
    m_testData.free[3]    =     -17.8288    ;
    m_testData.free[4]    =     -95.7194    ;
    m_testData.free[5]    =     0.0213697   ;
    m_testData.free[6]    =     -0.00414997 ;
    m_testData.free[7]    =     -0.00295142 ;
    m_testData.free[8]    =     -0.00503866 ;
    m_testData.free[9]    =     0.000587748 ;
    m_testData.free[10]   =     0.205327    ;
    m_testData.free[11]   =     2.59423     ;
    m_testData.free[12]   =     17.6974     ;
    m_testData.free[13]   =     -0.00363905 ;
    m_testData.free[14]   =     0.00237036  ;
    m_testData.free[15]   =     8.99147e-05 ;



    connect(m_testTimer, &QTimer::timeout, this, [this](){m_rainbowIFE->GetDataTest(m_testData);});
    m_testTimer->start();
    //QString b3 = "00 00 00 b3 ac c5 27 b7 ac c5 a7 38 a6 9b c4 3a 17 b7 51 3a 9c 50 88 b8 ed 0d be ba 49 d5 ae b7 ac c5 27 37 a4 d5 e9 39 38 89 cb b5 00 a2 90 b8 51 1c 02 b8 7a c4 34 41 50 49 37 41 d0 87 db 41 91 1e d8 41 cf f3 c8 41 ef da d3 41 0f 6f c8 c6 c4 0a 8c c7 7a c4 b4 41 41 68 d2 41 3d cf 55 41 22 52 41 41 3a de 25 bc 30 3e 19 3a b8 d2 1d be fc 8b 64 40 a5 e3 6b 42 42 6f 8a bb 68 ca 99 bc 34 19 87 3a 5d ef 30 bc 8e 26 87 bb 48 ef 36 c0 44 ae cf c1 5e 00 c0 42 99 48 ea bc fe 47 5a bd a1 0c 03 bc ba 00 00 00";

    //降低
    //QString b3 = "00 00 00 b3 ac c5 27 b7 ac c5 a7 38 a6 9b c4 3a 17 b7 51 3a 9c 50 88 b8 ed 0d be ba e6 8c d8 b7 ac c5 27 37 b3 18 f1 39 84 e8 0e b5 b4 f5 93 b8 0d 93 ff b7 d9 8b 3a 41 50 49 37 41 9e 09 e0 41 91 1e d8 41 3d cf cb 41 ef da d3 41 0f 6f c8 c6 c4 0a 8c c7 f8 49 b8 41 95 0b d2 41 92 24 59 41 22 52 41 41 b4 ed 43 bc d8 92 61 3a c0 b4 84 bd 98 c9 a1 40 9e 6e 85 42 d7 97 ac bb c5 6b 9d bc 20 df ad 3a 5d ef 30 bc 8e 26 87 bb 48 ef 36 c0 44 ae cf c1 5e 00 c0 42 99 48 ea bc fe 47 5a bd a1 0c 03 bc ba 00 00 00";

    //抬升
    //QString b3 = "00 00 00 b3 ac c5 27 b7 ac c5 a7 38 a6 9b c4 3a 17 b7 51 3a 9c 50 88 b8 ed 0d be ba 49 d5 ae b7 ac c5 27 37 a4 d5 e9 39 46 3f 86 b5 a9 d0 90 b8 51 1c 02 b8 7a c4 34 41 50 49 37 41 d0 87 db 41 91 1e d8 41 cf f3 c8 41 ef da d3 41 0f 6f c8 c6 c4 0a 8c c7 7a c4 b4 41 41 68 d2 41 3d cf 55 41 22 52 41 41 aa 5f 25 bc fc cd 16 3a d8 82 22 be 58 fb 60 40 10 6a 6b 42 33 c8 89 bb 4e cd 99 bc 8e 16 85 3a 5d ef 30 bc 8e 26 87 bb 48 ef 36 c0 44 ae cf c1 5e 00 c0 42 99 48 ea bc fe 47 5a bd a1 0c 03 bc ba 00 00 00";

//    QPointer<RainbowMainWindow> self(this);
//    auto aliveFlag = m_liveflag;

//    if(!aliveFlag || !self || !aliveFlag->load(std::memory_order_acquire)){
//            return;
//        }
//    else {
//        testb3pocket *m_testb3pocket = new testb3pocket();
//        // m_testData = m_testb3pocket->AnalysisB3(b3);
//        for (int i = 0; i < 16; i++) {
//            m_testData2.free[i] = m_testData.free[i];
//        }
//        connect(m_testTimer, &QTimer::timeout, this, [this](){m_rainbowIFE->GetDataTest(m_testData2);});
//        m_testTimer->start();
//    }
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
