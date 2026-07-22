#ifndef CONNECTWINDOW_H
#define CONNECTWINDOW_H

#include <QWidget>
#include <QLabel>
#include <QTimer>
#include <qcustomplot.h>

#include <QUdpSocket>
#include <QTcpSocket>

#include "connectdebug.h"
#include "pocketsconfig.h"
#include "serialtask.h"
#include "udptask.h"
#include "tcptask.h"


class ConnectDebug;
class analyzepackets;
namespace Ui {
class connectwindow;
}

namespace Ui {
class connectwindow;
}

class connectwindow : public QWidget
{
    Q_OBJECT

public:
    explicit connectwindow(QWidget *parent = nullptr);
    ~connectwindow();

public:
    bool Flag_ConnectSerial;        //串口连接标志位：未连接0、连接1；
    bool Flag_ConnectUDP = 0;
    bool Flag_ConnectTCP = 0;
    bool Flag_OriginalWaveLength;
    bool Flag_DynamicSave;
    bool Flag_Strain;

    int Flag_CH[MAX_CHANNELS];

    int Flag_TabPage;
    int ConnectModel;

    float StrainLow;
    float StrainTop;
    int AutoSaveCycle;
    int CampingFre;

public:
    QUdpSocket * UdpSocket = nullptr;
    QTcpSocket * TcpSocket = nullptr;
    QTcpServer * TcpServer = nullptr;
    QTimer * timer0 = new QTimer(this);

    QHostAddress LocalIpAddress;
    QHostAddress TargetIpAddress;
    quint16 LocalPortNum;
    quint16 TargetPortNum;
    QStandardItemModel * model = new QStandardItemModel();   //水平表头

public:
    void ReadParaConfig();
    void WriteParaConfig();
    void InitCustomPlot(QCustomPlot *customPlot);
    void ApplyCustomPlotTheme(QCustomPlot *customPlot, bool dark);
    void Show_Plot(QCustomPlot *customPlot);
    float* SendStrainPtr();
    void GetIPAddress();

public slots:
    void set_strain_AB(float m_A,float m_B);
    void handleWheel(QWheelEvent *event);
    void DispDiagnosis(QString temp);
    void setLED(int color, int size);

    void serialDataRec(const QString &data);
    void serialStatusChange(bool State);

    void udpDataRec(const QString &data);
    void udpStatusChange(bool State);

    void tcpDataRec(const QString &data);
    void tcpStatusChange(tcptask::ConnectionStatus status);

signals:
    void dataReady();

    void serialPortOpen(const QString &portName, qint32 baudRate);
    void serialPortClose();
    void serialDataSend(const QString &data);

    void udpOpen(const QHostAddress &address, const quint16 &port);
    void udpClose();
    void udpDataSend(const QString text,const  QHostAddress IPAddress,const  quint16 PortNum);

    void tcpOpen(const QHostAddress &address, const quint16 &port);
    void tcpClose();
    void tcpDataSend(const QString text);

    void dataFreshCH(const int* Flag);
    void dataReset();
    void dataSaveAuto(int AutoSaveCycle);
    void dataSaveManual();

private:
    void generateData();

private slots:
    void UpDataTable();
    void InitWindow();
    void InitSerial();
    void InitUdp();
    void InitTcp();
    void InitAnalyze();
    void RefreshNetAddress();

private slots:
    void on_pbtn_RefreshSerial_clicked();
    void on_pbtn_OpenSerial_clicked();
    void on_pbtn_SerialSend_clicked();
    void on_pbtn_SerialClear_clicked();
    void on_pbtn_Diagnosis_clicked();
    void on_pbtn_OpenNet_clicked();
    void on_pbtn_UdpSend_clicked();
    void on_pbtn_UdpClear_clicked();
    void on_pbtn_ParaSet_clicked();
    void on_pbtn_SelectAllCH_clicked();
    void on_pbtn_SelectCH_clicked();
    void on_pbtn_ParaRest_clicked();
    void on_tabWidget_currentChanged(int index);
    void on_pbtn_SaveDynamic_clicked();
    void on_pbtn_SaveStatic_clicked();
    bool on_pbtn_SaveSpectrum_clicked();
    void on_pbtn_TcpSend_clicked();
    void on_pbtn_TcpClear_clicked();
    void on_pbtn_WaveLengthReset_clicked();
    void sendPresetCommand(const QString &cmd);
    void SwitchToB();
    void SwitchToW();

private:
    Ui::connectwindow *ui;

    QThread * m_serialThread = nullptr;
    serialtask * m_serialtask = nullptr;

    QThread * m_UdpThread = nullptr;
    udptask * m_UdpTask = nullptr;

    QThread * m_TcpThread = nullptr;
    tcptask * m_TcpTask = nullptr;

    QThread * m_DataThread = nullptr;
    analyzepackets * m_DataTask = nullptr;

    connectdebug * ConnectDebug = nullptr;
};

#endif // CONNECTWINDOW_H
