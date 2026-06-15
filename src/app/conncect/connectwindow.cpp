#include "connectwindow.h"
#include "ui_connectwindow.h"

//串口通信
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>

#include <QString>
#include <QIODevice>
#include <QTextStream>
#include <QDir>
#include <QDateTime>
#include <QFileDialog>
#include <QTableWidget>
#include <qcustomplot.h>
#include <QNetworkInterface>
#include <QSettings>
#include <QtNetwork>
#include <QThread>

#include "connectdebug.h"
#include "serialtask.h"
#include "tcptask.h"
#include "analyzepackets.h"
#include "global.h"
#include "ConfigManager.h"

connectwindow::connectwindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::connectwindow)
{
    ui->setupUi(this);
    setLED(0,25);

    InitSerial();
    InitUdp();
    InitTcp();
    InitAnalyze();
    InitWindow();
}

connectwindow::~connectwindow()
{
    emit serialPortClose();
    emit udpClose();
    emit tcpClose();

    m_serialThread->quit();
    m_serialThread->wait();
    m_UdpThread->quit();
    m_UdpThread->wait();
    m_TcpThread->quit();
    m_TcpThread->wait();
    m_DataThread->quit();
    m_DataThread->wait();

    // 将对象移回调用线程，避免跨线程 delete 警告
    m_serialtask->moveToThread(nullptr);
    delete m_serialtask;
    delete m_serialThread;
    m_UdpTask->moveToThread(nullptr);
    delete m_UdpTask;
    delete m_UdpThread;
    m_TcpTask->moveToThread(nullptr);
    delete m_TcpTask;
    delete m_TcpThread;
    m_DataTask->moveToThread(nullptr);
    delete m_DataTask;
    delete m_DataThread;

    delete timer0;
    delete model;

    delete ui;
}

void connectwindow::ReadParaConfig()
{
    ConfigManager::instance().load();
    auto &appConfig = ConfigManager::instance().config();
    ui->textEdit_APara->setText(appConfig.strain.aPara);
    ui->textEdit_BPara->setText(appConfig.strain.bPara);
    ui->textEdit_StrainLowPara->setText(appConfig.strain.strainLowPara);
    ui->textEdit_StrainTopPara->setText(appConfig.strain.strainTopPara);
    ui->cbox_CampingFre->setCurrentText(appConfig.camp.getHz);
    ui->cbox_SaveCycle->setCurrentText(appConfig.camp.saveTime);
    on_pbtn_ParaSet_clicked();
}

void connectwindow::WriteParaConfig()
{
    auto &appConfig = ConfigManager::instance().config();
    appConfig.strain.aPara = ui->textEdit_APara->toPlainText();
    appConfig.strain.bPara = ui->textEdit_BPara->toPlainText();
    appConfig.strain.strainLowPara = ui->textEdit_StrainLowPara->toPlainText();
    appConfig.strain.strainTopPara = ui->textEdit_StrainTopPara->toPlainText();
    appConfig.camp.getHz = ui->cbox_CampingFre->currentText();
    appConfig.camp.saveTime = ui->cbox_SaveCycle->currentText();
    ConfigManager::instance().save();
}

