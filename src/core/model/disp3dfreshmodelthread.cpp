#include "disp3dfreshmodelthread.h"

#include <QDebug>

#include <vtkCaptionActor2D.h>
#include <global.h>
#include <trackingpoint.h>

#include <QFile>
#include <QTextStream>
#include <QtMath>

#include <testsavepointsdisp.h>

disp3dfreshmodelthread::disp3dfreshmodelthread(QObject *parent):QThread(parent)
{
    Flag_Save = true;
    m_dispsave = new testsavepointsdisp();
    m_dispsave->start();
}

disp3dfreshmodelthread::~disp3dfreshmodelthread()
{
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
            m_rainbowIFE->calculateDisp();                           //步骤1：调用有限元求解器计算最新位移场
            double Globalbeampointdisplace[3];

            double node_u_local[16];
            {
                QMutexLocker locker(&mutexStrain);
                memcpy(node_u_local, m_rainbowIFE->node_u, sizeof(node_u_local));
            }

            for (auto it = m_disp3dmodel->rawPointMap.begin(); it != m_disp3dmodel->rawPointMap.end(); ++it) {// 步骤2：for循环遍历模型每个点
                vtkIdType pid = it.key();
                const QVector<double>& p0 = it.value();//原始点坐标

                //m_rainbowIFE->ShapeFunction_New(p0[0],p0[1],ife->node_u,Globalbeampointdisplace); //杭研单机翼       //步骤3：调用有限元插值得到每点位移
                m_rainbowIFE->shapefunction(p0[0], p0[1], p0[2], node_u_local, Globalbeampointdisplace);

                if (m_dispsave && Flag_Save){
                    m_dispsave->appendPointSample(p0[0], p0[1], p0[2], Globalbeampointdisplace[0], Globalbeampointdisplace[1], Globalbeampointdisplace[2]
                    );
                }

                QVector<double> p(3),dp(3);
                for (int i=0; i<3; i++) {               //步骤4：循环更新最大变形位移
                    double dpmm = Globalbeampointdisplace[i]*1000;
                    if(dpmm > m_disp3dmodel->MaxDisplace)
                        m_disp3dmodel->MaxDisplace = dpmm;
                    dp[i] = dpmm;
                    p[i] = p0[i] + dp[i];
                }
//              long i = 1;p[0] = p0[0] + (0.0001 * i++);  p[1] = p0[1] + (0.0001 * i++);  p[2] = p0[2] + (0.0001 * i++);
                m_disp3dmodel->modelScalars.dispX->SetValue(pid, dp[0]);
                m_disp3dmodel->modelScalars.dispY->SetValue(pid, dp[1]);
                m_disp3dmodel->modelScalars.dispZ->SetValue(pid, dp[2]);

                m_disp3dmodel->pointpos[pid] = p;
                m_disp3dmodel->pointDisplace[pid] = dp;
            }

            vtkSmartPointer<vtkPoints> pts = m_disp3dmodel->dynPolyData->GetPoints();
            for (auto it = m_disp3dmodel->pointpos.begin(); it != m_disp3dmodel->pointpos.end(); ++it) {    //步骤5: 更新所有关联点的位移数据
                vtkIdType pid = it.key();
                const QVector<double>& p = it.value();
                pts->SetPoint(pid, p[0], p[1], p[2]);
            }

            // normalizeScalarArray(m_disp3dmodel->modelScalars.dispX);    //步骤6: 更新所有关联点的位移映射关系
            // normalizeScalarArray(m_disp3dmodel->modelScalars.dispY);
            // normalizeScalarArray(m_disp3dmodel->modelScalars.dispZ);

            for (TrackingPoint *tp : m_trackingpoint->trackingPoints) { //步骤7: 更新追踪点坐标/模型/文本
                double disp[3];
                // 使用形函数计算该点的位移（使用局部副本）
                //ife->ShapeFunction_New(tp->originalPos[0],tp->originalPos[1],ife->node_u,disp);      //单机翼单点测试
                m_rainbowIFE->shapefunction(tp->originalPos[0], tp->originalPos[1], tp->originalPos[2], node_u_local, disp);

//                ife->shapefunction(tp->originalPos[0],  // x (毫米)
//                                   tp->originalPos[1],  // y
//                                   tp->originalPos[2],  // z
//                                   node_u_local, disp);  // 使用局部副本

                // 保存当前变形量（毫米）- 总是更新，不管可见性
                tp->currentDisp[0] = disp[0] * 1000.0;
                tp->currentDisp[1] = disp[1] * 1000.0;
                tp->currentDisp[2] = disp[2] * 1000.0;

                // 计算新位置 (转换为米)
                double newPos[3];
                newPos[0] = tp->originalPos[0] + m_disp3dmodel->Displacefactor * disp[0] * 1000.0;
                newPos[1] = tp->originalPos[1] + m_disp3dmodel->Displacefactor * disp[1] * 1000.0;
                newPos[2] = tp->originalPos[2] + m_disp3dmodel->Displacefactor * disp[2] * 1000.0;

                // 只有可见的点才更新3D显示（球体和文本）
                if (tp->visible) {
                    // 更新球体位置
                    tp->sphereActor->SetPosition(newPos);
                    tp->sphereActor->Modified();
                    // 更新文本标签内容和3D附着点位置
                    char dispText[100];
                    sprintf(dispText, "(%.3f, %.3f, %.3f)", tp->currentDisp[0], tp->currentDisp[1], tp->currentDisp[2]);
                    tp->textActor->SetCaption(dispText);
                    tp->textActor->SetAttachmentPoint(newPos);
                    tp->textActor->Modified();
                }
            }

            pts->Modified();    // 通知 VTK 点已经被修改
            m_disp3dmodel->dynPolyData->Modified(); //模型渲染
            m_disp3dmodel->FreshKeyPoint();         //更新关键点模型/文本坐标
            msleep(10);
        } else {
            msleep(100);
        }
    }
}
