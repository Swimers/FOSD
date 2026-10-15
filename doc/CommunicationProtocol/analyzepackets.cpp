#include "analyzepackets.h"

#include <QFile>
#include <QDateTime>
#include <QTimer>
#include <QMessageBox>
#include <QIODevice>

#include "global.h"
#include "Timer.h"

namespace {
constexpr int PacketHeaderSize = 4;

quint8 packetByte(const QByteArray &bytes, int index)
{
    return static_cast<quint8>(bytes.at(index));
}

bool hasPacketBytes(const QByteArray &bytes, int minSize)
{
    return bytes.size() >= minSize;
}
}

analyzepackets::analyzepackets(QObject *parent) : QObject(parent)
{
    QFile file_txt("data/calibration/初始波长.txt");
    if(file_txt.open(QIODevice::ReadOnly|QIODevice::Text)){
        //读取文件
        QTextStream TextRead(&file_txt);
        QString data_line;
        int wave_count=0;
        while(!TextRead.atEnd()){
            data_line=TextRead.readLine();
            if(wave_count < 1024){
                original_wavelength[wave_count]= data_line.toFloat();
                wave_count++;
            }
        }
        file_txt.close();
    }

    // 在构造时创建定时器（此时还未 moveToThread，parent 安全）
    timer2 = new QTimer(this);
    timer3 = new QTimer(this);
}

analyzepackets::~analyzepackets()
{
    // 关闭自动保存文件句柄
    if (m_saveDynamicFile != nullptr) {
        if (m_saveDynamicFile->isOpen())
            m_saveDynamicFile->close();
        delete m_saveDynamicFile;
        m_saveDynamicFile = nullptr;
    }
    disconnect(m_saveConnection);

    // timer 的 parent 设为 nullptr 避免跨线程析构问题
    if (timer2) {
        timer2->stop();
        timer2->setParent(nullptr);
        delete timer2;
        timer2 = nullptr;
    }
    if (timer3) {
        timer3->stop();
        timer3->setParent(nullptr);
        delete timer3;
        timer3 = nullptr;
    }
}

void analyzepackets::setObj(connectwindow *obj)
{
    m_connectwindow = obj;
}

