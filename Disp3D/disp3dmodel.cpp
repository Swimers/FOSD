#include "disp3dmodel.h"

#include <QFile>
#include <QDebug>

#include <vtkActor.h>
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkCamera.h>
#include <vtkPolyData.h>
#include <vtkPolyDataMapper.h>
#include <vtkAppendPolyData.h>
#include <vtkCleanPolyData.h>
#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkSTLReader.h>
#include <vtkSTLWriter.h>
#include <vtkSphereSource.h>
#include <vtkTextProperty.h>
#include <vtkProperty.h>
#include <vtkPointData.h>
#include <vtkLookupTable.h>

#include <QTextCodec>
#include <stdio.h>

#include <disp3dinteractor.h>

disp3dmodel::disp3dmodel()
{
    renderer = vtkRenderer::New();
    renderWindow->AddRenderer(renderer);

    iren = vtkRenderWindowInteractor::New();            //创建及交互器
    iren->SetRenderWindow(renderWindow);                //交互器添加窗口
    disp3dinteractor* style = new disp3dinteractor();
    style->GetDisplayModelPtr(this);
    iren->SetInteractorStyle(style);
    style->SetDefaultRenderer(renderer);

    renderWindow->Render();
}

disp3dmodel::~disp3dmodel()
{

}

void disp3dmodel::PickerGetLabPtr(std::tuple<QLabel *, QLabel *, QLabel *, QLabel *>labels)
{
    disp3dinteractor * style1 = dynamic_cast<disp3dinteractor *>(iren->GetInteractorStyle());
    style1->lab_KeyPointID = std::get<0>(labels);
    style1->lab_KeyPointRawPos = std::get<1>(labels);
    style1->lab_KeyPointDisplace = std::get<2>(labels);
    style1->lab_KeyPointNewPos = std::get<3>(labels);
}

void disp3dmodel::PickerClear()
{
    disp3dinteractor * style1 = dynamic_cast<disp3dinteractor *>(iren->GetInteractorStyle());
    style1->ClearPicker();
}

void disp3dmodel::LoadModel(const QString &filePath)
{
    modelScalars.Init();
    QTextCodec *codec = QTextCodec::codecForName("GBK");
    std::string localPath = codec->fromUnicode(filePath).data();    //中文路径的兼容性
    reader->SetFileName(localPath.c_str());
    reader->Update();
    rawPolyData = reader->GetOutput();
    dynPolyData = vtkSmartPointer<vtkPolyData>::New();
    dynPolyData->DeepCopy(rawPolyData);
    mapper->SetInputData(dynPolyData);
    InitScalarsValues(modelScalars, dynPolyData);

    modelActor->SetMapper(mapper);
    modelActor->GetProperty()->SetEdgeVisibility(0);
    modelActor->GetProperty()->SetLineWidth(static_cast<float>(0.02));
    modelActor->GetProperty()->SetEdgeColor(0, 0.88, 0.88);
    modelActor->GetProperty()->SetRepresentationToSurface();
    modelActor->GetProperty()->SetColor(0.16, 0.582, 0.973);

    renderer->AddActor(modelActor);
    renderer->ResetCamera();
    renderWindow->Render();

    ReadPointQMap(dynPolyData);
    Flag_Actor = true;
}

void disp3dmodel::ExportModel(const QString &filePath)
{
    QTextCodec *codec = QTextCodec::codecForName("GBK");
    std::string localPath = codec->fromUnicode(filePath).data();    //中文路径的兼容性
    vtkSmartPointer<vtkSTLWriter> writer = vtkSmartPointer<vtkSTLWriter>::New();
    writer->SetFileName(localPath.c_str());
    writer->SetInputData(rawPolyData);
    writer->Write();
    qDebug() << "模型成功导出到：" << filePath;
}

void disp3dmodel::UnLoadModel()
{
    if (Flag_Actor){
        PickerClear();
        renderer->RemoveActor(modelActor);
    }

    rawPointMap.clear();
    pointDisplace.clear();
    keyPointSet.clear();
    ClearKeyPoint();

    Flag_Actor = false;
}

