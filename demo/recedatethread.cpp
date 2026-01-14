#include "recedatethread.h"
#include"mainwindow.h"
#include<QFile>
#include<QDateTime>
#include<QTimer>
#include<QMessageBox>
#include<QIODevice>

#pragma execution_character_set("utf-8")

ReceDateThread::ReceDateThread(QObject *parent)
    :QThread(parent)
{
    //读取初始波长

    QFile *file_txt=new QFile("./初始波长.txt");//声明QFile类指针

    file_txt->open(QIODevice::ReadOnly|QIODevice::Text);
    //读取文件
    QTextStream TextRead(file_txt);
    QString data_line;
    int wave_count=0;
    while(!TextRead.atEnd())
    {
        data_line=TextRead.readLine();
        original_wavelength[wave_count]= data_line.toFloat();
        wave_count++;
    }
     file_txt->close();

}
ReceDateThread::~ReceDateThread()
{

}

void ReceDateThread::setObj(DemoWindow* obj)
{
    guimain=obj;
}

void ReceDateThread::run()
{
   msleep(1);
   array.resize(guimain->udpSocket->pendingDatagramSize());//重置数组大小，使其与接收到的数据大小一致
          //读取数据0
   guimain->udpSocket->readDatagram(array.data(),array.size());
   connect(guimain->udpSocket, SIGNAL(readyRead()),this, SLOT(readyRead_slot()));

}

