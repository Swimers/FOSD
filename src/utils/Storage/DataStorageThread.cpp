/**
 * @file DataStorageThread.cpp
 * @brief 数据存储线程实现
 */

#include "DataStorageThread.h"

#include <QDir>
#include <QDateTime>
#include <QDebug>

DataStorageThread::DataStorageThread(QObject* parent) : QThread(parent) {
}

DataStorageThread::~DataStorageThread() {
    stopThread();
}

void DataStorageThread::enqueueData(const StorageData& data) {
    QMutexLocker locker(&m_mutex);
    m_dataQueue.enqueue(data);

    // 当队列有数据时，唤醒存储线程
    m_condition.wakeOne();
}

void DataStorageThread::setStoragePath(const QString& path) {
    QMutexLocker locker(&m_mutex);
    m_storagePath = path;
}

void DataStorageThread::stopThread() {
    {
        QMutexLocker locker(&m_mutex);
        m_stopFlag = true;
        m_condition.wakeAll();  // 唤醒线程以便它可以检查停止标志
    }

    // 等待线程结束
    if (isRunning()) {
        wait(5000);  // 最多等待5秒
        if (isRunning()) {
            qWarning() << "DataStorageThread: 强制终止线程";
            terminate();
            wait();
        }
    }
}

int DataStorageThread::queueSize() {
    QMutexLocker locker(&m_mutex);
    return m_dataQueue.size();
}

void DataStorageThread::run() {
    qDebug() << "DataStorageThread: 存储线程已启动";

    while (true) {
        QList<StorageData> batchData;

        // 从队列获取数据
        {
            QMutexLocker locker(&m_mutex);

            // 如果队列为空且没有停止，则等待
            while (m_dataQueue.isEmpty() && !m_stopFlag) {
                // 等待新数据或超时
                m_condition.wait(&m_mutex, MAX_WAIT_MS);
            }

            // 检查是否需要停止
            if (m_stopFlag && m_dataQueue.isEmpty()) {
                break;  // 队列已空，可以安全退出
            }

            // 批量取出数据（最多 BATCH_SIZE 条）
            int count = qMin(m_dataQueue.size(), BATCH_SIZE);
            for (int i = 0; i < count; ++i) {
                batchData.append(m_dataQueue.dequeue());
            }
        }

        // 在锁外执行文件写入（耗时操作）
        if (!batchData.isEmpty()) {
            writeToFile(batchData);
        }
    }

    qDebug() << "DataStorageThread: 存储线程已退出";
}

void DataStorageThread::writeToFile(const QList<StorageData>& dataList) {
    if (dataList.isEmpty()) {
        return;
    }

    QString path;
    {
        QMutexLocker locker(&m_mutex);
        path = m_storagePath;
    }

    if (path.isEmpty()) {
        qWarning() << "DataStorageThread: 存储路径未设置";
        return;
    }

    // 确保输出目录存在
    QString outputDir = path + "/ouput";
    QDir dir;
    if (!dir.exists(outputDir)) {
        if (!dir.mkpath(outputDir)) {
            qWarning() << "DataStorageThread: 创建目录失败:" << outputDir;
            return;
        }
    }

    // 按日期生成文件名
    QString dateStr = QDate::currentDate().toString("yyyy-MM-dd");
    QString filePath = outputDir + "/" + dateStr + "data.txt";

    // 打开文件（追加模式）
    QFile file(filePath);
    if (!file.open(QIODevice::Text | QIODevice::WriteOnly | QIODevice::Append)) {
        qWarning() << "DataStorageThread: 打开文件失败:" << file.errorString();
        return;
    }

    // 批量写入数据
    QTextStream out(&file);
    for (const StorageData& data : dataList) {
        out << formatDataLine(data);
    }

    file.close();

    // 可选：输出调试信息（生产环境可注释掉）
    // qDebug() << "DataStorageThread: 写入" << dataList.size() << "条数据";
}

#include <QtEndian>

QString DataStorageThread::formatDataLine(const StorageData& data) {

    qint32 flag = qToBigEndian<qint32>(data.flag);
    QByteArray flagBytes(reinterpret_cast<const char*>(&flag), 4);
    QByteArray flagHex = flagBytes.toHex();

    QStringList flagHexList;
    for (int i = 0; i < flagHex.size(); i += 2) {
        flagHexList << QString::fromLatin1(flagHex.mid(i, 2));
    }

    QStringList strianStrList;
    for (int i = 0; i < 12; ++i) {
        strianStrList << QString::number(data.strian[i]);
    }

    QStringList temperStrList;
    for (int i = 0; i < 12; ++i) {
        temperStrList << QString::number(data.temper[i]);
    }

    QStringList freeStrList;
    for (int i = 0; i < 16; ++i) {
        freeStrList << QString::number(data.free[i]);
    }

    return data.timestamp + " " +
           flagHexList.join(" ") + " " +
           strianStrList.join(" ") + " " +
           temperStrList.join(" ") + " " +
           freeStrList.join(" ") + "\n";
}

QString DataStorageThread::formatDataLine2(const StorageData& data)
{
    QByteArray rawData;
    rawData.reserve(4 + 12 * 4 + 12 * 4 + 16 * 4);

    // 1. flag 按 4 字节写入
    qint32 flag = qToBigEndian<qint32>(data.flag);
    rawData.append(reinterpret_cast<const char*>(&flag), 4);

    // 2. strian 按 float 原始4字节写入
    for (int i = 0; i < 12; ++i) {
        rawData.append(reinterpret_cast<const char*>(&data.strian[i]), sizeof(float));
    }

    // 3. temper 按 float 原始4字节写入
    for (int i = 0; i < 12; ++i) {
        rawData.append(reinterpret_cast<const char*>(&data.temper[i]), sizeof(float));
    }

    // 4. free 按 float 原始4字节写入
    for (int i = 0; i < 16; ++i) {
        rawData.append(reinterpret_cast<const char*>(&data.free[i]), sizeof(float));
    }

    // 转成十六进制字符串，例如 "00b213a3..."
    QByteArray hexData = rawData.toHex();

    // 插入空格，变成 "00 b2 13 a3 ..."
    QString hexString;
    hexString.reserve(hexData.size() + hexData.size() / 2);

    for (int i = 0; i < hexData.size(); i += 2) {
        if (i > 0) {
            hexString += ' ';
        }
        hexString += QString::fromLatin1(hexData.mid(i, 2));
    }

    // timestamp 保持原始格式
    return data.timestamp + " " + hexString + "\n";
}
