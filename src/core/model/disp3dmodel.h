#ifndef DISP3DMODEL_H
#define DISP3DMODEL_H

#include <QLabel>
#include <QWidget>
#include <QMap>
#include <QVector>
#include <QSet>

#include <vtkPolyDataMapper.h>
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
#include <vtkSTLReader.h>

#include <QVTKOpenGLNativeWidget.h>
#include <vtkGenericOpenGLRenderWindow.h>
#include <vtkRenderer.h>

#include <disp3dglobal.h>

class disp3dmodel
{
private:
    //vtkSmartPointer<vtkGenericOpenGLRenderWindow> m_renderWindow = vtkSmartPointer<vtkGenericOpenGLRenderWindow>::New();
    vtkSmartPointer<vtkRenderWindow> renderWindow = vtkSmartPointer<vtkRenderWindow>::New();
    vtkSmartPointer<vtkSTLReader> reader = vtkSmartPointer<vtkSTLReader>::New();
    vtkSmartPointer<vtkPolyDataMapper> mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    vtkSmartPointer<vtkActor> modelActor = vtkSmartPointer<vtkActor>::New();

    vtkSmartPointer<vtkRenderer>renderer = vtkSmartPointer<vtkRenderer>::New();                                                                    //声明渲染器
    vtkRenderWindowInteractor * iren;
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

    void LoadModel(const QString &filePath);
    void ExportModel(const QString &filePath);
    void UnLoadModel();
    void MergeModel(const QString &filePath);
    void ResetModel();
    void ReadPointQMap(vtkSmartPointer<vtkPolyData> PolyData);

    void FreshKeyPoint();
    void ClearKeyPoint();

    //模型参数相关
    void GetModelPointsNum();
    void GetModelPolysNum();
    void GetModelKeyPointsNum();
    void GetModelSize();

    //相机相关
    void ResetCamera(int Index);
    void CameraLock(bool index);
    void CameraFellow(bool index);

    //映射相关
    void InitScalarsValues(ModelScalars &scalars, vtkPolyData* poly);
    void SetupColorMapping(const std::string &scalarName);
    void OFFScalars();

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

    QMap<vtkIdType, struct SelectedPointEntity> SelectedPointMap;

    ModelScalars modelScalars;      //颜色映射表

    int displacefactor = 2;

};

#endif // DISP3DMODEL_H
