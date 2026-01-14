#include "disp3dmainwindow.h"
#include "ui_disp3dmainwindow.h"
#include <QMessageBox>
#include <QDebug>
#include <vtkFloatArray.h>
#include <vector>
#include "config.h"
#include "pointtable.h"
#pragma execution_character_set("utf-8")


disp3dMainWindow::disp3dMainWindow(const QString& privilege,QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::disp3dMainWindow)
{
    ui->setupUi(this);

    Init();
    on_btnLoadModel_clicked();
}

disp3dMainWindow::~disp3dMainWindow()
{
    if(this->calculationthread)
    {
        if (calculationthread->isRunning())
        {
            calculationthread->requestInterruption();
            calculationthread->wait();
        }
        delete calculationthread;
        calculationthread=nullptr;
    }
    if(this->displaymodel) delete displaymodel;
    if(this->CordDisplaySet1) delete CordDisplaySet1;
    delete ui;
}

void disp3dMainWindow::Init()
{
    QList<int> sizes1;
    sizes1 << 800 << 200 ;
    ui->splitter_2->setSizes(sizes1);    //设置水平分割器比例
    QList<int> sizes2;
    sizes2 << 600 << 400 ;
    ui->splitter->setSizes(sizes2);    //设置垂直分割器比例
    this->statusBar()->setSizeGripEnabled(false);//隐藏状态栏上的拖动图标

    QSplitter *splitter = new QSplitter(Qt::Horizontal, this);

    label0 = new QLabel("当前拾取点:",this);
    label1 = new QLabel("初始坐标:(*,*,*)",this);
    label2 = new QLabel("变形量:(*,*,*)",this);
    label3 = new QLabel("当前坐标:(*,*,*)",this);
    //splitter->addWidget(spacer);
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

    ui->comboBoxView->setCurrentIndex(6);
    //ui->groupBox->setStyleSheet("QGroupBox {border: 0;}");


    displaymodel = new displayModel();
    ui->qvtkWidget->SetRenderWindow(displaymodel->GetvtkRenderWindow());               //将displaymodel的vtkRenderWindow窗口内容映射到qvtkWidget中。
    displaymodel->SetvtkRenderBackColor2(0.625,0.6797,0.762,0.109, 0.1797, 0.2773);
    displaymodel->AddAxesActor();       //添加坐标系
    displaymodel->PickerGetQLabelPtr(std::make_tuple(label0, label1, label2, label3));//将标签控件传入picker中

    connect(ui->actionModelDock,&QAction::triggered,this,&disp3dMainWindow::ModelDockShow);
    connect(ui->actionInforDock,&QAction::triggered,this,&disp3dMainWindow::InforDockShow);

    ui->actionStop->setDisabled(true);       //失能、变灰

    timer_disp3d = new QTimer(this);
    connect(timer_disp3d,SIGNAL(timeout()),this,SLOT(handleTimeOut_WJBGL()));

    ui->comboBoxHz->setCurrentText(QString::number(Config::getInstance().getModelFlushFrep())+"hz");
    fps = ui->comboBoxHz->currentText().left(ui->comboBoxHz->currentText().indexOf('h')).toInt();   //注意，一定要在定时器初始化完成后再设置

    ui->comboBoxDisp->setCurrentIndex(Config::getInstance().getColorMode()-1);
    ui->editMinRange->setText(QString::number(Config::getInstance().getMinRange(), 'f',2));
    ui->editMaxRange->setText(QString::number(Config::getInstance().getMaxRange(), 'f',2));
    ui->editFilterCoeff->setText(QString::number(Config::getInstance().getFilterCoeff(), 'f',2));

}

