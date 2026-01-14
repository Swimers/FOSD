#ifndef DEMOWINDOW_H
#define DEMOWINDOW_H

#include <QWidget>
#include<QtSerialPort/QSerialPort>
#include<QtSerialPort/QSerialPortInfo>
#include <QWidget>
#include <QUdpsocket>
#include <QString>
#include <QHostAddress>
#include<QMessageBox>
#include <QTextDocument>
#include <QFileDialog>
#include<QTimer>
#include<QDebug>
#include <QStandardItemModel>
#include<QFile>
#include"qcustomplot.h"
#include"qcpdocumentobject.h"
#include"comthread.h"
#include <QMutexLocker>  // 新增
#include <QMutex>  // 新增
#include "pipemonitorwindow.h"

class ReceDateThread;
class ComThread;
namespace Ui {
class DemoWindow;
}

typedef union FLOATA
{
    int all;
    char u8Byte[4];

}FLOATA;

typedef union FLOATA2
{
    float all;
    char u8Byte[4];

}FLOATA2;

namespace Ui {
class DemoWindow;
}

class DemoWindow : public QWidget
{
    Q_OBJECT

    friend class ComThread;

public:
    explicit DemoWindow(const QString& privilege,QWidget *parent = nullptr);
    ~DemoWindow();


    float* SendStrainPtr();


    QUdpSocket *udpSocket =nullptr;


    int connect_Serialflag=0;//连接网络状态标志，若未连接，则为0，若已连接，则为1
    int connect_UDPflag=0;//连接网络状态标志，若未连接，则为0，若已连接，则为1
    int original_flag=0;//初始波长是否已保存，标志位，若未保存，则为0，若已保存，则为1
    int strain_flag=0; //显示应变按钮是否按下，若按下，则变为1
    int reset_flag=0;//复位键是否按下标志位，若按下，则变为1
    bool save_dynamic_flag=0;//动态保存按钮标志位，若正在保存，则为1，否则为0
    int save_static_flag=0;//静态保存按钮标志位，若正在保存，则为1，否则为0
    QStandardItemModel *model = new QStandardItemModel();
    QTimer *timer=new QTimer(this);//创建第一个定时器
//    QFile *save_dynamic_file=new QFile;//声明QFile类指针
//    QString fileName;//txt文件名

    int get_t;//数据采集频率
    int auto_save_time;//自动保存时间间隔

    float Strain_low;
    float Strain_top;

     QTimer *timer2=new QTimer(this);//第二个定时器

     QTimer *timer3=new QTimer(this);//第三个定时器用于串口通信
    int CH1_Flag=0;
    int CH2_Flag=0;
    int CH3_Flag=0;
    int CH4_Flag=0;
    int CH5_Flag=0;
    int CH6_Flag=0;
    int CH7_Flag=0;

    QTime time;

    void Show_Plot(QCustomPlot *customPlot);
    void Show_Plot1(QCustomPlot *customPlot);
    void init_customplot(QCustomPlot *customPlot);
    void ReadParaConfig(const QString& fileName);
    void WriteParaConfig(const QString& fileName);
signals:
    void close_main();
private slots:
    void initialize_widget();
    void Updatatabel_slot();

    void on_Button_connect_clicked();


    bool on_Button_save_spectrum_clicked();

//    void on_Button_quit_clicked();

    void on_Button_reset_clicked();
    void on_Button_Save_dynamic_clicked();
    void on_Button_Save_Static_clicked();
    void set_strain_AB(float m_A,float m_B);

    void setLED(QLabel *label,int color, int size);

    //参数设置
    void on_Button_selectshow_clicked();
    void on_Button_parameter_set_clicked();

/////串口模块
    void dataReceive();

    void on_btnrefresh_clicked();

    void on_btnsend_clicked();

    void on_btnclear_clicked();

    void on_btnclose_clicked();

    void on_btn_diagnosis_clicked();

private:
    Ui::DemoWindow *ui;
    ComThread *comthread=nullptr;//串口线程
    ReceDateThread* thread=nullptr;
    QString privilege;
    PipeMonitorWindow* pipewindow = nullptr;
};

#endif // DEMOWINDOW_H
