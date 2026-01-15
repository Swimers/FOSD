#ifndef UDPTASK_H
#define UDPTASK_H

#include <QObject>
#include <QFile>
#include <QDateTime>
#include <QTimer>
#include <QMessageBox>
#include <QString>
#include <QMessageBox>
#include <QDebug>

#include <QStandardItemModel>
#include <QUdpSocket>
#include <QtNetwork>
#include <QIODevice>
#include <QHostAddress>

#include "global.h"
#include "Timer.h"

class connectwindow;
class udptask : public QObject
{
    Q_OBJECT
public:
    explicit udptask(QObject *parent = nullptr);
    ~udptask();

public:
    QByteArray m_ReceArray;
    QString m_RecText;

public slots:
    void udpOpen(const QHostAddress &address,const quint16 &port);
    void udpClose();
    void udpSendData(const QString text,const  QHostAddress IPAddress,const  quint16 PortNum);

private slots:
    void udpRecData();

signals:
    void connectionStatusChanged(bool connected);
    void udpDataReceived(const QString &data);
    void udpPocketsReceived(const QByteArray datagram);

private:
    connectwindow * m_MainThread = nullptr;

    QUdpSocket * m_SendSocket = nullptr;
    QUdpSocket * m_ReceSocket = nullptr;
};

#endif // UDPTASK_H