void connectwindow::InitCustomPlot(QCustomPlot *customPlot)
{
    ApplyCustomPlotTheme(customPlot, true);

    customPlot->setInteractions(QCP::iRangeDrag|QCP::iRangeZoom| QCP::iSelectAxes |
                                      QCP::iSelectLegend | QCP::iSelectPlottables);
    customPlot->yAxis->setRange(-10000, 40000);
    customPlot->xAxis->setRange(0, 4000);
    customPlot->axisRect()->setRangeDrag(Qt::Horizontal | Qt::Vertical);
    customPlot->axisRect()->setRangeZoomFactor(1.0); // 禁用自动缩放
    customPlot->setNoAntialiasingOnDrag(true); // 提高拖拽性能
    // 连接鼠标滚轮事件到自定义槽函数
    connect(customPlot, SIGNAL(mouseWheel(QWheelEvent*)), this, SLOT(handleWheel(QWheelEvent*)));

    QPen pen1(Qt::blue); // 曲线1的笔（蓝色）
    pen1.setWidth(2);    // 设置笔宽度
    customPlot->addGraph();
    customPlot->graph(0)->setPen(pen1);
    QPen pen2(Qt::black); // 曲线2的笔（黑色）
    pen2.setWidth(2);     // 设置笔宽度
    customPlot->addGraph();
    customPlot->graph(1)->setPen(pen2);
    QPen pen3(Qt::yellow); // 曲线3的笔（黄色）
    pen3.setWidth(2);      // 设置笔宽度
    customPlot->addGraph();
    customPlot->graph(2)->setPen(pen3);
    QPen pen4(Qt::green); // 曲线4的笔（绿色）
    pen4.setWidth(2);     // 设置笔宽度
    customPlot->addGraph();
    customPlot->graph(3)->setPen(pen4);
    QPen pen5(Qt::gray); // 曲线5的笔（灰色）
    pen5.setWidth(2);    // 设置笔宽度
    customPlot->addGraph();
    customPlot->graph(4)->setPen(pen5);
    QPen pen6(Qt::magenta); // 曲线6的笔（洋红色）
    pen6.setWidth(2);       // 设置笔宽度
    customPlot->addGraph();
    customPlot->graph(5)->setPen(pen6);
    QPen pen7(Qt::cyan); // 曲线7的笔（青色）
    pen7.setWidth(2);    // 设置笔宽度
    customPlot->addGraph();
    customPlot->graph(6)->setPen(pen7);
    customPlot->replot();
}

void connectwindow::ApplyCustomPlotTheme(QCustomPlot *customPlot, bool dark)
{
    if (!customPlot) {
        return;
    }

    const QColor widgetBg = dark ? QColor("#302f3a") : QColor("#f4f4f4");
    const QColor plotBg = dark ? QColor("#24232b") : QColor("#ffffff");
    const QColor textColor = dark ? QColor("#f5f5f5") : QColor("#202020");
    const QColor axisColor = dark ? QColor("#cfd3dc") : QColor("#202020");
    const QColor gridColor = dark ? QColor("#4b4a55") : QColor("#d0d0d0");
    const QColor subGridColor = dark ? QColor("#3a3942") : QColor("#e8e8e8");

    customPlot->setBackground(QBrush(widgetBg));
    customPlot->axisRect()->setBackground(QBrush(plotBg));
    customPlot->legend->setBrush(QBrush(plotBg));
    customPlot->legend->setBorderPen(QPen(gridColor));
    customPlot->legend->setTextColor(textColor);

    const QList<QCPAxis*> axes = {
        customPlot->xAxis,
        customPlot->yAxis,
        customPlot->xAxis2,
        customPlot->yAxis2
    };

    for (QCPAxis *axis : axes) {
        axis->setBasePen(QPen(axisColor));
        axis->setTickPen(QPen(axisColor));
        axis->setSubTickPen(QPen(axisColor));
        axis->setLabelColor(textColor);
        axis->setTickLabelColor(textColor);
        axis->grid()->setPen(QPen(gridColor, 1, Qt::DotLine));
        axis->grid()->setSubGridPen(QPen(subGridColor, 1, Qt::DotLine));
        axis->grid()->setZeroLinePen(QPen(gridColor));
    }

    customPlot->replot();
}

void connectwindow::handleWheel(QWheelEvent *event)
{
    // 判断Shift键是否按下
    bool shiftPressed = event->modifiers() & Qt::ShiftModifier;
    // 缩放因子，值越小缩放速度越快
    double factor = qPow(0.999, event->angleDelta().y());
    if (shiftPressed) {
        // Shift+滚轮：缩放Y轴
        QCPAxis *yAxis = ui->cplot_Spectrum->yAxis;
        double center = yAxis->pixelToCoord(event->pos().y());
        double yMin = yAxis->range().lower;
        double yMax = yAxis->range().upper;
        // 围绕鼠标位置缩放
        double newYMin = center - (center - yMin) * factor;
        double newYMax = center + (yMax - center) * factor;
        yAxis->setRange(newYMin, newYMax);
    } else {
        // 普通滚轮：缩放X轴
        QCPAxis *xAxis = ui->cplot_Spectrum->xAxis;
        double center = xAxis->pixelToCoord(event->pos().x());
        double xMin = xAxis->range().lower;
        double xMax = xAxis->range().upper;
        // 围绕鼠标位置缩放
        double newXMin = center - (center - xMin) * factor;
        double newXMax = center + (xMax - center) * factor;
        xAxis->setRange(newXMin, newXMax);
    }
    ui->cplot_Spectrum->replot();
    event->accept(); // 阻止事件继续传播
}