void disp3dmodel::MergeModel(const QString &filePath)
{
    QTextCodec *codec = QTextCodec::codecForName("GBK");
    std::string localPath = codec->fromUnicode(filePath).data();    //中文路径的兼容性
    vtkSmartPointer<vtkSTLReader> newReader = vtkSmartPointer<vtkSTLReader>::New();
    newReader->SetFileName(localPath.c_str());
    newReader->Update();
    vtkSmartPointer<vtkPolyData> newPolyData = newReader->GetOutput();
    vtkSmartPointer<vtkAppendPolyData> appender = vtkSmartPointer<vtkAppendPolyData>::New();
    appender->AddInputData(rawPolyData);
    appender->AddInputData(newPolyData);
    appender->Update();
    vtkSmartPointer<vtkCleanPolyData> cleaner = vtkSmartPointer<vtkCleanPolyData>::New();
    cleaner->SetInputConnection(appender->GetOutputPort());
    cleaner->Update();
    rawPolyData = cleaner->GetOutput();
    mapper->SetInputData(rawPolyData);
    renderer->ResetCamera();
    renderWindow->Render();

//    qDebug() << "合并新模型：";
//    qDebug() << "点数: " << newPolyData->GetNumberOfPoints()
//              << " 面数: " << newPolyData->GetNumberOfPolys();
//    qDebug() << "模型合并完成：";
//    qDebug() << "合并后点数: " << rawPolyData->GetNumberOfPoints()
    //              << " 面数: " << rawPolyData->GetNumberOfPolys();
}

void disp3dmodel::InitScalarsValues(ModelScalars &scalars, vtkPolyData *poly)
{
    vtkIdType nPoints = poly->GetNumberOfPoints();
    scalars.dispX->SetNumberOfValues(0);
    scalars.dispY->SetNumberOfValues(0);
    scalars.dispZ->SetNumberOfValues(0);
    scalars.strain->SetNumberOfValues(0);
    scalars.temp->SetNumberOfValues(0);
    for (vtkIdType i = 0; i < nPoints; i++){
        scalars.dispX->InsertNextValue(0);
        scalars.dispY->InsertNextValue(0);
        scalars.dispZ->InsertNextValue(0);
        scalars.strain->InsertNextValue(static_cast<float>(0.1));
        scalars.temp->InsertNextValue(static_cast<float>(0.1));
    }
    vtkPointData* pd = poly->GetPointData();
    pd->AddArray(scalars.dispX);
    pd->AddArray(scalars.dispY);
    pd->AddArray(scalars.dispZ);
    pd->AddArray(scalars.strain);
    pd->AddArray(scalars.temp);
    pd->SetActiveScalars("Displacement_X");
    mapper->ScalarVisibilityOff();
}

void disp3dmodel::SetupColorMapping(const std::string &scalarName)
{
    mapper->SetInputData(rawPolyData);
    mapper->SetScalarModeToUsePointData();
    mapper->ScalarVisibilityOn();
    mapper->SelectColorArray(scalarName.c_str());
    vtkSmartPointer<vtkLookupTable> lut = vtkSmartPointer<vtkLookupTable>::New();
    lut->SetNumberOfColors(256);
    lut->SetHueRange(0.66, 0.0);
    lut->Build();
    mapper->SetLookupTable(lut);
    mapper->ScalarVisibilityOn();
    modelActor->GetProperty()->SetColor(1,1,1);
    renderWindow->Render();
}

void disp3dmodel::ReadPointQMap(vtkSmartPointer<vtkPolyData> PolyData)
{
    vtkSmartPointer<vtkPoints> points = PolyData->GetPoints();
    vtkIdType numPoints = points->GetNumberOfPoints();
    for (vtkIdType id= 0; id<numPoints; ++id) {
        double coords[3];
        points->GetPoint(id, coords);
        QVector<double> pointCoords = {0,0,0};
        pointDisplace.insert(id, pointCoords);
        pointCoords[0] = coords[0];  // x坐标
        pointCoords[1] = coords[1];  // y坐标
        pointCoords[2] = coords[2];  // z坐标
        //qDebug() << id << coords[0] << coords[1] << coords[2];
        rawPointMap.insert(id, pointCoords);
    }
    pointpos = rawPointMap;
}

