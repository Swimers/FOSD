#include "analyzepackets.h"

#include <QFile>
#include <QDateTime>
#include <QTimer>
#include <QMessageBox>
#include <QIODevice>

#include "global.h"
#include "Timer.h"

analyzepackets::analyzepackets(QObject *parent) : QObject(parent)
{
    QFile *file_txt=new QFile("./初始波长.txt");//声明QFile类指针
    file_txt->open(QIODevice::ReadOnly|QIODevice::Text);
    //读取文件
    QTextStream TextRead(file_txt);
    QString data_line;
    int wave_count=0;
    while(!TextRead.atEnd()){
        data_line=TextRead.readLine();
        original_wavelength[wave_count]= data_line.toFloat();
        wave_count++;
    }
    file_txt->close();
}

analyzepackets::~analyzepackets()
{
    delete timer2;
    delete timer3;
}

void analyzepackets::setObj(connectwindow *obj)
{
    m_connectwindow = obj;
}

void analyzepackets::processChannel(int channel)
{
    FLOATA data_count_xy;
    FLOATA data_count_x;
    // 读取通道总点数（array[6]和array[7]）
    data_count_xy.u8Byte[0] = array[6];
    data_count_xy.u8Byte[1] = array[7];
    data_count_xy.u8Byte[2] = 0;
    data_count_xy.u8Byte[3] = 0;
    CH_point[channel] = data_count_xy.all;

    // 读取X轴数据（从索引8开始，每个点2字节）
    for (int j = 0; j < CH_point[channel]; j++) {
        data_count_x.u8Byte[0] = array[2 * j + 8];
        data_count_x.u8Byte[1] = array[2 * j + 9];
        data_count_x.u8Byte[2] = 0;
        data_count_x.u8Byte[3] = 0;
        data_x_trans[channel][j] = data_count_x.all;
    }

    // 读取Y轴数据（从索引708开始，每个点2字节）
    for (int j = 0; j < CH_point[channel]; j++) {
        data_count_x.u8Byte[0] = array[2 * j + 708];
        data_count_x.u8Byte[1] = array[2 * j + 709];
        data_count_x.u8Byte[2] = 0;
        data_count_x.u8Byte[3] = 0;
        data_y_trans[channel][j] = data_count_x.all;
    }
}

void analyzepackets::processReset()
{
    count_num++;
    for(int i=0;i<CH_SUM;i++){
        FLOATA2 original_datagram;
        original_datagram.u8Byte[0] = array.data()[i*4+DataAddress];
        original_datagram.u8Byte[1] = array.data()[i*4+DataAddress + 1];
        original_datagram.u8Byte[2] = array.data()[i*4+DataAddress + 2];
        original_datagram.u8Byte[3] = array.data()[i*4+DataAddress + 3];
        original_wavelength_temp[i] = original_wavelength_temp[i] + original_datagram.all;
    }
    if(count_num == 20){
        QFile *file_txt=new QFile("./初始波长.txt");//声明QFile类指针
        file_txt->open(QIODevice::WriteOnly|QIODevice::Text);
        QTextStream TextWrite(file_txt);
        for(int i=0;i<CH_SUM;i++){
            original_wavelength[i] = original_wavelength_temp[i]/20;
            original_wavelength_temp[i] = 0;//初始波长过渡数组清零，为下一次复位键按下做准备
            QString data_line;
            TextWrite << original_wavelength[i] << endl;//将初始波长逐行写入TXT文件
        }
     file_txt->close();
     Flag_Reset = 0;//标志位设为0，在下一次复位键按下之前不再改变初始波长的值
     count_num = 0;
     qDebug() << "FINISH RESET";
     emit resetOriginalWavelengthFinished();
     }
}

