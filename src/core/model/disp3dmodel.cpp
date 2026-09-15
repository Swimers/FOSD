#include "disp3dmodel.h"

#include <QFile>
#include <QFileInfo>
#include <QDebug>

#include <vtkActor.h>
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkCamera.h>
#include <vtkPolyData.h>
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
#include <vtkTriangleFilter.h>
#include <vtkLoopSubdivisionFilter.h>
#include <vtkWindowedSincPolyDataFilter.h>
#include <vtkPolyDataNormals.h>
#include <vtkSTLWriter.h>
#include <QDir>
#include <QCoreApplication>

#include <QTextCodec>
#include <stdio.h>

#include <disp3dinteractor.h>

disp3dmodel::disp3dmodel()
{
    renderWindow = vtkSmartPointer<vtkRenderWindow>::New();
    renderer = vtkSmartPointer<vtkRenderer>::New();

    renderWindow->AddRenderer(renderer);

    //std::cout << "VTK Version: " << vtkVersion::GetVTKVersion() << std::endl;
}

disp3dmodel::~disp3dmodel()
{
    ClearKeyPoint();           // 内部会把 keyPointEntity / keyPointText 从 renderer 移除并 Delete
                               // 并清空 SelectedPointMap
    rawPointMap.clear();
    pointDisplace.clear();
    pointpos.clear();
    keyPointSet.clear();
    SelectedPointMap.clear();

    if (renderer && modelActor) {
        renderer->RemoveActor(modelActor);
    }
    if (iren) {
        iren->SetInteractorStyle(nullptr);
        iren = nullptr;
    }
    if (renderer) {
        if (renderWindow) {
            renderWindow->RemoveRenderer(renderer);
        }
        renderer = nullptr;
    }
}