float *connectwindow::SendStrainPtr()
{
    return m_DataTask->strain_float;
}

void connectwindow::GetIPAddress()
{
    ui->cbox_LocakIP->clear();
    QString localHostName = QHostInfo::localHostName();
    QHostInfo info = QHostInfo::fromName(localHostName);
    foreach(QHostAddress address,info.addresses()){
        if(address.protocol() == QAbstractSocket::IPv4Protocol){
            ui->cbox_LocakIP->addItem(address.toString());
        }
    }
}

void connectwindow::setLED(int color, int size)
{
    ui->label_led->setText("");
    QString min_width = QString("min-width: %1px;").arg(size);              // 最小宽度：size
    QString min_height = QString("min-height: %1px;").arg(size);            // 最小高度：size
    QString max_width = QString("max-width: %1px;").arg(size);              // 最小宽度：size
    QString max_height = QString("max-height: %1px;").arg(size);            // 最小高度：size
    QString border_radius = QString("border-radius: %1px;").arg(size/2);    // 边框是圆角，半径为size/2
    QString border = QString("border:1px solid black;");                    // 边框为1px黑色
    QString background = "background-color:";
    switch (color) {
    case 0:
        background += "rgb(190,190,190)";    // 灰色
        break;
    case 1:
        background += "rgb(255,0,0)";        // 红色
        break;
    case 2:
        background += "rgb(0,255,0)";        // 绿色
        break;
    case 3:
        background += "rgb(255,255,0)";      // 黄色
        break;
    default:
        break;
    }
    const QString SheetStyle = min_width + min_height + max_width + max_height + border_radius + border + background;
    ui->label_led->setStyleSheet(SheetStyle);
}

void connectwindow::serialDataRec(const QString& data)
{
    ui->ptedit_SerialRec->insertPlainText(data);
    ui->ptedit_SerialRec->moveCursor(QTextCursor::End);
}

void connectwindow::serialStatusChange(bool State)
{
    if(State){
        DispDiagnosis("串口打开成功");
        Flag_ConnectSerial = 1;
        setLED(3,25);
        ui->pbtn_OpenSerial->setText("关闭串口");
    }
    else{
        DispDiagnosis("串口打开失败");
        Flag_ConnectSerial = 0;
        setLED(1,25);
    }
}

void connectwindow::udpDataRec(const QString &data)
{
    ui->ptedit_UdpRec->insertPlainText(data);
    ui->ptedit_UdpRec->moveCursor(QTextCursor::End);
}

void connectwindow::udpStatusChange(bool State)
{
    if(State){
        if (timer0->isActive()){
            timer0->stop();
        }
        timer0->start(CampingFre);
        connect(timer0,SIGNAL(timeout()),this, SLOT(UpDataTable()));
        setLED(2,25);
        DispDiagnosis("UDP连接成功!");
        ui->pbtn_OpenNet->setText("断开连接");
        ui->cbox_CommuteChoose->setEnabled(false);
        Flag_ConnectUDP = 1;
    }
    else {
        setLED(1,25);
        DispDiagnosis("连接失败!");
    }
}

void connectwindow::tcpDataRec(const QString &data)
{
    ui->ptedit_TcpRec->insertPlainText(data);
    ui->ptedit_TcpRec->moveCursor(QTextCursor::End);
}

void connectwindow::tcpStatusChange(tcptask::ConnectionStatus status)
{
    switch (status) {
    case tcptask::StatusListeningStarted:
        DispDiagnosis("TCP启动监听!");
        setLED(3,25);
        break;

    case tcptask::StatusListeningFailed:
        DispDiagnosis("TCP监听失败");
        setLED(1,25);
        break;

    case tcptask::StatusListeningStopped:
        DispDiagnosis("TCP监听停止");
        setLED(0,25);
        break;

    case tcptask::StatusClientConnected:
        setLED(2,25);
        if (timer0->isActive()){
            timer0->stop();
        }
        timer0->start(CampingFre);
        connect(timer0,SIGNAL(timeout()),this, SLOT(UpDataTable()));
        ui->cbox_CommuteChoose->setEnabled(false);
        Flag_ConnectTCP = 1;
        break;

    case tcptask::StatusClientDisconnected:
        DispDiagnosis("TCP客户端连接断开");
        setLED(3,25);
        break;

    case tcptask::StatusConnectionError:
        DispDiagnosis("TCP客户端连接错误");
        setLED(1,25);
        break;

    default:

        break;
    }
}

