#include "disp3dwindow.h"
#include "ui_disp3dwindow.h"

#include <disp3dbackground.h>
#include <trackingpoint.h>

#include <stdio.h>
#include <QDebug>
#include <QMessageBox>
#include <QVBoxLayout>
#include <QFileDialog>

#include <vtkPolyDataMapper.h>
#include <vtkAutoInit.h>
#include <QVTKOpenGLNativeWidget.h>
#include <vtkGenericOpenGLRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkPoints.h>
#include <vtkCaptionActor2D.h>

#include <ConfigManager.h>

#include <QTimer>

disp3dwindow::disp3dwindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::disp3dwindow)
{
    ui->setupUi(this);

    InitWindow();

    QTimer::singleShot(0, this, [this]() {
        if (!ui || !ui->vtk_openGL || !m_disp3dmodel) {
            return;
        }
        vtkRenderWindow *renderWindow = m_disp3dmodel->GetVTKRenderWindow();
        vtkRenderer *renderer = m_disp3dmodel->GetVTKRenderer();
        if (!renderWindow || !renderer) {
            return;
        }
        renderer->ResetCameraClippingRange();
        renderWindow->Render();
        ui->vtk_openGL->update();
    });
}

disp3dwindow::~disp3dwindow()
{
    if (freshTimer) {
        freshTimer->stop();
        freshTimer = nullptr;   // 防止后续误用
    }

    if (m_disp3dthread) {
        m_disp3dthread->stopThread();
        delete m_disp3dthread;
        m_disp3dthread = nullptr;
    }

    if (m_disp3dinteractor) {
        delete m_disp3dinteractor;
        m_disp3dinteractor = nullptr;
    }
    if (m_disp3dbackground) {
        delete m_disp3dbackground;
        m_disp3dbackground = nullptr;
    }
    if (m_disp3dmodel) {
        delete m_disp3dmodel;
        m_disp3dmodel = nullptr;
    }

    if (ui) {
        delete ui;
        ui = nullptr;
    }
}

void disp3dwindow::InitWindow()
{
    m_vtkrenderwindow = ui->vtk_openGL->GetRenderWindow();
    InitDockPanels();

    m_disp3dmodel = new disp3dmodel();
    m_disp3dmodel->SetRenderWindow(m_vtkrenderwindow);
    m_vtkrenderwindow->AddRenderer(m_disp3dmodel->GetVTKRenderer());

    m_iren = ui->vtk_openGL->GetInteractor();
    m_disp3dmodel->SetInteractor(m_iren);
    disp3dinteractor *style = disp3dinteractor::New();
    style->GetDisplayModelPtr(m_disp3dmodel);
    m_iren->SetInteractorStyle(style);
    style->SetDefaultRenderer(m_disp3dmodel->GetVTKRenderer());
    style->Delete();  //智能指针管理，这里可以Delete
    m_disp3dmodel->GetVTKRenderWindow()->Render();
    //原版本render传递逻辑
    //ui->vtk_stage->SetRenderWindow(m_disp3dmodel->GetVTKRenderWindow());
    //ui->vtk_openGL->setRenderWindow(m_disp3dmodel->GetVTKRenderWindow());
    //m_disp3dmodel->initdisp3dmodel();

    m_disp3dbackground = new disp3dbackground(m_disp3dmodel->GetVTKRenderer(),m_vtkrenderwindow);
    m_disp3dbackground->SetVtkRenderBackGradient(0.53, 0.81, 0.98, 0.87, 0.94, 1.0);
    m_disp3dbackground->AddAxesActor();
    m_disp3dbackground->AddAxesGrid();
    m_disp3dbackground->GetModel(m_disp3dmodel);

    m_trackingpoint = new trackingpoint(m_disp3dmodel->GetVTKRenderer(),m_vtkrenderwindow);
    m_trackingpoint->SetDisplayModel(m_disp3dmodel);

    ui->actionReset->setDisabled(true);
    ui->actionStart->setDisabled(false);
    ui->actionStop->setEnabled(false);
    ui->actionPause->setDisabled(true);

    freshTimer = new QTimer(this);
    FPS = m_modelDockPanel ? m_modelDockPanel->fps() : 1;

    InitPointInfor();

    // 测试代码段，初始化加载模型
    auto &appConfig = ConfigManager::instance().config();
    QString filePath = appConfig.modlepath.ModlePath;
    ConfigManager::instance().save();
    bool loadOk = m_disp3dmodel->LoadModel(filePath);
    if (loadOk) {
        m_disp3dbackground->CreateWorld();
        m_disp3dmodel->GetVTKRenderWindow()->Render();
    }
    //m_trackingpoint->LoadTrackingPointsFromFile("data/trackingpoint/tracking_points_example_modified.csv");
    //m_disp3dmodel->LoadModel("C:/Users/Swimers/Desktop/Test_Model/LeftWing2_new.stl");
    if(m_disp3dmodel->Flag_Actor)
    RefreshInfor();

    connect(freshTimer,SIGNAL(timeout()),this,SLOT(RefreshStage()));
}

