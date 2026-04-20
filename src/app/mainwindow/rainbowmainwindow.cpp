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

    InitMainwindow_refresh();
}

RainbowMainWindow::~RainbowMainWindow()
{
    delete ui;
    if(m_liveflag){
        m_liveflag->store(false, std::memory_order_release);
    }
}

bool RainbowMainWindow::ParseOneFrame(const QString& line)
{
    // 按空白符切分，自动适应多个空格/tab
    QStringList parts = line.split(QRegExp("\\s+"), QString::SkipEmptyParts);

    // 1(时间戳) + 12(strian) + 12(temper) + 16(free) = 41
    const int expectedCount = 41;
    if (parts.size() != expectedCount)
    {
        qDebug() << "Column count error, got:" << parts.size()
                 << " expected:" << expectedCount;
        return false;
    }

    int index = 0;

    // 第1列：时间戳
    QString timestamp = parts[index++];
    qDebug() << "timestamp =" << timestamp;

    // 如果你的 m_testData 里有时间戳字段，可以保存
    // m_testData.timestamp = timestamp;

    bool ok = false;

    // 12列 strian
    for (int i = 0; i < 12; ++i)
    {
        double value = parts[index++].toDouble(&ok);
        if (!ok)
        {
            qDebug() << "Parse strian failed at index:" << i;
            return false;
        }
        m_testData.strian[i] = value;
    }

    // 12列 temper
    for (int i = 0; i < 12; ++i)
    {
        double value = parts[index++].toDouble(&ok);
        if (!ok)
        {
            qDebug() << "Parse temper failed at index:" << i;
            return false;
        }
        m_testData.temper[i] = value;
    }

    // 16列 free
    for (int i = 0; i < 16; ++i)
    {
        double value = parts[index++].toDouble(&ok);
        if (!ok)
        {
            qDebug() << "Parse free failed at index:" << i;
            return false;
        }
        m_testData.free[i] = value;
    }

    return true;
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

    m_testData.strian[0]    =     1.34753e-07 ;
    m_testData.strian[1]    =     -7.54395e-09;
    m_testData.strian[2]    =     2.16337e-05 ;
    m_testData.strian[3]    =     -0.0001712  ;
    m_testData.strian[4]    =     1.62092e-06 ;
    m_testData.strian[5]    =     0.000600131 ;
    m_testData.strian[6]    =     -9.6297e-06 ;
    m_testData.strian[7]    =     1e-05       ;
    m_testData.strian[8]    =     0.000274251 ;
    m_testData.strian[9]    =     -1.46806e-06;
    m_testData.strian[10]   =     -4.28546e-05;
    m_testData.strian[11]   =     2.63123e-05 ;
    m_testData.temper[0]    =     13.4199     ;
    m_testData.temper[1]    =     14.1432     ;
    m_testData.temper[2]    =     -26911.6    ;
    m_testData.temper[3]    =     -71783.3    ;
    m_testData.temper[4]    =     25.878      ;
    m_testData.temper[5]    =     23.6607     ;
    m_testData.temper[6]    =     -25655.5    ;
    m_testData.temper[7]    =     -71701.5    ;
    m_testData.temper[8]    =     -58513.5    ;
    m_testData.temper[9]    =     -64028.1    ;
    m_testData.temper[10]   =     18.9787     ;
    m_testData.temper[11]   =     20.5338     ;
    m_testData.free[0]      =     -0.00612021 ;
    m_testData.free[1]      =     0.000293988 ;
    m_testData.free[2]      =     -0.127511   ;
    m_testData.free[3]      =     1.81128     ;
    m_testData.free[4]      =     35.8849     ;
    m_testData.free[5]      =     -0.00160754 ;
    m_testData.free[6]      =     -0.0115843  ;
    m_testData.free[7]      =     0.000532961 ;
    m_testData.free[8]      =     -0.0073519  ;
    m_testData.free[9]      =     0.00220374  ;
    m_testData.free[10]     =     1.03886     ;
    m_testData.free[11]     =     13.4526     ;
    m_testData.free[12]     =     30.7584     ;
    m_testData.free[13]     =     0.00769909  ;
    m_testData.free[14]     =     -0.00150323 ;
    m_testData.free[15]     =     0.00387649  ;

    connect(m_testTimer, &QTimer::timeout, this, [this](){m_rainbowIFE->GetDataTest(m_testData);});
    m_testTimer->start();
}

void RainbowMainWindow::InitMainwindow_refresh()
{
    m_rainbowIFE = new rainbowIFE();
    m_disp3dwindow = new disp3dwindow();
    m_disp3dwindow->GetRainbowIFEPtr(m_rainbowIFE);

    QVBoxLayout* layout = new QVBoxLayout();
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(m_disp3dwindow);
    ui->centralContentWidget->setLayout(layout);

    // =========================
    // txt逐帧伪注入测试
    // =========================
    m_testTimer = new QTimer(this);
    m_testTimer->setInterval(25);                  // 25 ms发送一帧
    m_testTimer->setTimerType(Qt::PreciseTimer);

    // 打开数据文件
    QString filePath = "C:/Users/Swimers/Desktop/蒲城/采样数据/2026-04-18data.txt";   // 改成你的实际路径
    m_dataFile = new QFile(filePath, this);

    if (!m_dataFile->open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug() << "Open file failed:" << filePath;
        return;
    }

    m_dataStream = new QTextStream(m_dataFile);

    connect(m_testTimer, &QTimer::timeout, this, [this]()
    {
        if (m_dataStream == nullptr)
            return;

        // 文件结束：停止定时器
        if (m_dataStream->atEnd())
        {
            qDebug() << "Data injection finished.";
            m_testTimer->stop();
            m_dataFile->close();
            return;
        }

        QString line = m_dataStream->readLine().trimmed();

        // 跳过空行
        if (line.isEmpty())
            return;

        // 解析当前帧
        if (!ParseOneFrame(line))
        {
            qDebug() << "Parse frame failed:" << line;
            return;
        }

        // 向后续程序注入一帧数据
        m_rainbowIFE->GetDataTest(m_testData);
    });

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
