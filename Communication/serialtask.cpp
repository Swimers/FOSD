#include "serialtask.h"
#include <QThread>
#include <QDebug>

serialtask::serialtask(QObject *parent) : QObject(parent)
{
    m_SerialPort = new QSerialPort(this);
    connect(m_SerialPort, &QSerialPort::readyRead,
            this, &serialtask::handleReadyRead);
    connect(m_SerialPort, QOverload<QSerialPort::SerialPortError>::of(&QSerialPort::error),
            this, &serialtask::handleError);
}

serialtask::~serialtask()
{
    if (m_SerialPort->isOpen()) {
        m_SerialPort->close();
    }
    delete m_SerialPort;
}

void serialtask::getMainThread(connectwindow *obj)
{
    m_MainThread = obj;
}

void serialtask::openSerialPort(const QString &portName, qint32 baudRate)
{
    if (m_SerialPort->isOpen()) {
        m_SerialPort->close();
    }
    m_SerialPort->setPortName(portName);
    m_SerialPort->setBaudRate(baudRate);
    m_SerialPort->setDataBits(QSerialPort::Data8);
    m_SerialPort->setParity(QSerialPort::NoParity);
    m_SerialPort->setStopBits(QSerialPort::OneStop);
    m_SerialPort->setFlowControl(QSerialPort::NoFlowControl);

    if (m_SerialPort->open(QIODevice::ReadWrite)) {
        emit connectionStatusChanged(true);
        qDebug() << "SerialThreadNum:" << QThread::currentThreadId();
    } else {
        emit errorOccurred(m_SerialPort->errorString());
        emit connectionStatusChanged(false);
    }
}

void serialtask::closeSerialPort()
{
    if (m_SerialPort->isOpen()) {
        m_SerialPort->close();
    }
}

void serialtask::writeData(const QString &data)
{
    QByteArray data1 = data.toUtf8();

    if (m_SerialPort->isOpen()) {
        m_SerialPort->write(data1);
    }
}

void serialtask::handleReadyRead()
{   
    serial_array = m_SerialPort->readAll();
    if(!serial_array.isEmpty()){
        QString str = QString::fromUtf8(serial_array);
        strdata = str;
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
                data = str.mid(i-gap,gap).toDouble();
            }
            i++;
        }
        serial_array.clear();
    }
    emit dataReceived(strdata);
}

void serialtask::handleError(QSerialPort::SerialPortError error)
{
    if (error != QSerialPort::NoError) {
        emit errorOccurred(m_SerialPort->errorString());
        if (m_SerialPort->isOpen()) {
            m_SerialPort->close();
            emit connectionStatusChanged(false);
        }
    }
}
