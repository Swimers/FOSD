#ifndef TESTSAVEPOINTSDISP_H
#define TESTSAVEPOINTSDISP_H

#include <QThread>
#include <QMutex>
#include <QWaitCondition>
#include <QQueue>
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QtMath>

class testsavepointsdisp : public QThread
{
    Q_OBJECT

public:
    struct PointDispSample
    {
        double x;
        double y;
        double z;

        double dx;
        double dy;
        double dz;
    };

public:
    explicit testsavepointsdisp(QObject* parent = nullptr);
    ~testsavepointsdisp();

    // 主运算线程只调用这个函数，不判断目标点
    void appendPointSample(double x, double y, double z,
                           double dx, double dy, double dz);

    void stopThread();

protected:
    void run() override;

private:
    bool openFiles();
    void closeFiles();

    void processSample(const PointDispSample& sample);

    bool isNegTargetPoint(double x, double y, double z) const;
    bool isPosTargetPoint(double x, double y, double z) const;

private:
    QMutex m_mutex;
    QWaitCondition m_waitCondition;
    QQueue<PointDispSample> m_queue;

    bool m_running = true;

    QFile m_fileNeg;
    QFile m_filePos;

    QTextStream* m_outNeg = nullptr;
    QTextStream* m_outPos = nullptr;

private:
    const double m_eps = 1e-2;

    const double m_targetX = 7931.09;
    const double m_targetY = 121.16;
    const double m_targetZNeg = -13500.00;
    const double m_targetZPos = 13500.00;
};

#endif // TESTSAVEPOINTSDISP_H
