#include "disp3dfreshmodelthread.h"

disp3dfreshmodelthread::disp3dfreshmodelthread(QObject *parent):QThread(parent)
{

}

void disp3dfreshmodelthread::GetPtr(disp3dmodel *externdisplaymodel, IFE *externife)
{
    m_disp3dmodel = externdisplaymodel;
    ife = externife;
}

void disp3dfreshmodelthread::run()
{
    long i = 1;
    while (!isInterruptionRequested()){
        if (!isPaused){

            for (auto it = m_disp3dmodel->rawPointMap.begin(); it != m_disp3dmodel->rawPointMap.end(); ++it) {
                vtkIdType pid = it.key();
                const QVector<double>& p0 = it.value();               // 原始点
                const QVector<double>& dp = m_disp3dmodel->pointDisplace[pid];       // 点位移

                QVector<double> p(3);
                p[0] = p0[0] + dp[0];
                p[1] = p0[1] + dp[1];
                p[2] = p0[2] + dp[2];
//                p[0] = p0[0] + (0.0001 * i++);  p[1] = p0[1] + (0.0001 * i++);  p[2] = p0[2] + (0.0001 * i++);

                m_disp3dmodel->pointpos[pid] = p;
            }

            vtkSmartPointer<vtkPoints> pts = m_disp3dmodel->dynPolyData->GetPoints();
            for (auto it = m_disp3dmodel->pointpos.begin(); it != m_disp3dmodel->pointpos.end(); ++it) {
                vtkIdType pid = it.key();
                const QVector<double>& p = it.value();

                pts->SetPoint(pid, p[0], p[1], p[2]);
            }

            pts->Modified();            // 通知 VTK 点已经被修改
            m_disp3dmodel->dynPolyData->Modified();
            m_disp3dmodel->FreshKeyPoint();
            msleep(10);
        } else {
            msleep(100);
        }
    }
}
