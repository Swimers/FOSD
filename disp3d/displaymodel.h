#ifndef DISPLAYMODEL_H
#define DISPLAYMODEL_H

#include "QVTKWidget.h"
#include <QWidget>
#include <vtkAutoInit.h>
#include <vtkRenderWindow.h>
#include <vtkSmartPointer.h>
#include <QMap>
#include "selfdefinedata.h"
#include "vtkPoints.h"
#include "vtkCellArray.h"
#include "vtkUnstructuredGrid.h"
#include "vtkActor.h"
#include "vtkDataSetMapper.h"
#include "vtkAxesActor.h"
#include "vtkCamera.h"
#include "vtkTextActor.h"
//#include "picker.h"
#include <QLabel>
#define PI 3.1415926
typedef double(*babianxingpoint)[3]; //定义存储六边形上点信息的集合
VTK_MODULE_INIT(vtkRenderingOpenGL2)
VTK_MODULE_INIT(vtkInteractionStyle)
VTK_MODULE_INIT(vtkRenderingFreeType);

class displayModel
{
    /************************新的一些***********************************/
    //想法: 应该是一个vtkRenderWindow包含多个vtkRender；一个vtkRender包含多个vtkactor，但因为关键点的设置变形量的设置需要调整，目前没写
public:
    displayModel();
    ~displayModel();

    enum ScalarTable{
        strain = 1,
        temperature =2,
        dispx = 3,
        dispy = 4,
        dispz = 5
    };

private:
    vtkSmartPointer<vtkRenderWindow> renderWindow = vtkSmartPointer<vtkRenderWindow>::New();    //视窗
    //PointPickerInteractorStyle *style;//创建交互器对象
    vtkRenderer* renderer;
    vtkRenderWindowInteractor *iren;
public:
    vtkRenderWindow* GetvtkRenderWindow()       //得到窗口指针
    {
        return renderWindow;
    }
    vtkRenderer* GetvtkRenderer()       //得到窗口指针
    {
        return renderer;
    }
    bool SetvtkRenderBackColor(double r,double g,double b);   //设置背景颜色
    bool SetvtkRenderBackColor2(double r1,double g1,double b1,double r2,double g2,double b2);     //设置渐变类型背景颜色



    /*************************旧的一些******************************/
private:
    //读取TXT储存数据
    int node_num=0;       //结点数
    int element_num=0;       //单元数
    int shell_num=0;       //壳单元数
    int triangle_number=0;//三角单元数
    int quad_number=0;    //方形单元数
    int section_number=0;
    int beam_number = 0;    //梁单元数
    int flag_number = 0;  //关键点数
    int reversibleele_num = 0;
public:
    int Displacefactor = 1;
    float SphereSize = 0.02f;
public:
    QMap<int,node*> nodes;  //节点集合
    QMap<int,shell*> shells;  //节点集合
    QMap<int,beam*> beams;  //梁单元集合
    QMap<int,sectionInfo*> sectionInfos;
    QMap<int, int> LineModeFlagpointToNodePoint; //线面模型标记点到节点(TXT中)的序列关系,这个是关键的集合
    QMap<int, int> SolidModeFlagPointToNodePoint;//实体模型标记点到节点的序列关系
    QMap<int, int> SolidModeNodePointToFlagPoint;//实体模型节点到标记点的序列关系
    QMap<int, ReversibleElementPointSet*> ReversibleElementDvaluepointSet; //所有逆有限单元中的差值点
    QMap<int, ReversibleElement*> ReversibleElementSet; //定义可逆单元坐标系的集合
    QMap<int, shell_len* >ReversibleLen;
    QMap<int, ChannalEle*> SensorANDInserverRelationSet; //定义传感器数据与可逆单元特定位置应变数据的关联集合
    QMap<int, QVector<double>*> strainset;  //应变集合
    QMap< int, surface* > new_element_info1;

    QMap< int, nodedisplace*> PointDisplaceSet; //所有点位移的集合
    //新旧节点关系列表，这个在创建新的节点其他信息时非常有用
    QMap< int, QVector<int>* > new_node_ex_node_info;
    //这个是旧节点对应新节点的集合
    QMap< int, struct nodeinfo*> new_node_info1;
    QList<TextActor*> TextActorLink;
    QList <CordDisplayFlag*> DisplayTextNodeFlagLink;
    QList<struct cordinfo*> DisplayCord;//显示的结点信息的集合
    QList<struct cordinfo*> CurrentDisplayCord; //变形之后结点位置坐标集合

    vtkFloatArray* scalars_dispx = nullptr;      //标量数据--应变
    vtkFloatArray* scalars_dispy = nullptr;      //标量数据--应变
    vtkFloatArray* scalars_dispz = nullptr;      //标量数据--应变
    vtkFloatArray* scalars_strain = nullptr;      //标量数据--应变
    vtkFloatArray* scalars_temp = nullptr;        //标量数据--温度
public:
    bool ActorFlag = false;
    vtkPoints *points = nullptr;//网格模型的点集
    vtkPoints *S_M_points = nullptr;//表面模型的点集
    vtkCellArray * cellarray = nullptr; //线框单元集合
    vtkCellArray * S_M_cellarray = nullptr;//表面模型的集合
    vtkUnstructuredGrid* cubeGrid = nullptr;//线框模型数据集
    vtkUnstructuredGrid* S_M_cubeGrid = nullptr;//表面模型的数据集