void disp3dmodel::initdisp3dmodel()
{
    //原版本render传递逻辑
    //renderer = vtkRenderer::New();
    //renderWindow->AddRenderer(renderer);
    //iren = vtkRenderWindowInteractor::New();
    //iren->SetRenderWindow(renderWindow);
    //iren->Initialize();
    //disp3dinteractor* style = new disp3dinteractor();
    //style->SetDefaultRenderer(renderer);
    //style->GetDisplayModelPtr(this);
    //iren->SetInteractorStyle(style);
    renderWindow->Render();
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

// =============================================================================
// OptimizeMesh — Loop细分 + Sinc平滑 模型优化
//   输入: 原始低模 PolyData
//   输出: 细分平滑后的高模 PolyData
//   配置: MeshOptimizeConfig 结构体常量
// =============================================================================
vtkSmartPointer<vtkPolyData> disp3dmodel::OptimizeMesh(vtkSmartPointer<vtkPolyData> input)
{
    static const MeshOptimizeConfig cfg;

    if (!input || input->GetNumberOfPoints() == 0) {
        qWarning() << "[OptimizeMesh] 输入数据为空";
        return input;
    }

    const vtkIdType origPoints = input->GetNumberOfPoints();
    const vtkIdType origFaces  = input->GetNumberOfPolys();

    // ---- Step 0: 确保全是三角面 ----
    vtkSmartPointer<vtkTriangleFilter> triFilter = vtkSmartPointer<vtkTriangleFilter>::New();
    triFilter->SetInputData(input);
    triFilter->Update();
    vtkSmartPointer<vtkPolyData> current = triFilter->GetOutput();

    // ---- Step 1: Loop 细分 ----
    if (cfg.subdivisionLevel > 0) {
        vtkSmartPointer<vtkLoopSubdivisionFilter> loop = vtkSmartPointer<vtkLoopSubdivisionFilter>::New();
        loop->SetInputData(current);
        loop->SetNumberOfSubdivisions(cfg.subdivisionLevel);
        loop->Update();
        current = loop->GetOutput();
    }

    // ---- Step 2: Sinc 平滑 ----
    if (cfg.enableSmooth && cfg.smoothIterations > 0) {
        vtkSmartPointer<vtkWindowedSincPolyDataFilter> sinc = vtkSmartPointer<vtkWindowedSincPolyDataFilter>::New();
        sinc->SetInputData(current);
        sinc->SetNumberOfIterations(cfg.smoothIterations);
        sinc->SetPassBand(cfg.smoothPassBand);
        sinc->FeatureEdgeSmoothingOff();
        sinc->BoundarySmoothingOn();
        sinc->NormalizeCoordinatesOn();
        sinc->Update();
        current = sinc->GetOutput();
    }

    // ---- Step 3: 重算法线 ----
    vtkSmartPointer<vtkPolyDataNormals> normals = vtkSmartPointer<vtkPolyDataNormals>::New();
    normals->SetInputData(current);
    normals->ComputePointNormalsOn();
    normals->ComputeCellNormalsOff();
    normals->SplittingOff();
    normals->Update();
    current = normals->GetOutput();

    // ---- Step 4: 可选保存 ----
    if (cfg.saveOptimized) {
        // 保存到 data/mesh_optimized/ 目录，文件名加 _optimized 后缀
        QString saveDir = QCoreApplication::applicationDirPath() + "/data/mesh_optimized";
        QDir().mkpath(saveDir);
        QString savePath = saveDir + "/optimized_model.stl";

        QTextCodec *codec = QTextCodec::codecForName("GBK");
        std::string localSavePath = codec->fromUnicode(savePath).data();

        vtkSmartPointer<vtkSTLWriter> writer = vtkSmartPointer<vtkSTLWriter>::New();
        writer->SetFileName(localSavePath.c_str());
        writer->SetInputData(current);
        writer->Write();

        qDebug() << "[OptimizeMesh] 优化模型已保存:" << savePath;
    }

    // ---- 日志 ----
    const vtkIdType newPoints = current->GetNumberOfPoints();
    const vtkIdType newFaces  = current->GetNumberOfPolys();
    qDebug() << QString("[OptimizeMesh] 完成: 面数 %1 → %2, 点数 %3 → %4")
                .arg(origFaces).arg(newFaces).arg(origPoints).arg(newPoints);

    return current;
}

bool disp3dmodel::LoadModel(const QString &filePath)
{
    // ---- 路径有效性校验 ----
    if (filePath.isEmpty()) {
        qWarning() << "[LoadModel] 文件路径为空";
        Flag_Actor = false;
        return false;
    }

    QFileInfo fileInfo(filePath);
    if (!fileInfo.exists()) {
        qWarning() << "[LoadModel] 文件不存在:" << filePath;
        Flag_Actor = false;
        return false;
    }
    if (!fileInfo.isFile()) {
        qWarning() << "[LoadModel] 路径不是文件:" << filePath;
        Flag_Actor = false;
        return false;
    }
    if (!fileInfo.isReadable()) {
        qWarning() << "[LoadModel] 文件不可读:" << filePath;
        Flag_Actor = false;
        return false;
    }
    if (fileInfo.suffix().compare("stl", Qt::CaseInsensitive) != 0) {
        qWarning() << "[LoadModel] 文件格式不是STL:" << filePath;
        Flag_Actor = false;
        return false;
    }
    if (fileInfo.size() == 0) {
        qWarning() << "[LoadModel] 文件大小为0:" << filePath;
        Flag_Actor = false;
        return false;
    }

    modelScalars.Init();

    QTextCodec *codec = QTextCodec::codecForName("GBK");
    std::string localPath = codec->fromUnicode(filePath).data(); //中文路径的兼容性

    //std::string localPath = filePath.toStdString();

    reader->SetFileName(localPath.c_str());
    reader->Update();
    rawPolyData = reader->GetOutput();

    // ---- VTK读取结果校验 ----
    if (!rawPolyData || rawPolyData->GetNumberOfPoints() == 0) {
        qWarning() << "[LoadModel] VTK读取失败或模型无有效点数据:" << filePath;
        Flag_Actor = false;
        return false;
    }

    // ---- 模型细分优化（Loop + Sinc） ----
    //rawPolyData = OptimizeMesh(rawPolyData);

//    // ========== 新增：模型平移核心逻辑 ==========
//    // 1. 创建平移变换对象，设置x/y/z方向的平移量
//    vtkSmartPointer<vtkTransform> transform = vtkSmartPointer<vtkTransform>::New();
//    transform->Translate(-12.15, -754.94, -13642.09); // 平移核心API

//    // 2. 创建PolyData变换过滤器，将平移应用到模型数据
//    vtkSmartPointer<vtkTransformPolyDataFilter> transformFilter = vtkSmartPointer<vtkTransformPolyDataFilter>::New();
//    transformFilter->SetInputData(rawPolyData); // 输入原始模型数据
//    transformFilter->SetTransform(transform);   // 绑定平移变换规则
//    transformFilter->Update();                  // 执行变换计算

//    // 3. 获取平移后的模型数据
//    vtkPolyData* translatedPolyData = transformFilter->GetOutput();
//    rawPolyData->DeepCopy(translatedPolyData);
//    // ========== 平移逻辑结束 ==========

    dynPolyData = vtkSmartPointer<vtkPolyData>::New();
    dynPolyData->DeepCopy(rawPolyData);
    mapper->SetInputData(dynPolyData);

    InitScalarsValues(modelScalars, dynPolyData);

    modelActor->SetMapper(mapper);
    modelActor->GetProperty()->SetEdgeVisibility(0);   // 边线默认关闭（大网格上会显得杂乱）；
                                                        // 若想要 HUD 轮廓效果，把 0 改为 1 即可
    modelActor->GetProperty()->SetLineWidth(static_cast<float>(1.0));
    modelActor->GetProperty()->SetEdgeColor(0.0, 0.83, 1.0);   // accent 冰青
    modelActor->GetProperty()->SetRepresentationToSurface();
    modelActor->GetProperty()->SetColor(static_cast<double>(ModelColor[0]), static_cast<double>(ModelColor[1]), static_cast<double>(ModelColor[2]));

    renderer->AddActor(modelActor);
    renderer->ResetCamera();
    renderWindow->Render();

    ReadPointQMap(dynPolyData);

    AllKeyPoint();
    GetModelPointsNum();
    GetModelPolysNum();
    GetModelKeyPointsNum();
    GetModelSize();

    Flag_Actor = true;
    return true;
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

void disp3dmodel::ResetModel()
{
    if (!Flag_Actor || !dynPolyData || !rawPolyData) {
        return;
    }

    vtkPoints* points = dynPolyData->GetPoints();
    if (!points) {
        return;
    }

    vtkIdType numPoints = points->GetNumberOfPoints();

    pointDisplace.clear();
    pointpos.clear();

    QMap<vtkIdType, QVector<double>>::const_iterator it;
    for (it = rawPointMap.constBegin(); it != rawPointMap.constEnd(); ++it) {
        vtkIdType id = it.key();
        const QVector<double> &coords = it.value();

        if (id >= 0 && id < numPoints && coords.size() >= 3) {
            points->SetPoint(id, coords[0], coords[1], coords[2]);
        }

        QVector<double> zero(3);
        zero[0] = 0.0;
        zero[1] = 0.0;
        zero[2] = 0.0;
        pointDisplace.insert(id, zero);

        QVector<double> pos(3);
        pos[0] = coords[0];
        pos[1] = coords[1];
        pos[2] = coords[2];
        pointpos.insert(id, pos);

        modelScalars.dispX->SetValue(id, 0);
        modelScalars.dispY->SetValue(id, 0);
        modelScalars.dispZ->SetValue(id, 0);
        modelScalars.temp->SetValue(id, 0.01);
        modelScalars.strain->SetValue(id, 0.01);
    }
    MaxDisplace = 0.0;

    dynPolyData->Modified();
    renderWindow->Render();
}

void disp3dmodel::InitScalarsValues(ModelScalars &scalars, vtkPolyData *poly)
{
    vtkIdType nPoints = poly->GetNumberOfPoints();
    scalars.dispX->SetNumberOfValues(0);
    scalars.dispY->SetNumberOfValues(0);
    scalars.dispZ->SetNumberOfValues(0);
    scalars.dispMag->SetNumberOfValues(0);
    scalars.strain->SetNumberOfValues(0);
    scalars.temp->SetNumberOfValues(0);
    for (vtkIdType i = 0; i < nPoints; i++){
        scalars.dispX->InsertNextValue(0);
        scalars.dispY->InsertNextValue(0);
        scalars.dispZ->InsertNextValue(0);
        scalars.dispMag->InsertNextValue(0);
        scalars.strain->InsertNextValue(0.1);
        scalars.temp->InsertNextValue(0.1);
    }
    vtkPointData* pd = poly->GetPointData();
    pd->AddArray(scalars.dispX);
    pd->AddArray(scalars.dispY);
    pd->AddArray(scalars.dispZ);
    pd->AddArray(scalars.dispMag);
    pd->AddArray(scalars.strain);
    pd->AddArray(scalars.temp);

    vtkSmartPointer<vtkLookupTable> lut = vtkSmartPointer<vtkLookupTable>::New();
    lut->SetNumberOfColors(256);
    // 冷→暖映射：低位移=冰青，高位移=红，方向符合告警语义
    // （原为 0.6→0.1，起点偏蓝紫）
    lut->SetHueRange(0.55, 0.0);
    lut->Build();
    mapper->SetLookupTable(lut);
    mapper->SelectColorArray("Displacement_X");
    mapper->ScalarVisibilityOff();
    mapper->SetScalarRange(0,1);
    renderWindow->Render();
}

void disp3dmodel::SetupColorMapping(const std::string &scalarName)
{
    m_activeScalarName = scalarName;

    dynPolyData->GetPointData()->SetActiveScalars(scalarName.c_str());
    if (m_normalize) {
        mapper->SetScalarRange(0, 1);
    } else {
        mapper->SetScalarRange(m_customRangeMin, m_customRangeMax);
    }
    mapper->ScalarVisibilityOn();
    renderWindow->Render();
}

void disp3dmodel::SetNormalizeMode(bool enabled)
{
    m_normalize = enabled;
    if (!m_activeScalarName.empty()) {
        if (enabled) {
            mapper->SetScalarRange(0, 1);
        } else {
            mapper->SetScalarRange(m_customRangeMin, m_customRangeMax);
        }
        renderWindow->Render();
    }
}

void disp3dmodel::SetCustomRange(double rangeMin, double rangeMax)
{
    m_customRangeMin = rangeMin;
    m_customRangeMax = rangeMax;
    if (!m_normalize && !m_activeScalarName.empty()) {
        mapper->SetScalarRange(rangeMin, rangeMax);
        renderWindow->Render();
    }
}

void disp3dmodel::normalizeScalarArray(vtkSmartPointer<vtkDoubleArray> arr)
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
        for (vtkIdType i = 0; i < n; i++) {
            arr->SetValue(i, 0.0);
        }
        return;
    }

    for (vtkIdType i = 0; i < n; i++) {
        double v = arr->GetValue(i);
        arr->SetValue(i, (v - minV) / range);
    }
}

