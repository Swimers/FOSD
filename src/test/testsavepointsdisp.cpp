#include "testsavepointsdisp.h"
#include <QDir>

testsavepointsdisp::testsavepointsdisp(QObject* parent)
    : QThread(parent),
#ifdef FOSD_RK3588_ARM_LINUX
      m_fileNeg("data/outputs/disp_point_neg13500.txt"),
      m_filePos("data/outputs/disp_point_pos13500.txt")
#else
      m_fileNeg("D:/disp_point_neg13500.txt"),
      m_filePos("D:/disp_point_pos13500.txt")
#endif
{
}

testsavepointsdisp::~testsavepointsdisp()
{
    stopThread();
    wait();
}

void testsavepointsdisp::appendPointSample(double x, double y, double z,
                                            double dx, double dy, double dz)
{
    QMutexLocker locker(&m_mutex);

    PointDispSample sample;
    sample.x = x;
    sample.y = y;
    sample.z = z;
    sample.dx = dx;
    sample.dy = dy;
    sample.dz = dz;

    m_queue.enqueue(sample);

    m_waitCondition.wakeOne();
}

void testsavepointsdisp::stopThread()
{
    QMutexLocker locker(&m_mutex);
    m_running = false;
    m_waitCondition.wakeOne();
}

bool testsavepointsdisp::openFiles()
{
#ifdef FOSD_RK3588_ARM_LINUX
    if (!QDir().mkpath("data/outputs"))
        return false;
#endif
    if (!m_fileNeg.open(QIODevice::Append | QIODevice::Text))
    {
        qDebug() << "Open neg file failed:" << m_fileNeg.fileName();
        return false;
    }

    if (!m_filePos.open(QIODevice::Append | QIODevice::Text))
    {
        qDebug() << "Open pos file failed:" << m_filePos.fileName();
        m_fileNeg.close();
        return false;
    }

    m_outNeg = new QTextStream(&m_fileNeg);
    m_outPos = new QTextStream(&m_filePos);

    m_outNeg->setRealNumberNotation(QTextStream::FixedNotation);
    m_outNeg->setRealNumberPrecision(6);

    m_outPos->setRealNumberNotation(QTextStream::FixedNotation);
    m_outPos->setRealNumberPrecision(6);

    return true;
}

void testsavepointsdisp::closeFiles()
{
    if (m_outNeg)
    {
        m_outNeg->flush();
        delete m_outNeg;
        m_outNeg = nullptr;
    }

    if (m_outPos)
    {
        m_outPos->flush();
        delete m_outPos;
        m_outPos = nullptr;
    }

    if (m_fileNeg.isOpen())
        m_fileNeg.close();

    if (m_filePos.isOpen())
        m_filePos.close();
}

bool testsavepointsdisp::isNegTargetPoint(double x, double y, double z) const
{
    return qAbs(x - m_targetX) < m_eps &&
           qAbs(y - m_targetY) < m_eps &&
           qAbs(z - m_targetZNeg) < m_eps;
}

bool testsavepointsdisp::isPosTargetPoint(double x, double y, double z) const
{
    return qAbs(x - m_targetX) < m_eps &&
           qAbs(y - m_targetY) < m_eps &&
           qAbs(z - m_targetZPos) < m_eps;
}

void testsavepointsdisp::processSample(const PointDispSample& sample)
{
    if (isNegTargetPoint(sample.x, sample.y, sample.z))
    {
        if (m_outNeg)
        {
            (*m_outNeg) << sample.dx << " "
                        << sample.dy << " "
                        << sample.dz << "\n";
        }

        return;
    }

    if (isPosTargetPoint(sample.x, sample.y, sample.z))
    {
        if (m_outPos)
        {
            (*m_outPos) << sample.dx << " "
                        << sample.dy << " "
                        << sample.dz << "\n";
        }

        return;
    }
}

void testsavepointsdisp::run()
{
    if (!openFiles())
    {
        return;
    }

    while (true)
    {
        QList<PointDispSample> localSamples;

        {
            QMutexLocker locker(&m_mutex);

            while (m_queue.isEmpty() && m_running)
            {
                m_waitCondition.wait(&m_mutex);
            }

            if (!m_running && m_queue.isEmpty())
            {
                break;
            }

            while (!m_queue.isEmpty())
            {
                localSamples.append(m_queue.dequeue());
            }
        }

        for (const PointDispSample& sample : localSamples)
        {
            processSample(sample);
        }

        if (m_outNeg)
            m_outNeg->flush();

        if (m_outPos)
            m_outPos->flush();
    }

    closeFiles();

    qDebug() << "testsavepointsdisp stopped.";
}