void disp3dwindow::InitDockPanels()
{
    m_dockManager = new Disp3DDockManager(this, this);
    m_modelDockPanel = new Disp3DModelDockPanel(this);
    m_viewDockPanel = new Disp3DViewDockPanel(this);
    m_infoDockPanel = new Disp3DInfoDockPanel(this);

    m_dockManager->registerPanel(m_modelDockPanel, ui->actionModelDock);
    m_dockManager->registerPanel(m_viewDockPanel, ui->actionViewDock);
    m_dockManager->registerPanel(m_infoDockPanel, ui->actionInforDock);

    connect(m_modelDockPanel, &Disp3DModelDockPanel::modelLoadRequested, this, &disp3dwindow::LoadModel);
    connect(m_modelDockPanel, &Disp3DModelDockPanel::modelClearRequested, this, &disp3dwindow::ClearModel);
    connect(m_modelDockPanel, &Disp3DModelDockPanel::modelMergeRequested, this, &disp3dwindow::MergeModel);
    connect(m_modelDockPanel, &Disp3DModelDockPanel::modelExportRequested, this, &disp3dwindow::ExportModel);
    connect(m_modelDockPanel, &Disp3DModelDockPanel::keyPointEditRequested, this, &disp3dwindow::EditKeyPoint);
    connect(m_modelDockPanel, &Disp3DModelDockPanel::keyPointSelectRequested, this, &disp3dwindow::SelectKeyPoint);
    connect(m_modelDockPanel, &Disp3DModelDockPanel::scalarSetRequested, this, &disp3dwindow::SetScalarMapping);
    connect(m_modelDockPanel, &Disp3DModelDockPanel::scalarClearRequested, this, &disp3dwindow::ClearScalarMapping);
    connect(m_modelDockPanel, &Disp3DModelDockPanel::fpsChanged, this, &disp3dwindow::SetFPS);

    connect(m_viewDockPanel, &Disp3DViewDockPanel::axesVisibleChanged, this, &disp3dwindow::SetAxesVisible);
    connect(m_viewDockPanel, &Disp3DViewDockPanel::axesSizeChanged, this, &disp3dwindow::SetAxisSize);
    connect(m_viewDockPanel, &Disp3DViewDockPanel::gridVisibleChanged, this, &disp3dwindow::SetGridVisible);
    connect(m_viewDockPanel, &Disp3DViewDockPanel::gridSizeChangeFinished, this, &disp3dwindow::SetGridSize);
    connect(m_viewDockPanel, &Disp3DViewDockPanel::xoyGridVisibleChanged, this, &disp3dwindow::SetXOYGridVisible);
    connect(m_viewDockPanel, &Disp3DViewDockPanel::xozGridVisibleChanged, this, &disp3dwindow::SetXOZGridVisible);
    connect(m_viewDockPanel, &Disp3DViewDockPanel::zoyGridVisibleChanged, this, &disp3dwindow::SetZOYGridVisible);
    connect(m_viewDockPanel, &Disp3DViewDockPanel::cameraLockChanged, this, &disp3dwindow::SetCameraLock);
    connect(m_viewDockPanel, &Disp3DViewDockPanel::cameraFollowChanged, this, &disp3dwindow::SetCameraFollow);
    connect(m_viewDockPanel, &Disp3DViewDockPanel::switchViewRequested, this, &disp3dwindow::SwitchView);
}

