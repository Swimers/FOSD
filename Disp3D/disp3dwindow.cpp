#include "disp3dwindow.h"
#include "ui_disp3dwindow.h"

#include <disp3dbackground.h>

#include <QDebug>
#include <QMessageBox>
#include <QVBoxLayout>
#include <QFileDialog>

disp3dwindow::disp3dwindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::disp3dwindow)
{
    ui->setupUi(this);

    InitWindow();
}

disp3dwindow::~disp3dwindow()
{
    delete m_disp3dmodel;
    delete m_disp3dbackground;
    delete m_disp3dinteractor;
    delete ui;
}

void disp3dwindow::InitWindow()
{
    m_disp3dmodel = new disp3dmodel();
    ui->vtk_stage->SetRenderWindow(m_disp3dmodel->GetVTKRenderWindow());

    m_disp3dbackground = new disp3dbackground(m_disp3dmodel->GetVTKRenderer(),m_disp3dmodel->GetVTKRenderWindow());
    m_disp3dbackground->SetVtkRenderBackGradient(0.625,0.6797,0.762,0.109, 0.1797, 0.2773);
    m_disp3dbackground->AddAxesActor();
    m_disp3dbackground->AddAxesGrid();

    ui->actionStart->setDisabled(false);
    ui->actionStop->setEnabled(false);

    ui->hsd_ViewPar->setValue(50);

    freshTimer = new QTimer(this);
    FPS = ui->cbox_FPS->currentText().left(ui->cbox_FPS->currentText().indexOf('H')).toInt();

    InitPointInfor();

    m_disp3dmodel->LoadModel("C:/Users/Swimers/Desktop/Test_Model/LeftWing2_new.stl");
    RefreshInfor();

    connect(freshTimer,SIGNAL(timeout()),this,SLOT(RefreshStage()));
}

void disp3dwindow::InitPointInfor()
{
    ui->label->setStyleSheet("QLabel { font-weight: bold; color: blue; }");
    ui->lab_KeyPointID->setStyleSheet("QLabel { font-weight: bold; color: blue; }");
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
    ui->lab_NumPoints->setText(QString::number(m_disp3dmodel->GetModelNumPoints()));
    ui->lab_NumPolys->setText(QString::number(m_disp3dmodel->GetModelNumPolys()));
    ui->lab_NumKeyPoints->setText(QString::number(m_disp3dmodel->GetModelNumKeyPoints()));
    ui->lab_MaxDeformation->setText("00.00");
}

void disp3dwindow::RefreshStage()
{
    m_disp3dmodel->GetVTKRenderWindow()->Render();
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

void disp3dwindow::on_checkBox_clicked(bool checked)
{
    m_disp3dbackground->ShowAxesGrid(checked);
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
    ui->actionStop->setEnabled(true);
    freshTimer->start(1000/FPS);
    if(m_disp3dthread == nullptr){  //这里使用线程的方式计算数据，如果采用定时器的话，会造成界面很卡
        m_disp3dthread = new disp3dfreshmodelthread(this);
        m_disp3dthread->GetPtr(m_disp3dmodel, ife);
        m_disp3dthread->start();
    }
    m_disp3dthread->startThread();
}

void disp3dwindow::on_actionStop_triggered()
{
    if(!m_disp3dmodel->Flag_Actor){
        QMessageBox::critical(this, "错误", "模型不存在");
        return ;
    }
    ui->actionStart->setEnabled(true);
    ui->actionStop->setDisabled(true);
    m_disp3dthread->pauseThread();
    freshTimer->stop();
}

void disp3dwindow::on_actionPause_triggered()
{

}

void disp3dwindow::on_actionReset_triggered()
{

}

void disp3dwindow::on_cbox_FPS_currentTextChanged(const QString &arg1)
{
    FPS = arg1.left(ui->cbox_FPS->currentText().indexOf('H')).toInt();
    if(freshTimer->isActive()){
        freshTimer->stop();
        freshTimer->start(1000/FPS);
    }
}
