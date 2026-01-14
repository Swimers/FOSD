#ifndef SELFDEFINEDATA_H
#define SELFDEFINEDATA_H


#include <QVector>
#include <vtkTextActor.h>
#include <vtkActor.h>
//结点结构体，存储结点坐标和对应位置信息
struct node
{
   int  NodeNo; //结点编号
   double *cord; //结点的坐标
};


typedef struct shell
{
   int  ElementNo; //单元编号
   int  *NodeNo;   //单元结点
   double ElementThick; //单元厚度
   int ElementType;   //单元类型 指的是三角形还是四边形
}shell;

typedef struct beam
{
   int ElementNo; //单元编号
   int *NodeNo;   //单元结点
   int sectionNo; //单元截面号
   int ElementType; //单元类型  是梁还是杆
}beam;

typedef struct sectionInfo
{
   int sectionNo;   //截面编号
   int sectionshape;  //截面形状
   double *sectionGEo;  //截面几何属性
}sectionInfo;

//定义可逆单元的所需差值点的集合
typedef struct ReversibleElementPointSet
{
    int  elementNo;  //单元编号
    int  DvaluePointNum; // 单元差值点的个数
    QVector<int>* Dvaluepoint;//本单元差值点的集合
}ReversibleElementPointSet;

typedef struct ReversibleElement
{
   int eleNO;//逆有限单元编号
   double * ipoint; //逆有限单元坐标系i点坐标
   double * jpoint;//逆有限单元坐标系j点坐标
   double * kpoint;//逆有限单元坐标系k点坐标
}ReversibleElement;

struct shell_len
{
   double length; //结点编号
   double width; //结点的坐标
};

//定义通道的元素数据编号
typedef  struct   ChannalEle
{
   int ChannalNO;//通道编号
   int Channalbit; //通道中的第几个数据
} ChannalEle;

/*定义一个梁板实体单元*/
typedef struct surface
{
   int elementtype;//主要是梁板单元的
   int * leftface;//实体梁或板单元的一个端面上的点
   int * rightface;//实体梁或板单元的另一个端面上的点
}surface;

//定义实体单元的自定义数据类型
typedef struct nodeinfo
{
   int bianhao;//实体模型新造结点的编号
   double x_cord; //结点个x轴坐标
   double y_cord; //结点上y轴坐标
   double z_cord; //结点上z轴坐标
}nodeinfo;

typedef struct nodedisplace
{
   double xdisplace; //x向位移
   double ydisplace; //y向位移
   double zdisplace; //z向位移
} nodedisplace;

typedef struct TextActor
{
   int nodeNo;  //结点标号
   vtkTextActor* Actor; //位移文本对象
}TextActor;

typedef struct CordDisplayFlag
{
  int nodeNo; //节点编号
  vtkActor* Actor; //重要点标记对象
}CordDisplayFlag;


//显示坐标的自定义数据结构
typedef struct cordinfo
{
   int bianhao;//实体模型新造结点的编号
   double x_cord; //结点个x轴坐标
   double y_cord; //结点上y轴坐标
   double z_cord; //结点上z轴坐标
}cordinfo;
#endif // SELFDEFINEDATA_H
