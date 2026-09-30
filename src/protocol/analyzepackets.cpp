#include "analyzepackets.h"

#include <QFile>
#include <QDateTime>
#include <QTimer>
#include <algorithm>
#include <QIODevice>
#include <QDir>
#include <QTextStream>
#include <QStringList>

#include "global.h"

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
    QFile file_txt("./初始波长.txt");
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
        original_wavelength_temp[i] += m_wavelengthFrame.records[i].wavelength;
    }
    if(count_num == 20){
        QFile file_txt("./初始波长.txt");
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
    WavelengthFrame frame;
    QString error;
    if (!parseLegacyWavelengthFrame(array, frame, &error)) {
        qWarning() << "Legacy wavelength packet rejected:" << error;
        return;
    }
    processWavelengthFrame(frame);
}

void analyzepackets::processWavelengthFrame(const WavelengthFrame &frame)
{
    QMutexLocker locker(&mutexAnalyze);
    // A changed sensor layout must not mix samples during the 20-frame reset.
    bool sameLayout = frame.records.size() == m_wavelengthFrame.records.size();
    for (int i = 0; sameLayout && i < frame.records.size(); ++i) {
        sameLayout = frame.records[i].channel == m_wavelengthFrame.records[i].channel &&
                     frame.records[i].sensor == m_wavelengthFrame.records[i].sensor;
    }
    if (Flag_Reset && !sameLayout) {
        count_num = 0;
        std::fill(original_wavelength_temp, original_wavelength_temp + 1024, 0.0f);
    }
    m_wavelengthFrame = frame;
    CH_SUM = frame.records.size();
    std::fill(channel_counts, channel_counts + MAX_CHANNELS, 0);
    std::fill(CH_point, CH_point + MAX_CHANNELS, 0);
    for (const auto &record : frame.records)
        ++channel_counts[record.channel - 1];
    for (int i = 0; i < 1024; ++i) {
        FBG_count[i].clear();
        wavelength[i].clear();
        strain[i].clear();
        CH_datagram[i] = 0;
        NewStrain[i] = 0;
        wavelength_static[i].clear();
        strain_static[i].clear();
    }
    if (Flag_Reset && CH_SUM > 0)
        processReset();

    QStringList names, waves, strains;
    int selected = 0;
    for (int i = 0; i < CH_SUM; ++i) {
        const auto &record = frame.records[i];
        if (!channel_flags[record.channel - 1])
            continue;
        FBG_count[selected] = QString("CH%1_%2").arg(record.channel).arg(record.sensor);
        CH_datagram[selected] = record.wavelength;
        wavelength[selected] = QString::number(double(record.wavelength), 'f', 5);
        NewStrain[selected] = (0.00075 * (record.wavelength - original_wavelength[i])) / A + B;
        strain[selected] = QString::number(double(NewStrain[selected]), 'f', 5);
        wavelength_static[selected] = wavelength[selected];
        strain_static[selected] = strain[selected];
        names.append(FBG_count[selected]);
        waves.append(wavelength[selected]);
        strains.append(strain[selected]);
        ++selected;
    }
    // Finalize after filtering: the final physical channel may be deselected.
    fbg_txt_1 = "time," + names.join(',') + "," + names.join(',');
    wavelength_txt_1 = waves.join(',');
    strain_txt_1 = strains.join(',');
    str_txt = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") +
              "," + wavelength_txt_1 + "," + strain_txt_1;
    wavelength_static_txt = wavelength_txt_1;
    strain_static_txt = strain_txt_1;
    fbg_txt.clear();
    wavelength_txt.clear();
    strain_txt.clear();
    wavelength_static_txt_temp.clear();
    strain_static_txt_temp.clear();
    updateData();
}

void analyzepackets::analyzeData(const QByteArray datagram)
{
    if (isWavelengthFrame(datagram)) {
        WavelengthFrame frame;
        QString error;
        if (!parseWavelengthFrame(datagram, frame, &error)) {
            qWarning() << "Wavelength packet rejected:" << error;
            return;
        }
        array = datagram;
        processWavelengthFrame(frame);
        return;
    }

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
    count_num = 0;
    std::fill(original_wavelength_temp, original_wavelength_temp + 1024, 0.0f);
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
