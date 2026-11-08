#pragma once
#include "FOSD-Classic-QWidgets/common/PanelPresenter.h"
#include <QTextCodec>
#include <QSet>
namespace fosd {
namespace ui {
namespace classic {
class ConnectionPresenter final : public PanelPresenter {
    Q_OBJECT
  public:
    ConnectionPresenter(PanelId, IView *, IWorkbenchView *host = nullptr,
                        QObject *parent = nullptr);
    void initialize() override;
    void refresh() override;
    void poll() override;
  public slots:
    void rawReceived(fosd::communication::Channel, const QByteArray &,
                     const fosd::communication::Endpoint &);
    void rawSent(fosd::communication::Channel, const QByteArray &,
                 const fosd::communication::Endpoint &);
    void clearReceiveLogs();

  protected:
    bool localAction(const QString &, const QVariantMap &) override;
    bool prepareAction(const QString &, QVariantMap &) override;
    bool handleAction(const QString &, const QVariantMap &, QString &, QVariantMap &) override;
    void bindModel() override;

  private:
    void publishCalibration();
    void initializePorts(const QString &serialPort = QString(),
                         const QString &localAddress = QString());
    void initializeLocalAddresses(const QString &localAddress = QString());
    void publishNetworkConfiguration(int);
    struct TrafficEntry {
        quint64 id = 0;
        QDateTime at;
        bool outgoing = false;
        QString endpoint;
        int byteCount = 0;
        QByteArray preview;
        QString utf8;
        QString summary;
        bool showPayload = true;
    };

    void appendTraffic(communication::Channel, const QByteArray &, const communication::Endpoint &,
                       bool, const QString &summary = QString(), int byteCount = -1,
                       bool showPayload = true, const QDateTime &at = QDateTime());
    QString formatTraffic(int, const TrafficEntry &) const;
    QString udpLogEntry() const;
    void flushUdpTraffic();
    void clearTraffic(int);
    bool m_rawHex[3] = {false, true, true};
    QVector<TrafficEntry> m_traffic[3];
    quint64 m_nextTrafficId[3] = {0, 0, 0};
    quint64 m_publishedTrafficId[3] = {0, 0, 0};
    bool m_resetTraffic[3] = {false, false, false};
    std::unique_ptr<QTextDecoder> m_receiveDecoder[3];
    communication::SerialConfig m_logSerial;
    communication::UdpConfig m_logUdp;
    communication::TcpConfig m_logTcp;
    communication::Endpoint m_logTcpPeer;
    core::FrameSnapshotPtr m_udpLogFrame;
    QByteArray m_udpPreview;
    communication::Endpoint m_udpPeer;
    QDateTime m_udpReceivedAt;
    int m_udpPacketBytes = 0;
    quint64 m_udpPacketCount = 0;
    QString m_udpLogStatus;
    bool m_udpLogPending = false;

    config::ParameterStore m_parameters;
    QTimer m_receiveLogCleanupTimer;
    QMap<QString, QVariantList> m_history;
    QElapsedTimer m_historyClock;
    quint64 m_presentedFrame = 0;
    QSet<int> m_channels;
    bool m_filterChannels = false;
    communication::Channel m_networkChannel = communication::Channel::Udp;
    communication::TcpRole m_tcpRole = communication::TcpRole::Client;
    communication::ConnectionState m_previousStates[3] = {communication::ConnectionState::Closed,
                                                          communication::ConnectionState::Closed,
                                                          communication::ConnectionState::Closed};
};
} // namespace classic
} // namespace ui
} // namespace fosd
