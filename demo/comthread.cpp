#include "comthread.h"
#include<QSerialPortInfo>
#include<QDebug>
#include<mainwindow.h>
#include<ui_mainwindow.h>

#pragma execution_character_set("utf-8")

ComThread::ComThread(QObject *parent) : QThread(parent)
  //构造函数中，用到了qobject_cast对象转换，将QObject指针转为MainWindow，这是QT中常用的一种在对象树中传递对象指针和恢复的方法。
  ,guimain(qobject_cast<DemoWindow*>(parent))
{
    qDebug("thread exit");
}

ComThread::~ComThread()
{

}


bool ComThread::InitSerial(QString portname, int baudRate)
{
    port->setPortName(portname);
    port->setBaudRate(baudRate);
    if(!port->open(QIODevice::ReadWrite))
    {
        qDebug()<<"串口打开失败";
        return 0;
    }
    qDebug()<<"串口打开成功";
    return 1;
}

void ComThread::CloseSerial()
{
    if(port->isOpen())
    {
        port->clear();
        port->close();
        serial_array.clear();
    }

}
void ComThread::run()
{
    connect(port,SIGNAL(readyRead()),this,SLOT(readyRead_serial_slot()));
}
//读取数据并发送信号
void ComThread::readyRead_serial_slot()
{
    serial_array =port->readAll();
    if(!serial_array.isEmpty())
    {

            do_DataHandler(serial_array);
            serial_array.clear();
    }
}

void ComThread::do_DataHandler(QByteArray bufferData)
{
    QString str=QString::fromUtf8(bufferData);
    strdata=str;

    int i=0;
    int left=0;
    int right=0;

    while(i<str.size())
    {
        if(str[i]=='\r')
        {
            num++;
            right=i;
            int gap=right-left;
            //strdata=str.mid(i-gap,gap);
            data=str.mid(i-gap,gap).toDouble();
        }
        i++;
    }
    guimain->dataReceive();
}




//发送数据
void ComThread::SendSerialData(QByteArray data)
{
    QString str=QString::fromUtf8(data);
    str+="\r\n";
    QByteArray data1=str.toUtf8();
    port->write(data1);
}
