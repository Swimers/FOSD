#include "disp2dwindow.h"
#include "ui_disp2dwindow.h"
#include <QDebug>
#include <QFile>
#include <QFileDialog>
#pragma execution_character_set("utf-8")



Disp2DWindow::Disp2DWindow(const QString& privilege,QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Disp2DWindow)
{
    ui->setupUi(this);

    initialize_widget();
    ife = new IFE();
    ife->modifyOriginalCoordinates("Data/Input/OriginalCoordiante.csv");
    //qDebug()<<IFE::positionOriginal[0][0];
    LoadCoordinates(ife->positionOriginal,POSITION_ORIGINAL);

}

Disp2DWindow::~Disp2DWindow()
{
    if(this->TwoD_model)   delete TwoD_model;
    if(this->timer)   delete timer;


    if(parawindow)
    {
        delete parawindow;
        parawindow = nullptr;
    }

    delete ui;
}

/*
 * 功能:初始化表头，并且将所有数据置0
 */
void Disp2DWindow::initialize_widget()
{
    QStringList list_two;
    list_two<< "编号" <<"X初始坐标"<<"Y初始坐标"<<"Z初始坐标"<< "X变形值" << "Y变形值"<<"Z变形值"<<"X坐标"<<"Y坐标"<<"Z坐标";
    TwoD_model->setHorizontalHeaderLabels(list_two);
    TwoD_model->setItem(MARKPOINTNUM-1,9,new QStandardItem(""));
    //TwoD_model->setRowCount(30);
    ui->TwoD_tableView->setModel(TwoD_model);
    ui->TwoD_tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);//所有列都扩展自适应宽度，填充充满整个屏幕宽度
    ui->TwoD_tableView->verticalHeader()->setVisible(false);
    //初始化表格，除第一列外全部初始化为0
    for (int i = 0; i < MARKPOINTNUM; ++i)
    {
        for (int j = 0; j < 10; ++j)
        {
            if(j==0)  //第一列为编号
            {
                QStandardItem *item = new QStandardItem(QString::number(i+1));
                item->setTextAlignment(Qt::AlignCenter);
                item->setForeground(QBrush(QColor(0, 0, 0)));
                item->setFont(QFont( "Times New Roman", 10, QFont::Bold) );
                TwoD_model->setItem(i,j,item);
            }
            else
            {
                QStandardItem *item = new QStandardItem(QString::number(0));
                item->setTextAlignment(Qt::AlignCenter);
                item->setForeground(QBrush(QColor(0, 0, 0)));
                item->setFont(QFont( "Times New Roman", 10, QFont::Normal) );
                TwoD_model->setItem(i,j,item);
            }
        }
    }
    ui->TwoD_tableView->show();

    connect(timer,SIGNAL(timeout()),this, SLOT(Updatatabel_slot())); //按照定时器设置频率显示
}


/*
 * 功能:将数据显示在表格中
 * 参数：float (&p)[MARKPOINTNUM][3] ----二维数组
 *      columntype num ----POSITION_ORIGINAL（2~4列）,POSITION_DISP（5~7列）,POSITION_FINAL（8~10列）
 */
void Disp2DWindow::LoadCoordinates(float (&p)[MARKPOINTNUM][3],const columntype num)
{
    for(int i=0;i<MARKPOINTNUM;i++)
    {
        for(int j=0;j<3;j++)
        {
            QStandardItem* item = TwoD_model->item(i,3*num+j+1);        //获取model的item指针
            item->setData(QString::number(p[i][j]),Qt::DisplayRole);    //修改数据
        }
    }
}

/*
 * 功能:选择文件，改变初始坐标
 */
void Disp2DWindow::on_Button_ChangeOriginalCoordinates_clicked()
{
    QString path=QFileDialog::getOpenFileName(this,"open","./Data/Input","CSV(*.csv)");
    if(path.isEmpty())
    {
        qDebug()<<"未选中文件";
    }
    else
    {
        QString relativePath = QDir("./").relativeFilePath(path);
        ife->modifyOriginalCoordinates(relativePath);       //得到相对路径
        LoadCoordinates(ife->positionOriginal,POSITION_ORIGINAL);
    }
}

void Disp2DWindow::on_Button_ChangeBackGround_clicked()
{
    QString s = QFileDialog::getOpenFileName(this,"选择文件","./resource/resource_disp2d", "Files(*.jpg *.png)");
    if(s.isEmpty())
    {
        qDebug()<<"未选中图片";
    }
    else
    {
        QImage image;
        image.load(s);
        ui->TwoD_Label_picture->setPixmap(QPixmap::fromImage(image).scaled(ui->TwoD_Label_picture->size()));
    }
}

void Disp2DWindow::on_Button_Observe_clicked()
{
    if(observe_flag == 0)   //按下即代表启动监控
    {
        ui->Button_Observe->setText("停止监控");
        timer->start(500);//数据刷新频率

    }
    else    //按下即代表停止监控
    {
        ui->Button_Observe->setText("启动监控");
        timer->stop();
    }
    observe_flag = !observe_flag;
}

void Disp2DWindow::Updatatabel_slot()
{
    ife->calculateDisp();
    LoadCoordinates(ife->positionDisp,POSITION_DISP);
    LoadCoordinates(ife->positionFinal,POSITION_FINAL);
}

void Disp2DWindow::on_Button_ParaRevise_clicked()
{
    if(!parawindow)
    {
        parawindow = new ParaReviseWindow();
        parawindow->GetIFEPtr(ife);
    }
    parawindow->init();
    parawindow->show();
}
