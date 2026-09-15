#ifndef DISP3DMODEL_H
#define DISP3DMODEL_H

#include <QLabel>
#include <QWidget>
#include <QMap>
#include <QVector>
#include <QSet>

#include <vtkAutoInit.h>
#include <vtkSmartPointer.h>
#include <vtkPoints.h>
#include <vtkPolyData.h>
#include <vtkPolyDataMapper.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkActor.h>
#include <vtkCamera.h>
#include <vtkTextActor.h>
#include <vtkSTLReader.h>
#include <vtkRenderer.h>
#include <vtkGenericOpenGLRenderWindow.h>
#include <QVTKOpenGLNativeWidget.h>
#include <vtkTriangleFilter.h>
#include <vtkLoopSubdivisionFilter.h>
#include <vtkWindowedSincPolyDataFilter.h>
#include <vtkPolyDataNormals.h>
#include <vtkSTLWriter.h>

#include <disp3dglobal.h>

// 模型细分优化配置（编译期常量）
struct MeshOptimizeConfig
{
    const int   subdivisionLevel   = 1;     // Loop细分等级（每级面数×4）
    const bool  enableSmooth       = false;  // 是否启用Sinc平滑
    const int   smoothIterations   = 10;    // Sinc平滑迭代次数
    const double smoothPassBand    = 0.1;   // Sinc通带系数（0~2，越小越平滑）
    const bool  smoothBoundary     = false;  // 是否平滑边界
    const bool  normalizeCoords    = true;  // 是否归一化坐标（防数值漂移）
    const bool  saveOptimized      = true;  // 是否保存优化后的模型
};

class disp3dmodel
{
private:
    //vtkSmartPointer<vtkGenericOpenGLRenderWindow> m_renderWindow = vtkSmartPointer<vtkGenericOpenGLRenderWindow>::New();
    vtkSmartPointer<vtkRenderWindow> renderWindow = vtkSmartPointer<vtkRenderWindow>::New();
    vtkSmartPointer<vtkSTLReader> reader = vtkSmartPointer<vtkSTLReader>::New();
    vtkSmartPointer<vtkPolyDataMapper> mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    vtkSmartPointer<vtkActor> modelActor = vtkSmartPointer<vtkActor>::New();

    vtkSmartPointer<vtkRenderer>renderer = vtkSmartPointer<vtkRenderer>::New();                                                                    //声明渲染器
    vtkRenderWindowInteractor * iren = nullptr;
    vtkSmartPointer<vtkPolyData> rawPolyData = nullptr;

public:
    disp3dmodel();
    ~disp3dmodel();
    void initdisp3dmodel();

public:
    vtkRenderWindow* GetVTKRenderWindow(){return renderWindow;}
    vtkRenderer* GetVTKRenderer(){return renderer;}
    vtkActor* GetVTKActor(){return modelActor;}
    void SetInteractor(vtkRenderWindowInteractor* interactor) { iren = interactor; }
    void SetRenderWindow(vtkRenderWindow* renWin) {renderWindow = renWin; }

    void PickerGetLabPtr(std::tuple<QLabel *,QLabel *,QLabel *,QLabel *>);
    void PickerClear();

    bool LoadModel(const QString &filePath);
    void ExportModel(const QString &filePath);
    vtkSmartPointer<vtkPolyData> OptimizeMesh(vtkSmartPointer<vtkPolyData> input);
    void UnLoadModel();
    void MergeModel(const QString &filePath);
    void ResetModel();
    void ReadPointQMap(vtkSmartPointer<vtkPolyData> PolyData);

    void AllKeyPoint();
    void FreshKeyPoint();
    void ClearKeyPoint();

    //模型参数相关
    void GetModelPointsNum();
    void GetModelPolysNum();
    void GetModelKeyPointsNum();
    void GetModelSize();

    void setModelColor();

    //相机相关
    void ResetCamera(int Index);
    void CameraLock(bool index);
    void CameraFellow(bool index);

    //映射相关
    void InitScalarsValues(ModelScalars &scalars, vtkPolyData* poly);
    void SetupColorMapping(const std::string &scalarName);
    void SetNormalizeMode(bool enabled);
    void SetCustomRange(double rangeMin, double rangeMax);
    void OFFScalars();
    void normalizeScalarArray(vtkSmartPointer<vtkDoubleArray> arr);

public:
    bool Flag_Actor = false;

public:
    vtkSmartPointer<vtkPolyData> dynPolyData = nullptr;

    QMap<vtkIdType, QVector<double>> rawPointMap;   //原始点数据（编号，坐标向量）
    QMap<vtkIdType, QVector<double>> pointDisplace; //各点形变量（编号，坐标向量）
    QMap<vtkIdType, QVector<double>> pointpos;      //各点实际位置（编号，坐标向量）
    QSet<vtkIdType> keyPointSet;                    //关键点列表

    double MaxDisplace = 0; //最大形变
    double Displacefactor = 1;

    uint ModelPointsNum;
    uint ModelPolysNum;
    uint ModelKeyPointsNum;
    double ModelSize;
    // 显控台深色场景下的模型基色：冷调钢蓝，与近黑背景拉开对比又不喧宾夺主。
    // 原值 {1.0, 1.0, 0.2} 是亮黄，在深色场景里过于刺眼。
    QVector<float> ModelColor{0.1800f, 0.2800f, 0.4000f};

    QMap<vtkIdType, struct SelectedPointEntity> SelectedPointMap;

    ModelScalars modelScalars;      //颜色映射表

    bool m_normalize = true;        //归一化映射模式
    double m_customRangeMin = 0.0;  //自定义映射范围最小值
    double m_customRangeMax = 50.0; //自定义映射范围最大值
    std::string m_activeScalarName; //当前激活的标量名称

    int displacefactor = 2;
};

#endif // DISP3DMODEL_H
