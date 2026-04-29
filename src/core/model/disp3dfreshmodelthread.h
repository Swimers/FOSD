#ifndef DISP3DFRESHMODELTHREAD_H
#define DISP3DFRESHMODELTHREAD_H

#include <QThread>
#include <QMetaType>
#include <QMutex>
#include <QVector>

#include <atomic>

#include <disp3dmodel.h>
#include <trackingpoint.h>
#include <IFE.h>
#include <rainbowife.h>

#include <testsavepointsdisp.h>

struct DisplayPointUpdate {
    vtkIdType id;
    double position[3];
    double displacement[3];
};

struct TrackingPointFrameUpdate {
    uint id;
    double position[3];
    double displacement[3];
};

typedef QVector<DisplayPointUpdate> DisplayPointUpdateList;
typedef QVector<TrackingPointFrameUpdate> TrackingPointFrameUpdateList;

Q_DECLARE_METATYPE(DisplayPointUpdateList)
Q_DECLARE_METATYPE(TrackingPointFrameUpdateList)

class disp3dfreshmodelthread: public QThread
{
public:
    explicit disp3dfreshmodelthread(QObject *parent);
    ~disp3dfreshmodelthread() override;

    void startThread();
    void pauseThread();
    void stopThread();

    //弃用
    void GetPtr(disp3dmodel* externdisplaymodel,IFE* externife);
    //改为单独获取对象指针
    void GetModelPtr(disp3dmodel* renptr){m_disp3dmodel = renptr;}
    void GetRainbowIFE(rainbowIFE *renptr){m_rainbowIFE = renptr;}
    void GetTrackingPointPtr(trackingpoint* trackingpoint){m_trackingpoint = trackingpoint;}
    void SetFrameReceiver(QObject *receiver){m_frameReceiver = receiver;}

    void normalizeScalarArray(vtkSmartPointer<vtkDoubleArray> arr);

protected:
    void run() override;

private:
    struct RawPointSample {
        vtkIdType id;
        double position[3];
    };

    struct TrackingPointSample {
        uint id;
        double originalPos[3];
    };

    void captureInputData();

    disp3dmodel* m_disp3dmodel = nullptr;
    trackingpoint* m_trackingpoint = nullptr;
    QObject *m_frameReceiver = nullptr;

    IFE* ife = nullptr;
    rainbowIFE *m_rainbowIFE = nullptr;
    QLabel* label = nullptr;

    testsavepointsdisp* m_dispsave;
    bool Flag_Save;

    QVector<RawPointSample> m_rawPointSamples;
    QVector<TrackingPointSample> m_trackingPointSamples;
    QMutex m_inputMutex;
    double m_displaceFactor = 1.0;

private:
    std::atomic_bool isPaused{true};
};

#endif // DISP3DFRESHMODELTHREAD_H