void disp3dwindow::InitPointInfor()
{
    ui->label->setStyleSheet("QLabel { font-weight: bold; color: #4169E1; }");
    ui->lab_KeyPointID->setStyleSheet("QLabel { font-weight: bold; color: #4169E1; }");
    ui->label_6->setStyleSheet("QLabel { font-weight: bold; color: green; }");
    ui->lab_KeyPointRawPos->setStyleSheet("QLabel { font-weight: bold; color: green; }");
    ui->label_8->setStyleSheet("QLabel { font-weight: bold; color: green; }");
    ui->lab_KeyPointDisplace->setStyleSheet("QLabel { font-weight: bold; color: green; }");
    ui->label_10->setStyleSheet("QLabel { font-weight: bold; color: green; }");
    ui->lab_KeyPointNewPos->setStyleSheet("QLabel { font-weight: bold; color: green; }");
    m_disp3dmodel->PickerGetLabPtr(std::make_tuple(ui->lab_KeyPointID, ui->lab_KeyPointRawPos, ui->lab_KeyPointDisplace, ui->lab_KeyPointNewPos));
}

void disp3dwindow::RefreshInfor()
{
    if (!m_infoDockPanel || !m_disp3dmodel) {
        return;
    }

    m_infoDockPanel->setModelInfo(m_disp3dmodel->ModelPointsNum,
                                  m_disp3dmodel->ModelPolysNum,
                                  m_disp3dmodel->ModelKeyPointsNum);
    m_infoDockPanel->setMaxDeformation("00.00");
}

void disp3dwindow::RefreshStage()
{
    m_disp3dmodel->GetVTKRenderWindow()->Render();
    char str[8];
    if(m_disp3dmodel){
        QString maxDeformStr = QString::number(m_disp3dmodel->MaxDisplace, 'f', 4);
        if (m_infoDockPanel) {
            m_infoDockPanel->setMaxDeformation(maxDeformStr);
        }
    }
}

void disp3dwindow::ApplyDisplayFrame(const DisplayPointUpdateList &pointUpdates,
                                     double maxDisplace,
                                     const TrackingPointFrameUpdateList &trackingUpdates)
{
    if (!m_disp3dmodel || !m_disp3dmodel->dynPolyData) {
        return;
    }

    vtkPoints *points = m_disp3dmodel->dynPolyData->GetPoints();
    if (!points) {
        return;
    }

    const vtkIdType pointCount = points->GetNumberOfPoints();
    for (const DisplayPointUpdate &update : pointUpdates) {
        if (update.id < 0 || update.id >= pointCount) {
            continue;
        }

        QVector<double> pos(3), disp(3);
        for (int i = 0; i < 3; ++i) {
            pos[i] = update.position[i];
            disp[i] = update.displacement[i];
        }

        m_disp3dmodel->modelScalars.dispX->SetValue(update.id, disp[0]);
        m_disp3dmodel->modelScalars.dispY->SetValue(update.id, disp[1]);
        m_disp3dmodel->modelScalars.dispZ->SetValue(update.id, disp[2]);
        m_disp3dmodel->pointpos[update.id] = pos;
        m_disp3dmodel->pointDisplace[update.id] = disp;
        points->SetPoint(update.id, pos[0], pos[1], pos[2]);
    }

    m_disp3dmodel->MaxDisplace = maxDisplace;

    for (const TrackingPointFrameUpdate &update : trackingUpdates) {
        TrackingPoint *tp = m_trackingpoint ? m_trackingpoint->GetTrackingPointById(update.id) : nullptr;
        if (!tp) {
            continue;
        }

        for (int i = 0; i < 3; ++i) {
            tp->currentDisp[i] = update.displacement[i];
        }

        if (tp->visible) {
            tp->sphereActor->SetPosition(update.position[0], update.position[1], update.position[2]);
            tp->sphereActor->Modified();

            char dispText[100];
            sprintf(dispText, "(%.3f, %.3f, %.3f)",
                    tp->currentDisp[0],
                    tp->currentDisp[1],
                    tp->currentDisp[2]);
            tp->textActor->SetCaption(dispText);
            tp->textActor->SetAttachmentPoint( update.position[0], update.position[1], update.position[2]);
            tp->textActor->Modified();
        }
    }

    points->Modified();
    m_disp3dmodel->modelScalars.dispX->Modified();
    m_disp3dmodel->modelScalars.dispY->Modified();
    m_disp3dmodel->modelScalars.dispZ->Modified();
    m_disp3dmodel->dynPolyData->Modified();
    m_disp3dmodel->FreshKeyPoint();
}

