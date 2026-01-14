#include "demowindow.h"
#include "ui_demowindow.h"
//#include <QStandardItemModel>
#include<QIODevice>
#include<QTextStream>
#include<QDir>
#include<QDateTime>
#include<QFileDialog>
#include<QTableWidget>
#include "recedatethread.h"
#include"comthread.h"
#include "pipemonitorwindow.h"

#pragma execution_character_set("utf-8")

DemoWindow::DemoWindow(const QString& privilege,QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DemoWindow)
{


    ui->setupUi(this);

    this->privilege = privilege;

    initialize_widget();
    //LED灯测试程序
    setLED(ui->label_led,0,25);

    //创建套接字
     udpSocket=new QUdpSocket(this);
     thread=new ReceDateThread(this);
     thread->setObj(this);

     ui->textEdit_parameter_set->setText(QString::number(thread->A,'f', 2));
     ui->textEdit_parameter_set2->setText(QString::number(thread->B,'f', 2));
     //m_log = new LogWidget;
     //串口类
     comthread = new ComThread(this);
     on_btnrefresh_clicked();

     ui->cmbportbaud->setCurrentText("115200");

     ReadParaConfig("paraConfig.ini");

}

DemoWindow::~DemoWindow()
{
    if (udpSocket)
    {
        udpSocket->close();
        delete udpSocket;
        udpSocket = nullptr;
    }

    if (thread)
    {
        if (thread->isRunning()) {
            thread->requestInterruption();
            thread->wait();
        }
        delete thread;
        thread = nullptr;
    }

    if (comthread) {
        if (comthread->isRunning()) {
            comthread->requestInterruption();
            comthread->wait();
        }
        delete comthread;
        comthread = nullptr;
    }
    if(pipewindow)
    {
        delete pipewindow;
        pipewindow = nullptr;
    }

    delete timer;
    delete timer2;
    delete timer3;
    delete model;

    delete ui;
}

float* DemoWindow::SendStrainPtr()
{
    return thread->strain_float;
}


//打开串口
//void DemoWindow::on_btnopen_clicked()
//{
//     if(!comthread->InitSerial(ui->cmbportname->currentText()))
//     {
//         ui->plainTextEdit->setPlainText("串口打开失败!");
//         return;
//     }
//     ui->plainTextEdit->setPlainText("串口打开成功");
//     connect(comthread,SIGNAL(UpdateSerialData(QByteArray)),this,SLOT(dataReceive(QByteArray)));
//}

void DemoWindow::on_btnsend_clicked()
{
    QByteArray dataSend=ui->ptesend->toPlainText().toUtf8();
    //port->write(arr2);
    comthread->SendSerialData(dataSend);

}

//串口刷新
void DemoWindow::on_btnrefresh_clicked()
{
    //清空串口名
    ui->cmbportname->clear();
    //遍历串口信息
    foreach(const QSerialPortInfo &info,QSerialPortInfo::availablePorts())
    {
        ui->cmbportname->addItem(info.portName());
    }
}

void DemoWindow::on_btnclose_clicked()
{
    if(connect_Serialflag==0)
    {
        comthread->start();
        setLED(ui->label_led,3,25);
        ui->plainTextEdit->setPlainText("正在打开串口!");
        connect_Serialflag=1;


        if(!comthread->InitSerial(ui->cmbportname->currentText(),ui->cmbportbaud->currentText().toInt()))
        {
            ui->plainTextEdit->setPlainText("串口打开失败!");
            return;
        }
        ui->btnclose->setText("断开串口");
        ui->plainTextEdit->setPlainText("串口打开成功");
    }
    else
    {
        setLED(ui->label_led,0,25);
        connect_Serialflag=0;
        ui->btnclose->setText("打开串口");
        original_flag=0;//初始波长是否已保存，标志位

        ui->plainTextEdit->setPlainText("串口关闭成功");
        setLED(ui->label_led,0,25);
        comthread->CloseSerial();
    }


}

void DemoWindow::on_btnclear_clicked()
{
    ui->ptereceive->clear();
    comthread->strdata="";
}