void connectwindow::InitSerial()
{
    m_serialThread = new QThread(this);
    m_serialtask = new serialtask();

    // 将串口工作对象移到子线程
    m_serialtask->moveToThread(m_serialThread);

    // 串口控制信号
    connect(this, &connectwindow::serialPortOpen, m_serialtask, &serialtask::openSerialPort);
    connect(this, &connectwindow::serialPortClose, m_serialtask, &serialtask::closeSerialPort);
    connect(this, &connectwindow::serialDataSend, m_serialtask, &serialtask::writeData);
    // 串口状态信号
    connect(m_serialtask, &serialtask::dataReceived, this, &connectwindow::serialDataRec);
    connect(m_serialtask, &serialtask::connectionStatusChanged, this, &connectwindow::serialStatusChange);

    Flag_ConnectSerial = 0;
    on_pbtn_RefreshSerial_clicked();
    ui->cbox_SerialBaudChoose->setCurrentText("921600");
    ui->cbox_SerialNameChoose->setCurrentText("COM7");
}

void connectwindow::InitUdp()
{
    qRegisterMetaType<QHostAddress>("QHostAddress");
    m_UdpThread = new QThread(this);
    m_UdpTask = new udptask();

    // 将UDP工作对象移到子线程
    m_UdpTask->moveToThread(m_UdpThread);

    // UDP控制信号
    connect(this, &connectwindow::udpOpen, m_UdpTask, &udptask::udpOpen);
    connect(this, &connectwindow::udpClose, m_UdpTask, &udptask::udpClose);
    connect(this, &connectwindow::udpDataSend, m_UdpTask, &udptask::udpSendData);
    // UDP状态信号
    connect(m_UdpTask, &udptask::udpDataReceived, this, &connectwindow::udpDataRec);
    connect(m_UdpTask, &udptask::connectionStatusChanged, this, &connectwindow::udpStatusChange);

    m_UdpThread->start();
}

void connectwindow::InitTcp()
{
    qRegisterMetaType<tcptask::ConnectionStatus>("tcptask::ConnectionStatus");
    m_TcpThread = new QThread(this);
    m_TcpTask = new tcptask();

    // 将UDP工作对象移到子线程
    m_TcpTask->moveToThread(m_TcpThread);

    // UDP控制信号
    connect(this, &connectwindow::tcpOpen, m_TcpTask, &tcptask::tcpOpen);
    connect(this, &connectwindow::tcpClose, m_TcpTask, &tcptask::tcpClose);
    connect(this, &connectwindow::tcpDataSend, m_TcpTask, &tcptask::tcpSendData);
    // TCP状态信号
    connect(m_TcpTask, &tcptask::tcpDataReceived, this, &connectwindow::tcpDataRec);
    connect(m_TcpTask, &tcptask::connectionStatusChanged, this, &connectwindow::tcpStatusChange);
    // 使用带客户端信息的信号，避免跨线程裸读成员变量
    connect(m_TcpTask, &tcptask::connectionStatusChangedWithInfo,
            this, [this](tcptask::ConnectionStatus status,
                         const QHostAddress &addr, quint16 port) {
                if (status == tcptask::StatusClientConnected) {
                    DispDiagnosis("TCP客户端连接成功:客户端地址" + addr.toString() + ":"
                                  + QString::number(port));
                }
            });

    m_TcpThread->start();
}

void connectwindow::InitAnalyze()
{
    qRegisterMetaType<int*>("int*");
    m_DataThread = new QThread (this);
    m_DataTask = new analyzepackets();

    connect(this, &connectwindow::dataFreshCH, m_DataTask, &analyzepackets::freshCH);
    connect(this, &connectwindow::dataReset, m_DataTask, &analyzepackets::resetOriginalWavelength);

    connect(this, &connectwindow::dataSaveAuto, m_DataTask, &analyzepackets::saveAuto);
    connect(this, &connectwindow::dataSaveManual, m_DataTask, &analyzepackets::saveManual);
    connect(m_DataTask, &analyzepackets::saveManualFinished, this,[=](){ui->pbtn_SaveDynamic->setEnabled(true);});

    m_DataTask->moveToThread(m_DataThread);

    m_DataThread->start();
}

