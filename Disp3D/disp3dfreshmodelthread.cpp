#include "disp3dfreshmodelthread.h"
#include <QDebug>
#include "Timer.h"
#include <vtkFloatArray.h>

#include <QDebug>

disp3dfreshmodelthread::disp3dfreshmodelthread(QObject *parent):QThread(parent)
{

}

void disp3dfreshmodelthread::GetPtr(disp3dmodel *externdisplaymodel, IFE *externife, QLabel *externlabel)
{
    displaymodel = externdisplaymodel;
    ife = externife;
    this->label = externlabel;
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
<<<<<<< Updated upstream
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
=======
            double Globalbeampointdisplace[3];  //存储全局坐标系下点的变形量 [x, y, z]
            ife->calculateDisp();               //步骤1：调用有限元求解器计算最新位移场
            int elementnum = displaymodel->ReversibleElementDvaluepointSet.size(); //获取需要处理的元素数量

            for (int i = 1; i <= elementnum; ++i){  //步骤2：for循环遍历每个逆有限元单元
                QVector<int>* Dvaluepoint = displaymodel->ReversibleElementDvaluepointSet[i]->Dvaluepoint;  //获取当前元素的点编号
                int pointnumber = displaymodel->ReversibleElementDvaluepointSet[i]->DvaluePointNum;         //获取集合内点个数

                for (int j = 0; j < pointnumber; j++){  //步骤3：for循环遍历单元内所有点
                    int nodeNo = (*Dvaluepoint)[j];
                    double  BFx, BFy, BFz;  /*BF全局坐标，AF局部坐标*/
                    BFx = (displaymodel->nodes[nodeNo]->cord[0]) * 1000;    //将节点坐标转换为毫米单位 (假设原始单位为米)
                    BFy = (displaymodel->nodes[nodeNo]->cord[1]) * 1000;
                    BFz = (displaymodel->nodes[nodeNo]->cord[2]) * 1000;

                    ife->ShapeFunction_New(BFx,BFy,ife->node_u,Globalbeampointdisplace);  // 步骤4: 计算全局坐标系下的点变形

                    QVector<int>* pointset = displaymodel->new_node_ex_node_info[nodeNo];   // 获取与当前节点关联的所有点集
                    int pnum = (*pointset).size();

                    for (int k = 0; k < pnum; ++k){  // 步骤5: 更新所有关联点的位移数据
                        int  nodeno = (*pointset)[k];
                        double point[3] = { 0.0 };
                        displaymodel->PointDisplaceSet[nodeno]->xdisplace = Globalbeampointdisplace[0];    // 存储计算得到的位移值 (转换回米单位)
                        displaymodel->PointDisplaceSet[nodeno]->ydisplace = Globalbeampointdisplace[1];
                        displaymodel->PointDisplaceSet[nodeno]->zdisplace = Globalbeampointdisplace[2];
                        displaymodel->scalars_dispx->InsertTuple1(nodeno-1,   std::abs(Globalbeampointdisplace[0])*1000);    // 更新标量数据数组 (用于颜色映射)
                        displaymodel->scalars_dispy->InsertTuple1(nodeno-1,   std::abs(Globalbeampointdisplace[1])*1000);
                        displaymodel->scalars_dispz->InsertTuple1(nodeno-1,   std::abs(Globalbeampointdisplace[2])*1000);
                        point[0] = displaymodel->new_node_info1[nodeno]->x_cord +   // 步骤6: 计算变形后的新坐标:原始坐标 + 变形因子 × 计算得到的位移
                                   displaymodel->Displacefactor * displaymodel->PointDisplaceSet[nodeno]->xdisplace;//Displacefactor*
                        point[1] = displaymodel->new_node_info1[nodeno]->y_cord +
                                   displaymodel->Displacefactor * displaymodel->PointDisplaceSet[nodeno]->ydisplace;//Displacefactor*
                        point[2] = displaymodel->new_node_info1[nodeno]->z_cord +
                                   displaymodel->Displacefactor * displaymodel->PointDisplaceSet[nodeno]->zdisplace;//Displacefactor*
                        displaymodel->S_M_points->InsertPoint(nodeno-1, point[0], point[1], point[2]);  // 更新vtkPoints中的点坐标
                    }
                }
            }
            displaymodel->S_M_points->Modified();
            //label->setStyleSheet("QLabel { font-weight: bold; color: green; }");
            //label->setText("主方向最大变形："+QString::number(ife->node_u[4],'f',2));
>>>>>>> Stashed changes
            msleep(10);
        } else {
            // 在暂停状态下，线程会休眠一段时间以减少 CPU 占用
            msleep(100);
        }
    }
}