void DemoWindow::initialize_widget()
{
    ui->tabWidget->setCurrentIndex(0);
    //QPalette   pal;
    //pal.setColor(QPalette::ButtonText,Qt::white);
    QStringList list;
    list<< "通道" << "波长" << "应变"<<"温度";
    model->setHorizontalHeaderLabels(list);
    model->setItem(9,2,new QStandardItem(""));
    ui->tableView->setModel(model);
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableView-> horizontalHeader()->setSectionResizeMode(0, QHeaderView::Fixed);

    ui->tableView->show();

    if (this->privilege=="3") {
        ui->comboBox_parameter_set->setDisabled(true);
        ui->textEdit_parameter_set->setDisabled(true);
        ui->textEdit_parameter_set2->setDisabled(true);
        ui->textEdit_parameter_set3->setDisabled(true);
        ui->textEdit_parameter_set4->setDisabled(true);
        ui->comboBox_3->setDisabled(true);
        ui->comboBox->setDisabled(true);


        }
    else if (this->privilege=="2") {
        ui->comboBox_parameter_set->setEnabled(true);
        ui->textEdit_parameter_set->setEnabled(true);
        ui->textEdit_parameter_set2->setEnabled(true);
        ui->textEdit_parameter_set3->setEnabled(true);
        ui->textEdit_parameter_set4->setEnabled(true);
        ui->comboBox_3->setEnabled(true);
        ui->comboBox->setEnabled(true);
    }
    else if (this->privilege=="1"){
        ui->comboBox_parameter_set->setEnabled(true);
        ui->textEdit_parameter_set->setEnabled(true);
        ui->textEdit_parameter_set2->setEnabled(true);
        ui->textEdit_parameter_set3->setEnabled(true);
        ui->textEdit_parameter_set4->setEnabled(true);
        ui->comboBox_3->setEnabled(true);
        ui->comboBox->setEnabled(true);
    }
    init_customplot(ui->widget);

}

void DemoWindow::init_customplot(QCustomPlot *customPlot)
{

    customPlot->setInteractions(QCP::iRangeDrag|QCP::iRangeZoom| QCP::iSelectAxes |
                                      QCP::iSelectLegend | QCP::iSelectPlottables);

    customPlot->yAxis->setRange(-10000, 40000);
    customPlot->xAxis->setRange(0, 4000);
    customPlot->axisRect()->setRangeDrag(Qt::Horizontal);
    customPlot->axisRect()->setRangeZoom(Qt::Horizontal);


        QPen pen1(Qt::blue); // 曲线1的笔（蓝色）
        pen1.setWidth(2);    // 设置笔宽度
        customPlot->addGraph();
        customPlot->graph(0)->setPen(pen1);

        QPen pen2(Qt::black); // 曲线2的笔（黑色）
        pen2.setWidth(2);     // 设置笔宽度
        customPlot->addGraph();
        customPlot->graph(1)->setPen(pen2);

        QPen pen3(Qt::yellow); // 曲线3的笔（黄色）
        pen3.setWidth(2);      // 设置笔宽度
        customPlot->addGraph();
        customPlot->graph(2)->setPen(pen3);

        QPen pen4(Qt::green); // 曲线4的笔（绿色）
        pen4.setWidth(2);     // 设置笔宽度
        customPlot->addGraph();
        customPlot->graph(3)->setPen(pen4);

        QPen pen5(Qt::gray); // 曲线5的笔（灰色）
        pen5.setWidth(2);    // 设置笔宽度
        customPlot->addGraph();
        customPlot->graph(4)->setPen(pen5);

        QPen pen6(Qt::magenta); // 曲线6的笔（洋红色）
        pen6.setWidth(2);       // 设置笔宽度
        customPlot->addGraph();
        customPlot->graph(5)->setPen(pen6);

        QPen pen7(Qt::cyan); // 曲线7的笔（青色）
        pen7.setWidth(2);    // 设置笔宽度
        customPlot->addGraph();
        customPlot->graph(6)->setPen(pen7);
}