void connectwindow::InitWindow()
{
    ReadParaConfig();
    InitCustomPlot(ui->cplot_Spectrum);
    on_pbtn_ParaSet_clicked();

    ui->tabWidget->setCurrentIndex(0);
    QStringList list;
    list<< "通道" << "波长" << "应变"<<"温度";
    model->setHorizontalHeaderLabels(list);
    model->setItem(9,2,new QStandardItem(""));
    ui->tablev_Wavelength->setModel(model);
    ui->tablev_Wavelength->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tablev_Wavelength->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Fixed);
    ui->tablev_Wavelength->show();
    ConnectModel = ui->cbox_CommuteChoose->currentIndex();
    connect(ui->cbox_CommuteChoose, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, [this](int index) {ConnectModel = index;
    });

    connect(ui->cbox_LocakIP,SIGNAL(activated(int)), this,SLOT(RefreshNetAddress()));
    connect(ui->edit_LocalPort,SIGNAL(editingFinished()), this,SLOT(RefreshNetAddress()));
    connect(ui->edit_TargetIP,SIGNAL(editingFinished()), this,SLOT(RefreshNetAddress()));
    connect(ui->edit_TargetPort,SIGNAL(editingFinished()), this,SLOT(RefreshNetAddress()));

    RefreshNetAddress();
    GetIPAddress();
    DispDiagnosis(" ");
    on_pbtn_SelectAllCH_clicked();

    ConnectModel = 0;
}

void connectwindow::RefreshNetAddress()
{
    LocalIpAddress = QHostAddress(ui->cbox_LocakIP->currentText());
    LocalPortNum = ui->edit_LocalPort->text().toUShort();
    TargetIpAddress = QHostAddress(ui->edit_TargetIP->text());
    TargetPortNum = ui->edit_TargetPort->text().toUShort();
    DispDiagnosis("IP地址更新");
    //qDebug() << "LocalIpAddress:" << LocalIpAddress << ";" << "LocalPortNum:" << LocalPortNum;
    //qDebug() << ui->edit_TargetIP->text();
    //qDebug() << "TargetIpAddress:" << TargetIpAddress << ";" << "TargetPortNum:" << TargetPortNum;
}

void connectwindow::DispDiagnosis(QString temp)
{
    QDateTime dateTime = QDateTime::currentDateTime();
    QString strTime = dateTime.toString("[yyyy-MM-dd hh:mm:ss]");
    ui->lab_State->setText(strTime + temp);
}

void connectwindow::set_strain_AB(float m_A, float m_B)
{
    m_DataTask->A = m_A;
    m_DataTask->B = m_B;
}

void connectwindow::on_pbtn_SerialSend_clicked()
{
    QString dataSend = ui->ptedit_SerialSend->toPlainText() + "\r\n";
    emit serialDataSend(dataSend);
}

void connectwindow::on_pbtn_SerialClear_clicked()
{
    ui->ptedit_SerialRec->clear();
}

void connectwindow::on_pbtn_RefreshSerial_clicked()
{
    GetIPAddress();
    QString temp;
    ui->cbox_SerialNameChoose->clear();
    QList<QSerialPortInfo> serialList = QSerialPortInfo::availablePorts();

    for (const QSerialPortInfo& serialInfo : serialList) {
        QString displayText = serialInfo.portName();
        if (!serialInfo.description().isEmpty()) {
            displayText += QString(" (%1)").arg(serialInfo.description());
        }

        temp += displayText + ";";
        ui->cbox_SerialNameChoose->addItem(displayText, serialInfo.portName()); // 保存原始端口名作为数据
    }

    DispDiagnosis(temp);
}

