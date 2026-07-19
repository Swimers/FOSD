#include "udptask.h"

udptask::udptask(QObject *parent) : QObject(parent)
{
    m_SendSocket = new QUdpSocket(this);
    m_ReceSocket = new QUdpSocket(this);

    connect(m_ReceSocket, SIGNAL(readyRead()),this, SLOT(udpRecData()));
}

udptask::~udptask()
{
    m_SendSocket->close();
    m_ReceSocket->close();
    delete m_SendSocket;
    delete m_ReceSocket;
}

void udptask::udpOpen(const QHostAddress &address, const quint16 &port)
{
    if(m_ReceSocket->bind(address,port)){
        emit connectionStatusChanged(true);
        qDebug() << "UdpThreadNum:" << QThread::currentThreadId();
    }
    else
        emit connectionStatusChanged(false);
}

void udptask::udpClose()
{
    m_ReceSocket->close();
}

void udptask::udpSendData(const QString text,const  QHostAddress IPAddress,const  quint16 PortNum)
{
    QByteArray datagram = text.toUtf8();
    m_SendSocket->writeDatagram(datagram.data(),datagram.size(),IPAddress,PortNum);
}

void udptask::udpRecData()
{
     while (m_ReceSocket->hasPendingDatagrams()) {
         QByteArray datagram;
         datagram.resize(static_cast<int>(m_ReceSocket->pendingDatagramSize()));
         m_ReceSocket->readDatagram(datagram.data(),datagram.size());
         if(datagram.size() >= 4 &&
            static_cast<uint8_t>(datagram.at(0)) == 0xFA &&
            static_cast<uint8_t>(datagram.at(2)) == 0xFA){
             emit udpPocketsReceived(datagram);
             emit udpDataReceived("Packet Received\r\n");
         }
         else {
             m_RecText.clear();
             m_RecText.prepend(datagram);
             m_RecText += "\r\n";
             emit udpDataReceived(m_RecText);
         }
     }
}
