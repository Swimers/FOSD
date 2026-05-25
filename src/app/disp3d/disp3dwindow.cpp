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

    ui->dock_Infor->setVisible(false);
    ui->actionReset->setDisabled(true);
    ui->actionStart->setDisabled(false);
    ui->actionStop->setEnabled(false);
    ui->actionPause->setDisabled(true);

    freshTimer = new QTimer(this);
    FPS = ui->cbox_FPS->currentText().left(ui->cbox_FPS->currentText().indexOf('H')).toInt();

    InitPointInfor();

    // 测试代码段，初始化加载模型
    m_disp3dmodel->LoadModel("data/model/rainbow.stl");
    m_disp3dmodel->Flag_Actor = true;
    m_disp3dbackground->CreateWorld();
    m_disp3dmodel->GetVTKRenderWindow()->Render();
    m_trackingpoint->LoadTrackingPointsFromFile("data/trackingpoint/tracking_points_example_modified.csv");

    //m_disp3dmodel->LoadModel("C:/Users/Swimers/Desktop/Test_Model/LeftWing2_new.stl");
    if(m_disp3dmodel->Flag_Actor)
    RefreshInfor();

    connect(freshTimer,SIGNAL(timeout()),this,SLOT(RefreshStage()));
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
    ui->lab_NumPoints->setText(QString::number(m_disp3dmodel->ModelPointsNum));
    ui->lab_NumPolys->setText(QString::number(m_disp3dmodel->ModelPolysNum));
    ui->lab_NumKeyPoints->setText(QString::number(m_disp3dmodel->ModelKeyPointsNum));
    ui->lab_MaxDeformation->setText("00.00");
}

void disp3dwindow::RefreshStage()
{
    m_disp3dmodel->GetVTKRenderWindow()->Render();
    char str[8];
    if(m_disp3dmodel){
        QString maxDeformStr = QString::number(m_disp3dmodel->MaxDisplace, 'f', 4);
        ui->lab_MaxDeformation->setText(maxDeformStr);
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

void disp3dwindow::on_pbtn_ModelLoad_clicked()
{
    if(!m_disp3dmodel->Flag_Actor){
        QString filter = tr("STL模型 (*.stl)");
        QString filePath = QFileDialog::getOpenFileName(this, tr("选择文件"), "", filter);
        if (!filePath.isEmpty()) {
            m_disp3dmodel->LoadModel(filePath);
            m_disp3dbackground->CreateWorld();
            m_disp3dmodel->GetVTKRenderWindow()->Render();
            RefreshInfor();
        }
        else {
            QMessageBox::information(this, tr("提示"), tr("未选择任何文件！"));
        }
    }
    else {
        QMessageBox::critical(this, "警告", "模型已存在");
    }
}

void disp3dwindow::on_cbox_AxisAble_clicked(bool checked)
{
    m_disp3dbackground->ShowAxes(checked);
}

void disp3dwindow::on_cbox_GridAble_clicked(bool checked)
{
    Flag_ShowGrid = checked;
    if(checked){
        m_disp3dbackground->ShowGridXY(ui->cbox_XOYGrid->isChecked());
        m_disp3dbackground->ShowGridXZ(ui->cbox_XOZGrid->isChecked());
        m_disp3dbackground->ShowGridYZ(ui->cbox_ZOYGrid->isChecked());
    }
    else {
        m_disp3dbackground->ShowGridXY(false);
        m_disp3dbackground->ShowGridXZ(false);
        m_disp3dbackground->ShowGridYZ(false);
    }
}

void disp3dwindow::on_pbtn_ModelClear_clicked()
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
                ui->lab_NumPoints->clear();
                ui->lab_NumPolys->clear();
                ui->lab_NumKeyPoints->clear();
                ui->lab_MaxDeformation->clear();
            }
    }
    else {
        QMessageBox::critical(this, "警告", "模型不存在");
    }
}

void disp3dwindow::on_pbtn_SwitchView_clicked()
{
    if(m_disp3dmodel->Flag_Actor){
        int ViewFlag = ui->cbox_SelectView->currentIndex();
        m_disp3dmodel->ResetCamera(ViewFlag);
        m_disp3dmodel->GetVTKRenderWindow()->Render();   //刷新
    }
    else {
        QMessageBox::critical(this, "错误", "模型不存在");
    }
}

void disp3dwindow::on_pbtn_ModelMerge_clicked()
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

void disp3dwindow::on_pbtn_ModelExport_clicked()
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

void disp3dwindow::on_pbtn_KeyPointEdit_clicked()
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

void disp3dwindow::on_hsd_AxisSize_valueChanged(int value)
{
    m_disp3dbackground->SetAxesLength(value);
}

void disp3dwindow::on_hsd_GridSize_sliderReleased()
{
    m_disp3dbackground->SetAxesGridSize(ui->hsd_GridSize->value());

    ui->cbox_XOYGrid->setChecked(false);
    ui->cbox_XOZGrid->setChecked(false);
    ui->cbox_ZOYGrid->setChecked(false);
    ui->cbox_GridAble->setChecked(false);
}

void disp3dwindow::on_pbtn_KeyPointSelect_clicked()
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

void disp3dwindow::on_actionModelDock_triggered()
{
    ui->dock_Model->setVisible(true);
}

void disp3dwindow::on_actionInforDock_triggered()
{
    ui->dock_Infor->setVisible(true);
}

void disp3dwindow::on_actionViewDock_triggered()
{
    ui->dock_View->setVisible(true);
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

void disp3dwindow::on_cbox_FPS_currentTextChanged(const QString &arg1)
{
    FPS = arg1.left(ui->cbox_FPS->currentText().indexOf('H')).toInt();
    if(freshTimer->isActive()){
        freshTimer->stop();
        freshTimer->start(1000/FPS);
    }
}

void disp3dwindow::on_pbtn_ScalarSet_clicked()
{
    if(!m_disp3dmodel->Flag_Actor){
        QMessageBox::critical(this, "错误", "模型不存在");
        return ;
    }


    int index = ui->cbox_ScalarSelect->currentIndex();
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

void disp3dwindow::on_pbtn_ScalarClear_clicked()
{
    if(!m_disp3dmodel->Flag_Actor){
        QMessageBox::critical(this, "错误", "模型不存在");
        return ;
    }
    m_disp3dmodel->OFFScalars();
}

void disp3dwindow::on_cbox_XOZGrid_clicked(bool checked)
{
    if(Flag_ShowGrid)
        m_disp3dbackground->ShowGridXZ(checked);
    else
        return;
}

void disp3dwindow::on_cbox_XOYGrid_clicked(bool checked)
{
    if(Flag_ShowGrid)
        m_disp3dbackground->ShowGridXY(checked);
    else
        return;
}

void disp3dwindow::on_cbox_ZOYGrid_clicked(bool checked)
{
    if(Flag_ShowGrid)
        m_disp3dbackground->ShowGridYZ(checked);
    else
        return;
}

void disp3dwindow::on_cbox_CareamLockAble_clicked(bool checked)
{
    m_disp3dmodel->CameraLock(checked);
}

void disp3dwindow::on_cbox_FellowAble_clicked(bool checked)
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