#include "Timer.h"
void ReceDateThread::readyRead_slot()//数据处理
{
    QDateTime dateTime=QDateTime::currentDateTime();//获取当前系统时间
    QString strTime=dateTime.toString("yyyy-MM-dd hh:mm:ss");


    while (guimain->udpSocket->hasPendingDatagrams()) //hasPendingDatagrams()若没有读完则返回true
    {
        //Timer time;

        array.resize(guimain->udpSocket->pendingDatagramSize());//重置数组大小，使其与接收到的数据大小一致
        //读取数据
        guimain->udpSocket->readDatagram(array.data(),array.size());

         FLOATA data_count_xy;
         FLOATA data_count_x;

        int pack_size=array.size();



        if(static_cast<uint8_t>(array.data()[0])==0xFA &&static_cast<uint8_t>(array.data()[1])==0xFB
                &&static_cast<uint8_t>(array.data()[2])==0xFA &&static_cast<uint8_t>(array.data()[3])==0xFB)//判断标志位
         {

            for(int j=0;j<1024;j++)
            {
                FBG_count[j]="";
                newStrain[j]=0;
                wavelength[j]="";
                strain[j]="";

            }
              //data_xy.clear();//容器不清空就会自动初始化两个字节
              CH1=array.data()[4];//通道1传感器数目
              CH2=array.data()[5];
              CH3=array.data()[6];
              CH4=array.data()[7];
              CH5=array.data()[8];
              CH6=array.data()[9];
              CH7=array.data()[10];
              CH_SUM=array.data()[11];//总传感器数目



              //实际显示的传感器数目
              int Checked_CH_Count = 0;
              //int sum_point_leijia = 0;


              //每个通道xy数据点个数（13~28）
              for(int i=0;i<7;i++)
              {
                  data_count_xy.u8Byte[0]=array.data()[i*2+12];
                  data_count_xy.u8Byte[1]=array.data()[i*2+13];
                  data_count_xy.u8Byte[2]=0;
                  data_count_xy.u8Byte[3]=0;
                  CH_point[i]=data_count_xy.all;
              }

              //暂未修改
              if(guimain->reset_flag==1)
              {
                  count_num++;
                  for(int i=0;i<CH_SUM;i++)
                  {
                      FLOATA2 original_datagram;
                      original_datagram.u8Byte[0]=array.data()[i*4+28];
                      original_datagram.u8Byte[1]=array.data()[i*4+29];
                      original_datagram.u8Byte[2]=array.data()[i*4+30];
                      original_datagram.u8Byte[3]=array.data()[i*4+31];
                      original_wavelength_temp[i]=original_wavelength_temp[i]+original_datagram.all;
                      //qDebug()<<original_datagram.all;
                  }
                  if(count_num==20)
                  {
                      QFile *file_txt=new QFile("./初始波长.txt");//声明QFile类指针
                      file_txt->open(QIODevice::WriteOnly|QIODevice::Text);
                      QTextStream TextWrite(file_txt);

                      for(int i=0;i<CH_SUM;i++)
                      {
                          original_wavelength[i]=original_wavelength_temp[i]/20;
                          original_wavelength_temp[i]=0;//初始波长过渡数组清零，为下一次复位键按下做准备
                          QString data_line;
                          TextWrite<<original_wavelength[i]<<endl;//将初始波长逐行写入TXT文件
                      }
                      file_txt->close();
                      guimain->reset_flag=0;//标志位设为0，在下一次复位键按下之前不再改变初始波长的值
                      count_num=0;

                  }
              }

              for(int i=0;i<CH_SUM;i++)
              {
                  FLOATA2 datagram;
                  datagram.u8Byte[0]=array.data()[i*4+28];
                  datagram.u8Byte[1]=array.data()[i*4+29];
                  datagram.u8Byte[2]=array.data()[i*4+30];
                  datagram.u8Byte[3]=array.data()[i*4+31];

                  if(i<CH1&&CH1!=0)
                  {
                      if(guimain->CH1_Flag)
                      {
                          int num1=Checked_CH_Count;//2通道的传感器编号
                          FBG_count[Checked_CH_Count]=QString("CH1_%1").arg(num1+1);
                          CH_datagram[Checked_CH_Count]=datagram.all;
                          wavelength[Checked_CH_Count]=QString::number(datagram.all,'f',5);//波长
                          //newStrain[Checked_CH_Count]=(datagram.all-original_wavelength[i])*A+B;//浮点型应变
                          newStrain[Checked_CH_Count]=(0.00075*(datagram.all-original_wavelength[i]))/A+B;

                          strain[Checked_CH_Count]=QString::number(newStrain[Checked_CH_Count],'f',5);//应变


                          //动态保存至TXT文本
                          fbg_txt=fbg_txt+"\t"+FBG_count[Checked_CH_Count];
                          strain_txt=strain_txt+"\t"+strain[Checked_CH_Count];
                          wavelength_txt=wavelength_txt+"\t"+wavelength[Checked_CH_Count];
                          if(i==CH_SUM-1)
                          {
                              fbg_txt_1="time\t"+fbg_txt+"\t"+fbg_txt;
                              strain_txt_1=strain_txt;
                              wavelength_txt_1=strTime+"\t"+wavelength_txt;
                              str_txt=wavelength_txt_1+"\t"+strain_txt_1+"\n";
                              fbg_txt="";
                              strain_txt="";
                              wavelength_txt="";
                          }

                          Checked_CH_Count++;
                      }

                  }
                  if((CH1<=i)&&(i<CH1+CH2)&&CH2!=0)
                  {
                      if(guimain->CH2_Flag)
                      {
                          int num2=Checked_CH_Count-(guimain->CH1_Flag)*CH1;//2通道的传感器编号
                          FBG_count[Checked_CH_Count]=QString("CH2_%1").arg(num2+1);
                          CH_datagram[Checked_CH_Count]=datagram.all;
                          wavelength[Checked_CH_Count]=QString::number(datagram.all,'f',5);//波长
                          newStrain[Checked_CH_Count]=(0.00075*(datagram.all-original_wavelength[i]))/A+B;
                          strain[Checked_CH_Count]=QString::number(newStrain[Checked_CH_Count],'f',5);//应变


                          //动态保存至TXT文本
                          fbg_txt=fbg_txt+"\t"+FBG_count[Checked_CH_Count];
                          strain_txt=strain_txt+"\t"+strain[Checked_CH_Count];
                          wavelength_txt=wavelength_txt+"\t"+wavelength[Checked_CH_Count];
                          if(i==CH_SUM-1)
                          {
                              fbg_txt_1="time\t"+fbg_txt+"\t"+fbg_txt;
                              strain_txt_1=strain_txt;
                              wavelength_txt_1=strTime+"\t"+wavelength_txt;
                              str_txt=wavelength_txt_1+"\t"+strain_txt_1+"\n";
                              fbg_txt="";
                              strain_txt="";
                              wavelength_txt="";
                          }


                          Checked_CH_Count++;
                      }
                  }
                  if((CH1+CH2<=i&&i<CH1+CH2+CH3)&&CH3!=0)
                  {
                      if(guimain->CH3_Flag)
                      {
                          int num3=Checked_CH_Count-(guimain->CH1_Flag)*CH1-(guimain->CH2_Flag)*CH2;//3通道的传感器编号
                          FBG_count[Checked_CH_Count]=QString("CH3_%1").arg(num3+1);
                          CH_datagram[Checked_CH_Count]=datagram.all;
                          wavelength[Checked_CH_Count]=QString::number(datagram.all,'f',5);//波长
                          newStrain[Checked_CH_Count]=(0.00075*(datagram.all-original_wavelength[i]))/A+B;
                          strain[Checked_CH_Count]=QString::number(newStrain[Checked_CH_Count],'f',5);//应变


                          //动态保存至TXT文本
                          fbg_txt=fbg_txt+"\t"+FBG_count[Checked_CH_Count];
                          strain_txt=strain_txt+"\t"+strain[Checked_CH_Count];
                          wavelength_txt=wavelength_txt+"\t"+wavelength[Checked_CH_Count];
                          if(i==CH_SUM-1)
                          {
                              fbg_txt_1="time\t"+fbg_txt+"\t"+fbg_txt;
                              strain_txt_1=strain_txt;
                              wavelength_txt_1=strTime+"\t"+wavelength_txt;
                              str_txt=wavelength_txt_1+"\t"+strain_txt_1+"\n";
                              fbg_txt="";
                              strain_txt="";
                              wavelength_txt="";
                          }

                          Checked_CH_Count++;
                      }
                  }
                  if((CH1+CH2+CH3<=i&&i<CH1+CH2+CH3+CH4)&&CH4!=0)
                  {
                      if(guimain->CH4_Flag)
                      {
                          int num4=Checked_CH_Count-(guimain->CH1_Flag)*CH1-(guimain->CH2_Flag)*CH2-(guimain->CH3_Flag)*CH3;//4通道的传感器编号
                          FBG_count[Checked_CH_Count]=QString("CH4_%1").arg(num4+1);
                          CH_datagram[Checked_CH_Count]=datagram.all;
                          wavelength[Checked_CH_Count]=QString::number(datagram.all,'f',5);//波长
                          //newStrain[Checked_CH_Count]=(datagram.all-original_wavelength[i])*A+B;//浮点型应变
                          newStrain[Checked_CH_Count]=(0.00075*(datagram.all-original_wavelength[i]))/A+B;
                          strain[Checked_CH_Count]=QString::number(newStrain[Checked_CH_Count],'f',5);//应变

                          //动态保存至TXT文本
                          fbg_txt=fbg_txt+"\t"+FBG_count[Checked_CH_Count];
                          strain_txt=strain_txt+"\t"+strain[Checked_CH_Count];
                          wavelength_txt=wavelength_txt+"\t"+wavelength[Checked_CH_Count];
                          if(i==CH_SUM-1)
                          {
                              fbg_txt_1="time\t"+fbg_txt+"\t"+fbg_txt;
                              strain_txt_1=strain_txt;
                              wavelength_txt_1=strTime+"\t"+wavelength_txt;
                              str_txt=wavelength_txt_1+"\t"+strain_txt_1+"\n";
                              fbg_txt="";
                              strain_txt="";
                              wavelength_txt="";
                          }

                          Checked_CH_Count++;
                      }
                  }
                  if((CH1+CH2+CH3+CH4<=i&&i<CH1+CH2+CH3+CH4+CH5)&&CH5!=0)
                  {
                      if(guimain->CH5_Flag)
                      {
                          int num5=Checked_CH_Count-(guimain->CH1_Flag)*CH1-(guimain->CH2_Flag)*CH2-(guimain->CH3_Flag)*CH3-(guimain->CH4_Flag)*CH4;//5通道的传感器编号
                          FBG_count[Checked_CH_Count]=QString("CH5_%1").arg(num5+1);
                          CH_datagram[Checked_CH_Count]=datagram.all;
                          wavelength[Checked_CH_Count]=QString::number(datagram.all,'f',5);//波长
                          newStrain[Checked_CH_Count]=(0.00075*(datagram.all-original_wavelength[i]))/A+B;
                          strain[Checked_CH_Count]=QString::number(newStrain[Checked_CH_Count],'f',5);//应变


                          //动态保存至TXT文本
                          fbg_txt=fbg_txt+"\t"+FBG_count[Checked_CH_Count];
                          strain_txt=strain_txt+"\t"+strain[Checked_CH_Count];
                          wavelength_txt=wavelength_txt+"\t"+wavelength[Checked_CH_Count];
                          if(i==CH_SUM-1)
                          {
                              fbg_txt_1="time\t"+fbg_txt+"\t"+fbg_txt;
                              strain_txt_1=strain_txt;
                              wavelength_txt_1=strTime+"\t"+wavelength_txt;
                              str_txt=wavelength_txt_1+"\t"+strain_txt_1+"\n";
                              fbg_txt="";
                              strain_txt="";
                              wavelength_txt="";
                          }


                          Checked_CH_Count++;
                      }
                  }
                  if((CH1+CH2+CH3+CH4+CH5<=i&&i<CH1+CH2+CH3+CH4+CH5+CH6)&&CH6!=0)
                  {
                      if(guimain->CH6_Flag)
                      {
                          int num6=Checked_CH_Count-(guimain->CH1_Flag)*CH1-(guimain->CH2_Flag)*CH2-(guimain->CH3_Flag)*CH3-(guimain->CH4_Flag)*CH4-(guimain->CH5_Flag)*CH5;//6通道的传感器编号
                          FBG_count[Checked_CH_Count]=QString("CH6_%1").arg(num6+1);
                          CH_datagram[Checked_CH_Count]=datagram.all;
                          wavelength[Checked_CH_Count]=QString::number(datagram.all,'f',5);//波长
                          newStrain[Checked_CH_Count]=(0.00075*(datagram.all-original_wavelength[i]))/A+B;
                          strain[Checked_CH_Count]=QString::number(newStrain[Checked_CH_Count],'f',5);//应变

                          //动态保存至TXT文本
                          fbg_txt=fbg_txt+"\t"+FBG_count[Checked_CH_Count];
                          strain_txt=strain_txt+"\t"+strain[Checked_CH_Count];
                          wavelength_txt=wavelength_txt+"\t"+wavelength[Checked_CH_Count];
                          if(i==CH_SUM-1)
                          {
                              fbg_txt_1="time\t"+fbg_txt+"\t"+fbg_txt;
                              strain_txt_1=strain_txt;
                              wavelength_txt_1=strTime+"\t"+wavelength_txt;
                              str_txt=wavelength_txt_1+"\t"+strain_txt_1+"\n";
                              fbg_txt="";
                              strain_txt="";
                              wavelength_txt="";
                          }


                          Checked_CH_Count++;
                      }
                  }
                  if((CH1+CH2+CH3+CH4+CH5+CH6<=i&&i<CH1+CH2+CH3+CH4+CH5+CH6+CH7)&&CH7!=0)
                  {
                      if(guimain->CH7_Flag)
                      {
                          int num7=Checked_CH_Count-(guimain->CH1_Flag)*CH1-(guimain->CH2_Flag)*CH2-(guimain->CH3_Flag)*CH3-(guimain->CH4_Flag)*CH4-(guimain->CH5_Flag)*CH5-(guimain->CH6_Flag)*CH6;//6通道的传感器编号
                           FBG_count[Checked_CH_Count]=QString("CH7_%1").arg(num7+1);
                           CH_datagram[Checked_CH_Count]=datagram.all;
                           wavelength[Checked_CH_Count]=QString::number(datagram.all,'f',5);//波长
                           newStrain[Checked_CH_Count]=(0.00075*(datagram.all-original_wavelength[i]))/A+B;
                           strain[Checked_CH_Count]=QString::number(newStrain[Checked_CH_Count],'f',5);//应变

                           //动态保存至TXT文本
                           fbg_txt=fbg_txt+"\t"+FBG_count[Checked_CH_Count];
                           strain_txt=strain_txt+"\t"+strain[Checked_CH_Count];
                           wavelength_txt=wavelength_txt+"\t"+wavelength[Checked_CH_Count];
                           if(i==CH_SUM-1)
                           {
                               fbg_txt_1="time\t"+fbg_txt+"\t"+fbg_txt;
                               strain_txt_1=strain_txt;
                               wavelength_txt_1=strTime+"\t"+wavelength_txt;
                               str_txt=wavelength_txt_1+"\t"+strain_txt_1+"\n";
                               fbg_txt="";
                               strain_txt="";
                               wavelength_txt="";
                           }


                           Checked_CH_Count++;
                      }
                  }

              }//for循环结束


                  //手动保存
                  for (int i=0;i<Checked_CH_Count;i++)
                  {


                       //修改
                       wavelength_static[i]=QString::number(CH_datagram[i],'f',5);
                       strain_static[i]=QString::number(newStrain[i],'f',5);


                       wavelength_static_txt_temp=wavelength_static_txt_temp+"\t"+wavelength_static[i];
                       strain_static_txt_temp=strain_static_txt_temp+"\t"+strain_static[i];
                       if(i==Checked_CH_Count-1)
                       {
                           wavelength_static_txt=wavelength_static_txt_temp;
                           wavelength_static_txt_temp="";
                           strain_static_txt=strain_static_txt_temp;
                           strain_static_txt_temp="";

                       }
                   }
                  updateStrain();

        }//首个包接收完
        else if(static_cast<uint8_t>(array.data()[0])==0xFA &&static_cast<uint8_t>(array.data()[1])==0x00
                &&static_cast<uint8_t>(array.data()[2])==0xFA &&static_cast<uint8_t>(array.data()[3])==0x00)
        {

            FLOATA data_count_x;
            FLOATA data_count_xy;

            //(7~8)通道1总点数
            data_count_xy.u8Byte[0]=array.data()[6];
            data_count_xy.u8Byte[1]=array.data()[7];
            data_count_xy.u8Byte[2]=0;
            data_count_xy.u8Byte[3]=0;
            CH_point[0]=data_count_xy.all;

            for(int j=0;j<CH_point[0];j++)  //(9~708)
            {
                data_count_x.u8Byte[0]=array.data()[2*j+8];
                data_count_x.u8Byte[1]=array.data()[2*j+9];
                data_count_x.u8Byte[2]=0;
                data_count_x.u8Byte[3]=0;

                data_x_trans[0][j]=data_count_x.all;
            }

            for(int j=0;j<CH_point[0];j++)  //(709~1408)
            {
                data_count_x.u8Byte[0]=array.data()[2*j+708];
                data_count_x.u8Byte[1]=array.data()[2*j+709];
                data_count_x.u8Byte[2]=0;
                data_count_x.u8Byte[3]=0;

                data_y_trans[0][j]=data_count_x.all;
            }
        }
        else if(static_cast<uint8_t>(array.data()[0])==0xFA &&static_cast<uint8_t>(array.data()[1])==0x01
                &&static_cast<uint8_t>(array.data()[2])==0xFA &&static_cast<uint8_t>(array.data()[3])==0x01)
        {

            FLOATA data_count_x;
            FLOATA data_count_xy;

            //(7~8)通道2总点数
            data_count_xy.u8Byte[0]=array.data()[6];
            data_count_xy.u8Byte[1]=array.data()[7];
            data_count_xy.u8Byte[2]=0;
            data_count_xy.u8Byte[3]=0;
            CH_point[1]=data_count_xy.all;

            for(int j=0;j<CH_point[1];j++)  //(9~708)
            {
                data_count_x.u8Byte[0]=array.data()[2*j+8];
                data_count_x.u8Byte[1]=array.data()[2*j+9];
                data_count_x.u8Byte[2]=0;
                data_count_x.u8Byte[3]=0;

                data_x_trans[1][j]=data_count_x.all;
            }

            for(int j=0;j<CH_point[1];j++)  //(709~1408)
            {
                data_count_x.u8Byte[0]=array.data()[2*j+708];
                data_count_x.u8Byte[1]=array.data()[2*j+709];
                data_count_x.u8Byte[2]=0;
                data_count_x.u8Byte[3]=0;

                data_y_trans[1][j]=data_count_x.all;
            }

        }
        else if(static_cast<uint8_t>(array.data()[0])==0xFA &&static_cast<uint8_t>(array.data()[1])==0x02
                &&static_cast<uint8_t>(array.data()[2])==0xFA &&static_cast<uint8_t>(array.data()[3])==0x02)
        {

            FLOATA data_count_x;
            FLOATA data_count_xy;

            //(7~8)通道3总点数
            data_count_xy.u8Byte[0]=array.data()[6];
            data_count_xy.u8Byte[1]=array.data()[7];
            data_count_xy.u8Byte[2]=0;
            data_count_xy.u8Byte[3]=0;
            CH_point[2]=data_count_xy.all;

            for(int j=0;j<CH_point[2];j++)  //(9~708)
            {
                data_count_x.u8Byte[0]=array.data()[2*j+8];
                data_count_x.u8Byte[1]=array.data()[2*j+9];
                data_count_x.u8Byte[2]=0;
                data_count_x.u8Byte[3]=0;

                data_x_trans[2][j]=data_count_x.all;
            }

            for(int j=0;j<CH_point[2];j++)  //(709~1408)
            {
                data_count_x.u8Byte[0]=array.data()[2*j+708];
                data_count_x.u8Byte[1]=array.data()[2*j+709];
                data_count_x.u8Byte[2]=0;
                data_count_x.u8Byte[3]=0;

                data_y_trans[2][j]=data_count_x.all;
            }
        }
        else if(static_cast<uint8_t>(array.data()[0])==0xFA &&static_cast<uint8_t>(array.data()[1])==0x03
                &&static_cast<uint8_t>(array.data()[2])==0xFA &&static_cast<uint8_t>(array.data()[3])==0x03)
        {

            FLOATA data_count_x;
            FLOATA data_count_xy;

            //(7~8)通道4总点数
            data_count_xy.u8Byte[0]=array.data()[6];
            data_count_xy.u8Byte[1]=array.data()[7];
            data_count_xy.u8Byte[2]=0;
            data_count_xy.u8Byte[3]=0;
            CH_point[3]=data_count_xy.all;

            for(int j=0;j<CH_point[3];j++)  //(9~708)
            {
                data_count_x.u8Byte[0]=array.data()[2*j+8];
                data_count_x.u8Byte[1]=array.data()[2*j+9];
                data_count_x.u8Byte[2]=0;
                data_count_x.u8Byte[3]=0;

                data_x_trans[3][j]=data_count_x.all;
            }

            for(int j=0;j<CH_point[3];j++)  //(709~1408)
            {
                data_count_x.u8Byte[0]=array.data()[2*j+708];
                data_count_x.u8Byte[1]=array.data()[2*j+709];
                data_count_x.u8Byte[2]=0;
                data_count_x.u8Byte[3]=0;

                data_y_trans[3][j]=data_count_x.all;
            }

        }
        else if(static_cast<uint8_t>(array.data()[0])==0xFA &&static_cast<uint8_t>(array.data()[1])==0x04
                &&static_cast<uint8_t>(array.data()[2])==0xFA &&static_cast<uint8_t>(array.data()[3])==0x04)
        {

            FLOATA data_count_x;
            FLOATA data_count_xy;

            //(7~8)通道5总点数
            data_count_xy.u8Byte[0]=array.data()[6];
            data_count_xy.u8Byte[1]=array.data()[7];
            data_count_xy.u8Byte[2]=0;
            data_count_xy.u8Byte[3]=0;
            CH_point[4]=data_count_xy.all;

            for(int j=0;j<CH_point[4];j++)  //(9~708)
            {
                data_count_x.u8Byte[0]=array.data()[2*j+8];
                data_count_x.u8Byte[1]=array.data()[2*j+9];
                data_count_x.u8Byte[2]=0;
                data_count_x.u8Byte[3]=0;

                data_x_trans[4][j]=data_count_x.all;
            }

            for(int j=0;j<CH_point[4];j++)  //(709~1408)
            {
                data_count_x.u8Byte[0]=array.data()[2*j+708];
                data_count_x.u8Byte[1]=array.data()[2*j+709];
                data_count_x.u8Byte[2]=0;
                data_count_x.u8Byte[3]=0;

                data_y_trans[4][j]=data_count_x.all;
            }

        }
        else if(static_cast<uint8_t>(array.data()[0])==0xFA &&static_cast<uint8_t>(array.data()[1])==0x05
                &&static_cast<uint8_t>(array.data()[2])==0xFA &&static_cast<uint8_t>(array.data()[3])==0x05)
        {
            FLOATA data_count_x;
            FLOATA data_count_xy;

            //(7~8)通道6总点数
            data_count_xy.u8Byte[0]=array.data()[6];
            data_count_xy.u8Byte[1]=array.data()[7];
            data_count_xy.u8Byte[2]=0;
            data_count_xy.u8Byte[3]=0;
            CH_point[5]=data_count_xy.all;

            for(int j=0;j<CH_point[5];j++)  //(9~708)
            {
                data_count_x.u8Byte[0]=array.data()[2*j+8];
                data_count_x.u8Byte[1]=array.data()[2*j+9];
                data_count_x.u8Byte[2]=0;
                data_count_x.u8Byte[3]=0;

                data_x_trans[5][j]=data_count_x.all;
            }

            for(int j=0;j<CH_point[5];j++)  //(709~1408)
            {
                data_count_x.u8Byte[0]=array.data()[2*j+708];
                data_count_x.u8Byte[1]=array.data()[2*j+709];
                data_count_x.u8Byte[2]=0;
                data_count_x.u8Byte[3]=0;

                data_y_trans[5][j]=data_count_x.all;
            }

        }
        else if(static_cast<uint8_t>(array.data()[0])==0xFA &&static_cast<uint8_t>(array.data()[1])==0x06
                &&static_cast<uint8_t>(array.data()[2])==0xFA &&static_cast<uint8_t>(array.data()[3])==0x06)
        {
            FLOATA data_count_x;
            FLOATA data_count_xy;

            //(7~8)通道7总点数
            data_count_xy.u8Byte[0]=array.data()[6];
            data_count_xy.u8Byte[1]=array.data()[7];
            data_count_xy.u8Byte[2]=0;
            data_count_xy.u8Byte[3]=0;
            CH_point[6]=data_count_xy.all;

            for(int j=0;j<CH_point[6];j++)  //(9~708)
            {
                data_count_x.u8Byte[0]=array.data()[2*j+8];
                data_count_x.u8Byte[1]=array.data()[2*j+9];
                data_count_x.u8Byte[2]=0;
                data_count_x.u8Byte[3]=0;

                data_x_trans[6][j]=data_count_x.all;
            }

            for(int j=0;j<CH_point[6];j++)  //(709~1408)
            {
                data_count_x.u8Byte[0]=array.data()[2*j+708];
                data_count_x.u8Byte[1]=array.data()[2*j+709];
                data_count_x.u8Byte[2]=0;
                data_count_x.u8Byte[3]=0;

                data_y_trans[6][j]=data_count_x.all;
            }
        }

        //接其他包结束

   }//while结束

}//readyRead_slot()结束

#include "global.h"
void ReceDateThread::updateStrain()
{

    QMutexLocker locker(&strainMutex);
    for (int i = 0; i < 1024; i++)
    {
        strain_float[i] = this->newStrain[i];
    }
    //qDebug()<<strain_float<<strain_float[0]<<strain_float[1]<<strain_float[2]<<strain_float[3]<<strain_float[4]<<strain_float[5];
}



