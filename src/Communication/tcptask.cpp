#include "tcptask.h"

#include <QFile>
#include <QDateTime>
#include <QTimer>
#include <QMessageBox>
#include <QIODevice>
#include <QThread>

#include "global.h"
#include "Timer.h"

tcptask::tcptask(QObject *parent) : QObject(parent)
{
    m_TcpServer = new QTcpServer(this);
    m_TcpSocket = nullptr;

    m_isListening = false;
    m_hasConnection = false;

    connect(m_TcpServer, &QTcpServer::newConnection, this, &tcptask::handleNewConnection);
}

tcptask::~tcptask()
{
    tcpClose();
    delete m_TcpServer;
    delete m_TcpSocket;
}

void tcptask::getMainThread(connectwindow *obj)
{
    m_MainThread = obj;
}

void tcptask::tcpOpen(const QHostAddress &address, const quint16 &port)
{
    qDebug() << "TCPThreadNum:" << QThread::currentThreadId();
    tcpClose();
    if (!m_TcpServer->listen(address, port)) {
        emit connectionStatusChanged(StatusListeningFailed);
        qDebug() << "TCP监听失败:" << m_TcpServer->errorString();
    }
    m_isListening = true;
    emit connectionStatusChanged(StatusListeningStarted);
    qDebug() << "TCP服务器已启动，监听地址:" << address.toString() << "端口:" << port;
}

void tcptask::tcpClose()
{
    disconnectCurrentClient();
    if (m_isListening) {
        m_TcpServer->close();
        m_isListening = false;
        emit connectionStatusChanged(StatusListeningStopped);
        qDebug() << "TCP服务器已停止";
    }
}

void tcptask::handleNewConnection()
{
    disconnectCurrentClient();

    m_TcpSocket = m_TcpServer->nextPendingConnection();
    if (!m_TcpSocket) {
        emit connectionStatusChanged(StatusConnectionError);
        qDebug() << "获取新连接失败";
        return;
    }

    // 记录客户端信息
    m_clientAddress = m_TcpSocket->peerAddress();
    m_clientPort = m_TcpSocket->peerPort();

    // 更新状态
    m_hasConnection = true;
    emit connectionStatusChanged(StatusClientConnected);
    qDebug() << "客户端连接成功:" << m_clientAddress.toString() << ":" << m_clientPort;

    connect(m_TcpSocket, &QTcpSocket::readyRead, this, &tcptask::tcpRecData);
    connect(m_TcpSocket, &QTcpSocket::disconnected, this, &tcptask::handleClientDisconnected);
}

void tcptask::handleClientDisconnected()
{
    if (!m_TcpSocket) return;
    QString clientInfo = tr("[%1:%2]").arg(
        m_clientAddress.toString()).arg(m_clientPort);
    qDebug() << "客户端断开连接:" << clientInfo;
    emit connectionStatusChanged(StatusClientDisconnected);
    m_TcpSocket->deleteLater();
    m_TcpSocket = nullptr;
    m_hasConnection = false;
}

void tcptask::disconnectCurrentClient()
{
    if (m_TcpSocket) {
        m_TcpSocket->disconnect();
        m_TcpSocket->disconnectFromHost();
        if (m_TcpSocket->state() != QAbstractSocket::UnconnectedState) {
            m_TcpSocket->waitForDisconnected(1000);
        }
        m_TcpSocket->deleteLater();
        m_TcpSocket = nullptr;
        m_hasConnection = false;
    }
}

void tcptask::tcpSendData(const QString text)
{
    if(m_hasConnection){
        m_TcpSocket->write(text.toUtf8().data());
    }
    else {
        qDebug() << "TCP未连接";
    }
}

void tcptask::tcpRecData()
{
    QByteArray datagram;
    datagram = m_TcpSocket->readAll();
    if(datagram.size() >= 4 &&
       static_cast<uint8_t>(datagram.at(0)) == 0xFA &&
       static_cast<uint8_t>(datagram.at(2)) == 0xFA){
        emit tcpPocketsReceived(datagram);
    }
    else {
        m_RecText.clear();
        m_RecText.prepend(datagram);
        m_RecText += "\r\n";
        qDebug() << "UdpRec:" << m_RecText;
        emit tcpDataReceived(m_RecText);
    }
}
