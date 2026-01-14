#ifndef RECEDATETHREAD_H
#define RECEDATETHREAD_H

#include <QThread>
#include <QUdpSocket>
#include <QString>
#include <QHostAddress>
#include<QMessageBox>

#include<QTimer>
#include<QDebug>
#include <QStandardItemModel>
#include"demowindow.h"

class DemoWindow;

class ReceDateThread : public QThread
{
    Q_OBJECT
public:
    explicit ReceDateThread(QObject *parent);
    ~ReceDateThread();
    void setObj(DemoWindow* obj);

    float A=1;
    float B=0;
    float original_wavelength[1024]={0};//存储第一次接收到的波长数据，当作初始波长
    float   original_wavelength_temp[1024]={0};//初始波长过度数组
    int count_num=0;//计数变量
    QByteArray array;//接收解调仪数据

    int CH1;//通道1传感器数目
    int CH2;
    int CH3;
    int CH4;
    int CH5;
    int CH6;
    int CH7;
    int CH_SUM;//总传感器数目
    QString FBG_count[1024]={""};//传感器编号数组
    QString wavelength[1024]={""};//波长字符串数组
    float newStrain[1024]={0};
    float strain_float[1024]={0};//浮点数据类型的应变
    QString strain[1024]={""};//应变字符串数组

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



    //

    //


    //绘图需要用到的
    int head_flag=0,tail_flag=0;//大的报头与大的报尾标志位
      int x_flag=0,y_flag=0;

    int x_index=0;
    int y_index=0;


    int sum_index=0;
    float CH_datagram[1024]={0};
    int data_count_array[8]={0};
    int CH_point[7]={0};

    int sum_point=0;

    int data_x_trans[20][5000]={0};
    int data_y_trans[20][5000]={0};



public slots:
    void readyRead_slot();//接收数据响应槽函数


protected:
    void run();

signals:
    //void TestSignal();

private:
    void updateStrain();
    DemoWindow* guimain=nullptr;
};


#endif // RECEDATETHREAD_H
