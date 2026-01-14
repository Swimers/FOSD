#include "modelcalculationthread.h"
#include <QDebug>
#include "Timer.h"
#include <vtkFloatArray.h>
#pragma execution_character_set("utf-8")

ModelCalculationThread::ModelCalculationThread(QObject *parent)
{

}

void ModelCalculationThread::run()
{
    while (!isInterruptionRequested())
    {

        if (!isPaused)
        {
            double Globalbeampointdisplace[3] = {0};//全局坐标系下点的变形
            ife->calculateDisp();//更新自由度
            int elementnum = displaymodel->ReversibleElementDvaluepointSet.size();

            for (int i = 1; i <= elementnum; ++i)
            {
                QVector<int>* Dvaluepoint = displaymodel->ReversibleElementDvaluepointSet[i]->Dvaluepoint;
                int pointnumber = displaymodel->ReversibleElementDvaluepointSet[i]->DvaluePointNum;

                for (int j = 0; j < pointnumber; j++)
                {
                    int nodeNo = (*Dvaluepoint)[j];
                    double  BFx, BFy, BFz;/*BF全局坐标，AF局部坐标*/

                    //BFx = (displaymodel->nodes[nodeNo]->cord[0]) * 1000;
                    //BFy = (displaymodel->nodes[nodeNo]->cord[1]) * 1000;
                    //BFz = (displaymodel->nodes[nodeNo]->cord[2]) * 1000;
                    //(*cordinfo).x_cord = (*(displaymodel->new_node_info1[nodeNo])).x_cord;
                    //(*cordinfo).y_cord = (*(displaymodel->new_node_info1[nodeNo])).y_cord;
                    //(*cordinfo).z_cord = (*(displaymodel->new_node_info1[nodeNo])).z_cord;
                    // ife->N_shapefunction(BFx,BFy,BFz,ife->node_u,Globalbeampointdisplace);
                    //qDebug()<<"model"<<BFx<<BFy<<BFz<<Globalbeampointdisplace[0]<<Globalbeampointdisplace[1]<<Globalbeampointdisplace[2];

                    QVector<int>* pointset = displaymodel->new_node_ex_node_info[nodeNo];
                    int pnum = (*pointset).size();
                    for (int k = 0; k < pnum; ++k)
                    {
                        int  nodeno = (*pointset)[k];
                        double point[3] = { 0.0 };
                        BFx = displaymodel->new_node_info1[nodeno]->x_cord;
                        BFy = displaymodel->new_node_info1[nodeno]->y_cord;
                        BFz = displaymodel->new_node_info1[nodeno]->z_cord;
                        ife->N_shapefunction(BFx,BFy,BFz,ife->node_u,Globalbeampointdisplace);

                        displaymodel->PointDisplaceSet[nodeno]->xdisplace = Globalbeampointdisplace[0];
                        displaymodel->PointDisplaceSet[nodeno]->ydisplace = Globalbeampointdisplace[1];
                        displaymodel->PointDisplaceSet[nodeno]->zdisplace = Globalbeampointdisplace[2];

                        displaymodel->scalars_dispx->InsertTuple1(nodeno-1, (Globalbeampointdisplace[0]*1000));
                        displaymodel->scalars_dispy->InsertTuple1(nodeno-1, (Globalbeampointdisplace[1]*1000));
                        displaymodel->scalars_dispz->InsertTuple1(nodeno-1, (Globalbeampointdisplace[2]*1000));


                        point[0] = displaymodel->new_node_info1[nodeno]->x_cord+displaymodel->Displacefactor*displaymodel->PointDisplaceSet[nodeno]->xdisplace;//Displacefactor*
                        point[1] = displaymodel->new_node_info1[nodeno]->y_cord+displaymodel->Displacefactor*displaymodel->PointDisplaceSet[nodeno]->ydisplace;//Displacefactor*
                        point[2] = displaymodel->new_node_info1[nodeno]->z_cord+displaymodel->Displacefactor*displaymodel->PointDisplaceSet[nodeno]->zdisplace;//Displacefactor*
                        displaymodel->S_M_points->InsertPoint(nodeno-1, point[0], point[1], point[2]);
                    }
                }
            }
            displaymodel->S_M_points->Modified();



            msleep(20); // 数据计算频率，其实刷新频率到不了那么高
        } else {
            // 在暂停状态下，线程会休眠一段时间以减少 CPU 占用
            msleep(100);
        }
    }
}