void connectwindow::on_pbtn_OpenSerial_clicked()
{
    if(Flag_ConnectSerial){     //连接状态
        emit serialPortClose();
        ui->pbtn_OpenSerial->setText("打开串口");
        DispDiagnosis("串口关闭成功");
        Flag_ConnectSerial = 0;
        Flag_OriginalWaveLength = 0;    //初始波长是否已保存，标志位
        setLED(0,25);
        m_serialThread->terminate();  // 强制终止线程
        m_serialThread->wait();       // 等待线程完全停止
    }
    else {                      //未连接状态
        m_serialThread->start();
        emit serialPortOpen(ui->cbox_SerialNameChoose->currentData().toString()
                            ,ui->cbox_SerialBaudChoose->currentText().toInt());
        DispDiagnosis("正在打开串口!");
    };
}

void connectwindow::on_pbtn_Diagnosis_clicked()
{
    if(ConnectDebug == nullptr){
        ConnectDebug = new connectdebug();
    }
    ConnectDebug->show();
}

void connectwindow::on_pbtn_OpenNet_clicked()
{
    if(ConnectModel == 0){  //UDP通信
        if(Flag_ConnectUDP){
            disconnect(m_UdpTask, &udptask::udpPocketsReceived, m_DataTask, &analyzepackets::analyzeData);
            emit udpClose();
            setLED(0,25);
            DispDiagnosis("UDP暂未连接!");
            Flag_ConnectUDP = 0;
            ui->pbtn_OpenNet->setText("连接网络");
            Flag_OriginalWaveLength = 0;  //初始波长是否已保存，标志位
            ui->cbox_CommuteChoose->setEnabled(true);
        }
        else {
            connect(m_UdpTask, &udptask::udpPocketsReceived, m_DataTask, &analyzepackets::analyzeData);
            emit udpOpen(LocalIpAddress,LocalPortNum);
            setLED(3,25);
            DispDiagnosis("UDP正在连接!");
        }
    }
    else if (ConnectModel == 1){    //TCP通信
        if(Flag_ConnectTCP){
            disconnect(m_TcpTask, &tcptask::tcpPocketsReceived, m_DataTask, &analyzepackets::analyzeData);
            emit tcpClose();
            Flag_ConnectTCP = 0;
            Flag_OriginalWaveLength = 0;
            ui->pbtn_OpenNet->setText("连接网络");
            ui->cbox_CommuteChoose->setEnabled(true);
        }
        else {
            connect(m_TcpTask, &tcptask::tcpPocketsReceived, m_DataTask, &analyzepackets::analyzeData);
            emit tcpOpen(LocalIpAddress,LocalPortNum);
            Flag_ConnectTCP = 1;
            ui->pbtn_OpenNet->setText("断开连接");
            ui->cbox_CommuteChoose->setEnabled(false);
        }

    }
}

void connectwindow::UpDataTable()
{
    int CheckedCH_SUM = 0;
    // 一次锁保护整个表格读取过程，保证帧内数据一致性
    {
        QMutexLocker locker(&mutexAnalyze);
        for (int i=0;i<MAX_CHANNELS;i++) {
            CheckedCH_SUM += m_DataTask->channel_counts[i] * Flag_CH[i];
        }

        this->model->removeRows(0,model->rowCount());
        model->setItem(9,2,new QStandardItem(""));
        if (Flag_ConnectUDP == 1){
            for(int i=0;i<CheckedCH_SUM;i++){
                this->model->setItem(i,0,new QStandardItem (m_DataTask->FBG_count[i])) ;
                this->model->setItem(i,1,new QStandardItem (m_DataTask->wavelength[i]));
                if (Flag_Strain == 1){
                         bool isValidStrain = true;
                         if ( m_DataTask->strain_float[i] < StrainLow) {
                             isValidStrain = false;
                         }
                         if ( m_DataTask->strain_float[i] > StrainTop) {
                             isValidStrain = false;
                         }
                         if (isValidStrain) {
                             model->setItem(i, 2, new QStandardItem(m_DataTask->strain[i]));
                         }
                         else {
                             model->setItem(i, 2, new QStandardItem("outRange"));
                         }
                 }
             }
               ui->tablev_Wavelength->setModel(model);
               ui->tablev_Wavelength->show();
        }
    } // 锁在此释放
    // Show_Plot 内部独立加锁
    Show_Plot(ui->cplot_Spectrum);
}

