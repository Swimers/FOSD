#pragma once
#include "protocol/ProtocolControl.h"
#include "calculation/CalculationControl.h"
#include "visualization/VisualizationControl.h"
#include "config/ParameterStore.h"
#include <QTimer>
#include <QElapsedTimer>
#include <QQueue>
namespace fosd {
namespace application {
enum class DisplayState { Stopped, Running, Paused };
enum class RecordingState { Stopped, Starting, Recording, Stopping };
struct WorkbenchState {
    bool hasModel = false, solving = true, configurationPending = false, modelPending = false;
    DisplayState display = DisplayState::Stopped;
    RecordingState recording = RecordingState::Stopped;
};

// GUI-thread business coordinator. No widget identifiers or presentation maps.
class WorkbenchModel final : public QObject {
    Q_OBJECT
  public:
    explicit WorkbenchModel(QObject *parent = nullptr);
    ~WorkbenchModel() override;
    void initialize();
    bool attach(vtkRenderWindow *window);
    WorkbenchState state() const;
    const Calculation::CalculationConfiguration &configuration() const {
        return m_configuration;
    }
    core::FrameSnapshotPtr frame() const {
        return m_frame;
    }
    core::CalibratedDataPtr hardware() const {
        return m_hardware;
    }
    core::CalculationResultPtr result() const {
        return m_result;
    }
    communication::ConnectionState connectionState(communication::Channel c) const {
        return m_states[int(c)];
    }

    bool applyCalibration(const Calculation::CalculationConfiguration &, QString *error = nullptr);
    bool loadCalibration(QString *error = nullptr);
    bool baseline(QString *error = nullptr);
    bool setCalculationRunning(bool enabled, QString *error = nullptr);
    bool setDisplayState(DisplayState state, QString *error = nullptr);
    bool setRecording(bool enabled, const QString &path, QString *error = nullptr);
    bool sendSerialCommand(const QString &, QString *error = nullptr);
    void openSerial(const communication::SerialConfig &c) {
        m_protocol->communication().openSerial(c);
    }
    void openUdp(const communication::UdpConfig &c) {
        m_protocol->communication().openUdp(c);
    }
    void openTcp(const communication::TcpConfig &c) {
        m_protocol->communication().openTcp(c);
    }
    void closeChannel(communication::Channel c) {
        m_protocol->communication().close(c);
    }
    void send(communication::Channel c, const QByteArray &bytes,
              const communication::Endpoint &peer = {}) {
        m_protocol->communication().send(c, bytes, peer);
    }
    bool loadModel(const QString &path) {
        return m_visualization->loadModel(path);
    }

    void clearModel();
    bool selectAllKeyPoints();
    bool addKeyPoint(vtkIdType id);
    bool removeKeyPoint(vtkIdType id);
    bool updateKeyPointsInBox(const std::array<double, 3> &first,
                              const std::array<double, 3> &second, bool add);
    bool upsertDisplayedPoint(const visualization::DisplayedPointDefinition &point);
    bool removeDisplayedPoints(const QVector<vtkIdType> &ids);
    bool displayAllKeyPoints();
    void resetDeformation();
    
    protocol::ProtocolControl &protocol() { return *m_protocol; }
    Calculation::CalculationControl &calculation() { return *m_calculation; }
    visualization::VisualizationControl &visualization() { return *m_visualization; }
    core::ModelDataPtr originalModel() const { return m_visualization->originalModel(); }

    qint64 modelPolygonCount() const { return m_visualization->modelPolygonCount(); }
    void setfreshFPS(int rate) { m_visualization->setfreshFPS(rate); }
    void setDeformationScale(double scale) { m_visualization->setDeformationScale(scale); }
    bool setScalarColorSettings(const visualization::ScalarColorSettings &s) { return m_visualization->setScalarColorSettings(s); }
    bool setModelColor(const std::array<double, 3> &c) { return m_visualization->setModelColor(c); }
    bool setAxesSettings(const visualization::AxesSettings &s) { return m_visualization->setAxesSettings(s); }
    bool setGridSettings(const visualization::GridSettings &s) { return m_visualization->setGridSettings(s); }
    bool setBackgroundSettings(const visualization::BackgroundSettings &s) { return m_visualization->setBackgroundSettings(s); }
    bool setKeyPoints(const QVector<visualization::KeyPointDefinition> &p) { return m_visualization->setKeyPoints(p); }
    bool setDisplayedPoints(const QVector<visualization::DisplayedPointDefinition> &p) { return m_visualization->setDisplayedPoints(p); }
    
