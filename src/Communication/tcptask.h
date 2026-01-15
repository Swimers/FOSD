#ifndef TCPTASK_H
#define TCPTASK_H

#include <QObject>

#include<QTcpServer>//监听套接字
#include<QTcpSocket>//通信套接字

#include <QString>
#include <QHostAddress>
#include <QMessageBox>

#include <QTimer>
#include <QDebug>
#include <QStandardItemModel>

class connectwindow;
class tcptask : public QObject
{
    Q_OBJECT
public:
    explicit tcptask(QObject *parent = nullptr);
    ~tcptask();

    void getMainThread(connectwindow * obj);

public:
    QByteArray m_ReceArray;
    QString m_RecText;

    QHostAddress m_clientAddress;
    quint16 m_clientPort;

    enum ConnectionStatus {
        StatusNone = 0,           // 无状态
        StatusListeningStarted,   // 开始监听
        StatusListeningFailed,    // 监听失败
        StatusListeningStopped,   // 停止监听
        StatusClientConnected,    // 客户端已连接
        StatusClientDisconnected, // 客户端已断开
        StatusConnectionError     // 连接错误
    };

public slots:
    void tcpOpen(const QHostAddress &address,const quint16 &port);
    void tcpClose();
    void tcpSendData(const QString text);

private slots:
    void tcpRecData();
    void handleNewConnection();
    void handleClientDisconnected();

private:
    void disconnectCurrentClient();

signals:
    void connectionStatusChanged(tcptask::ConnectionStatus status);
    void tcpDataReceived(const QString &data);
    void tcpPocketsReceived(const QByteArray datagram);

private:
    connectwindow * m_MainThread = nullptr;

    QTcpServer * m_TcpServer = nullptr;
    QTcpSocket * m_TcpSocket = nullptr;

    bool m_isListening;
    bool m_hasConnection;
};

#endif // TCPTASK_H
