#ifndef DISP3DMODEL_H
#define DISP3DMODEL_H

#include <QLabel>
#include <QWidget>
#include <QMap>
#include <QVector>
#include <QSet>

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
public:
    disp3dmodel();
    ~disp3dmodel();
    void initdisp3dmodel();

public:
    vtkGenericOpenGLRenderWindow* GetVTKRenderWindow(){return renderWindow;}
    vtkRenderer* GetVTKRenderer(){return renderer;}
    vtkActor* GetVTKActor(){return modelActor;}

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

    uint GetModelNumPoints();
    uint GetModelNumPolys();
    uint GetModelNumKeyPoints();

    void ResetCamera(int Index);
    void CameraLock(bool index);
    void CameraFellow(bool index);

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
    double MaxDisplace = 0;

    QMap<vtkIdType, struct SelectedPointEntity> SelectedPointMap;

    ModelScalars modelScalars;      //颜色映射表

    int displacefactor = 2;

private:
    vtkSmartPointer<vtkGenericOpenGLRenderWindow> renderWindow = vtkSmartPointer<vtkGenericOpenGLRenderWindow>::New();
    vtkSmartPointer<vtkSTLReader> reader = vtkSmartPointer<vtkSTLReader>::New();
    vtkSmartPointer<vtkPolyDataMapper> mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    vtkSmartPointer<vtkActor> modelActor = vtkSmartPointer<vtkActor>::New();

    vtkRenderer * renderer;                                                                     //声明渲染器
    vtkRenderWindowInteractor * iren;
    vtkSmartPointer<vtkPolyData> rawPolyData = nullptr;
};

#endif // DISP3DMODEL_H
