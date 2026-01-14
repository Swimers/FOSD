#ifndef COMTHREAD_H
#define COMTHREAD_H

#include<QThread>
#include <QObject>
#include<QSerialPort>
#include<QVector>
#include<QTimer>
//#include"mainwindow.h"
class DemoWindow;
class ComThread : public QThread
{
    Q_OBJECT
public:
    explicit ComThread(QObject *parent = nullptr);
    //void setObj(MainWindow* obj);

    //explicit ComThread(QObject *parent);
    ~ComThread();
    QByteArray serial_array;//接收串口数据
    int len_data=0;//接收的数据长度
    //QStringList GetSerialAvailable();
    bool InitSerial(QString portname, int baudRate);
    void CloseSerial();

    bool head_flag=false;//识别到报头标志位。
    bool tail_flag=false;//识别到报头标志位。



    int CH1;//通道1传感器数目
    int CH2;
    int CH3;
    int CH4;
    int CH5;
    int CH6;
    int CH7;
    int CH_SUM;//总传感器数目
    QString FBG_count[1024]={""};//传感器编号数组
    //QString wavelength[1024]={""};//波长字符串数组
    double wavelength[1024]={};
    float strain_float[1024]={0};//浮点数据类型的应变
    QString strain[1024]={""};//应变字符串数组

    QVector<int> data_xy;
    int count_num=0;//计数变量


    int serial_count_num=0;//串口线程计数变量


    float original_wavelength[1024]={0};//存储第一次接收到的波长数据，当作初始波长
    float   original_wavelength_temp[1024]={0};//初始波长过度数组


//    QString FBG_count[1024]={""};//传感器编号数组
//    QString wavelength[1024]={""};//波长字符串数组
//    float strain_float[1024]={0};//浮点数据类型的应变
//    QString strain[1024]={""};//应变字符串数组

    //保存数据字符串，动态保存至TXT文件
    QString strain_txt;
    QString strain_txt_1;
    QString wavelength_txt;
    QString wavelength_txt_1;
    QString fbg_txt;
    QString fbg_txt_1;
    QString str_txt;
    //静态保存所需字符串数组
    int count_static=0;
    QString wavelength_static[1024]={""};//波长字符串数组
    float wavelength_static_temp[20][1024]={0};//波长字符串数组
    QString wavelength_static_txt;
    QString strain_static[1024]={""};//应变字符串数组
    float strain_static_temp[20][1024]={0};//应变字符串数组
    QString strain_static_txt;

    QString wavelength_static_txt_temp="";//临时存放数组
    QString strain_static_txt_temp="";

    void do_DataHandler(QByteArray bufferData);

    //测试数组
    QString strdata;
    double data=0;
    double num=0;

public slots:
    void readyRead_serial_slot();//从串口接收数据
    void SendSerialData(QByteArray data);
protected:
    void run();
signals:
    void UpdateSerialData(QByteArray data);
private:
    QSerialPort *port=new QSerialPort;//串口
    DemoWindow* guimain=nullptr;
};

#endif // COMTHREAD_H
