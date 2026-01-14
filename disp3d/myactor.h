#ifndef MYACTOR_H
#define MYACTOR_H

#include <QString>
#include <QMap>
#include "selfdefinedata.h"


#include"vtkPoints.h"

class MyActor
{
public:
    MyActor();
    ~MyActor();

private:
    //读取TXT储存数据
    int node_num=0;       //结点数
    int element_num=0;       //单元数
    int shell_num=0;       //壳单元数
    //int section_number=0;
    int triangle_number=0;//三角单元数
    int quad_number=0;    //方形单元数
    int section_number=0;
    int beam_number = 0;    //梁单元数
    int flag_number = 0;  //关键点数
    int reversibleele_num = 0;


    QMap<int,node*> nodes;  //节点集合
    QMap<int,shell*> shells;  //节点集合
    QMap<int,beam*> beams;  //梁单元集合
    QMap<int,sectionInfo*> sectionInfos;
    QMap<int, int> LineModeFlagpointToNodePoint; //线面模型标记点到节点(TXT中)的序列关系
    QMap<int, ReversibleElementPointSet*> ReversibleElementDvaluepointSet; //所有逆有限单元中的差值点
    QMap<int, ReversibleElement*> ReversibleElementSet; //定义可逆单元坐标系的集合
    QMap<int, shell_len* >ReversibleLen;
    QMap<int, ChannalEle*> SensorANDInserverRelationSet; //定义传感器数据与可逆单元特定位置应变数据的关联集合
    QMap<int, QVector<double>*> strainset;  //应变集合

private:
    vtkPoints *points;//网格模型的点集



public:
    bool readdata(QString path);
    void CreatePointSet();
    void CreateCellSet();
};

#endif // MYACTOR_H
