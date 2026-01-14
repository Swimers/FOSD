#ifndef ANALYZEPACKETS_H
#define ANALYZEPACKETS_H

#include <QObject>
#include <QString>
#include <QHostAddress>
#include <QMessageBox>
#include <QTimer>
#include <QDebug>
#include <QStandardItemModel>

#include <pocketsconfig.h>
#include <connectwindow.h>

class connectwindow;
class analyzepackets : public QObject
{
    Q_OBJECT
public:
    explicit analyzepackets(QObject *parent = nullptr);
    ~analyzepackets();

    void setObj(connectwindow* obj);

public:
    void processChannel(int channel);
    void processReset();
    void processMain();

public slots:
    void analyzeData(const QByteArray datagram);
    void updateData();
    void resetOriginalWavelength();
    void freshCH(const int* Flag);

    void saveAuto(int auto_save_time);
    void saveManual();

signals:
    void resetOriginalWavelengthFinished();
    void saveManualFinished();

public:
    bool Flag_Reset;
    bool Flag_DynamicSave;

    int  channel_counts[MAX_CHANNELS];              //每个通道传感器个数
    int  channel_flags[MAX_CHANNELS];               //通道选择标志位
    int  CH_SUM;                                    //总传感器数目

    float A;
    float B;

    float NewStrain[1024]={0};
    float StrainFloat[1024]={0};                    //浮点数据类型的应变

    int CH_point[MAX_POINTS]={0};                            // 每个通道的数据点数
    int data_x_trans[20][MAX_POINTS]={{0}};
    int data_y_trans[20][MAX_POINTS]={{0}};

    QByteArray array;                               //接收解调仪数据
    QString RecText;

    QString strain_txt;                             //保存数据字符串，动态保存至TXT文件
    QString strain_txt_1;
    QString wavelength_txt;
    QString wavelength_txt_1;
    QString fbg_txt;
    QString fbg_txt_1;
    QString str_txt;

    int count_static=0;                             //静态保存所需字符串数组
    QString wavelength_static[1024]={""};           //波长字符串数组
    float wavelength_static_temp[20][1024]={{0}};   //波长字符串数组
    QString wavelength_static_txt;
    QString strain_static[1024]={""};               //应变字符串数组
    float strain_static_temp[20][1024]={{0}};       //应变字符串数组
    QString strain_static_txt;

    QString FBG_count[1024]={""};                   //传感器编号数组
    QString wavelength[1024]={""};                  //波长字符串数组
    QString strain[1024]={""};                      //应变字符串数组
    float CH_datagram[1024]={0};                    //浮点数据类型的波长
    float strain_float[1024]={0};                   //浮点数据类型的应变

    int count_num = 0;                              //计数变量

    float original_wavelength_temp[1024] = {0};     //初始波长过度数组
    float original_wavelength[1024] = {0};          //存储第一次接收到的波长数据，当作初始波长

    QString wavelength_static_txt_temp="";//临时存放数组
    QString strain_static_txt_temp="";

private:
    connectwindow * m_connectwindow = nullptr;
    QTimer *timer2 = new QTimer(this);//第二个定时器
    QTimer *timer3 = new QTimer(this);//第三个定时器用于串口通信

};

#endif // ANALYZEPACKETS_H