    void setCameraLocked(bool locked) { m_visualization->setCameraLocked(locked); }
    void setCameraPreset(visualization::CameraPreset preset) { m_visualization->setCameraPreset(preset); }
    std::array<double, 3> modelColor() const { return m_visualization->modelColor(); }

    bool displayedPointsVisible() const { return m_visualization->displayedPointsVisible(); }
    
    bool showGrid(visualization::GridPlane plane, bool visible) { return m_visualization->showGrid(plane, visible); }
    void showGrid(bool visible) { m_visualization->showGrid(visible); }
    void showAxes(bool visible) { m_visualization->showAxes(visible); }
    void showDisplayedPoints(bool visible) { m_visualization->showDisplayedPoints(visible); }

    visualization::ScalarColorSettings scalarColorSettings() const { return m_visualization->scalarColorSettings(); }
    visualization::GridSettings gridSettings() const { return m_visualization->gridSettings(); }
    visualization::AxesSettings axesSettings() const { return m_visualization->axesSettings(); }
    visualization::BackgroundSettings backgroundSettings() const { return m_visualization->backgroundSettings(); }
    visualization::CameraSettings cameraSettings() const { return m_visualization->cameraSettings(); }
    visualization::PickedPointInfo selectedPoint() const { return m_visualization->selectedPoint(); }

    QVector<visualization::KeyPointDefinition> keyPoints() const { return m_visualization->keyPoints(); }
    QVector<visualization::DisplayedPointDefinition> displayedPoints() const { return m_visualization->displayedPoints(); }
    QVector<visualization::PickedPointInfo> displayedPointInfos() const { return m_visualization->displayedPointInfos(); }
        
    
  signals:
    void stateChanged();    // 推送状态更新
    void dataChanged();     // 推送数据更新

    void configurationChanged();
    void configurationApplied(quint64 revision);
    void messageChanged(const QString &message);
    void rawDataReceived(fosd::communication::Channel channel, const QByteArray &bytes,
                         const fosd::communication::Endpoint &peer);
    void sendRequested(fosd::communication::Channel channel, const QByteArray &bytes,
                       const fosd::communication::Endpoint &peer);
    void frameObserved(const fosd::core::FrameSnapshotPtr &frame);
    void decoderRejected(const fosd::communication::DecoderError &error);
    void protocolRejected(const fosd::protocol::ProtocolError &error);
    void serialConfigurationChanged(const fosd::communication::SerialConfig &configuration);
    void udpConfigurationChanged(const fosd::communication::UdpConfig &configuration);
    void tcpConfigurationChanged(const fosd::communication::TcpConfig &configuration);
  public slots:
    void notice(const QString &message);
  private slots:
    void frameReceived(const fosd::core::FrameSnapshotPtr &);
    void hardwareReady(const fosd::core::CalibratedDataPtr &);
    void resultReady(const fosd::core::CalculationResultPtr &);
    void modelLoaded(const fosd::core::ModelDataPtr &);
    void modelAccepted(quint64);
    void configurationAccepted(quint64);
    void transportState(fosd::communication::Channel, fosd::communication::ConnectionState);
    void calculationError(const fosd::Calculation::CalculationError &);
    void storageError(const fosd::storage::StorageError &);
    void storageStarted(const QString &);
    void storageStopped();

  private:
    bool startStorage(const QString &, QString *);
    std::unique_ptr<protocol::ProtocolControl> m_protocol;
    std::unique_ptr<Calculation::CalculationControl> m_calculation;
    std::unique_ptr<visualization::VisualizationControl> m_visualization;
    Calculation::CalculationConfiguration m_configuration, m_pendingConfiguration;
    core::FrameSnapshotPtr m_frame;
    core::CalibratedDataPtr m_hardware;
    core::CalculationResultPtr m_result;
    config::ParameterStore m_parameters;
    int m_baselineCount = 0, m_baselineTarget = 20, m_baselineTimeout = 10000;
    QVector<double> m_baselineSum;
    QString m_baselineIdentity, m_activeIdentity, m_pendingIdentity, m_lastNotice;
    QElapsedTimer m_baselineClock;
    QTimer m_baselineTimer;
    quint64 m_baselineRevision = 0, m_runStartFrameRevision = 0, m_modelRevision = 0,
            m_configurationRevision = 0, m_modelGeneration = 0;
    bool m_solving = true, m_running = false, m_displayPaused = false, m_recording = false,
         m_storagePending = false, m_modelPending = false, m_configurationPending = false;
    QQueue<quint64> m_modelRequests;
    communication::ConnectionState m_states[3] = {communication::ConnectionState::Closed,
                                                  communication::ConnectionState::Closed,
                                                  communication::ConnectionState::Closed};
};
} // namespace application
} // namespace fosd