void analyzepackets::processMain()
{
    FLOATA data_count_xy;
    QDateTime dateTime = QDateTime::currentDateTime();
    QString strTime = dateTime.toString("yyyy-MM-dd hh:mm:ss");
<<<<<<< Updated upstream
    QMutexLocker locker(&mutexAnalyze);
=======
    QMutexLocker locker(&analyzeMutex);
>>>>>>> Stashed changes
    //数组初始化
    for(int j=0;j<1024;j++){
        FBG_count[j]  = "";
        NewStrain[j]  = 0;
        wavelength[j] = "";
        strain[j]     = "";
    }
    //获取传感器数量
    for (int i=0;i<MAX_CHANNELS;i++) {
        channel_counts[i] = array.data()[i+4];//通道1传感器数目
    }
    CH_SUM = array.data()[MAX_CHANNELS+4];//总传感器数目
    int Checked_CH_Count = 0;
    //获取每个通道的计数点数
    for(int i=0;i<7;i++){
        data_count_xy.u8Byte[0]=0;
        data_count_xy.u8Byte[1]=0;
        data_count_xy.u8Byte[2]=0;
        data_count_xy.u8Byte[3]=0;
        CH_point[i]=data_count_xy.all;
    }
    //初始波长校准
    if(Flag_Reset == 1){
        processReset();
    }
    // 按通道分类处理数据（7个通道）
    int   channel_offsets [MAX_CHANNELS + 1] = {0};                  // 计算通道偏移量数组
    for (int ch = 0; ch < MAX_CHANNELS; ch++) {
        channel_offsets[ch + 1] = channel_offsets[ch] + channel_counts[ch];
    }
    for (int i = 0; i < CH_SUM; i++) {
        FLOATA2 datagram;                                 // 提取数据
        datagram.u8Byte[0] = array.data()[i * 4 + DataAddress];
        datagram.u8Byte[1] = array.data()[i * 4 + DataAddress + 1];
        datagram.u8Byte[2] = array.data()[i * 4 + DataAddress + 2];
        datagram.u8Byte[3] = array.data()[i * 4 + DataAddress + 3];
        int channel = -1;                                 // 确定当前数据属于哪个通道
        for (int ch = 0; ch < MAX_CHANNELS; ch++) {
            if (i >= channel_offsets[ch] && i < channel_offsets[ch + 1] && channel_counts[ch] != 0) {
                channel = ch;
                break;
            }
        }
        if (channel != -1 && channel_flags[channel]) {   // 处理有效通道数据
            int sensor_num = i - channel_offsets[channel];// 计算通道内传感器编号
            int global_index = Checked_CH_Count;
            int offset_sum = 0;                           // 计算累积偏移量
            for (int ch = 0; ch < channel; ch++) {
                offset_sum += (channel_flags[ch]) * channel_counts[ch];
            }
            // 设置数据
            FBG_count[global_index]   = QString("CH%1_%2").arg(channel + 1).arg(sensor_num + 1);                          //传感器编码号
            CH_datagram[global_index] = datagram.all;                                                                     //原始波长数据
            wavelength[global_index]  = QString::number(static_cast<double>(datagram.all), 'f', 5);                       //波长字符串数据
            NewStrain[global_index]   =(0.00075*(datagram.all-original_wavelength[i]))/A+B;   //计算得到应变
            strain[global_index]      = QString::number(static_cast<double>(NewStrain[global_index]), 'f', 5);            //应变字符串数组
            // 动态保存至TXT文本
            fbg_txt        = fbg_txt + "\t" + FBG_count[global_index];
            wavelength_txt = wavelength_txt + "\t" + wavelength[global_index];
            strain_txt     = strain_txt + "\t" + strain[global_index];
            // 最后一个数据点处理
            if (i == CH_SUM - 1) {
                fbg_txt_1 = "time\t" + fbg_txt + "\t" + fbg_txt;
                strain_txt_1 = strain_txt;
                wavelength_txt_1 = strTime + "\t" + wavelength_txt;
                str_txt = wavelength_txt_1 + "\t" + strain_txt_1 + "\n";
                fbg_txt = "";
                strain_txt = "";
                wavelength_txt = "";
            }
            Checked_CH_Count++;
        }
    }
    // 静态保存字符串格式化
    for (int i = 0; i < Checked_CH_Count; i++){
         wavelength_static[i] = QString::number(static_cast<double>(CH_datagram[i]),'f',5);
         strain_static[i] = QString::number(static_cast<double>(NewStrain[i]),'f',5);
         wavelength_static_txt_temp = wavelength_static_txt_temp + "\t" + wavelength_static[i];
         strain_static_txt_temp = strain_static_txt_temp + "\t" + strain_static[i];
         if(i == Checked_CH_Count - 1){
             wavelength_static_txt=wavelength_static_txt_temp;
             wavelength_static_txt_temp="";
             strain_static_txt=strain_static_txt_temp;
             strain_static_txt_temp="";
         }
     }
    updateData();
}