void DemoWindow::on_Button_connect_clicked()
{

        if(connect_UDPflag==0)
        {
            setLED(ui->label_led,3,25);
            ui->plainTextEdit->setPlainText("正在连接!");
            connect_UDPflag=1;
            if(udpSocket->bind(5060)==true)
            {
               ui->Button_connect->setText("断开连接");
               thread->start();//启动子线程，接收网口数据

               if (timer->isActive())
               {
                   timer->stop();
               }
               timer->start(1000/ui->comboBox->currentText().toInt());

               connect(timer,SIGNAL(timeout()),this, SLOT(Updatatabel_slot()));

              setLED(ui->label_led,2,25);
              ui->plainTextEdit->setPlainText("连接成功!");
            }
            else
            {
                setLED(ui->label_led,1,25);
                ui->plainTextEdit->setPlainText("连接失败!");
            }
        }


        else
          {
              setLED(ui->label_led,0,25);
              ui->plainTextEdit->setPlainText("暂未连接!");
              connect_UDPflag=0;
               ui->Button_connect->setText("连接网络");
               original_flag=0;//初始波长是否已保存，标志位
               udpSocket->close();

          }


}

//串口数据接收
void DemoWindow::dataReceive()
{
    ui->ptereceive->insertPlainText(comthread->strdata);
    ui->ptereceive->moveCursor(QTextCursor::End);


}

void DemoWindow:: Updatatabel_slot()
 {
     //数据表格显示
     int CH1=thread->CH1;
     int CH2=thread->CH2;
     int CH3=thread->CH3;
     int CH4=thread->CH4;
     int CH5=thread->CH5;
     int CH6=thread->CH6;
     int CH7=thread->CH7;
     int CheckedCH_SUM=CH1_Flag*CH1+CH2_Flag*CH2+CH3_Flag*CH3+CH4_Flag*CH4+CH5_Flag*CH5+CH6_Flag*CH6+CH7_Flag*CH7;

     this->model->removeRows(0,model->rowCount());//删除所有行，重新显示
     model->setItem(9,2,new QStandardItem(""));

     if (connect_UDPflag==1) //该语句为了防止循环错乱
     {
         for(int i=0;i<CheckedCH_SUM;i++)
        {
             this->model->setItem(i,0,new QStandardItem (thread->FBG_count[i])) ;
             this->model->setItem(i,1,new QStandardItem (thread->wavelength[i]));

            if (strain_flag == 1)
            {
                      bool isValidStrain = true; // 默认认为数据有效

                      // 检查上下限条件
                      if ( thread->strain_float[i] < Strain_low) {
                          isValidStrain = false;
                      }
                      if ( thread->strain_float[i] > Strain_top) {
                          isValidStrain = false;
                      }

                      // 如果满足条件，设置第2列的数据
                      if (isValidStrain) {
                          model->setItem(i, 2, new QStandardItem(thread->strain[i]));
                      }
                      else {
                          model->setItem(i, 2, new QStandardItem("outRange"));
                      }
              }
          }

            ui->tableView->setModel(model);
            ui->tableView->show();
     }

     //绘图
     Show_Plot(ui->widget);

 }
