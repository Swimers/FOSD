#include "testdatainjector.h"
#include <cstring>

static float littleEndianBytesToFloat(const uchar *p)
{
    quint32 u = qFromLittleEndian<quint32>(p);
    float f = 0.0f;
    std::memcpy(&f, &u, sizeof(float));
    return f;
}

TestDataInjector::TestDataInjector(rainbowIFE* rainbowIFE, QObject* parent)
    : QObject(parent),
      m_rainbowIFE(rainbowIFE)
{
    m_timer = new QTimer(this);
    m_timer->setTimerType(Qt::PreciseTimer);
}

TestDataInjector::~TestDataInjector()
{
    stop();
}

void TestDataInjector::stop()
{
    if (m_timer && m_timer->isActive())
    {
        m_timer->stop();
    }

    if (m_dataFile)
    {
        if (m_dataFile->isOpen())
        {
            m_dataFile->close();
        }

        m_dataFile->deleteLater();
        m_dataFile = nullptr;
    }

    if (m_dataStream)
    {
        delete m_dataStream;
        m_dataStream = nullptr;
    }
}

void TestDataInjector::startStaticInjection(int intervalMs)
{
    if (!m_rainbowIFE)
    {
        qDebug() << "TestDataInjector error: rainbowIFE pointer is null.";
        return;
    }

    stop();

    initStaticTestData();

    m_timer->setInterval(intervalMs);

    disconnect(m_timer, nullptr, this, nullptr);

    connect(m_timer, &QTimer::timeout, this, [this]()
    {
        if (!m_rainbowIFE)
            return;

        m_rainbowIFE->GetDataTest(m_testData);
    });

    m_timer->start();

    qDebug() << "Static test data injection started.";
}

bool TestDataInjector::startTxtInjection(const QString& filePath, int intervalMs)
{
    if (!m_rainbowIFE)
    {
        qDebug() << "TestDataInjector error: rainbowIFE pointer is null.";
        return false;
    }

    stop();

    m_dataFile = new QFile(filePath, this);

    if (!m_dataFile->open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug() << "Open file failed:" << filePath;

        m_dataFile->deleteLater();
        m_dataFile = nullptr;

        return false;
    }

    m_dataStream = new QTextStream(m_dataFile);

    m_timer->setInterval(intervalMs);

    disconnect(m_timer, nullptr, this, nullptr);

    connect(m_timer, &QTimer::timeout, this, [this]()
    {
        if (!m_dataStream || !m_dataFile)
            return;

        if (m_dataStream->atEnd())
        {
            qDebug() << "Data injection finished.";
            stop();
            return;
        }

        QString line = m_dataStream->readLine().trimmed();

        if (line.isEmpty())
            return;

        if (!parseOneFrame(line))
        {
            qDebug() << "Parse frame failed:" << line;
            return;
        }

        if (m_rainbowIFE)
        {
            m_rainbowIFE->GetDataTest(m_testData);
        }
    });

    m_timer->start();

    qDebug() << "Txt data injection started:" << filePath;

    return true;
}

bool TestDataInjector::parseOneFrame(const QString& line)
{
#if QT_VERSION < QT_VERSION_CHECK(5, 15, 0)
    QStringList parts = line.split(QRegExp("\\s+"), QString::SkipEmptyParts);
#else
    QStringList parts = line.split(QRegExp("\\s+"), Qt::SkipEmptyParts);
#endif

    // 1列时间戳 + 12列strain + 12列temperature + 16列free = 41列
    const int expectedCount = 41;

    if (parts.size() != expectedCount)
    {
        qDebug() << "Column count error, got:" << parts.size()
                 << " expected:" << expectedCount;
        return false;
    }

    int index = 0;

    QString timestamp = parts[index++];
    qDebug() << "timestamp =" << timestamp;

    bool ok = false;

    // 12列 strain
    for (int i = 0; i < 12; ++i)
    {
        double value = parts[index++].toDouble(&ok);

        if (!ok)
        {
            qDebug() << "Parse strain failed at index:" << i;
            return false;
        }

        m_testData.strian[i] = value;
    }

    // 12列 temperature
    for (int i = 0; i < 12; ++i)
    {
        double value = parts[index++].toDouble(&ok);

        if (!ok)
        {
            qDebug() << "Parse temperature failed at index:" << i;
            return false;
        }

        m_testData.temper[i] = value;
    }

    // 16列 free
    for (int i = 0; i < 16; ++i)
    {
        double value = parts[index++].toDouble(&ok);

        if (!ok)
        {
            qDebug() << "Parse free failed at index:" << i;
            return false;
        }

        m_testData.free[i] = value;
    }

    return true;
}

void TestDataInjector::initStaticTestData()
{
    m_testData.strian[0]    =     1.34753e-07 ;
    m_testData.strian[1]    =    -7.54395e-09;
    m_testData.strian[2]    =     2.16337e-05 ;
    m_testData.strian[3]    =    -0.0001712  ;
    m_testData.strian[4]    =     1.62092e-06 ;
    m_testData.strian[5]    =     0.000600131 ;
    m_testData.strian[6]    =    -9.6297e-06 ;
    m_testData.strian[7]    =     1e-05       ;
    m_testData.strian[8]    =     0.000274251 ;
    m_testData.strian[9]    =    -1.46806e-06;
    m_testData.strian[10]   =    -4.28546e-05;
    m_testData.strian[11]   =     2.63123e-05 ;

    m_testData.temper[0]    =     13.4199     ;
    m_testData.temper[1]    =     14.1432     ;
    m_testData.temper[2]    =    -26911.6    ;
    m_testData.temper[3]    =    -71783.3    ;
    m_testData.temper[4]    =     25.878      ;
    m_testData.temper[5]    =     23.6607     ;
    m_testData.temper[6]    =    -25655.5    ;
    m_testData.temper[7]    =    -71701.5    ;
    m_testData.temper[8]    =    -58513.5    ;
    m_testData.temper[9]    =    -64028.1    ;
    m_testData.temper[10]   =     18.9787     ;
    m_testData.temper[11]   =     20.5338     ;

    m_testData.free[0]      =    -0.00612021 ;
    m_testData.free[1]      =     0.000293988 ;
    m_testData.free[2]      =    -0.127511   ;
    m_testData.free[3]      =     1.81128     ;
    m_testData.free[4]      =     35.8849     ;
    m_testData.free[5]      =    -0.00160754 ;
    m_testData.free[6]      =    -0.0115843  ;
    m_testData.free[7]      =     0.000532961 ;
    m_testData.free[8]      =    -0.0073519  ;
    m_testData.free[9]      =     0.00220374  ;
    m_testData.free[10]     =     1.03886     ;
    m_testData.free[11]     =     13.4526     ;
    m_testData.free[12]     =     30.7584     ;
    m_testData.free[13]     =     0.00769909  ;
    m_testData.free[14]     =    -0.00150323 ;
    m_testData.free[15]     =     0.00387649  ;
}

NET_SEND_BUFF4 TestDataInjector::AnalysisB3(const QString &b3)
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
