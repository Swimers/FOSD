#ifndef TESTDATAINJECTOR_H
#define TESTDATAINJECTOR_H

#include <QObject>
#include <QTimer>
#include <QFile>
#include <QTextStream>
#include <QString>
#include <QStringList>
#include <QDebug>
#include <QtEndian>
#include <QRegularExpression>

#include <teststorge.h>
#include "rainbowife.h"

class TestDataInjector : public QObject
{
    Q_OBJECT

public:
    explicit TestDataInjector(rainbowIFE* rainbowIFE, QObject* parent = nullptr);
    ~TestDataInjector();

public:
    // 启动静态数据注入
    void startStaticInjection(int intervalMs = 25);

    // 启动txt逐帧动态数据注入
    bool startTxtInjection(const QString& filePath, int intervalMs = 25);

    // 停止注入
    void stop();

    NET_SEND_BUFF4 AnalysisB3(const QString &b3);

private:
    // 解析txt中的一行数据
    bool parseOneFrame(const QString& line);

    // 初始化静态测试数据
    void initStaticTestData();

private:
    rainbowIFE* m_rainbowIFE = nullptr;

    QTimer* m_timer = nullptr;

    QFile* m_dataFile = nullptr;
    QTextStream* m_dataStream = nullptr;

    NET_SEND_BUFF4 m_testData;
};

#endif // TESTDATAINJECTOR_H
