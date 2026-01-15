#include "disp3dfreshmodelthread.h"

#include <QDebug>

disp3dfreshmodelthread::disp3dfreshmodelthread(QObject *parent):QThread(parent)
{

}

void disp3dfreshmodelthread::GetPtr(disp3dmodel *externdisplaymodel, IFE *externife)
{
    m_disp3dmodel = externdisplaymodel;
    ife = externife;
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
        // 所有值相等，全部设为 0.5 也可以，这里设为 0 以保持稳健
        for (vtkIdType i = 0; i < n; i++)
            arr->SetValue(i, 0.0);
        return;
    }

    for (vtkIdType i = 0; i < n; i++) {
        double v = arr->GetValue(i);
        // 归一化到 [0,1]
        double norm = (v - minV) / range;
        arr->SetValue(i, norm);
    }
}

void disp3dfreshmodelthread::run()
{
    while (!isInterruptionRequested()){
        if (!isPaused){
            ife->calculateDisp();               //步骤1：调用有限元求解器计算最新位移场
            double Globalbeampointdisplace[3];
            for (auto it = m_disp3dmodel->rawPointMap.begin(); it != m_disp3dmodel->rawPointMap.end(); ++it) {
                vtkIdType pid = it.key();
                const QVector<double>& p0 = it.value();                              // 原始点
                ife->ShapeFunction_New(p0[0],p0[1],ife->node_u,Globalbeampointdisplace);
                QVector<double> p(3),dp(3);
                for (int i=0; i<3; i++) {
                    double dpmm = Globalbeampointdisplace[i]*1000;
                    if(dpmm > m_disp3dmodel->MaxDisplace)
                        m_disp3dmodel->MaxDisplace = dpmm;
                    dp[i] = dpmm;
                    p[i] = p0[i] + dp[i];
                }
//                long i = 1;p[0] = p0[0] + (0.0001 * i++);  p[1] = p0[1] + (0.0001 * i++);  p[2] = p0[2] + (0.0001 * i++);

                m_disp3dmodel->modelScalars.dispX->SetValue(pid, dp[0]);
                m_disp3dmodel->modelScalars.dispY->SetValue(pid, dp[1]);
                m_disp3dmodel->modelScalars.dispZ->SetValue(pid, dp[2]);

                m_disp3dmodel->pointpos[pid] = p;
                m_disp3dmodel->pointDisplace[pid] = dp;
            }

            vtkSmartPointer<vtkPoints> pts = m_disp3dmodel->dynPolyData->GetPoints();
            for (auto it = m_disp3dmodel->pointpos.begin(); it != m_disp3dmodel->pointpos.end(); ++it) {
                vtkIdType pid = it.key();
                const QVector<double>& p = it.value();
                pts->SetPoint(pid, p[0], p[1], p[2]);
            }

            normalizeScalarArray(m_disp3dmodel->modelScalars.dispX);
            normalizeScalarArray(m_disp3dmodel->modelScalars.dispY);
            normalizeScalarArray(m_disp3dmodel->modelScalars.dispZ);

            pts->Modified();            // 通知 VTK 点已经被修改
            m_disp3dmodel->dynPolyData->Modified();
            m_disp3dmodel->FreshKeyPoint();
            msleep(10);
        } else {
            msleep(100);
        }
    }
}