void DemoWindow::Show_Plot(QCustomPlot *customPlot)//绘图
{

    QVector<QVector<double>>key;//存放绘图的xy点数组
    QVector<QVector<double>>value;
    int num=0;

    QVector<double> tmp_x(4000);
    for (int i = 0; i < 4000; ++i)
    {
        tmp_x[i] = i;
    }
    QVector<double> tmp_y(4000,0);

    for(int i=0;i<7;i++){
        key.push_back(tmp_x);
        value.push_back(tmp_y);
    }


   for(int k=0;k<7;k++){
       for(int i=0;i<thread->CH_point[k];i++)
       {
          value[k][thread->data_x_trans[k][i]]=thread->data_y_trans[k][i];
      }
   }


  if(CH1_Flag)
  {
      customPlot->graph(num)->setData(key[0],value[0]);
      customPlot->replot();
      num++;
  }
  else
  {
      customPlot->graph(num)->clearData();
      num++;
  }

  if(CH2_Flag)
  {
       customPlot->graph(num)->setData(key[1],value[1]);//给曲线添加数据
      // customPlot->graph(num)->rescaleAxes(true);

       num++;
       customPlot->replot();
  }
  else
  {
      customPlot->graph(num)->clearData();
      num++;
  }
  if(CH3_Flag)
  {
       customPlot->graph(num)->setData(key[2],value[2]);//给曲线添加数据
       num++;
       customPlot->replot();
  }
  else
  {
      customPlot->graph(num)->clearData();
      num++;
  }
  if(CH4_Flag)
  {
       customPlot->graph(num)->setData(key[3],value[3]);//给曲线添加数据
       //customPlot->graph(num)->rescaleAxes(true);
       num++;
       customPlot->replot();
  }
  else
  {
      customPlot->graph(num)->clearData();
      num++;
  }
  if(CH5_Flag)
  {
       customPlot->graph(num)->setData(key[4],value[4]);//给曲线添加数据
       //customPlot->graph(num)->rescaleAxes(true);

       num++;
       customPlot->replot();
  }
  else
  {
      customPlot->graph(num)->clearData();
      num++;
  }
  if(CH6_Flag)
  {
      customPlot->graph(num)->setData(key[5],value[5]);//给曲线添加数据

       num++;
        customPlot->replot();
  }
  else
  {
      customPlot->graph(num)->clearData();
      num++;
  }
  if(CH7_Flag)
  {

       customPlot->graph(num)->setData(key[6],value[6]);//给曲线添加数据

       num++;
       customPlot->replot();
  }
  else
  {
      customPlot->graph(num)->clearData();
      num++;
  }
}


//绘图保存图片
bool DemoWindow::on_Button_save_spectrum_clicked()
{

    QString fileName1 = QFileDialog::getSaveFileName(this, "Save Document...", qApp->applicationDirPath());

    if (fileName1.isEmpty())
    {
        QMessageBox::information(this, "Fail", "保存失败");
        return false;
    }

    QSize saveSize(ui->widget->width(), ui->widget->height());

    QPixmap pixmap = ui->widget->grab();
    pixmap = pixmap.scaled(saveSize);

    bool saved = pixmap.save(fileName1);
    if (saved) {
        QMessageBox::information(this, "Success", "成功保存图片");
    } else {
        QMessageBox::critical(this, "Error", "保存图片失败");
    }
    return saved;

}
void DemoWindow::on_Button_Save_dynamic_clicked()//自动保存
{
    save_dynamic_flag=!save_dynamic_flag;
    static QFile *save_dynamic_file = nullptr;
    static QMetaObject::Connection connection;
    if(save_dynamic_flag==1)
    {
        timer2->start(auto_save_time);
        ui->Button_Save_Static->setEnabled(false);
        QDateTime data_time=QDateTime::currentDateTime();
        QString time_str = data_time.toString("yyyy-MM-dd hh_mm_ss");
        QDir *DataFile=new QDir;
        bool exist=DataFile->exists("自动存储");
        if(!exist)
        {
            bool isok=DataFile->mkdir("自动存储");
            if(!isok)
            {
              QMessageBox::warning(this,"sdf","新建文件夹失败",QMessageBox::Yes) ;
            }
        }
        QString auto_save_time_tmp=ui->comboBox_3->currentText();
        QString time_ms_str=time_str+" "+auto_save_time_tmp+"ms ";
        save_dynamic_file=new QFile("自动存储/"+time_ms_str+"data.txt");
        save_dynamic_file->open(QIODevice::Text|QIODevice::WriteOnly|QIODevice::Append);
        ui->Button_Save_dynamic->setText("取消保存");
        connection = connect(timer2,&QTimer::timeout,[=](){
                //打开文件流
                QTextStream stream(save_dynamic_file);
                stream.seek(save_dynamic_file->size());
                if(!save_dynamic_file->size())
                {
                    stream << thread->fbg_txt_1 << "\n";
                }
                stream << thread->str_txt << "\n";
            });
    }
    if(save_dynamic_flag==0)
    {
         timer2->stop();
         ui->Button_Save_dynamic->setText("自动保存");
         ui->Button_Save_Static->setEnabled(true);
         if (save_dynamic_file!=nullptr)
         {
             if(save_dynamic_file->isOpen())
                 save_dynamic_file->close();

             delete save_dynamic_file;
             save_dynamic_file = nullptr;
         }
         disconnect(connection);
    }
}

