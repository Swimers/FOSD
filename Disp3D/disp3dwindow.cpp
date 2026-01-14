#include "disp3dwindow.h"
#include "ui_disp3dwindow.h"

#include <vtkFloatArray.h>

#include "QMessageBox"
#include "QDebug"


<<<<<<< Updated upstream
#include <stdio.h>
#include <QDebug>
#include <QMessageBox>
#include <QVBoxLayout>
#include <QFileDialog>
=======
>>>>>>> Stashed changes

#include <vtkPolyDataMapper.h>

#include <vtkAutoInit.h>
VTK_MODULE_INIT(vtkRenderingOpenGL2);    // 必须有：OpenGL 渲染后端
VTK_MODULE_INIT(vtkInteractionStyle);    // 交互风格
VTK_MODULE_INIT(vtkRenderingFreeType);   // 文本渲染（若用到）

disp3dwindow::disp3dwindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::disp3dwindow)
{
    ui->setupUi(this);
    InitWindow();
    //m_disp3dmodel->Disp3DModelDemo();
    on_pbtn_LoadModel_clicked();
}

disp3dwindow::~disp3dwindow()
{
<<<<<<< Updated upstream
    if (freshTimer) {
        freshTimer->stop();
        freshTimer = nullptr;   // 防止后续误用
    }

    if (m_disp3dthread) {
        m_disp3dthread->pauseThread();
        m_disp3dthread->wait();
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
=======
    if(this->m_freshmodelthread)
    {
        if (m_freshmodelthread->isRunning())
        {
            m_freshmodelthread->requestInterruption();
            m_freshmodelthread->wait();
        }
        delete m_freshmodelthread;
        m_freshmodelthread = nullptr;
    }
    if(this->m_disp3dmodel) delete m_disp3dmodel;
    if(this->m_disp3dpoint) delete m_disp3dpoint;
    delete ui;
>>>>>>> Stashed changes
}

void disp3dwindow::InitStatusBar()
{
    timer = new QTimer(this);

    QList<int> sizes1;
    sizes1 << 800 << 200 ;
    ui->splitter_2->setSizes(sizes1);               //设置水平分割器比例
    QList<int> sizes2;
    sizes2 << 600 << 400 ;
    ui->splitter->setSizes(sizes2);                 //设置垂直分割器比例
    this->statusBar()->setSizeGripEnabled(false);   //隐藏状态栏上的拖动图标
    QSplitter *splitter = new QSplitter(Qt::Horizontal, this);
    label0 = new QLabel("当前拾取点:",this);
    label1 = new QLabel("初始坐标:(*,*,*)",this);
    label2 = new QLabel("变形量:(*,*,*)",this);
    label3 = new QLabel("当前坐标:(*,*,*)",this);
    splitter->addWidget(label0);
    splitter->addWidget(label1);
    splitter->addWidget(label2);
    splitter->addWidget(label3);
    label0->setMargin(10);
    label0->setStyleSheet("QLabel { font-weight: bold; color: blue; }");
    label1->setMargin(5);
    label1->setStyleSheet("QLabel { font-weight: bold; color: green; }");
    label2->setMargin(5);
    label2->setStyleSheet("QLabel { font-weight: bold; color: green; }");
    label3->setMargin(5);
    label3->setStyleSheet("QLabel { font-weight: bold; color: green; }");
    ui->statusBar->addWidget(splitter);

    ui->actionStop->setDisabled(true);       //失能、变灰
    ui->cbox_FreSeclect->setCurrentText("20hz");

    ui->cbox_AixalSelect->setCurrentIndex(2);
    ui->cbox_SelectView->setCurrentIndex(6);
    ui->groupBox->setStyleSheet("QGroupBox {border: 0;}");
    fps = ui->cbox_FreSeclect->currentText().left(ui->cbox_FreSeclect->currentText().indexOf('h')).toInt();   //注意，一定要在定时器初始化完成后再设置

    connect(ui->actionModelDock,&QAction::triggered,this,&disp3dwindow::ModelDockShow);
    connect(ui->actionInforDock,&QAction::triggered,this,&disp3dwindow::InforDockShow);
}

void disp3dwindow::InitWindow()
{
    InitStatusBar();

    m_disp3dmodel = new disp3dmodel();
    ui->vtkw_Disp3D->SetRenderWindow(m_disp3dmodel->GetVtkRenderWindow());                  //将displaymodel的vtkRenderWindow窗口内容映射到qvtkWidget中。
    m_disp3dmodel->SetVtkRenderBackGradient(0.625,0.6797,0.762,0.109, 0.1797, 0.2773);
    m_disp3dmodel->AddAxesActor();                                                          //添加坐标系
    m_disp3dmodel->PickerGetQLabelPtr(std::make_tuple(label0, label1, label2, label3));     //将标签控件传入picker中

<<<<<<< Updated upstream
    ui->dock_Infor->setVisible(false);

    m_disp3dbackground = new disp3dbackground(m_disp3dmodel->GetVTKRenderer(),m_disp3dmodel->GetVTKRenderWindow());
    m_disp3dbackground->SetVtkRenderBackGradient(0.625,0.6797,0.762,0.109, 0.1797, 0.2773);
    m_disp3dbackground->AddAxesActor();
    m_disp3dbackground->AddAxesGrid();

    ui->actionReset->setDisabled(true);
    ui->actionStart->setDisabled(false);
    ui->actionStop->setEnabled(false);

    freshTimer = new QTimer(this);
    FPS = ui->cbox_FPS->currentText().left(ui->cbox_FPS->currentText().indexOf('H')).toInt();

    InitPointInfor();

    m_disp3dmodel->LoadModel("C:/Users/Swimers/Desktop/Test_Model/LeftWing2_new.stl");
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
    ui->lab_NumPoints->setText(QString::number(m_disp3dmodel->GetModelNumPoints()));
    ui->lab_NumPolys->setText(QString::number(m_disp3dmodel->GetModelNumPolys()));
    ui->lab_NumKeyPoints->setText(QString::number(m_disp3dmodel->GetModelNumKeyPoints()));
    ui->lab_MaxDeformation->setText("00.00");
}

void disp3dwindow::RefreshStage()
{
    m_disp3dmodel->GetVTKRenderWindow()->Render();
    char str[8];
    sprintf(str, "%0.4f", m_disp3dmodel->MaxDisplace);
    ui->lab_MaxDeformation->setText(QString(str));
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
=======
    connect(timer,SIGNAL(timeout()),this,SLOT(TimeOutHandel_Fresh()));
    //connect(ui->cbox_Fellow, &QCheckBox::toggled, m_disp3dmodel, &disp3dmodel::FlagChange);
}

void disp3dwindow::ModelDockShow()
>>>>>>> Stashed changes
{
    ui->dock_Model->setVisible(true);
}

void disp3dwindow::InforDockShow()
{
    ui->dock_Infor->setVisible(true);
}

void disp3dwindow::TimeOutHandel_Fresh()
{
    ui->label_dispy->setStyleSheet("QLabel { font-weight: bold; color: green; }");
    ui->label_dispy->setText("主方向最大变形："+QString::number(ife->node_u[12]*1000,'f',2));
    m_disp3dmodel->DynamicFresh();
}

void disp3dwindow::on_pbtn_LoadModel_clicked()
{
    if(m_disp3dmodel->Flag_Actor == false){
        if(m_disp3dmodel->readData("./Data/Input/data.txt")){
            m_disp3dmodel->CreatePointSet();
            m_disp3dmodel->CreateCellSet();
            m_disp3dmodel->CreateDataSet();

            m_disp3dmodel->ExecutePipeline();
            m_disp3dmodel->GetVtkRenderWindow()->Render();   //刷新

            on_pbtn_ChangeView_clicked();
            ui->edit_SphereSize->setText(QString::number(static_cast<double>(m_disp3dmodel->GetSphereSize())));
            ui->cbox_DispAxis->setChecked(true);
            on_cbox_DispAxis_clicked(true);

            QString str1 = "节点数: " + QString::number(m_disp3dmodel->S_M_points->GetNumberOfPoints());
            ui->label_point->setText(str1);
            QString str2 = "单元数: " + QString::number(m_disp3dmodel->S_M_cellarray->GetNumberOfCells());
            ui->label_element->setText(str2);
            QString str3 = "关键点数: " + QString::number(m_disp3dmodel->SolidModeFlagPointToNodePoint.size());
            ui->label_flagpoint->setText(str3);
        }
        else{
            QMessageBox::critical(this, "错误", "缺失模型文件");
        }
    }
    else{
        QMessageBox::critical(this, "警告", "模型已存在");
    }
}

void disp3dwindow::on_pbtn_ChangeView_clicked()
{
    if(m_disp3dmodel->Flag_Actor == true){
        int ViewFlag = ui->cbox_SelectView->currentIndex() + 1;
        m_disp3dmodel->Resetcamera(ViewFlag);
        m_disp3dmodel->GetVtkRenderWindow()->Render();   //刷新
    }
    else{
        QMessageBox::critical(this, "错误", "模型不存在");
    }
}

void disp3dwindow::on_cbox_DispAxis_clicked(bool checked)
{
    m_disp3dmodel->AxesShow(checked);
}

void disp3dwindow::on_cbox_DispEdge_clicked(bool checked)
{
    if(m_disp3dmodel->Flag_Actor == true){
        m_disp3dmodel->LineShowPipeline(checked);
    }
    else{
        QMessageBox::critical(this, "错误", "模型不存在");
    }
}

void disp3dwindow::on_pbtn_SetSize_clicked()
{
    QString inputText = ui->edit_SphereSize->text(); // 获取用户输入的文本
    bool isNumeric;                                  // 标志变量，用于标识是否是数字
    float integerValue = inputText.toFloat(&isNumeric);
    if(!isNumeric) {QMessageBox::critical(this, "错误", "输入参数错误"); return;};
    if(integerValue > 0.5f || integerValue < 0) {QMessageBox::critical(this, "错误", "输入参数应在0~0.5内"); return;};

    m_disp3dmodel->ModifySphereSize(integerValue);
}

void disp3dwindow::on_pbtn_AddPoint_clicked()
{
    if(m_disp3dmodel->Flag_Actor == false){
        QMessageBox::critical(this, "错误", "模型不存在");
        return ;
    }
    if(m_disp3dpoint == nullptr){
        m_disp3dpoint = new disp3dpoint();
        m_disp3dpoint->GetDisplayModelPtr(m_disp3dmodel);
        m_disp3dpoint->initialize();
        m_disp3dpoint->setModal(false);
        m_disp3dpoint->show();

        connect(m_disp3dpoint->addDisplayNode,SIGNAL(selectNodeInf_deformed(int,double,double,double)),
                this,SLOT(deal_addcord(int,double,double,double)));
        connect(m_disp3dpoint,SIGNAL(delete_node(int)),
                this,SLOT(deal_delcord(int)));
        connect(m_disp3dpoint->editDisplayNode,SIGNAL(editNodeInf_deformed(int,int,double,double,double)),
                this,SLOT(deal_editcord(int,int,double,double,double)));
    }
    else{
        m_disp3dpoint->show();
    }
}

void disp3dwindow::deal_addcord(int num,double x,double y,double z)
{
    this->m_disp3dmodel->createDisplayPointDisplaceText(num,x,y,z);
    vtkActor *actor = this->m_disp3dmodel->CreatesphereSource(x,y,z);//创建重要点的标记对象
    this->m_disp3dmodel->Addsphere(actor,num);//创建重要点的标记对象
}

void disp3dwindow::deal_delcord(int nodeNo)
{
    m_disp3dmodel->DeleteDisplayCordText(m_disp3dmodel->SolidModeFlagPointToNodePoint[nodeNo]);
    m_disp3dmodel->Deletesphere(m_disp3dmodel->SolidModeFlagPointToNodePoint[nodeNo]);
}

void disp3dwindow::deal_editcord(int inicord,int num, double x, double y, double z)
{
    m_disp3dmodel->EditDisplayCordText(inicord,num,x,y,z);
    m_disp3dmodel->EditDisplayCordFlagInfo(inicord,num,x,y,z);
}

void disp3dwindow::on_cbox_FreSeclect_currentIndexChanged(const QString &arg1)
{
    fps = arg1.left(arg1.indexOf('h')).toInt();
    if(timer->isActive()){
        timer->stop();
        timer->start(1000/fps);
    }
}

void disp3dwindow::on_pbtn_DispAnalysis_clicked()
{
    if(m_disp3dmodel->Flag_Actor == false){
        QMessageBox::critical(this, "错误", "模型不存在");
        return ;
    }
    QString inputText = ui->edit_ColorMaxRange->text();
    bool isNumeric;
    float integerValue = inputText.toFloat(&isNumeric);
    if(!isNumeric) {QMessageBox::critical(this, "错误", "输入参数错误"); return;};

    if(m_freshmodelthread == nullptr){
        m_freshmodelthread = new disp3dfreshmodelthread(this);
        m_freshmodelthread->GetPtr(m_disp3dmodel,ife,ui->label_dispy);
        m_freshmodelthread->pauseThread();
        m_freshmodelthread->start();
    }

    int index = ui->cbox_AixalSelect->currentIndex();
    disp3dmodel::ScalarTable scalarIndex;
    switch (index) {
        case 0:
            scalarIndex = disp3dmodel::dispx;
            break;
        case 1:
            scalarIndex = disp3dmodel::dispy;
            break;
        case 2:
            scalarIndex = disp3dmodel::dispz;
            break;
        default:
            qDebug() << "Invalid index!";
            return;
    }

   // 调用相应的函数
   m_disp3dmodel->SetScalarMapping(scalarIndex,integerValue);
}

void disp3dwindow::on_actionStart_triggered()
{
    if(m_disp3dmodel->Flag_Actor == false){
        QMessageBox::critical(this, "错误", "模型不存在");
        return ;
    }
    ui->actionStart->setDisabled(true);
    ui->actionReset->setDisabled(true);

    ui->actionPause->setEnabled(true);
    ui->actionStop->setEnabled(true);
    timer->start(1000/fps);             //数据刷新频率
    if(m_freshmodelthread == nullptr){  //这里使用线程的方式计算数据，如果采用定时器的话，会造成界面很卡
        m_freshmodelthread = new disp3dfreshmodelthread(this);
        m_freshmodelthread->GetPtr(m_disp3dmodel,ife,ui->label_dispy);
        m_freshmodelthread->start();
    }
    m_freshmodelthread->startThread();
}

void disp3dwindow::on_actionStop_triggered()
{
    if(m_disp3dmodel->Flag_Actor == false)
    {
        QMessageBox::critical(this, "错误", "模型不存在");
        return ;
    }
    ui->actionReset->setEnabled(true);

    ui->actionStart->setDisabled(true);
    ui->actionPause->setDisabled(true);
    ui->actionStop->setDisabled(true);
<<<<<<< Updated upstream

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
=======
    m_freshmodelthread->pauseThread();
    timer->stop();
>>>>>>> Stashed changes
}

void disp3dwindow::on_actionReset_triggered()
{
<<<<<<< Updated upstream
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
=======
    if(m_disp3dmodel->Flag_Actor == false){
        QMessageBox::critical(this, "错误", "模型不存在");
        return ;
>>>>>>> Stashed changes
    }
    if(!ui->actionStart->isEnabled()){
        QMessageBox::critical(this, "错误", "请先关闭监测");
        return ;
    }
    int elementnum = m_disp3dmodel->ReversibleElementDvaluepointSet.size();
    for (int i = 1; i <= elementnum; ++i){
        QVector<int>* Dvaluepoint = m_disp3dmodel->ReversibleElementDvaluepointSet[i]->Dvaluepoint;    //第i个逆有限单元中有限单元的编号
        int pointnumber = m_disp3dmodel->ReversibleElementDvaluepointSet[i]->DvaluePointNum;           //第i个逆有限单元中有限单元的总数
        for (int j = 0; j < pointnumber; j++){
            int nodeNo = (*Dvaluepoint)[j];//需要差值的结点的编号
            double  BFx, BFy, BFz;/*BF全局坐标，AF局部坐标*/
            BFx = (m_disp3dmodel->nodes[nodeNo]->cord[0]) * 1000; //将点的坐标单元转化为毫米
            BFy = (m_disp3dmodel->nodes[nodeNo]->cord[1]) * 1000;
            BFz = (m_disp3dmodel->nodes[nodeNo]->cord[2]) * 1000;
            QVector<int>* pointset = m_disp3dmodel->new_node_ex_node_info[nodeNo];
            int pnum = (*pointset).size();
            for (int k = 0; k < pnum; ++k){
                //获取关联点的编号
                int  nodeno = (*pointset)[k];
                double point[3] = { 0.0 };
                //重置位移值为0
                m_disp3dmodel->PointDisplaceSet[nodeno]->xdisplace = 0;
                m_disp3dmodel->PointDisplaceSet[nodeno]->ydisplace = 0;
                m_disp3dmodel->PointDisplaceSet[nodeno]->zdisplace = 0;
                //更新位移标量数据（用于可视化）
                m_disp3dmodel->scalars_dispx->InsertTuple1(nodeno-1,  0);
                m_disp3dmodel->scalars_dispy->InsertTuple1(nodeno-1,  0);
                m_disp3dmodel->scalars_dispz->InsertTuple1(nodeno-1,  0);
                //根据初始坐标和位移因子重新计算点的位置
                point[0] = m_disp3dmodel->new_node_info1[nodeno]->x_cord + m_disp3dmodel->Displacefactor
                        * m_disp3dmodel->PointDisplaceSet[nodeno]->xdisplace;//Displacefactor*
                point[1] = m_disp3dmodel->new_node_info1[nodeno]->y_cord + m_disp3dmodel->Displacefactor
                        * m_disp3dmodel->PointDisplaceSet[nodeno]->ydisplace;//Displacefactor*
                point[2] = m_disp3dmodel->new_node_info1[nodeno]->z_cord + m_disp3dmodel->Displacefactor
                        * m_disp3dmodel->PointDisplaceSet[nodeno]->zdisplace;//Displacefactor*
                m_disp3dmodel->S_M_points->InsertPoint(nodeno-1, point[0], point[1], point[2]);
            }
        }
    }
    m_disp3dmodel->S_M_points->Modified();
    m_disp3dmodel->DynamicFresh();
}

void disp3dwindow::SwitchToB()
{
    ui->toolBar->setStyleSheet("background-color:#302f3a;");
    ui->statusBar->setStyleSheet("background-color:#302f3a;");
    ui->dock_Infor->setStyleSheet("background-color:#302f3a;");
    ui->dock_Model->setStyleSheet("background-color:#302f3a;");
}

void disp3dwindow::SwitchToW()
{
    ui->toolBar->setStyleSheet("background-color:#ececec;");
    ui->statusBar->setStyleSheet("background-color:#ececec;");
    ui->dock_Infor->setStyleSheet("background-color:#ececec;");
    ui->dock_Model->setStyleSheet("background-color:#ececec;");
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