void disp3dmodel::FreshKeyPoint()
{
    if (SelectedPointMap.isEmpty()) {
        return;
    }

    QMap<vtkIdType, SelectedPointEntity>::const_iterator it;
    for (it = SelectedPointMap.constBegin(); it != SelectedPointMap.constEnd(); ++it) {
        const SelectedPointEntity &ent = it.value();
        if (!ent.keyPointEntity || !ent.keyPointText) {
            continue;
        }
        double worldPos[3];
        ent.keyPointEntity->GetPosition(worldPos);
        renderer->SetWorldPoint(worldPos[0], worldPos[1], worldPos[2], 1.0);
        renderer->WorldToDisplay();
        double displayPos[3];
        renderer->GetDisplayPoint(displayPos);
        ent.keyPointText->SetDisplayPosition(static_cast<int>(std::round(displayPos[0])),
                                             static_cast<int>(std::round(displayPos[1])));
        char buf[64];
        sprintf(buf, "(%0.2f,%0.2f,%0.2f)", worldPos[0], worldPos[1], worldPos[2]);
        ent.keyPointText->SetInput(buf);
    }

    // 最后触发一次渲染更新
    if (renderWindow) {
        renderWindow->Render();
    }
}

void disp3dmodel::ClearKeyPoint()
{
    QMap<vtkIdType, SelectedPointEntity>::iterator it;
    for (it = SelectedPointMap.begin(); it != SelectedPointMap.end(); ++it) {

        SelectedPointEntity entity = it.value();
        if (entity.keyPointEntity) {
            renderer->RemoveActor(entity.keyPointEntity);
            entity.keyPointEntity->Delete();
            entity.keyPointEntity = nullptr;
        }
        if (entity.keyPointText) {
            renderer->RemoveActor2D(entity.keyPointText);
            entity.keyPointText->Delete();
            entity.keyPointText = nullptr;
        }
    }
    SelectedPointMap.clear();
    renderWindow->Render();
}

uint disp3dmodel::GetModelNumPoints()
{
    vtkIdType numPoints = rawPolyData->GetNumberOfPoints();
    return static_cast<uint>(numPoints);
}

uint disp3dmodel::GetModelNumPolys()
{
    vtkIdType numPolys = rawPolyData->GetNumberOfPolys();
    return static_cast<uint>(numPolys);
}

uint disp3dmodel::GetModelNumKeyPoints()
{
    vtkIdType numKeyPoints = keyPointSet.size();
    return static_cast<uint>(numKeyPoints);
}

void disp3dmodel::ResetCamera(int Index)
{
    double* x = modelActor->GetCenter();
    vtkCamera* camera = renderer->GetActiveCamera();
    camera->SetClippingRange(0.1, 2000); // 设置裁剪范围

    double bounds[6];
    modelActor->GetBounds(bounds);
    double dx = bounds[1] - bounds[0];
    double dy = bounds[3] - bounds[2];
    double dz = bounds[5] - bounds[4];
    double diagonal = sqrt(dx * dx + dy * dy + dz * dz);
    double distance = 2 * diagonal; // 自动调节距离（原本是固定 4）
    switch (Index){
    case 0: // 正前视图 (+Y方向)
        camera->SetFocalPoint(x[0], x[1], x[2]);
        camera->SetPosition(x[0], x[1] - distance, x[2]);
        camera->SetViewUp(0, 0, 1);
        break;

    case 1: // 左侧视图 (+X方向)
        camera->SetFocalPoint(x[0], x[1], x[2]);
        camera->SetPosition(x[0] - distance, x[1], x[2]);
        camera->SetViewUp(0, 0, 1);
        break;

    case 2: // 右侧视图 (-X方向)
        camera->SetFocalPoint(x[0], x[1], x[2]);
        camera->SetPosition(x[0] + distance, x[1], x[2]);
        camera->SetViewUp(0, 0, 1);
        break;

    case 3: // 上方视图 (+Z方向)
        camera->SetFocalPoint(x[0], x[1], x[2]);
        camera->SetPosition(x[0], x[1], x[2] + distance);
        camera->SetViewUp(0, 1, 0);
        break;

    case 4: // 下方视图 (-Z方向)
        camera->SetFocalPoint(x[0], x[1], x[2]);
        camera->SetPosition(x[0], x[1], x[2] - distance);
        camera->SetViewUp(0, 1, 0);
        break;

    case 5: // 正后视图 (-Y方向)
        camera->SetFocalPoint(x[0], x[1], x[2]);
        camera->SetPosition(x[0], x[1] + distance, x[2]);
        camera->SetViewUp(0, 0, 1);
        break;

    default:
        std::cerr << "Invalid DisplayFlag in Resetcamera(): " << Index << std::endl;
        break;
    }

    renderer->ResetCameraClippingRange();
}
