#include "disp3dfreshmodelthread.h"

#include <QDebug>
#include <QFile>
#include <QMetaType>
#include <QMetaObject>
#include <QMutexLocker>
#include <QTextStream>
#include <QtMath>

#include <cstring>

#include <vtkCaptionActor2D.h>

#include <global.h>
#include <trackingpoint.h>
#include <testsavepointsdisp.h>

disp3dfreshmodelthread::disp3dfreshmodelthread(QObject *parent):QThread(parent)
{
    qRegisterMetaType<DisplayPointUpdateList>("DisplayPointUpdateList");
    qRegisterMetaType<TrackingPointFrameUpdateList>("TrackingPointFrameUpdateList");

    Flag_Save = false;
    m_dispsave = new testsavepointsdisp();
    m_dispsave->start();
}

disp3dfreshmodelthread::~disp3dfreshmodelthread()
{
    requestInterruption();
    isPaused = false;
    wait();

    if(m_dispsave){
        m_dispsave->stopThread();
        m_dispsave->wait();
        delete m_dispsave;
        m_dispsave = nullptr;
    }
}

void disp3dfreshmodelthread::GetPtr(disp3dmodel *externdisplaymodel, IFE *externife)
{
    m_disp3dmodel = externdisplaymodel;
    ife = externife;
}

void disp3dfreshmodelthread::startThread()
{
    captureInputData();
    isPaused = false;

    if (!isRunning()) {
        start();
    }
}

void disp3dfreshmodelthread::pauseThread()
{
    isPaused = true;
}

void disp3dfreshmodelthread::stopThread()
{
    requestInterruption();
    isPaused = false;
    wait();
}

void disp3dfreshmodelthread::captureInputData()
{
    QMutexLocker locker(&m_inputMutex);
    m_rawPointSamples.clear();
    m_trackingPointSamples.clear();
    m_displaceFactor = m_disp3dmodel ? m_disp3dmodel->Displacefactor : 1.0;

    if (m_disp3dmodel) {
        m_rawPointSamples.reserve(m_disp3dmodel->rawPointMap.size());
        for (auto it = m_disp3dmodel->rawPointMap.constBegin();
             it != m_disp3dmodel->rawPointMap.constEnd(); ++it) {
            if (it.value().size() < 3) {
                continue;
            }

            RawPointSample sample;
            sample.id = it.key();
            sample.position[0] = it.value()[0];
            sample.position[1] = it.value()[1];
            sample.position[2] = it.value()[2];
            m_rawPointSamples.append(sample);
        }
    }

    if (m_trackingpoint) {
        m_trackingPointSamples.reserve(m_trackingpoint->trackingPoints.size());
        for (TrackingPoint *tp : m_trackingpoint->trackingPoints) {
            if (!tp) {
                continue;
            }

            TrackingPointSample sample;
            sample.id = tp->id;
            sample.originalPos[0] = tp->originalPos[0];
            sample.originalPos[1] = tp->originalPos[1];
            sample.originalPos[2] = tp->originalPos[2];
            m_trackingPointSamples.append(sample);
        }
    }
}

void disp3dfreshmodelthread::normalizeScalarArray(vtkSmartPointer<vtkDoubleArray> arr)
{
    double minV = VTK_DOUBLE_MAX;
    double maxV = -VTK_DOUBLE_MAX;

    vtkIdType n = arr->GetNumberOfTuples();
    for (vtkIdType i = 0; i < n; i++) {
        double v = arr->GetValue(i);
        if (v < minV) minV = v;
        if (v > maxV) maxV = v;
    }

    double range = maxV - minV;
    if (range < 1e-12) {
        for (vtkIdType i = 0; i < n; i++) {
            arr->SetValue(i, 0.0);
        }
        return;
    }

    for (vtkIdType i = 0; i < n; i++) {
        double v = arr->GetValue(i);
        double norm = (v - minV) / range;
        arr->SetValue(i, norm);
    }
}

void disp3dfreshmodelthread::run()
{
    while (!isInterruptionRequested()) {
        if (isPaused) {
            msleep(100);
            continue;
        }

        if (!ife) {
            msleep(100);
            continue;
        }

        QVector<RawPointSample> rawPointSamples;
        QVector<TrackingPointSample> trackingPointSamples;
        double displaceFactor = 1.0;
        {
            QMutexLocker locker(&m_inputMutex);
            rawPointSamples = m_rawPointSamples;
            trackingPointSamples = m_trackingPointSamples;
            displaceFactor = m_displaceFactor;
        }

        ife->calculateDisp();

        double node_u_local[20];
        {
            QMutexLocker locker(&mutexStrain);
            memcpy(node_u_local, ife->node_u, sizeof(node_u_local));
        }

        DisplayPointUpdateList pointUpdates;
        pointUpdates.reserve(rawPointSamples.size());
        double frameMaxDisplace = 0.0;

        for (const RawPointSample &sample : rawPointSamples) {
            double disp[3] = {0.0, 0.0, 0.0};
            ife->ShapeFunction_New(sample.position[0],
                                   sample.position[1],
                                   node_u_local,
                                   disp);

            if (m_dispsave && Flag_Save) {
                m_dispsave->appendPointSample(sample.position[0], sample.position[1], sample.position[2], disp[0], disp[1], disp[2]);
            }

            DisplayPointUpdate update;
            update.id = sample.id;
            for (int i = 0; i < 3; ++i) {
                update.displacement[i] = disp[i] * 1000.0;
                update.position[i] = sample.position[i] + update.displacement[i];
                const double absDisplacement = qAbs(update.displacement[i]);
                if (absDisplacement > frameMaxDisplace) {
                    frameMaxDisplace = absDisplacement;
                }
            }
            pointUpdates.append(update);
        }

        TrackingPointFrameUpdateList trackingUpdates;
        trackingUpdates.reserve(trackingPointSamples.size());
        for (const TrackingPointSample &sample : trackingPointSamples) {
            double disp[3] = {0.0, 0.0, 0.0};
            ife->ShapeFunction_New(sample.originalPos[0],
                                        sample.originalPos[1],
                                        node_u_local,
                                        disp);

            TrackingPointFrameUpdate update;
            update.id = sample.id;
            for (int i = 0; i < 3; ++i) {
                update.displacement[i] = disp[i] * 1000.0;
                update.position[i] = sample.originalPos[i] + displaceFactor * update.displacement[i];
            }
            trackingUpdates.append(update);
        }

        if (m_frameReceiver) {
            QMetaObject::invokeMethod(m_frameReceiver,
                                      "ApplyDisplayFrame",
                                      Qt::QueuedConnection,
                                      Q_ARG(DisplayPointUpdateList, pointUpdates),
                                      Q_ARG(double, frameMaxDisplace),
                                      Q_ARG(TrackingPointFrameUpdateList, trackingUpdates));
        }
        msleep(10);
    }
}
