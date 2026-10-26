#pragma once
#include "FOSD-Classic-QWidgets/common/WorkbenchServices.h"
#include "protocol/ProtocolControl.h"
#include "calculation/CalculationControl.h"
#include "visualization/VisualizationControl.h"
#include <QTimer>
#include <QTemporaryDir>
#include <QSet>
#include <QQueue>
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
    bool loadPlate(QString *error = nullptr);
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

private:
    bool connectionAction(const ui::classic::UiRequest &, ui::classic::UiResponse *);
    bool visualizationAction(const ui::classic::UiRequest &, ui::classic::UiResponse *);
    bool dataAction(const ui::classic::UiRequest &, ui::classic::UiResponse *);
    bool configure(const Calculation::CalculationConfiguration &, QString *error);
    bool baseline(QString *error);
    void publishCalibration();
    void publishModel();
    void publishRunState();
    void initializePorts();
    bool startStorage(const QString &path, bool single, QString *error);
    static QString text(const QVariantMap &fields, const QString &name);
    static int index(const QVariantMap &fields, const QString &name);
    std::unique_ptr<protocol::ProtocolControl> m_protocol;
    std::unique_ptr<Calculation::CalculationControl> m_calculation;
    std::unique_ptr<visualization::VisualizationControl> m_visualization;
    Calculation::CalculationConfiguration m_configuration;
    core::FrameSnapshotPtr m_frame;
    core::CalibratedDataPtr m_hardware;
    core::CalculationResultPtr m_result;
    QTimer m_timer;
    QTemporaryDir m_assets;
    QString m_modelPath;
    QString m_lastNotice;
    QString m_rawText[3];
    communication::ConnectionState m_states[3] = {communication::ConnectionState::Closed,
                                                  communication::ConnectionState::Closed,
                                                  communication::ConnectionState::Closed};
    communication::Channel m_networkChannel = communication::Channel::Udp;
    QSet<int> m_channels;
    bool m_filterChannels = false;
    bool m_running = true;
    bool m_observing = false;
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
    QString m_presentedRaw[3];
    bool m_originalKeyCoordinates = false;
};
} // namespace application
} // namespace fosd
