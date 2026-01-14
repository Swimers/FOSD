#ifndef DISP3DMODEL_H
#define DISP3DMODEL_H

#include <QLabel>
#include <QVTKWidget.h>
#include <QWidget>
#include <QMap>
#include <vtkAutoInit.h>
#include <vtkRenderWindow.h>
#include <vtkSmartPointer.h>

#include <vtkPoints.h>
#include <vtkCellArray.h>
#include <vtkUnstructuredGrid.h>
#include <vtkActor.h>
#include <vtkDataSetMapper.h>
#include <vtkAxesActor.h>
#include <vtkCamera.h>
#include <vtkTextActor.h>
#include "disp3ddeginedata.h"
//#include "picker.h"

typedef double(*babianxingpoint)[3]; //定义存储六边形上点信息的集合
#define PI 3.1415926
VTK_MODULE_INIT(vtkRenderingOpenGL2)
VTK_MODULE_INIT(vtkInteractionStyle)
VTK_MODULE_INIT(vtkRenderingFreeType)

class disp3dmodel
{
public:
    disp3dmodel();
    ~disp3dmodel();

public:
    enum ScalarTable{
        strain = 1,
        temperature =2,
        dispx = 3,
        dispy = 4,
        dispz = 5
    };

    int   Displacefactor = 2;
    float SphereSize = 0.02f;

<<<<<<< Updated upstream
    void LoadModel(const QString &filePath);
    void ExportModel(const QString &filePath);
    void UnLoadModel();
    void MergeModel(const QString &filePath);
    void ResetModel();
    void ReadPointQMap(vtkSmartPointer<vtkPolyData> PolyData);
=======
    bool Flag_Actor = 0;
>>>>>>> Stashed changes

private:    //读取TXT储存数据
    int node_num = 0;           //结点数
    int element_num = 0;        //单元数
    int shell_num =0;           //壳单元数
    int triangle_number = 0;    //三角单元数
    int quad_number = 0;        //方形单元数
    int section_number = 0;     //
    int beam_number = 0;        //梁单元数
    int flag_number = 0;        //关键点数
    int reversibleele_num = 0;  //

    int               *Type        = nullptr; //线面模型单元类型
    int               *S_M_Type    = nullptr; //实体模型单元类型
    vtkLine           **lines      = nullptr; //定义线单元
    vtkTriangle       **triangles  = nullptr; //定义三角形单元
    vtkQuad           **quads      = nullptr; //定义四边形单元
    vtkWedge          **wedge      = nullptr; //三角形壳单元
    vtkHexagonalPrism **liulengzhu = nullptr; //六边形梁单元
    vtkHexahedron     **Hexahdron1 = nullptr; //四边形壳单元

<<<<<<< Updated upstream
    void ResetCamera(int Index);
    void CameraLock(bool index);
    void CameraFellow(bool index);

    void InitScalarsValues(ModelScalars &scalars, vtkPolyData* poly);
    void SetupColorMapping(const std::string &scalarName);
    void OFFScalars();
=======
    int LineNumber;             //线单元的个数
    int Beam_Number;            //梁单元的个数
    int TriangleNumber;         //三角形单元的个数
    int Shell_Triangle_Number;  //三角形板单元的个数
    int QuadNumber;             //四边形单元的个数
    int Shell_Quad_Number;      //四边形板单元的个数
>>>>>>> Stashed changes

public:
    vtkPoints *points = nullptr;//网格模型的点集
    vtkPoints *S_M_points = nullptr;//表面模型的点集
    vtkCellArray * cellarray = nullptr; //线框单元集合
    vtkCellArray * S_M_cellarray = nullptr;//表面模型的集合
    vtkUnstructuredGrid* cubeGrid = nullptr;//线框模型数据集
    vtkUnstructuredGrid* S_M_cubeGrid = nullptr;//表面模型的数据集

    vtkFloatArray* scalars_dispx = nullptr;      //标量数据--应变
    vtkFloatArray* scalars_dispy = nullptr;      //标量数据--应变
    vtkFloatArray* scalars_dispz = nullptr;      //标量数据--应变
    vtkFloatArray* scalars_strain = nullptr;     //标量数据--应变
    vtkFloatArray* scalars_temp = nullptr;       //标量数据--温度

    vtkDataSetMapper* GridMapper = nullptr;
    vtkActor* GridActor = nullptr;

public:
    //这个是旧节点对应新节点的集合
    QMap <int, struct nodeinfo*> new_node_info1;        //
    QList<TextActor*> TextActorLink;                    //三维场景中所有文本标签
    QList<CordDisplayFlag*> DisplayTextNodeFlagLink;    //
    QList<struct cordinfo*> DisplayCord;                //显示的结点信息的集合
    QList<struct cordinfo*> CurrentDisplayCord;         //变形之后结点位置坐标集合
    QMap <int, nodedisplace*> PointDisplaceSet;         //所有点位移的集合

<<<<<<< Updated upstream
    QMap<vtkIdType, QVector<double>> rawPointMap;   //原始点数据（编号，坐标向量）
    QMap<vtkIdType, QVector<double>> pointDisplace; //各点形变量（编号，坐标向量）
    QMap<vtkIdType, QVector<double>> pointpos;      //各点实际位置（编号，坐标向量）
    QSet<vtkIdType> keyPointSet;                    //关键点列表
    double MaxDisplace = 0;
=======
    QMap<int,node*>  nodes;     //节点集合
    QMap<int,shell*> shells;    //节点集合
    QMap<int,beam*>  beams;     //梁单元集合
    QMap<int,sectionInfo*> sectionInfos;
    QMap<int, int> LineModeFlagpointToNodePoint;        //线面模型标记点到节点的序列关系,这个是关键的集合
    QMap<int, int> SolidModeFlagPointToNodePoint;       //实体模型标记点到节点的序列关系
    QMap<int, int> SolidModeNodePointToFlagPoint;       //实体模型节点到标记点的序列关系
    QMap<int, ReversibleElementPointSet*> ReversibleElementDvaluepointSet;     //所有逆有限单元中的差值点
    QMap<int, ReversibleElement*> ReversibleElementSet; //定义可逆单元坐标系的集合
    QMap<int, shell_len* >ReversibleLen;
    QMap<int, ChannalEle*> SensorANDInserverRelationSet;//定义传感器数据与可逆单元特定位置应变数据的关联集合
    QMap<int, QVector<double>*> strainset;              //应变集合
>>>>>>> Stashed changes