void disp3dMainWindow::on_btnLoadModel_clicked()
{
    if(displaymodel->ActorFlag == false)
    {
        if(displaymodel->readData("./Data/Input/data.txt"))
        {
            displaymodel->CreatePointSet();
            displaymodel->CreateCellSet();
            displaymodel->CreateDataSet();

            displaymodel->ExecutePipeline();

            displaymodel->GetvtkRenderWindow()->Render();   //刷新
            on_btnChangeView_clicked();
            ui->editSphereSize->setText(QString::number(displaymodel->GetSphereSize()));
            ui->checkBoxAxis->setChecked(true);
            on_checkBoxAxis_clicked(true);


            QString str1 = "节点数: " + QString::number(displaymodel->S_M_points->GetNumberOfPoints());
            ui->label_point->setText(str1);

            QString str2 = "单元数: " + QString::number(displaymodel->S_M_cellarray->GetNumberOfCells());
            ui->label_element->setText(str2);

            QString str3 = "关键点数: " + QString::number(displaymodel->SolidModeFlagPointToNodePoint.size());
            ui->label_flagpoint->setText(str3);

            ui->editSphereSize->setText(QString::number(Config::getInstance().getPointSize(),'f',3));
            displaymodel->ModifySphereSize(Config::getInstance().getPointSize());
        }
        else
        {
            QMessageBox::critical(this, "错误", "缺失模型文件");
        }
    }
    else
    {
        QMessageBox::critical(this, "警告", "模型已存在");
    }

    // 先看添加节点这个指针在不在，这里应该已经初始化了
    if(CordDisplaySet1 == nullptr)
    {
        CordDisplaySet1=new CordDisplaySet();
        CordDisplaySet1->GetDisplayModelPtr(displaymodel);
        CordDisplaySet1->initialize();
        CordDisplaySet1->setModal(false);
        //CordDisplaySet1->show();
        connect(CordDisplaySet1->addDisplayNode,SIGNAL(seleNodeInf_deformed(int,double,double,double)),
                this,SLOT(deal_addcord(int,double,double,double)));
        connect(CordDisplaySet1,SIGNAL(delete_node(int)),
                this,SLOT(deal_delecord(int)));
        connect(CordDisplaySet1->editDisplayNode,SIGNAL(editNodeInf_deformed(int,int,double,double,double)),
                this,SLOT(deal_editcord(int,int,double,double,double)));
    }

    std::unordered_map<int,Point3f> flagPointIndex = Config::getInstance().getFlagPoint1();

    // 浮点数比较的 lambda（可调精度）
    auto isClose = [](double a, double b, double eps = 1e-6) {
        qDebug()<<a<<b;
        return std::abs(a - b) < eps;
    };

    for(auto i:flagPointIndex){
        int nodeNo = displaymodel->SolidModeFlagPointToNodePoint[i.first];
        if(nodeNo <= displaymodel->S_M_points->GetNumberOfPoints()
                &&isClose((*(displaymodel->new_node_info1[nodeNo])).x_cord,i.second.x)
                &&isClose((*(displaymodel->new_node_info1[nodeNo])).y_cord,i.second.y)
                && isClose((*(displaymodel->new_node_info1[nodeNo])).z_cord,i.second.z))
        {
            CordDisplaySet1->add_node(i.first);
        }
        else{
            Config::getInstance().deleteFlagPoint1(i.first);
        }
    }

}


void disp3dMainWindow::ModelDockShow()
{
    ui->dockWidgetModel->setVisible(true);
}

void disp3dMainWindow::InforDockShow()
{
    ui->dockWidgetInfor->setVisible(true);
}

void disp3dMainWindow::on_btnChangeView_clicked()
{
    if(displaymodel->ActorFlag == true)
    {
        int ViewFlag = ui->comboBoxView->currentIndex() + 1;

        displaymodel->Resetcamera(ViewFlag);
        displaymodel->GetvtkRenderWindow()->Render();   //刷新
    }
    else
    {
        QMessageBox::critical(this, "错误", "模型不存在");
    }

}

void disp3dMainWindow::on_checkBoxEdge_clicked(bool checked)
{
    if(displaymodel->ActorFlag == true)
    {
        displaymodel->LineShowPipeline(checked);
    }
    else
    {
        QMessageBox::critical(this, "错误", "模型不存在");
    }
}


void disp3dMainWindow::on_checkBoxAxis_clicked(bool checked)
{
    displaymodel->AxesShow(checked);
}

void disp3dMainWindow::on_btnAddPoint_clicked()
{
    if(displaymodel->ActorFlag == false)
    {
        QMessageBox::critical(this, "错误", "模型不存在");
        return ;
    }

    CordDisplaySet1->show();
}

//添加关键点样式设置
void disp3dMainWindow::deal_addcord(int num,double x,double y,double z)
{
    this->displaymodel->createDisplayPointDisplaceText(num,x,y,z);
    vtkActor *actor = this->displaymodel->CreatesphereSource(x,y,z);//创建重要点的标记对象
    this->displaymodel->Addsphere(actor,num);//创建重要点的标记对象

    int nodeNo = displaymodel->SolidModeNodePointToFlagPoint[num];
    Config::getInstance().addFlagPoint1(nodeNo,{(*(displaymodel->new_node_info1[num])).x_cord,
                                                (*(displaymodel->new_node_info1[num])).y_cord,
                                                (*(displaymodel->new_node_info1[num])).z_cord});       //这里写的真抽象
    //qDebug()<<num<<nodeNo;
}
//删除关键点样式设置
void disp3dMainWindow::deal_delecord(int nodeNo)
{
    displaymodel->DeleteDisplayCordText(displaymodel->SolidModeFlagPointToNodePoint[nodeNo]);
    displaymodel->Deletesphere(displaymodel->SolidModeFlagPointToNodePoint[nodeNo]);


    Config::getInstance().deleteFlagPoint1(nodeNo);
    //qDebug()<<nodeNo;
}

