/**
 * @file DataStorageThread.h
 * @brief 数据存储线程类
 * @details 独立线程负责将数据写入文件，避免阻塞主线程
 */

#ifndef DATASTORAGETHREAD_H
#define DATASTORAGETHREAD_H

#include <QThread>
#include <QMutex>
#include <QWaitCondition>
#include <QQueue>
#include <QString>
#include <QFile>
#include <QTextStream>

/**
 * @struct StorageData
 * @brief 待存储的数据结构
 */
struct StorageData {
    float strian[12];
    float temper[12];
    float free[16];
    QString timestamp;  // 时间戳字符串
};

/**
 * @class DataStorageThread
 * @brief 数据存储线程
 * @details 从线程安全队列中获取数据，批量写入文件
 *          使用生产者-消费者模式，主线程生产数据，存储线程消费数据
 */
class DataStorageThread : public QThread {
    Q_OBJECT

public:
    explicit DataStorageThread(QObject* parent = nullptr);
    ~DataStorageThread();

    /**
     * @brief 添加数据到存储队列
     * @param data 待存储的数据
     * @details 线程安全，可从任意线程调用
     */
    void enqueueData(const StorageData& data);

    /**
     * @brief 设置存储路径
     * @param path 文件存储的基础路径
     */
    void setStoragePath(const QString& path);

    /**
     * @brief 停止存储线程
     * @details 会等待队列中的数据全部写入后再退出
     */
    void stopThread();

    /**
     * @brief 获取队列中待处理的数据数量
     * @return 队列大小
     */
    int queueSize();

protected:
    void run() override;

private:
    /**
     * @brief 将数据写入文件
     * @param dataList 待写入的数据列表
     */
    void writeToFile(const QList<StorageData>& dataList);

    /**
     * @brief 格式化单条数据为字符串
     * @param data 数据
     * @return 格式化后的字符串
     */
    QString formatDataLine(const StorageData& data);

private:
    QQueue<StorageData> m_dataQueue;    // 数据队列
    QMutex m_mutex;                      // 队列互斥锁
    QWaitCondition m_condition;          // 条件变量，用于等待新数据

    QString m_storagePath;               // 存储路径
    bool m_stopFlag = false;             // 停止标志

    static const int BATCH_SIZE = 50;    // 批量写入的数据条数
    static const int MAX_WAIT_MS = 500;  // 最大等待时间(ms)，超时也会写入
};

#endif // DATASTORAGETHREAD_H
