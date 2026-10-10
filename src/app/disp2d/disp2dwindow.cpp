#include "disp2dwindow.h"
#include "ui_disp2dwindow.h"

#include <QDebug>
#include <QFile>
#include <QFileDialog>

disp2dwindow::disp2dwindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::disp2dwindow)
{
    ui->setupUi(this);
    InitWindow();
    ife = new IFE();
    qDebug()<< "disp2d Creat IFE" << ife;
    ife->modifyOriginalCoordinates("data/input/OriginalCoordiante.csv");
    //qDebug()<<IFE::positionOriginal[0][0];
    LoadCoordinates(ife->positionOriginal,POSITION_ORIGINAL);
}

disp2dwindow::~disp2dwindow()
{
    if(this->TwoD_model)   delete TwoD_model;
    if(this->timer)   delete timer;
    if(m_disp2dpara){
        delete m_disp2dpara;
        m_disp2dpara = nullptr;
    }
    delete ui;
}

void disp2dwindow::InitWindow()
{
    QStringList list_two;
    list_two<< "编号" << "X初始坐标" <<"Y初始坐标" << "Z初始坐标" << "X变形值" << "Y变形值" << "Z变形值"<< "X坐标" << "Y坐标" << "Z坐标 ";
    TwoD_model->setHorizontalHeaderLabels(list_two);
    TwoD_model->setItem(MARKPOINTNUM-1,9,new QStandardItem(""));
    ui->table_Disp2DPara->setModel(TwoD_model);
    ui->table_Disp2DPara->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);   //所有列都扩展自适应宽度，填充充满整个屏幕宽度
    ui->table_Disp2DPara->verticalHeader()->setVisible(false);
    //初始化表格，除第一列外全部初始化为0
    for (int i = 0; i < MARKPOINTNUM; ++i){
        for (int j = 0; j < 10; ++j){
            if(j==0){  //第一列为编号
                QStandardItem *item = new QStandardItem(QString::number(i+1));
                item->setTextAlignment(Qt::AlignCenter);
                item->setForeground(QBrush(QColor(0, 0, 0)));
                item->setFont(QFont( "Times New Roman", 10, QFont::Bold) );
                TwoD_model->setItem(i,j,item);
            }
            else{
                QStandardItem *item = new QStandardItem(QString::number(0));
                item->setTextAlignment(Qt::AlignCenter);
                item->setForeground(QBrush(QColor(0, 0, 0)));
                item->setFont(QFont( "Times New Roman", 10, QFont::Normal) );
                TwoD_model->setItem(i,j,item);
            }
        }
    }
    ui->table_Disp2DPara->show();
    connect(timer,SIGNAL(timeout()),this, SLOT(UpdatatabelSlot())); //按照定时器设置频率显示
}

void disp2dwindow::LoadCoordinates(float (&p)[MARKPOINTNUM][3], const disp2dwindow::columntype num)
{
    for(int i=0;i<MARKPOINTNUM;i++){
        for(int j=0;j<3;j++){
            QStandardItem* item = TwoD_model->item(i,3 * num+j+1);        //获取model的item指针
            item->setData(QString::number(p[i][j]),Qt::DisplayRole);    //修改数据
            //qDebug() << p[i][j];
        }
    }
}

void disp2dwindow::UpdatatabelSlot()
{
    ife->calculateDisp();
    LoadCoordinates(ife->positionDisp,POSITION_DISP);
    LoadCoordinates(ife->positionFinal,POSITION_FINAL);
}

void disp2dwindow::on_pbtn_ChangeOriginalCoordinates_clicked()
{
    QString path=QFileDialog::getOpenFileName(this,"open","./data/input","CSV(*.csv)");
    if(path.isEmpty()){
        qDebug() << "未选中文件";
    }
    else{
        QString relativePath = QDir("./").relativeFilePath(path);
        ife->modifyOriginalCoordinates(relativePath);       //得到相对路径
        LoadCoordinates(ife->positionOriginal,POSITION_ORIGINAL);
    }
}

void disp2dwindow::on_pbtn_ParaRevise_clicked()
{
    if(!m_disp2dpara)
    {
        m_disp2dpara = new disp2dpara();
        m_disp2dpara->GetIFEPtr(ife);
    }
    m_disp2dpara->init();
    m_disp2dpara->show();

    connect(m_disp2dpara, &disp2dpara::DataSeted, this, &disp2dwindow::FreshData);
}

void disp2dwindow::on_pbtn_ChangeBackGround_clicked()
{
    QString s = QFileDialog::getOpenFileName(this,"选择文件","./Resource/backgrounds", "Files(*.jpg *.png)");
    if(s.isEmpty()){
        qDebug()<<"未选中图片";
    }
    else{
        QImage image;
        image.load(s);
        ui->lable_Picture->setPixmap(QPixmap::fromImage(image).scaled(ui->lable_Picture->size()));
    }
}

void disp2dwindow::on_pbtn_Observe_clicked()
{
    if(Flag_Observe == 0){                      //按下即代表启动监控
        ui->pbtn_Observe->setText("停止监控");
        timer->start(500);                      //数据刷新频率
        qDebug() << "Start Observr";
    }
    else{                                       //按下即代表停止监控
        ui->pbtn_Observe->setText("启动监控");
        timer->stop();
    }
    Flag_Observe = !Flag_Observe;
}

void disp2dwindow::FreshData()
{
    ife->modifyOriginalCoordinates("data/input/OriginalCoordiante.csv");
    //qDebug()<<IFE::positionOriginal[0][0];
    LoadCoordinates(ife->positionOriginal,POSITION_ORIGINAL);
}