void connectwindow::Show_Plot(QCustomPlot *customPlot)
{
    QVector<QVector<double>>key;
    QVector<QVector<double>>value;
    int num=0;
    QVector<double> tmp_x(4000);
    for (int i = 0; i < 4000; ++i){
        tmp_x[i] = i;
    }
    QVector<double> tmp_y(4000,0);
    for(int i=0;i<7;i++){
        key.push_back(tmp_x);
        value.push_back(tmp_y);
    }
    // 一次锁保护所有通道的读取
    {
        QMutexLocker locker(&mutexAnalyze);
        for(int k=0;k<7;k++){
            for(int i=0;i<m_DataTask->CH_point[k];i++){
               value[k][m_DataTask->data_x_trans[k][i]]=m_DataTask->data_y_trans[k][i];
           }
        }
    }
    const int CHANNEL_COUNT = 7;
    for(int ch = 0; ch < CHANNEL_COUNT; ch++) {
        if(Flag_CH[ch]) {
            customPlot->graph(num)->setData(key[ch], value[ch]);
        } else {
            customPlot->graph(num)->data()->clear();
        }
        num++;
    }
    customPlot->replot();
}

void connectwindow::on_pbtn_ParaSet_clicked()
{
    QString fre_tmp = ui->cbox_CampingFre->currentText();
    CampingFre = fre_tmp.toInt();
    if (timer0->isActive()){
       timer0->stop();
       timer0->start(1000/CampingFre);
    }
     QString auto_save_time_txt = ui->cbox_SaveCycle->currentText();
     AutoSaveCycle = auto_save_time_txt.toInt();
    if(ui->cbox_ModleSelect->currentText()=="应变"){
         QString Set_A = ui->textEdit_APara->toPlainText();
         float A = Set_A.toFloat();
         QString Set_B = ui->textEdit_BPara->toPlainText();
         float B = Set_B.toFloat();
         set_strain_AB(A,B);
         StrainLow = ui->textEdit_StrainLowPara->toPlainText().toFloat();
         StrainTop = ui->textEdit_StrainTopPara->toPlainText().toFloat();
    }
    WriteParaConfig();
}

void connectwindow::on_pbtn_SelectAllCH_clicked()
{
    QCheckBox* checkBoxes[] = {
        ui->ckbox_CH1, ui->ckbox_CH2, ui->ckbox_CH3,
        ui->ckbox_CH4, ui->ckbox_CH5, ui->ckbox_CH6,
        ui->ckbox_CH7, ui->ckbox_CH8, ui->ckbox_CH9,
        ui->ckbox_CH10, ui->ckbox_CH11, ui->ckbox_CH12,
        ui->ckbox_CH13, ui->ckbox_CH14, ui->ckbox_CH15,
        ui->ckbox_CH16
    };

    for (int i = 0; i < 16; i++){
        checkBoxes[i]->setCheckState(Qt::Checked);
    }
    on_pbtn_SelectCH_clicked();
}

void connectwindow::on_pbtn_SelectCH_clicked()
{
    QCheckBox* checkBoxes[] = {
        ui->ckbox_CH1, ui->ckbox_CH2, ui->ckbox_CH3,
        ui->ckbox_CH4, ui->ckbox_CH5, ui->ckbox_CH6,
        ui->ckbox_CH7, ui->ckbox_CH8, ui->ckbox_CH9,
        ui->ckbox_CH10, ui->ckbox_CH11, ui->ckbox_CH12,
        ui->ckbox_CH13, ui->ckbox_CH14, ui->ckbox_CH15,
        ui->ckbox_CH16
    };
    QString temp = "通道状态";
    for (int i = 0; i < 16; i++){
        Flag_CH[i] = checkBoxes[i]->isChecked() ? 1 : 0;
        temp += QString::number(Flag_CH[i]) + ";";
    }
    DispDiagnosis(temp);

    if(ui->cbox_ModleSelect->currentText()=="应变"){
        Flag_Strain=1;
    }
    else{
        Flag_Strain=0;
    }

    this->model->removeRows(0,model->rowCount());//删除所有行，重新显示
    model->setItem(9,2,new QStandardItem(""));
    ui->tablev_Wavelength->setModel(model);
    ui->tablev_Wavelength->show();
    Show_Plot(ui->cplot_Spectrum);

    emit dataFreshCH(Flag_CH);
}