void analyzepackets::analyzeData(const QByteArray datagram)
{

    array = datagram;
    uint8_t b0 = static_cast<uint8_t>(datagram.data()[0]);
    uint8_t b1 = static_cast<uint8_t>(datagram.data()[1]);
    uint8_t b2 = static_cast<uint8_t>(datagram.data()[2]);
    uint8_t b3 = static_cast<uint8_t>(datagram.data()[3]);

    if (b0 == 0xFA && b2 == 0xFA && b1 == 0xFB && b3 == 0xFB){
        processMain();
    }
    else if (b0 == 0xFA && b2 == 0xFA && b1 == b3) {
        int channel = static_cast<int>(b1);     // 通道编号就是b1（0x00~0x06）
        if (channel >= 0 && channel < 6) {      // 假设最多6个通道（0~5）检查通道编号有效性（避免数组越界，根据实际通道数量调整）
            processChannel(channel);
        }
    }
}

void analyzepackets::updateData()
{
<<<<<<< Updated upstream
    QMutexLocker locker(&mutexStrain);
=======
    QMutexLocker locker(&strainMutex);
>>>>>>> Stashed changes
    for (int i = 0; i < 1024; i++){
        strain_float[i] = this->NewStrain[i];
    }
}

void analyzepackets::resetOriginalWavelength()
{
    Flag_Reset = true;
}

void analyzepackets::freshCH(const int *Flag)
{
    for (int i=0; i<MAX_CHANNELS; i++) {
        channel_flags[i] = Flag[i];
    }
}

void analyzepackets::saveAuto(int AutoSaveCycle)
{
    Flag_DynamicSave =! Flag_DynamicSave;
    static QFile *save_dynamic_file = nullptr;
    static QMetaObject::Connection connection;

    if(Flag_DynamicSave == 0){               //关闭自动保存
        timer2->stop();
        if (save_dynamic_file!=nullptr)
        {
            if(save_dynamic_file->isOpen())
                save_dynamic_file->close();

            delete save_dynamic_file;
            save_dynamic_file = nullptr;
        }
        disconnect(connection);
    }
    if(Flag_DynamicSave == 1){                               //开启自动保存
        timer2->start(AutoSaveCycle);
        QDateTime data_time=QDateTime::currentDateTime();
        QString time_str = data_time.toString("yyyy-MM-dd hh_mm_ss");
        QDir *DataFile = new QDir;
        bool exist = DataFile->exists("自动存储");
        if(!exist){
            qDebug() << "No File";
            bool isok = DataFile->mkdir("自动存储");
            if(!isok){
                qDebug() << "新建文件夹失败";
//              QMessageBox::warning(this,"sdf"," 新建文件夹失败 ",QMessageBox::Yes) ;
            }
        }
        QString time_ms_str = time_str+" "+ QString::number(AutoSaveCycle) +"ms ";
        save_dynamic_file=new QFile("自动存储/"+time_ms_str+"data.txt");
        save_dynamic_file->open(QIODevice::Text|QIODevice::WriteOnly|QIODevice::Append);
        connection = connect(timer2,&QTimer::timeout,[=](){
                QTextStream stream(save_dynamic_file);
                stream.seek(save_dynamic_file->size());
                if(!save_dynamic_file->size()){
                    stream << fbg_txt_1 << "\n";
                }
                stream << str_txt << "\n";
                //qDebug() << "Dynamic Save Success" << time_str;
            });
    }
}

void analyzepackets::saveManual()
{
    QDateTime data_time=QDateTime::currentDateTime();
    QString strTime=data_time.toString("yyyy-MM-dd hh:mm:ss");
    QDir *DataFile=new QDir;
    bool exist=DataFile->exists("手动存储");
    if(!exist)
    {
        bool isok=DataFile->mkdir("手动存储");
        if(!isok){
            qDebug() << "创建文件夹失败";
//          QMessageBox::warning(this,"sdf","Creat Flie Fail",QMessageBox::Yes) ;
        }
    }
    QFile *save_static_file=new QFile("手动存储/data.txt");
    save_static_file->open(QIODevice::Text|QIODevice::WriteOnly|QIODevice::Append);
    QTextStream stream(save_static_file);
    stream.seek(save_static_file->size());
    if(!save_static_file->size()){              // 如果是新文件，先写入表头
        stream <<fbg_txt_1+"\n";
    }
    QString static_txt = strTime + "\t" + wavelength_static_txt + "\t" + strain_static_txt;   // 写入当前静态数据，包含时间戳、波长和应变信息
    stream << static_txt << "\n";
    save_static_file->close();
    emit saveManualFinished();
}