void disp3dmodel::OFFScalars()
{
    mapper->ScalarVisibilityOff();
    modelActor->GetProperty()->SetEdgeVisibility(0);   // 边线默认关闭（大网格上会显得杂乱）；
                                                        // 若想要 HUD 轮廓效果，把 0 改为 1 即可
    modelActor->GetProperty()->SetLineWidth(static_cast<float>(1.0));
    modelActor->GetProperty()->SetEdgeColor(0.0, 0.83, 1.0);   // accent 冰青
    modelActor->GetProperty()->SetRepresentationToSurface();
    modelActor->GetProperty()->SetColor(static_cast<double>(ModelColor[0]), static_cast<double>(ModelColor[1]), static_cast<double>(ModelColor[2]));
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
        pointpos.insert(id, pointCoords);
    }
}

void disp3dmodel::AllKeyPoint()
{
    for (auto it = rawPointMap.constBegin(); it != rawPointMap.constEnd(); ++it){
        vtkIdType id = it.key();
        keyPointSet.insert(id);
    }
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

void disp3dmodel::GetModelPointsNum()
{
    vtkIdType numPoints = rawPolyData->GetNumberOfPoints();
    ModelPointsNum = static_cast<uint>(numPoints);
}

void disp3dmodel::GetModelPolysNum()
{
    vtkIdType numPolys = rawPolyData->GetNumberOfPolys();
    ModelPolysNum = static_cast<uint>(numPolys);
}

void disp3dmodel::GetModelKeyPointsNum()
{
    vtkIdType numKeyPoints = keyPointSet.size();
    ModelKeyPointsNum = static_cast<uint>(numKeyPoints);
}

void disp3dmodel::GetModelSize()
{
    double bounds[6];
    modelActor->GetBounds(bounds);
    double dx = bounds[1] - bounds[0];
    double dy = bounds[3] - bounds[2];
    double dz = bounds[5] - bounds[4];
    double diag = std::sqrt(dx*dx + dy*dy + dz*dz);
    ModelSize = std::max(1e-6, diag * 0.008);
}

void disp3dmodel::setModelColor()
{
    if(Flag_Actor){
        modelActor->GetProperty()->SetColor(static_cast<double>(ModelColor[0]), static_cast<double>(ModelColor[1]), static_cast<double>(ModelColor[2]));
        qDebug() << "Current Color:" <<ModelColor;
        renderWindow->Render();
    }
    else{
        qWarning() << "模型未加载";
    }
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

void disp3dmodel::CameraLock(bool index)
{
    disp3dinteractor * style1 = dynamic_cast<disp3dinteractor *>(iren->GetInteractorStyle());
    style1->Flag_CameraLock = index;
}

void disp3dmodel::CameraFellow(bool index)
{
    disp3dinteractor * style1 = dynamic_cast<disp3dinteractor *>(iren->GetInteractorStyle());
    style1->Flag_CameraFellow = index;
}
