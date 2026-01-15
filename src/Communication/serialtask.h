#ifndef SERIALTASK_H
#define SERIALTASK_H

#include <QObject>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QByteArray>


class connectwindow;
class serialtask : public QObject
{
    Q_OBJECT
public:
    explicit serialtask(QObject *parent = nullptr);
    ~serialtask();

    void getMainThread(connectwindow * obj);

public:
    bool OpenState;
    QByteArray serial_array;

    QString strdata;
    double data = 0;
    double num = 0;

public slots:
    void openSerialPort(const QString &portName, qint32 baudRate);
    void closeSerialPort();
    void writeData(const QString &data);

private slots:
    void handleReadyRead();
    void handleError(QSerialPort::SerialPortError error);

signals:
    void dataReceived(const QString &data);
    void errorOccurred(const QString &error);
    void connectionStatusChanged(bool connected);

private:
    QSerialPort * m_SerialPort = nullptr;
    connectwindow * m_MainThread = nullptr;

};

#endif // SERIALTASK_H