    vtkDataSetMapper* GridMapper = nullptr;
    vtkActor* GridActor = nullptr;

    vtkAxesActor* axe = nullptr; //图形显示中的坐标原点上的坐标系对象

    int LineNumber; //线单元的个数
    int Beam_Number;//梁单元的个数
    int TriangleNumber;//三角形单元的个数
    int Shell_Triangle_Number;//三角形板单元的个数
    int QuadNumber;//四边形单元的个数
    int Shell_Quad_Number;//四边形板单元的个数
    int *Type = nullptr; //线面模型单元类型
    int *S_M_Type = nullptr;//实体模型单元类型
    void CreatePointSet();  //创建点集合
    void CreateCellSet();   //创建单元集合
    void CreateDataSet();   //创建数据集合
    /*函数caculatevectorji的前三个参数是第一个向量，后三个参数是第二个向量*/
    double * caculatevectorji(double x1, double y1, double z1, double  x2, double y2, double z2); //计算向量积
    double* tran_cord(double tran_Max[4][4], double l_cord[]);
    babianxingpoint Getzhengliubianxing(double r); //获得半径为r的正六边形
    void ExecutePipeline(); //显示模型函数 ，输入参数视图类的指针
    void SetScalarMapping(ScalarTable& scalarindex,float min,float max);
    void AddAxesActor();        //添加坐标系

    void Resetcamera(int DisplayFalg);//视图显示功能
    void AxesShow(bool flag);
    void LineShowPipeline(bool flag);

    void MoveCameraToPosition(vtkCamera* camera, double* initialPosition, double* finalPosition, double t);
    /*函数WorldCordToDisplayCord的参数 WorldCord是世界坐标系的坐标，DisplayCord是屏幕坐标系下的坐标，myview是视图类的指针*/
    void WorldCordToDisplayCord(double* WorldCord,double* DisplayCord);   //世界坐标转化为显示坐标
    /*函数CordToCordCHAR的参数x、y、x是位移信息，参数str是坐标转化为字符串的结果*/
    void CordToCordCHAR(double x, double y, double z, char *str);
    /*函数CreateCordTextobject的参数CordInfo显示文本的字符串信息，x,y为文本在屏幕坐标系下的位置*/
    vtkTextActor* CreateCordTextobject(const char *CordInfo, int x, int y);
    /*函数CreatesphereSource的参数x、y、z是重要点标记的位置，scale是标记的大小*/
    vtkActor* CreatesphereSource(double x, double y, double z);//创建重要点标记

    /*/*函数createDisplayPointDisplaceText的参数nodeNo是显示位移点的编号，参数x,y,z点的坐标，参数myview视图类的对象*/
    void createDisplayPointDisplaceText(int nodeNo, double x_cord, double y_cord, double z_cord);//创建并在图形上显示重要点的初始位移*/

    /*函数Addsphere的参数actor是标记对象，参数Node是标记点的编号，myview是视图类的指针*/
    void Addsphere(vtkActor*actor, int Node);//添加重要点标记
    /*函数Deletesphere的参数 Node是要删除重要点的编号，myview时视图类的指针*/
    void Deletesphere(int Node);//删除重要点标记
    /*函数DeleteDisplayCordText的参数nodeNO是重要点的编号，yview是视图类的指针*/
    void DeleteDisplayCordText(int nodeNO);//删除显示重要点位移的文本
    /*函数EditDisplayCordText的参数BFnodeNo和AFnodeNo是修改前后重要点的编号，参数x_cord、y_cord、 z_cord是修改后重要点的坐标，myview是视图类的指针*/
    void EditDisplayCordText(int BFnodeNo, int AFnodeNo, double x_cord, double y_cord, double z_cord);//修改显示重要点位移的文本
    /*函数EditDisplayCordFlagInfo的参数BFnodeNo和AFnodeNo是修改前后重要点的编号，参数x_cord、y_cord、 z_cord是修改后重要点的坐标，myview是视图类的指针*/
    void EditDisplayCordFlagInfo(int BFnodeNO, int AFnodeNo, double x_cord, double y_cord, double z_cord);//修改显示重要点标记信息
    void ModifySphereSize(float size);
    float GetSphereSize();

    bool readData(QString path);   //这个地方读取Actor的数据储存到node_num...中，但我觉得这些node_num应该是在Actor类下的参数，不应该这么写

    friend class CordDisplaySet;
    friend class SelectDisplayNode;

public:
    void DynamicFresh();
    void refreshFlagPointTextPoisiton(); //刷新关键点和文本
    void PickerGetQLabelPtr(std::tuple<QLabel *,QLabel *,QLabel *,QLabel *>);
};



#endif // DISPLAYMODEL_H