void disp3dMainWindow::deal_editcord(int inicord,int num, double x, double y, double z)
{
    displaymodel->EditDisplayCordText(inicord,num,x,y,z);
    displaymodel->EditDisplayCordFlagInfo(inicord,num,x,y,z);

    int nodeNo = displaymodel->SolidModeNodePointToFlagPoint[num];
    Config::getInstance().deleteFlagPoint1(displaymodel->SolidModeNodePointToFlagPoint[inicord]);
    Config::getInstance().addFlagPoint1(nodeNo,{(*(displaymodel->new_node_info1[num])).x_cord,
                                                (*(displaymodel->new_node_info1[num])).y_cord,
                                                (*(displaymodel->new_node_info1[num])).z_cord});
    //qDebug()<<inicord<<num<<displaymodel->SolidModeNodePointToFlagPoint[num];
}


void disp3dMainWindow::on_btkSetSize_clicked()
{
    QString inputText = ui->editSphereSize->text(); // 获取用户输入的文本
    bool isNumeric; // 标志变量，用于标识是否是数字
    float integerValue = inputText.toFloat(&isNumeric);
    if(!isNumeric) {QMessageBox::critical(this, "错误", "输入参数错误"); return;};
    if(integerValue>0.5f||integerValue<0) {QMessageBox::critical(this, "错误", "输入参数应在0~0.5内"); return;};

    displaymodel->ModifySphereSize(integerValue);
    Config::getInstance().setPointSize(integerValue);
}

void disp3dMainWindow::on_actionStart_triggered()
{
    if(displaymodel->ActorFlag == false)
    {
        QMessageBox::critical(this, "错误", "模型不存在");
        return ;
    }
    ui->actionStart->setDisabled(true);
    ui->actionStop->setEnabled(true);

    timer_disp3d->start(1000/fps);//数据刷新频率

    //这里使用线程的方式计算数据，如果采用定时器的话，会造成界面很卡/
    if(calculationthread == nullptr)
    {
        calculationthread = new ModelCalculationThread(this);
        calculationthread->GetPtr(displaymodel,ife);
        calculationthread->start();
    }
    calculationthread->startThread();

}

void disp3dMainWindow::on_actionStop_triggered()
{
    if(displaymodel->ActorFlag == false)
    {
        QMessageBox::critical(this, "错误", "模型不存在");
        return ;
    }
    ui->actionStart->setEnabled(true);
    ui->actionStop->setDisabled(true);
    calculationthread->pauseThread();

    timer_disp3d->stop();
}


void disp3dMainWindow::on_actionReset_triggered()
{
    if(displaymodel->ActorFlag == false)
    {
        QMessageBox::critical(this, "错误", "模型不存在");
        return ;
    }
    if(!ui->actionStart->isEnabled())
    {
        QMessageBox::critical(this, "错误", "请先关闭监测");
        return ;
    }
    int elementnum = displaymodel->ReversibleElementDvaluepointSet.size();
    for (int i = 1; i <= elementnum; ++i)
    {
        QVector<int>* Dvaluepoint = displaymodel->ReversibleElementDvaluepointSet[i]->Dvaluepoint;    //第i个逆有限单元中有限单元的编号
        int pointnumber = displaymodel->ReversibleElementDvaluepointSet[i]->DvaluePointNum;           //第i个逆有限单元中有限单元的总数

        for (int j = 0; j < pointnumber; j++)
        {
            int nodeNo = (*Dvaluepoint)[j];//需要差值的结点的编号
            double  BFx, BFy, BFz;/*BF全局坐标，AF局部坐标*/
            /*其中cord是从*/
            BFx = (displaymodel->nodes[nodeNo]->cord[0]) * 1000; //将点的坐标单元转化为毫米
            BFy = (displaymodel->nodes[nodeNo]->cord[1]) * 1000;
            BFz = (displaymodel->nodes[nodeNo]->cord[2]) * 1000;
            //根据线模型结点的编号和截面上点的编号关系
            //更新实体模型
            QVector<int>* pointset = displaymodel->new_node_ex_node_info[nodeNo];
            int pnum = (*pointset).size();
            //qDebug()<<"nodeno"<<nodeNo;
            for (int k = 0; k < pnum; ++k)
            {
                int  nodeno = (*pointset)[k];
                double point[3] = { 0.0 };
                //qDebug()<<"real"<<nodeno;
                displaymodel->PointDisplaceSet[nodeno]->xdisplace = 0;
                displaymodel->PointDisplaceSet[nodeno]->ydisplace = 0;
                displaymodel->PointDisplaceSet[nodeno]->zdisplace = 0;

                displaymodel->scalars_dispx->InsertTuple1(nodeno-1,  0);
                displaymodel->scalars_dispy->InsertTuple1(nodeno-1,  0);
                displaymodel->scalars_dispz->InsertTuple1(nodeno-1,  0);

                point[0] = displaymodel->new_node_info1[nodeno]->x_cord+displaymodel->Displacefactor*displaymodel->PointDisplaceSet[nodeno]->xdisplace;//Displacefactor*
                point[1] = displaymodel->new_node_info1[nodeno]->y_cord+displaymodel->Displacefactor*displaymodel->PointDisplaceSet[nodeno]->ydisplace;//Displacefactor*
                point[2] = displaymodel->new_node_info1[nodeno]->z_cord+displaymodel->Displacefactor*displaymodel->PointDisplaceSet[nodeno]->zdisplace;//Displacefactor*
                displaymodel->S_M_points->InsertPoint(nodeno-1, point[0], point[1], point[2]);
            }
        }
    }
    displaymodel->S_M_points->Modified();
    displaymodel->DynamicFresh();
    ife->clear();
}