void connectwindow::on_pbtn_ParaRest_clicked()
{
    ReadParaConfig();
}

void connectwindow::on_tabWidget_currentChanged(int index)
{
    //qDebug() << index;
    Flag_TabPage = index;
}

void connectwindow::on_pbtn_SaveDynamic_clicked()
{
    Flag_DynamicSave =! Flag_DynamicSave;
    if(Flag_DynamicSave == 1){
        emit dataSaveAuto(AutoSaveCycle);
        ui->pbtn_SaveStatic->setEnabled(false);
        ui->pbtn_SaveDynamic->setText("取消保存");
    }
    if(Flag_DynamicSave == 0){
        emit dataSaveAuto(AutoSaveCycle);
        ui->pbtn_SaveStatic->setEnabled(true);
        ui->pbtn_SaveDynamic->setText("自动保存");
    }

}

void connectwindow::on_pbtn_SaveStatic_clicked()
{
    ui->pbtn_SaveDynamic->setEnabled(false);
    emit dataSaveManual();
}

bool connectwindow::on_pbtn_SaveSpectrum_clicked()
{
    QDateTime currentTime = QDateTime::currentDateTime();
    QString defaultFileName = currentTime.toString("yyyy-MM-dd_hh-mm-ss");
    QString filter = "PNG 图片 (*.png);;JPEG 图片 (*.jpg);;BMP 图片 (*.bmp)";
    QString fileName = QFileDialog::getSaveFileName(
        this,
        "保存频谱",
        qApp->applicationDirPath() + "/" + defaultFileName,
        filter
    );
    if (fileName.isEmpty())
    {
        QMessageBox::information(this, "Fail", "保存失败");
        return false;
    }
    QString selectedFilter;
    if (!fileName.contains('.')) {
        if (filter.contains("PNG")) fileName += ".png";
        else if (filter.contains("JPEG")) fileName += ".jpg";
        else if (filter.contains("BMP")) fileName += ".bmp";
    }
    QSize saveSize(ui->cplot_Spectrum->width(), ui->cplot_Spectrum->height());
    QPixmap pixmap = ui->cplot_Spectrum->grab();
    pixmap = pixmap.scaled(saveSize);
    bool saved = false;
    QString format = fileName.section('.', -1).toUpper();
    if (format == "PNG") saved = pixmap.save(fileName, "PNG");
    else if (format == "JPG" || format == "JPEG") saved = pixmap.save(fileName, "JPEG");
    else if (format == "BMP") saved = pixmap.save(fileName, "BMP");
    else saved = pixmap.save(fileName); // 使用Qt自动检测格式
    if (saved) {
        QMessageBox::information(this, "成功", QString("图片已成功保存到:\n%1").arg(fileName));
    } else {
        QMessageBox::critical(this, "错误", QString("无法保存图片到:\n%1\n\n请检查文件权限或尝试其他格式").arg(fileName));
    }
    return saved;
}

void connectwindow::on_pbtn_UdpSend_clicked()
{
    if(Flag_ConnectUDP){
        RefreshNetAddress();
        QString text = ui->ptedit_UdpSend->toPlainText() + "\r\n";
        emit udpDataSend(text, TargetIpAddress, TargetPortNum);
    }
    else {
        QMessageBox::critical(this, "警告", "UDP连接未建立");
    }
}

void connectwindow::on_pbtn_UdpClear_clicked()
{
    ui->ptedit_UdpRec->clear();
    //SerialThread->strdata = "";
}

void connectwindow::on_pbtn_TcpSend_clicked()
{
    if(Flag_ConnectTCP){
        QString text = ui->ptedit_TcpSen->toPlainText();
        emit tcpDataSend(text);
    }
    else {
        QMessageBox::critical(this, "警告", "TCP连接未建立");
    }
}

void connectwindow::on_pbtn_TcpClear_clicked()
{
    ui->ptedit_TcpRec->clear();
}

void connectwindow::on_pbtn_WaveLengthReset_clicked()
{
    emit dataReset();
}

void connectwindow::SwitchToB()
{
    ApplyCustomPlotTheme(ui->cplot_Spectrum, true);
}

void connectwindow::SwitchToW()
{
    ApplyCustomPlotTheme(ui->cplot_Spectrum, false);
}