void disp3dwindow::on_checkBox_3_clicked(bool checked)
{
    m_disp3dbackground->ShowAxes(checked);
}

void disp3dwindow::LoadModel()
{
    if(!m_disp3dmodel->Flag_Actor){
        QString filter = tr("STL模型 (*.stl)");
        QString filePath = QFileDialog::getOpenFileName(this, tr("选择文件"), "", filter);
        if (!filePath.isEmpty()) {
            m_disp3dmodel->LoadModel(filePath);
            m_disp3dbackground->CreateWorld();
            m_disp3dmodel->GetVTKRenderWindow()->Render();
            RefreshInfor();

            auto &appConfig = ConfigManager::instance().config();
            appConfig.modlepath.ModlePath = filePath;
            ConfigManager::instance().save();
        }
        else {
            QMessageBox::information(this, tr("提示"), tr("未选择任何文件！"));
        }
    }
    else {
        QMessageBox::critical(this, "警告", "模型已存在");
    }
}

void disp3dwindow::SetAxesVisible(bool checked)
{
    m_disp3dbackground->ShowAxes(checked);
}

void disp3dwindow::SetGridVisible(bool checked)
{
    Flag_ShowGrid = checked;
    if(checked){
        m_disp3dbackground->ShowGridXY(m_viewDockPanel && m_viewDockPanel->xoyGridChecked());
        m_disp3dbackground->ShowGridXZ(m_viewDockPanel && m_viewDockPanel->xozGridChecked());
        m_disp3dbackground->ShowGridYZ(m_viewDockPanel && m_viewDockPanel->zoyGridChecked());
    }
    else {
        m_disp3dbackground->ShowGridXY(false);
        m_disp3dbackground->ShowGridXZ(false);
        m_disp3dbackground->ShowGridYZ(false);
    }
}

void disp3dwindow::ClearModel()
{
    if(m_disp3dmodel->Flag_Actor){
        QMessageBox::StandardButton result = QMessageBox::question(
                this,
                "请确认",
                "确定要执行这个操作吗？",
                QMessageBox::Ok | QMessageBox::Cancel, // 按钮组合
                QMessageBox::Cancel // 默认选中按钮
            );
            if (result == QMessageBox::Ok) {
                m_disp3dmodel->UnLoadModel();
                m_trackingpoint->ClearTrackingPoint();
                if (m_infoDockPanel) {
                    m_infoDockPanel->clearModelInfo();
                }
            }
    }
    else {
        QMessageBox::critical(this, "警告", "模型不存在");
    }
}

void disp3dwindow::SwitchView(int viewIndex)
{
    if(m_disp3dmodel->Flag_Actor){
        m_disp3dmodel->ResetCamera(viewIndex);
        m_disp3dmodel->GetVTKRenderWindow()->Render();   //刷新
    }
    else {
        QMessageBox::critical(this, "错误", "模型不存在");
    }
}

void disp3dwindow::MergeModel()
{
    if(m_disp3dmodel->Flag_Actor){
        QString filter = tr("STL模型 (*.stl)");
        QString filePath = QFileDialog::getOpenFileName(this, tr("选择文件"), "", filter);
        if (!filePath.isEmpty()) {
            m_disp3dmodel->MergeModel(filePath);
            RefreshInfor();
        }
        else {
            QMessageBox::information(this, tr("提示"), tr("未选择任何文件！"));
        }
    }
    else {
        QMessageBox::critical(this, "错误", "主模型不存在");
    }

}