void DemoWindow::on_Button_Save_Static_clicked()//手动保存
{
    ui->Button_Save_dynamic->setEnabled(false);

    QDateTime data_time=QDateTime::currentDateTime();
     QString strTime=data_time.toString("yyyy-MM-dd hh:mm:ss");
     QDir *DataFile=new QDir;
     bool exist=DataFile->exists("手动存储");
     if(!exist)
     {
         bool isok=DataFile->mkdir("手动存储");
         if(!isok)
         {
           QMessageBox::warning(this,"sdf","新建文件夹失败",QMessageBox::Yes) ;
         }
     }

     QFile *save_static_file=new QFile("手动存储/data.txt");
     save_static_file->open(QIODevice::Text|QIODevice::WriteOnly|QIODevice::Append);
     QTextStream stream(save_static_file);
     stream.seek(save_static_file->size());
        if(!save_static_file->size())
        {
            stream <<thread->fbg_txt_1+"\n";
        }
        QString static_txt=strTime+"\t"+thread->wavelength_static_txt+"\t"+thread->strain_static_txt;
        stream << static_txt<< "\n";
        save_static_file->close();
      ui->Button_Save_dynamic->setEnabled(true);

}



void DemoWindow::on_Button_reset_clicked()//复位
{
       reset_flag=1;
}





//参数设置确认按钮
void DemoWindow::on_Button_parameter_set_clicked()
{
    QString fre_tmp=ui->comboBox->currentText();
    int fre =fre_tmp.toInt();
    if (timer->isActive())
   {
       timer->stop();
       timer->start(1000/fre);
   }

    QString auto_save_time_txt=ui->comboBox_3->currentText();
    auto_save_time=auto_save_time_txt.toInt();
    if(timer2->isActive())
   {
       timer2->stop();
       timer2->start(auto_save_time);
   }
    //auto_save_time_txt=QString::number(auto_save_time);


    if(ui->comboBox_parameter_set->currentText()=="应变")
    {
        QString Set_A = ui->textEdit_parameter_set->toPlainText();
        float A=Set_A.toFloat();
        QString Set_B = ui->textEdit_parameter_set2->toPlainText();
        float B=Set_B.toFloat();
        set_strain_AB(A,B);
        Strain_low = ui->textEdit_parameter_set3->toPlainText().toFloat();
        Strain_top = ui->textEdit_parameter_set4->toPlainText().toFloat();
    }

    if(ui->comboBox_parameter_set->currentText()=="温度")
        {

    }

    if(ui->comboBox_parameter_set->currentText()=="位移")
        {

    }
    if(ui->comboBox_parameter_set->currentText()=="压力")
        {

    }
    if(ui->comboBox_parameter_set->currentText()=="液位")
        {

    }
    if(ui->comboBox_parameter_set->currentText()=="流量")
    {

    }
    WriteParaConfig("paraConfig.ini");
}

void DemoWindow::set_strain_AB(float  m_A,float m_B)
{
    thread->A=m_A;
    thread->B=m_B;
}