#include "Timer.h"
void disp3dMainWindow::handleTimeOut_WJBGL()
{
    double max_value = 0.0;
    double min_value = 0.0;

    // 从索引 5 开始，步长为 5，一直到 70
    for (int i = 2; i <= 70; i += 5) {
        //qDebug()<<i<<ife->node_u[i];
        double value = (ife->node_u[i]);
        if (value > max_value) {
            max_value = value;
        }
        if(value < min_value){
            min_value = value;
        }
    }
    ui->label_dispy_max->setStyleSheet("QLabel { font-weight: bold; color: green; }");
    ui->label_dispy_max->setText("主方向最大正变形："+QString::number(max_value*1000,'d',2));

    ui->label_dispy_min->setStyleSheet("QLabel { font-weight: bold; color: green; }");
    ui->label_dispy_min->setText("主方向最大负变形："+QString::number(min_value*1000,'d',2));

    displaymodel->DynamicFresh();
}

void disp3dMainWindow::on_comboBoxHz_currentIndexChanged(const QString &arg1)
{

    fps = arg1.left(arg1.indexOf('h')).toInt();
    if(timer_disp3d->isActive())
    {
        timer_disp3d->stop();
        timer_disp3d->start(1000/fps);
    }
    Config::getInstance().setModelFlushFrep(fps);
}

void disp3dMainWindow::on_btkDispAnalysis_clicked()
{
    if(displaymodel->ActorFlag == false)
    {
        QMessageBox::critical(this, "错误", "模型不存在");
        return ;
    }
    QString inputText1 = ui->editMinRange->text();
    bool isNumeric;
    float integerValue1 = inputText1.toFloat(&isNumeric);
    if(!isNumeric) {QMessageBox::critical(this, "错误", "输入参数错误"); return;};

    QString inputText2 = ui->editMaxRange->text();
    float integerValue2 = inputText2.toFloat(&isNumeric);
    if(!isNumeric) {QMessageBox::critical(this, "错误", "输入参数错误"); return;};


    if(calculationthread == nullptr)
    {
        calculationthread = new ModelCalculationThread(this);
        calculationthread->GetPtr(displaymodel,ife);
        calculationthread->pauseThread();
        calculationthread->start();
    }

    int index = ui->comboBoxDisp->currentIndex();
    displayModel::ScalarTable scalarIndex;
    switch (index) {
        case 0:
            scalarIndex = displayModel::dispx;
            break;
        case 1:
            scalarIndex = displayModel::dispy;
            break;
        case 2:
            scalarIndex = displayModel::dispz;
            break;
        default:
            qDebug() << "Invalid index!";
            return;
    }

   // 调用相应的函数
   displaymodel->SetScalarMapping(scalarIndex,integerValue1,integerValue2);
   Config::getInstance().setColorMode(index+1);
   Config::getInstance().setMinRange(integerValue1);
   Config::getInstance().setMaxRange(integerValue2);
}

void disp3dMainWindow::on_btkFilterCoeff_clicked()
{
    QString inputText = ui->editFilterCoeff->text();
    bool isNumeric;
    float integerValue = inputText.toFloat(&isNumeric);
    if(!isNumeric) {QMessageBox::critical(this, "错误", "输入参数错误"); return;};
    Config::getInstance().setFilterCoeff(integerValue);
}

void disp3dMainWindow::on_actionPointTable_triggered()
{
    PointTable* table = PointTable::instance(nullptr, ife);
    table->show();
}