void disp3dwindow::ExportModel()
{
    if(m_disp3dmodel->Flag_Actor){
        QString filePath = QFileDialog::getSaveFileName(
            this,
            tr("导出 STL 模型"),
            "",
            tr("STL Files (*.stl)")
        );
        if (filePath.isEmpty())
            return;
        if (!filePath.endsWith(".stl", Qt::CaseInsensitive))
            filePath += ".stl";
        m_disp3dmodel->ExportModel(filePath);
    }
    else {
        QMessageBox::critical(this, "错误", "模型不存在");
    }
}

void disp3dwindow::EditKeyPoint()
{
    if(m_disp3dmodel->Flag_Actor){
        disp3dkeypointedit* m_disp3dkeypointedit = disp3dkeypointedit::instance();
        m_disp3dkeypointedit->show();
        m_disp3dkeypointedit->GetDisplayModelPtr(m_disp3dmodel);
        m_disp3dkeypointedit->UpDataKeyInfor();

        connect(m_disp3dkeypointedit, &disp3dkeypointedit::SelectSuccess, this, &disp3dwindow::RefreshInfor);
    }
    else {
        QMessageBox::critical(this, "错误", "模型不存在");
    }
}

void disp3dwindow::SetAxisSize(int value)
{
    m_disp3dbackground->SetAxesLength(value);
}

void disp3dwindow::SetGridSize(int value)
{
    m_disp3dbackground->SetAxesGridSize(value);

    if (m_viewDockPanel) {
        m_viewDockPanel->resetGridChecks();
    }
}

void disp3dwindow::SelectKeyPoint()
{
    if(m_disp3dmodel->Flag_Actor){
        disp3dkeypointselect* m_disp3dkeypointselect = disp3dkeypointselect::instance();
        m_disp3dkeypointselect->show();
        m_disp3dkeypointselect->GetDisplayModelPtr(m_disp3dmodel);
        m_disp3dkeypointselect->UpDataKeyInfor();
        m_disp3dkeypointselect->InitWindow();
    }
    else {
        QMessageBox::critical(this, "错误", "模型不存在");
    }
}

void disp3dwindow::on_actionStart_triggered()
{
    if(!m_disp3dmodel->Flag_Actor){
        QMessageBox::critical(this, "错误", "模型不存在");
        return ;
    }

    ui->actionStart->setDisabled(true);
    ui->actionReset->setDisabled(true);
    ui->actionPause->setEnabled(true);
    ui->actionStop->setEnabled(true);

    freshTimer->start(1000/FPS);
    if(m_disp3dthread == nullptr){  //这里使用线程的方式计算数据，如果采用定时器的话，会造成界面很卡
        m_disp3dthread = new disp3dfreshmodelthread(this);
        m_disp3dthread->GetModelPtr(m_disp3dmodel);
        m_disp3dthread->GetRainbowIFE(m_rainbowIFE);
        m_disp3dthread->GetTrackingPointPtr(m_trackingpoint);
        m_disp3dthread->SetFrameReceiver(this);
    }
    m_disp3dthread->startThread();
}

void disp3dwindow::on_actionStop_triggered()
{
    if(!m_disp3dmodel->Flag_Actor){
        QMessageBox::critical(this, "错误", "模型不存在");
        return ;
    }
    ui->actionReset->setEnabled(true);

    ui->actionStart->setDisabled(true);
    ui->actionPause->setDisabled(true);
    ui->actionStop->setDisabled(true);

    m_disp3dthread->pauseThread();
    freshTimer->stop();
}

void disp3dwindow::on_actionPause_triggered()
{
    if(!m_disp3dmodel->Flag_Actor){
        QMessageBox::critical(this, "错误", "模型不存在");
        return ;
    }
    ui->actionPause->setDisabled(true);
    ui->actionReset->setDisabled(true);
    ui->actionStart->setEnabled(true);
    ui->actionStop->setEnabled(true);

    m_disp3dthread->pauseThread();
    freshTimer->stop();
}