    QMap<int, surface*> new_element_info1;              //存储所有新生成的实体单元信息(键为单元编号，值为 surface* 指针)
    QMap<int, QVector<int>*> new_node_ex_node_info;     //新旧节点关系列表，这个在创建新的节点其他信息时非常有用

public:
    vtkRenderWindow* GetVtkRenderWindow(){return renderWindow;}                                     //得到窗口指针
    vtkRenderer* GetvtkRenderer(){return renderer;}                                                 //得到渲染器指针

    bool readData(QString path);                                                                    //从路径读取node数据

    void CreatePointSet();  //创建点集合
    void CreateCellSet();   //创建单元集合
    void CreateDataSet();   //创建数据集合

    void Disp3DModelDemo();                                                                         //测试函数，用于显示stl模
    bool SetVtkRenderBackColor(double r,double g,double b);                                         //设置背景颜色
    bool SetVtkRenderBackGradient(double r1,double g1,double b1,double r2,double g2,double b2);     //设置渐变类型背景颜色
    void AddAxesActor();                                                                            //添加坐标系
    void ExecutePipeline();                                                                         //显示模型函数 ，输入参数视图类的指针
    void AxesShow(bool flag);
    float GetSphereSize();
    void Resetcamera(int DisplayFalg);                                                              //视图显示功能
    void LineShowPipeline(bool flag);
    void SetScalarMapping(ScalarTable& scalarindex,double range);
    void refreshFlagPointTextPoisiton();                                                            //刷新关键点和文本
    void PickerGetQLabelPtr(std::tuple<QLabel *,QLabel *,QLabel *,QLabel *>);                       //获取参数显示标签地址
    void DynamicFresh();                                                                            //动态刷新

    void ModifySphereSize(float size);                                                                      //调整标记点模型大小
    void createDisplayPointDisplaceText(int nodeNo, double x_cord, double y_cord, double z_cord);           //函数createDisplayPointDisplaceText的参数nodeNo是显示位移点的编号，参数x,y,z点的坐标，参数myview视图类的对象*/
    vtkActor* CreatesphereSource(double x, double y, double z);                                             //创建重要点标记/*函数CreatesphereSource的参数x、y、z是重要点标记的位置，scale是标记的大小*/
    void Addsphere(vtkActor*actor, int Node);                                                               //添加重要点标记/*函数Addsphere的参数actor是标记对象，参数Node是标记点的编号，myview是视图类的指针*/
    void DeleteDisplayCordText(int nodeNO);                                                                 //删除显示重要点位移的文本/*函数DeleteDisplayCordText的参数nodeNO是重要点的编号，yview是视图类的指针*/
    void Deletesphere(int Node);                                                                            //删除重要点标记/*函数Deletesphere的参数 Node是要删除重要点的编号，myview时视图类的指针*/
    void EditDisplayCordText(int BFnodeNo, int AFnodeNo, double x_cord, double y_cord, double z_cord);      //修改显示重要点位移的文本/*函数EditDisplayCordText的参数BFnodeNo和AFnodeNo是修改前后重要点的编号，参数x_cord、y_cord、 z_cord是修改后重要点的坐标，myview是视图类的指针*/
    void EditDisplayCordFlagInfo(int BFnodeNO, int AFnodeNo, double x_cord, double y_cord, double z_cord);  //修改显示重要点标记信息/*函数EditDisplayCordFlagInfo的参数BFnodeNo和AFnodeNo是修改前后重要点的编号，参数x_cord、y_cord、 z_cord是修改后重要点的坐标，myview是视图类的指针*/

    void CordToCordCHAR(double x, double y, double z, char *str);                                   //函数CordToCordCHAR的参数x、y、x是位移信息，参数str是坐标转化为字符串的结果
    double * caculatevectorji(double x1, double y1, double z1, double  x2, double y2, double z2);   //计算向量积
    babianxingpoint Getzhengliubianxing(double r); //获得半径为r的正六边形
    double* tran_cord(double tran_Max[4][4], double l_cord[]);
    void WorldCordToDisplayCord(double* WorldCord,double* DisplayCord);                             //函数WorldCordToDisplayCord的参数 WorldCord是世界坐标系的坐标，DisplayCord是屏幕坐标系下的坐标，myview是视图类的指针*/
    vtkTextActor* CreateCordTextobject(const char *CordInfo, double x, double y);                   //函数CreateCordTextobject的参数CordInfo显示文本的字符串信息，x,y为文本在屏幕坐标系下的位置*/

private:
    vtkSmartPointer<vtkRenderWindow> renderWindow = vtkSmartPointer<vtkRenderWindow>::New();        //声明窗口指针
    vtkRenderer * renderer;                                                                         //声明渲染器
    vtkAxesActor * axe = nullptr;                                                                   //声明图形显示中的坐标原点上的坐标系对象
    vtkRenderWindowInteractor * iren;

};

#endif // DISP3DMODEL_H
