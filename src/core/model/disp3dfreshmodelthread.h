#ifndef DISP3DFRESHMODELTHREAD_H
#define DISP3DFRESHMODELTHREAD_H

#include <QThread>

#include <disp3dmodel.h>
#include <trackingpoint.h>
#include <IFE.h>
#include <rainbowife.h>

#include <testsavepointsdisp.h>


class disp3dfreshmodelthread: public QThread
{
public:
    explicit disp3dfreshmodelthread(QObject *parent);
    ~disp3dfreshmodelthread() override;

    void startThread() {isPaused = false; start();}
    void pauseThread() {isPaused = true;}

    //弃用
    void GetPtr(disp3dmodel* externdisplaymodel,IFE* externife);
    //改为单独获取对象指针
    void GetModelPtr(disp3dmodel* renptr){m_disp3dmodel = renptr;}
    void GetRainbowIFE(rainbowIFE *renptr){m_rainbowIFE = renptr;}
    void GetTrackingPointPtr(trackingpoint* trackingpoint){m_trackingpoint = trackingpoint;}

    void normalizeScalarArray(vtkSmartPointer<vtkDoubleArray> arr);

protected:
    void run() override;

private:
    disp3dmodel* m_disp3dmodel;
    trackingpoint* m_trackingpoint;

    IFE* ife;
    rainbowIFE *m_rainbowIFE;
    QLabel* label;

    testsavepointsdisp* m_dispsave;
    bool Flag_Save;

private:
    bool isPaused;
};

#endif // DISP3DFRESHMODELTHREAD_H