void disp3dwindow::on_actionReset_triggered()
{
    m_disp3dmodel->ResetModel();
    m_trackingpoint->ResetTrackingPoint();
    m_disp3dmodel->ResetModel();

    ui->actionStart->setEnabled(true);
    ui->actionReset->setDisabled(true);
    ui->actionPause->setDisabled(true);
    ui->actionStop->setDisabled(true);

    RefreshStage();
}

void disp3dwindow::SetFPS(int fps)
{
    FPS = fps;
    if(freshTimer->isActive()){
        freshTimer->stop();
        freshTimer->start(1000/FPS);
    }
}

void disp3dwindow::SetScalarMapping(int index)
{
    if(!m_disp3dmodel->Flag_Actor){
        QMessageBox::critical(this, "错误", "模型不存在");
        return ;
    }


    qDebug() << index;
    switch(index){
        case 0:
            m_disp3dmodel->SetupColorMapping("Displacement_X");
            break;
        case 1:
            m_disp3dmodel->SetupColorMapping("Displacement_Y");
            break;
        case 2:
            m_disp3dmodel->SetupColorMapping("Displacement_Z");
            break;
        case 3:
            m_disp3dmodel->SetupColorMapping("Strain");
            break;
        case 4:
            m_disp3dmodel->SetupColorMapping("Temperature");
            break;
        default:
            qDebug() << "Invalid index!";
            return;
    }
}

void disp3dwindow::ClearScalarMapping()
{
    if(!m_disp3dmodel->Flag_Actor){
        QMessageBox::critical(this, "错误", "模型不存在");
        return ;
    }
    m_disp3dmodel->OFFScalars();
}

void disp3dwindow::SetXOZGridVisible(bool checked)
{
    if(Flag_ShowGrid)
        m_disp3dbackground->ShowGridXZ(checked);
    else
        return;
}

void disp3dwindow::SetXOYGridVisible(bool checked)
{
    if(Flag_ShowGrid)
        m_disp3dbackground->ShowGridXY(checked);
    else
        return;
}

void disp3dwindow::SetZOYGridVisible(bool checked)
{
    if(Flag_ShowGrid)
        m_disp3dbackground->ShowGridYZ(checked);
    else
        return;
}

void disp3dwindow::SetCameraLock(bool checked)
{
    m_disp3dmodel->CameraLock(checked);
}

void disp3dwindow::SetCameraFollow(bool checked)
{
    m_disp3dmodel->CameraFellow(checked);
    if(checked == false){
        m_disp3dmodel->GetVTKRenderer()->ResetCamera();
        m_disp3dmodel->GetVTKRenderWindow()->Render();
    }
}

void disp3dwindow::on_actionTrackingPoints_triggered()
{
    if(m_disp3dmodel->Flag_Actor){
        trackingpointmanager *m_trackingpointmanager = trackingpointmanager::instance();
        m_trackingpointmanager->SetDisplayModel(m_disp3dmodel);
        m_trackingpointmanager->SetTrackingPoint(m_trackingpoint);
        m_trackingpointmanager->show();
        m_trackingpointmanager->RefreshTable();
    }
    else {
        QMessageBox::critical(this, "错误", "模型不存在");
    }
}

void disp3dwindow::on_actionTrackingPointData_triggered()
{
    if(m_disp3dmodel->Flag_Actor){
        trackingpointdatatable *m_trackingpointdatatable = trackingpointdatatable::instance();
        m_trackingpointdatatable->SetDisplayModel(m_disp3dmodel);
        m_trackingpointdatatable->SetTrackingPoint(m_trackingpoint);
        m_trackingpointdatatable->show();

    }
    else {
        QMessageBox::critical(this, "错误", "模型不存在");
    }
}

void disp3dwindow::on_actionBackData_triggered()
{
    dispdata *m_dispdata = new dispdata(this);
    m_dispdata->GetRainbowIFE(m_rainbowIFE);
    m_dispdata->exec();
    delete m_dispdata;
}