void analyzepackets::processChannel(int channel)
{
    if (channel < 0 || channel >= MAX_CHANNELS || !hasPacketBytes(array, 8)) {
        qWarning() << "Invalid channel packet header, size:" << array.size() << "channel:" << channel;
        return;
    }

    FLOATA data_count_xy;
    FLOATA data_count_x;
    // 读取通道总点数（array[6]和array[7]）
    data_count_xy.u8Byte[0] = array[6];
    data_count_xy.u8Byte[1] = array[7];
    data_count_xy.u8Byte[2] = 0;
    data_count_xy.u8Byte[3] = 0;
    CH_point[channel] = data_count_xy.all;

    const int pointCount = CH_point[channel];
    const int requiredXBytes = 8 + pointCount * 2;
    const int requiredYBytes = 708 + pointCount * 2;
    const int requiredBytes = requiredXBytes > requiredYBytes ? requiredXBytes : requiredYBytes;
    if (pointCount < 0 || pointCount > MAX_POINTS ||
        !hasPacketBytes(array, requiredBytes)) {
        qWarning() << "Invalid channel packet length, size:" << array.size()
                   << "channel:" << channel
                   << "points:" << pointCount;
        CH_point[channel] = 0;
        return;
    }

    // 读取X轴数据（从索引8开始，每个点2字节）
    for (int j = 0; j < pointCount; j++) {
        data_count_x.u8Byte[0] = array[2 * j + 8];
        data_count_x.u8Byte[1] = array[2 * j + 9];
        data_count_x.u8Byte[2] = 0;
        data_count_x.u8Byte[3] = 0;
        data_x_trans[channel][j] = data_count_x.all;
    }

    // 读取Y轴数据（从索引708开始，每个点2字节）
    for (int j = 0; j < pointCount; j++) {
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
        QFile file_txt("data/calibration/初始波长.txt");
        if(file_txt.open(QIODevice::WriteOnly|QIODevice::Text)){
            QTextStream TextWrite(&file_txt);
            for(int i=0;i<CH_SUM;i++){
                original_wavelength[i] = original_wavelength_temp[i]/20;
                original_wavelength_temp[i] = 0;
                TextWrite << original_wavelength[i] << endl;
            }
            file_txt.close();
        }
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
    QMutexLocker locker(&mutexAnalyze);

    if (!hasPacketBytes(array, DataAddress)) {
        qWarning() << "Main packet too short:" << array.size();
        return;
    }

    const int totalSensors = packetByte(array, MAX_CHANNELS + 4);
    if (totalSensors > 1024 ||
        !hasPacketBytes(array, DataAddress + totalSensors * 4)) {
        qWarning() << "Invalid main packet length, size:" << array.size()
                   << "sensor count:" << totalSensors;
        return;
    }

    //数组初始化
    for(int j=0;j<1024;j++){
        FBG_count[j]  = "";
        NewStrain[j]  = 0;
        wavelength[j] = "";
        strain[j]     = "";
    }
    //获取传感器数量
    for (int i=0;i<MAX_CHANNELS;i++) {
        channel_counts[i] = packetByte(array, i + 4);//通道1传感器数目
    }
    CH_SUM = totalSensors;//总传感器数目
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
            // 动态保存至CSV
            if (Checked_CH_Count > 0) {
                fbg_txt        += ",";
                wavelength_txt += ",";
                strain_txt     += ",";
            }
            fbg_txt        += FBG_count[global_index];
            wavelength_txt += wavelength[global_index];
            strain_txt     += strain[global_index];
            // 最后一个数据点处理
            if (i == CH_SUM - 1) {
                fbg_txt_1 = "time," + fbg_txt + "," + fbg_txt;
                strain_txt_1 = strain_txt;
                wavelength_txt_1 = wavelength_txt;
                str_txt = strTime + "," + wavelength_txt_1 + "," + strain_txt_1;
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
         if (i > 0) {
             wavelength_static_txt_temp += ",";
             strain_static_txt_temp += ",";
         }
         wavelength_static_txt_temp += wavelength_static[i];
         strain_static_txt_temp += strain_static[i];
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

    if (!hasPacketBytes(datagram, PacketHeaderSize)) {
        qWarning() << "Packet too short:" << datagram.size();
        return;
    }
    array = datagram;
    uint8_t b0 = packetByte(datagram, 0);
    uint8_t b1 = packetByte(datagram, 1);
    uint8_t b2 = packetByte(datagram, 2);
    uint8_t b3 = packetByte(datagram, 3);

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
    QMutexLocker locker(&mutexStrain);
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

    if(Flag_DynamicSave == 0){               //关闭自动保存
        timer2->stop();
        if (m_saveDynamicFile!=nullptr)
        {
            if(m_saveDynamicFile->isOpen())
                m_saveDynamicFile->close();

            delete m_saveDynamicFile;
            m_saveDynamicFile = nullptr;
        }
        disconnect(m_saveConnection);
    }
    if(Flag_DynamicSave == 1){                               //开启自动保存
        timer2->start(AutoSaveCycle);
        QDateTime data_time=QDateTime::currentDateTime();
        QString time_str = data_time.toString("yyyy-MM-dd hh_mm_ss");
        QDir DataFile;
        DataFile.mkpath("data/outputs/自动存储");
        QString time_ms_str = time_str+" "+ QString::number(AutoSaveCycle) +"ms ";
        m_saveDynamicFile=new QFile("data/outputs/自动存储/"+time_ms_str+"data.csv");
        m_saveDynamicFile->open(QIODevice::Text|QIODevice::WriteOnly|QIODevice::Append);
        m_saveConnection = connect(timer2,&QTimer::timeout,[this](){
                QTextStream stream(m_saveDynamicFile);
                stream.seek(m_saveDynamicFile->size());
                if(!m_saveDynamicFile->size()){
                    stream << fbg_txt_1 << "\n";
                }
                stream << str_txt << "\n";
            });
    }
}

void analyzepackets::saveManual()
{
    QDateTime data_time=QDateTime::currentDateTime();
    QString strTime=data_time.toString("yyyy-MM-dd hh:mm:ss");
    QDir DataFile;
    DataFile.mkpath("data/outputs/手动存储");
    QFile save_static_file("data/outputs/手动存储/data.csv");
    save_static_file.open(QIODevice::Text|QIODevice::WriteOnly|QIODevice::Append);
    QTextStream stream(&save_static_file);
    stream.seek(save_static_file.size());
    if(!save_static_file.size()){
        stream <<fbg_txt_1+"\n";
    }
    QString static_txt = strTime + "," + wavelength_static_txt + "," + strain_static_txt;
    stream << static_txt << "\n";
    save_static_file.close();
    emit saveManualFinished();
}
