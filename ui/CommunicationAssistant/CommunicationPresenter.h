#pragma once

#include "communication/ConnectControl.h"
#include <QList>
#include <QStringList>
#include <QVariantMap>

namespace fosd {
namespace assistant {

// Lives on the UI thread. ConnectControl owns the existing I/O worker thread.
// Channel indices exposed to QML: 0 = serial, 1 = UDP, 2 = TCP.
class CommunicationPresenter final : public QObject {
    Q_OBJECT
    Q_PROPERTY(int selectedChannel READ selectedChannel WRITE setSelectedChannel NOTIFY selectionChanged)
    Q_PROPERTY(QStringList serialPorts READ serialPorts NOTIFY serialPortsChanged)
    Q_PROPERTY(QString statusText READ statusText NOTIFY connectionChanged)
    Q_PROPERTY(bool isOpen READ isOpen NOTIFY connectionChanged)
    Q_PROPERTY(bool canSend READ canSend NOTIFY connectionChanged)
    Q_PROPERTY(QString activityLog READ activityLog NOTIFY activityLogChanged)
    Q_PROPERTY(QString lastError READ lastError NOTIFY lastErrorChanged)
    Q_PROPERTY(bool hexDisplay READ hexDisplay WRITE setHexDisplay NOTIFY hexDisplayChanged)
    Q_PROPERTY(qulonglong receivedBytes READ receivedBytes NOTIFY countersChanged)
    Q_PROPERTY(qulonglong submittedBytes READ submittedBytes NOTIFY countersChanged)
public:
    explicit CommunicationPresenter(QObject *parent = nullptr);
    int selectedChannel() const { return m_selectedChannel; }
    void setSelectedChannel(int channel);
    QStringList serialPorts() const { return m_serialPorts; }
    QString statusText() const;
    bool isOpen() const;
    bool canSend() const;
    QString activityLog() const;
    QString lastError() const { return m_lastError; }
    bool hexDisplay() const { return m_hexDisplay; }
    void setHexDisplay(bool value);
    qulonglong receivedBytes() const { return m_receivedBytes; }
    qulonglong submittedBytes() const { return m_submittedBytes; }

    Q_INVOKABLE void refreshSerialPorts();
    Q_INVOKABLE bool openConnection(const QVariantMap &settings);
    Q_INVOKABLE void closeConnection();
    Q_INVOKABLE bool sendData(const QString &text, bool hex, bool appendCrLf,
                              const QString &peerAddress, const QString &peerPort);
    Q_INVOKABLE void clearLog();
    Q_INVOKABLE void resetCounters();

signals:
    void selectionChanged();
    void serialPortsChanged();
    void connectionChanged();
    void activityLogChanged();
    void lastErrorChanged();
    void hexDisplayChanged();
    void countersChanged();
    void dataReceived(int channel, const QByteArray &bytes);

private slots:
    void receiveBytes(fosd::communication::Channel channel, const QByteArray &bytes,
                      const fosd::communication::Endpoint &peer);
    void updateState(fosd::communication::Channel channel,
                     fosd::communication::ConnectionState state);
    void handleError(const fosd::communication::CommunicationError &error);

private:
    struct LogEntry {
        QString prefix;
        QByteArray bytes;
        QString message;
    };
    bool fail(const QString &operation, const QString &message);
    bool parsePort(const QString &text, quint16 &port);
    void record(fosd::communication::Channel channel, const QString &kind,
                const QByteArray &bytes, const QString &message = QString());
    fosd::communication::Channel channel() const;
    fosd::communication::ConnectControl m_control;
    fosd::communication::ConnectionState m_states[3];
    int m_selectedChannel = 0;
    QStringList m_serialPorts;
    QList<LogEntry> m_log;
    int m_logBytes = 0;
    QString m_lastError;
    bool m_hexDisplay = true;
    qulonglong m_receivedBytes = 0;
    qulonglong m_submittedBytes = 0;
};

} // namespace assistant
} // namespace fosd