void DemoWindow::setLED(QLabel *label,int color, int size)
{
    // 将label中的文字清空
       label->setText("");
       // 先设置矩形大小
       // 如果ui界面设置的label大小比最小宽度和高度小，矩形将被设置为最小宽度和最小高度；
       // 如果ui界面设置的label大小比最小宽度和高度大，矩形将被设置为最大宽度和最大高度；
       QString min_width = QString("min-width: %1px;").arg(size);              // 最小宽度：size
       QString min_height = QString("min-height: %1px;").arg(size);            // 最小高度：size
       QString max_width = QString("max-width: %1px;").arg(size);              // 最小宽度：size
       QString max_height = QString("max-height: %1px;").arg(size);            // 最小高度：size
       // 再设置边界形状及边框
       QString border_radius = QString("border-radius: %1px;").arg(size/2);    // 边框是圆角，半径为size/2
       QString border = QString("border:1px solid black;");                    // 边框为1px黑色
       // 最后设置背景颜色
       QString background = "background-color:";
       switch (color) {
       case 0:
           // 灰色
           background += "rgb(190,190,190)";
           break;
       case 1:
           // 红色
           background += "rgb(255,0,0)";
           break;
       case 2:
           // 绿色
           background += "rgb(0,255,0)";
           break;
       case 3:
           // 黄色
           background += "rgb(255,255,0)";
           break;
       default:
           break;
       }

       const QString SheetStyle = min_width + min_height + max_width + max_height + border_radius + border + background;
       label->setStyleSheet(SheetStyle);
}




//绘图筛选显示和应变显示
void DemoWindow::on_Button_selectshow_clicked()
{
    if(ui->checkBox_1->isChecked())
    {
        CH1_Flag=1;
    }
    else {
         CH1_Flag=0;
    }
    if(ui->checkBox_2->isChecked())
    {
         CH2_Flag=1;
    }
    else {
         CH2_Flag=0;
    }
    if(ui->checkBox_3->isChecked())
    {
         CH3_Flag=1;
    }
    else {
        CH3_Flag=0;
    }
    if(ui->checkBox_4->isChecked())
    {
         CH4_Flag=1;
    }
    else {
         CH4_Flag=0;
    }
    if(ui->checkBox_5->isChecked())
    {
         CH5_Flag=1;
    }
    else {
        CH5_Flag=0;
    }
    if(ui->checkBox_6->isChecked())
    {
         CH6_Flag=1;
    }
    else {
         CH6_Flag=0;
    }
    if(ui->checkBox_7->isChecked())
    {
         CH7_Flag=1;
    }
    else {
         CH7_Flag=0;
    }

    if(ui->comboBox_parameter_set->currentText()=="应变")
    {
        strain_flag=1;
    }
    else
    {
        strain_flag=0;
    }

    this->model->removeRows(0,model->rowCount());//删除所有行，重新显示
    model->setItem(9,2,new QStandardItem(""));
    ui->tableView->setModel(model);
    ui->tableView->show();
}


void DemoWindow::on_btn_diagnosis_clicked()
{
    if(!pipewindow)
    {
        pipewindow = new PipeMonitorWindow();
        pipewindow->GetReceDateThreadPtr(thread);
    }
    pipewindow->show();
}

void DemoWindow::ReadParaConfig(const QString& fileName)
{
    QSettings settings(fileName, QSettings::IniFormat);
    // 读取并设置 QLineEdit 的值
    ui->textEdit_parameter_set->setText(settings.value("A", "").toString());
    ui->textEdit_parameter_set2->setText(settings.value("B", "").toString());
    ui->textEdit_parameter_set3->setText(settings.value("Strain_low", "").toString());
    ui->textEdit_parameter_set4->setText(settings.value("Strain_top", "").toString());

    // 读取并设置 QComboBox 的值
    ui->comboBox->setCurrentText(settings.value("getHz", "").toString());
    ui->comboBox_3->setCurrentText(settings.value("saveTime", "").toString());
    on_Button_parameter_set_clicked();
}
void DemoWindow::WriteParaConfig(const QString& fileName)
{
    QSettings settings(fileName, QSettings::IniFormat);
    // 从 QLineEdit 获取值并写入配置
    settings.setValue("A", ui->textEdit_parameter_set->toPlainText());
    settings.setValue("B", ui->textEdit_parameter_set2->toPlainText());
    settings.setValue("Strain_low", ui->textEdit_parameter_set3->toPlainText());
    settings.setValue("Strain_top", ui->textEdit_parameter_set4->toPlainText());

    // 从 QComboBox 获取值并写入配置
    settings.setValue("getHz", ui->comboBox->currentText());
    settings.setValue("saveTime", ui->comboBox_3->currentText());
}



