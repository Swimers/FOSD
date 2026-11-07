#pragma once
#include "FOSD-Classic-QWidgets/common/WorkbenchServices.h"
#include "protocol/ProtocolControl.h"
#include "calculation/CalculationControl.h"
#include "visualization/VisualizationControl.h"
#include <QTimer>
#include <QElapsedTimer>
#include "config/ParameterStore.h"
#include <QSet>
#include <QQueue>
#include <QTextCodec>
namespace fosd {
namespace application {
class WorkbenchRuntime final : public ui::classic::WorkbenchServices {
    Q_OBJECT
public:
    explicit WorkbenchRuntime(QObject *parent = nullptr);
    ~WorkbenchRuntime() override;
    bool handleRequest(const ui::classic::UiRequest &request,
                       ui::classic::UiResponse *response) override;
    void initialize();
    bool attach(vtkRenderWindow *window);
    visualization::VisualizationControl &visualization() {
        return *m_visualization;
    }
    protocol::ProtocolControl &protocol() {
        return *m_protocol;
    }
    Calculation::CalculationControl &calculation() {
        return *m_calculation;
    }

private slots:
    void frameReceived(const fosd::core::FrameSnapshotPtr &frame);
    void hardwareReady(const fosd::core::CalibratedDataPtr &data);
    void resultReady(const fosd::core::CalculationResultPtr &result);
    void modelLoaded(const fosd::core::ModelDataPtr &model);
    void modelAccepted(quint64 revision);
    void configurationAccepted(quint64 revision);
    void transportState(fosd::communication::Channel channel,
                        fosd::communication::ConnectionState state);
    void rawReceived(fosd::communication::Channel channel, const QByteArray &bytes,
                     const fosd::communication::Endpoint &peer);
    void rawSent(fosd::communication::Channel channel, const QByteArray &bytes,
                 const fosd::communication::Endpoint &peer);
    void transportError(const fosd::communication::CommunicationError &error);
    void decoderError(const fosd::communication::DecoderError &error);
    void protocolError(const fosd::protocol::ProtocolError &error);
    void calculationError(const fosd::Calculation::CalculationError &error);
    void storageError(const fosd::storage::StorageError &error);
    void storageStarted(const QString &session);
    void storageStopped();
    void selectionChanged(const fosd::visualization::PickedPointInfo &info);
    void notice(const QString &message);
    void publishLatest();
    void publishScalar();
    void publishMarkers();
    void publishDisplayedPoints();
    void clearReceiveLogs();

private:
    bool connectionAction(const ui::classic::UiRequest &, ui::classic::UiResponse *);
    bool visualizationAction(const ui::classic::UiRequest &, ui::classic::UiResponse *);
    bool markerAction(const ui::classic::UiRequest &, ui::classic::UiResponse *);
    bool dataAction(const ui::classic::UiRequest &, ui::classic::UiResponse *);
    bool configure(const Calculation::CalculationConfiguration &, QString *error);
    bool baseline(QString *error);
    void publishCalibration();
    void publishModel();
    void publishRunState();
    void initializePorts(const QString &serialPort = QString(), const QString &localAddress = QString());
    void initializeLocalAddresses(const QString &selected = QString());
    void publishNetworkConfiguration(int mode);
    bool startStorage(const QString &path, QString *error);
    QString udpLogEntry() const;
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
    void appendTraffic(communication::Channel channel, const QByteArray &bytes,
                       const communication::Endpoint &peer, bool outgoing,
                       const QString &summary = QString(), int byteCount = -1,
                       bool showPayload = true, const QDateTime &at = QDateTime());
    QString formatTraffic(int channel, const TrafficEntry &entry) const;
    void flushUdpTraffic();
    void clearTraffic(int channel);
    bool sendSerialCommand(const QString &command, QString *error);
    static QString text(const QVariantMap &fields, const QString &name);
    static int index(const QVariantMap &fields, const QString &name);
    std::unique_ptr<protocol::ProtocolControl> m_protocol;
    std::unique_ptr<Calculation::CalculationControl> m_calculation;
    std::unique_ptr<visualization::VisualizationControl> m_visualization;
    Calculation::CalculationConfiguration m_configuration;
    Calculation::CalculationConfiguration m_pendingConfiguration;
    core::FrameSnapshotPtr m_frame;
    core::CalibratedDataPtr m_hardware;
    core::CalculationResultPtr m_result;
    QTimer m_timer;
    QTimer m_receiveLogCleanupTimer;
    config::ParameterStore m_parameters;
    int m_baselineCount = 0;
    int m_baselineTarget = 20;
    int m_baselineTimeout = 10000;
    QVector<double> m_baselineSum;
    QString m_baselineIdentity;
    QString m_activeIdentity;
    QString m_pendingIdentity;
    QElapsedTimer m_baselineClock;
    quint64 m_baselineRevision = 0;
    bool m_solving = true;
    quint64 m_runStartFrameRevision = 0;
    QMap<QString, QVariantList> m_history;
    QElapsedTimer m_historyClock;
    QString m_modelPath;
    QString m_lastNotice;
    bool m_rawHex[3] = {false, true, true};
    QVector<TrafficEntry> m_traffic[3];
    quint64 m_nextTrafficId[3] = {0,0,0};
    quint64 m_publishedTrafficId[3] = {0,0,0};
    bool m_resetTraffic[3] = {false,false,false};
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
    communication::ConnectionState m_states[3] = {communication::ConnectionState::Closed,
                                                  communication::ConnectionState::Closed,
                                                  communication::ConnectionState::Closed};
    communication::Channel m_networkChannel = communication::Channel::Udp;
    communication::TcpRole m_tcpRole = communication::TcpRole::Client;
    QSet<int> m_channels;
    bool m_filterChannels = false;
    bool m_running = true;
    bool m_hardwareRefresh = true;
    bool m_recording = false;
    bool m_storagePending = false;
    bool m_modelPending = false;
    bool m_configurationPending = false;
    quint64 m_modelRevision = 0;
    quint64 m_configurationRevision = 0;
    quint64 m_modelGeneration = 0;
    QQueue<quint64> m_modelRequests;
    quint64 m_presentedFrame = 0;
    quint64 m_presentedResult = 0;
    core::CalibratedDataPtr m_presentedHardware;
    vtkIdType m_selectedPoint = -1;
    bool m_originalKeyCoordinates = true;
};
} // namespace application
} // namespace fosd
