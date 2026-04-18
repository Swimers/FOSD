#include "testb3pocket.h"

testb3pocket::testb3pocket()
{

}

testb3pocket::~testb3pocket()
{

}


static float littleEndianBytesToFloat(const uchar *p)
{
    quint32 u = qFromLittleEndian<quint32>(p);
    float f = 0.0f;
    std::memcpy(&f, &u, sizeof(float));
    return f;
}

NET_SEND_BUFF4 testb3pocket::AnalysisB3(const QString &b3)
{
    NET_SEND_BUFF4 result = {};

    // 1. 去掉空格、冒号等非十六进制字符
    QString hexStr = b3;
    hexStr.remove(QRegularExpression("[^0-9A-Fa-f]"));

    // 2. 转为字节数组
    QByteArray data = QByteArray::fromHex(hexStr.toLatin1());

    // 协议长度：
    // 4字节 flag + 40个 float(160字节) + 4字节结束标记 = 168字节
    const int expectedSize = 4 + 40 * 4 + 4;
    if (data.size() < expectedSize) {
        qWarning() << "AnalysisB3: 数据长度不足, size =" << data.size()
                   << ", expected >=" << expectedSize;
        return result;
    }

    const uchar *buf = reinterpret_cast<const uchar *>(data.constData());

    // 3. 解析包头 flag（大端 4 字节）
    quint32 flag = qFromBigEndian<quint32>(buf);
    result.flag = static_cast<long>(flag);

    // 4. 解析 40 个小端 float，从偏移 4 开始
    int offset = 4;

    // strian[12]
    for (int i = 0; i < 12; ++i) {
        result.strian[i] = littleEndianBytesToFloat(buf + offset);
        offset += 4;
    }

    // temper[12]
    for (int i = 0; i < 12; ++i) {
        result.temper[i] = littleEndianBytesToFloat(buf + offset);
        offset += 4;
    }

    // free[16]
    for (int i = 0; i < 16; ++i) {
        result.free[i] = littleEndianBytesToFloat(buf + offset);
        offset += 4;
    }

    // 5. 如需检查结尾标记，可读取最后4字节
    // Python中只取了 data[164]，这里顺便完整读出来
    quint32 endFlag = qFromBigEndian<quint32>(buf + 164);
    qDebug() << "flag =" << QString("0x%1").arg(flag, 8, 16, QLatin1Char('0'));
    qDebug() << "endFlag =" << QString("0x%1").arg(endFlag, 8, 16, QLatin1Char('0'));

    return result;
}
