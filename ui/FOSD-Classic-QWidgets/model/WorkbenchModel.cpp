#include "FOSD-Classic-QWidgets/model/WorkbenchModel.h"
#include <QFileInfo>
#include <cmath>
namespace fosd {
namespace application {
using namespace communication;
WorkbenchModel::WorkbenchModel(QObject *parent)
    : QObject(parent), m_protocol(new protocol::ProtocolControl),
      m_calculation(new Calculation::CalculationControl(m_protocol->dataModel())),
      m_visualization(new visualization::VisualizationControl) {
    auto &c = m_protocol->communication();
    connect(&c, &communication::ConnectControl::stateChanged, this,
            &WorkbenchModel::transportState);
    connect(&c, &communication::ConnectControl::rawDataReceived, this,
            &WorkbenchModel::rawDataReceived);
    connect(&c, &communication::ConnectControl::requestSend, this, &WorkbenchModel::sendRequested);
    connect(&c, &communication::ConnectControl::requestOpenSerial, this,
            &WorkbenchModel::serialConfigurationChanged);
    connect(&c, &communication::ConnectControl::requestOpenUdp, this,
            &WorkbenchModel::udpConfigurationChanged);
    connect(&c, &communication::ConnectControl::requestOpenTcp, this,
            &WorkbenchModel::tcpConfigurationChanged);
    connect(&c, &communication::ConnectControl::errorOccurred, this,
            [this](const communication::CommunicationError &e) {
                notice(tr("通信失败（%1）：%2").arg(e.operation, e.message));
            });
    connect(&c, &communication::ConnectControl::decoderErrorOccurred, this,
            [this](const communication::DecoderError &e) {
                emit decoderRejected(e);
                notice(tr("拆包 %1：%2").arg(e.operation, e.message));
            });
    connect(m_protocol.get(), &protocol::ProtocolControl::errorOccurred, this,
            [this](const protocol::ProtocolError &e) {
                emit protocolRejected(e);
                notice(tr("协议 %1：%2").arg(e.protocolId, e.message));
            });
    connect(m_protocol.get(), &protocol::ProtocolControl::frameParsed, this,
            &WorkbenchModel::frameReceived);
    connect(m_protocol.get(), &protocol::ProtocolControl::frameParsed, &m_calculation->storage(),
            &storage::StorageControl::writeWavelengthFrame);
    connect(m_calculation.get(), &Calculation::CalculationControl::calibratedReady, this,
            &WorkbenchModel::hardwareReady);
    connect(m_calculation.get(), &Calculation::CalculationControl::resultReady, this,
            &WorkbenchModel::resultReady);
    connect(m_calculation.get(), &Calculation::CalculationControl::modelAccepted, this,
            &WorkbenchModel::modelAccepted);
    connect(m_calculation.get(), &Calculation::CalculationControl::configurationAccepted, this,
            &WorkbenchModel::configurationAccepted);
    connect(m_calculation.get(), &Calculation::CalculationControl::errorOccurred, this,
            &WorkbenchModel::calculationError);
    auto &s = m_calculation->storage();
    connect(&s, &storage::StorageControl::errorOccurred, this, &WorkbenchModel::storageError);
    connect(&s, &storage::StorageControl::started, this, &WorkbenchModel::storageStarted);
    connect(&s, &storage::StorageControl::stopped, this, &WorkbenchModel::storageStopped);
    connect(m_visualization.get(), &visualization::VisualizationControl::modelLoaded, this,
            &WorkbenchModel::modelLoaded);
    connect(m_visualization.get(), &visualization::VisualizationControl::selectionUpdated, this,
            [this](const visualization::PickedPointInfo &) { emit dataChanged(); });
    connect(m_visualization.get(), &visualization::VisualizationControl::pointPicked, this,
            [this](const visualization::PickedPointInfo &) { emit dataChanged(); });
    connect(m_visualization.get(), &visualization::VisualizationControl::keyPointsChanged, this,
            &WorkbenchModel::dataChanged);
    connect(m_visualization.get(), &visualization::VisualizationControl::displayedPointsChanged,
            this, &WorkbenchModel::dataChanged);
    connect(m_visualization.get(), &visualization::VisualizationControl::displayedPointsUpdated,
            this, &WorkbenchModel::dataChanged);
    connect(m_visualization.get(), &visualization::VisualizationControl::scalarColorSettingsChanged,
            this, [this](const visualization::ScalarColorSettings &) { emit dataChanged(); });
    connect(m_visualization.get(), &visualization::VisualizationControl::errorOccurred, this,
            &WorkbenchModel::notice);
    m_baselineTimer.setInterval(100);
    connect(&m_baselineTimer, &QTimer::timeout, this, [this] {
        if (m_baselineClock.isValid() && m_baselineClock.elapsed() > m_baselineTimeout) {
            m_baselineClock.invalidate();
            notice(tr("置零失败：采样超时"));
        }
    });
    m_baselineTimer.start();
    m_configuration.strainCalibration.A = 1.0;
    QString error;
    Calculation::CalculationConfiguration saved;
    if (m_parameters.read(&saved, true, &error)) {
        config::IniConfigStore store(config::ParameterStore::runtimePath());
        QVariant identity;
        if (store.readValue("classic.baseline", "identity", &identity))
            m_activeIdentity = identity.toString();
        applyCalibration(saved, &error);
    }
}
WorkbenchModel::~WorkbenchModel() {
    m_baselineTimer.stop();
    m_visualization->detach();
    m_calculation.reset();
    m_protocol.reset();
}
void WorkbenchModel::initialize() {
    m_visualization->setfreshFPS(40);
    emit stateChanged();
    emit configurationChanged();
    emit dataChanged();
    notice(tr("就绪：连接设备，采样置零后加载模型。"));
}
bool WorkbenchModel::attach(vtkRenderWindow *window) {
    return m_visualization->attach(window);
}
WorkbenchState WorkbenchModel::state() const {
    WorkbenchState s;
    s.hasModel = bool(originalModel());
    s.solving = m_solving;
    s.configurationPending = m_configurationPending;
    s.modelPending = m_modelPending;
    s.display = m_running ? DisplayState::Running
                          : m_displayPaused ? DisplayState::Paused : DisplayState::Stopped;
    s.recording = m_storagePending
                      ? (m_recording ? RecordingState::Stopping : RecordingState::Starting)
                      : m_recording ? RecordingState::Recording : RecordingState::Stopped;
    return s;
}
void WorkbenchModel::notice(const QString &s) {
    if (s != m_lastNotice) {
        m_lastNotice = s;
        emit messageChanged(s);
    }
}
void WorkbenchModel::transportState(communication::Channel c, communication::ConnectionState s) {
    m_states[int(c)] = s;
    if (s == communication::ConnectionState::Closed && m_baselineClock.isValid()) {
        m_baselineClock.invalidate();
        notice(tr("置零取消：通信已关闭"));
    }
    emit stateChanged();
}

void WorkbenchModel::frameReceived(const core::FrameSnapshotPtr &frame) {
    m_frame = frame;
    emit frameObserved(frame);
    auto wavelengths =
        frame ? std::dynamic_pointer_cast<const core::WavelengthFrame>(frame->data) : nullptr;
    QString layout;
    if (wavelengths)
        for (const auto &r : wavelengths->records)
            layout += QString("/%1:%2").arg(r.channel).arg(r.sensor);
    if (!m_activeIdentity.isEmpty() && layout != m_activeIdentity && !m_baselineClock.isValid()) {
        m_calculation->setRunning(false);
        m_solving = false;
        m_visualization->setResultRepository(nullptr);
        emit stateChanged();
        notice(tr("传感器布局改变，请重新置零并启动解算"));
    }
    if (!m_baselineClock.isValid())
        return;
    auto data =
        frame ? std::dynamic_pointer_cast<const core::WavelengthFrame>(frame->data) : nullptr;
    QString identity = frame ? frame->protocolId + frame->source.address +
                                   QString::number(frame->source.port) +
                                   QString::number(int(frame->source.transport))
                             : QString();
    if (data)
        for (const auto &r : data->records)
            identity += QString("/%1:%2").arg(r.channel).arg(r.sensor);
    if (!data || data->records.isEmpty() || (m_baselineCount && identity != m_baselineIdentity)) {
        m_baselineClock.invalidate();
        notice(tr("置零失败：来源或传感器布局改变"));
        return;
    }
    if (frame->revision == m_baselineRevision)
        return;
    for (const auto &r : data->records)
        if (!std::isfinite(r.wavelength) || r.wavelength <= 0) {
            m_baselineClock.invalidate();
            notice(tr("置零失败：无效波长"));
            return;
        }
    if (!m_baselineCount) {
        m_baselineIdentity = identity;
        m_baselineSum.fill(0, data->records.size());
    }
    m_baselineRevision = frame->revision;
    for (int i = 0; i < data->records.size(); ++i)
        m_baselineSum[i] += data->records[i].wavelength;
    ++m_baselineCount;
    notice(tr("置零采样 %1/%2").arg(m_baselineCount).arg(m_baselineTarget));
    if (m_baselineCount == m_baselineTarget) {
        m_baselineClock.invalidate();
        auto c = m_configuration;
        c.strainCalibration.originalWavelength.clear();
        for (double sum : m_baselineSum)
            c.strainCalibration.originalWavelength.push_back(sum / m_baselineTarget);
        if (c.channelCalibration.channelIndices.empty())
            for (int i = 0; i < qMin(12, m_baselineSum.size()); ++i) {
                c.channelCalibration.channelIndices.push_back(i);
                c.channelCalibration.calibrationFactors.push_back(1.0);
            }
        QString error;
        m_pendingIdentity = layout;
        if (!applyCalibration(c, &error)) {
            m_pendingIdentity.clear();
            notice(error);
        }
    }
}
void WorkbenchModel::hardwareReady(const core::CalibratedDataPtr &data) {
    if (m_solving && !m_configurationPending && data && data->sourceFrame &&
        data->sourceFrame->revision > m_runStartFrameRevision &&
        data->configurationRevision == m_configurationRevision)
        m_hardware = data;
}

void WorkbenchModel::resultReady(const core::CalculationResultPtr &result) {
    if (m_solving && !m_modelPending && !m_configurationPending && result && result->sourceFrame &&
        result->sourceFrame->revision > m_runStartFrameRevision &&
        result->modelRevision == m_modelRevision &&
        result->configurationRevision == m_configurationRevision) {
        m_result = result;
        if (m_running)
            m_visualization->setResultRepository(m_calculation->dataModel());
    }
}

void WorkbenchModel::modelLoaded(const core::ModelDataPtr &model) {
    m_running = false;
    m_displayPaused = false;
    m_result.reset();
    m_modelPending = true;
    m_modelRequests.enqueue(++m_modelGeneration);
    // Until the worker acknowledges the replacement, do not apply a stale model result.
    m_visualization->setResultRepository(nullptr);
    m_calculation->setModel(*model);
    emit stateChanged();
}

void WorkbenchModel::modelAccepted(quint64 revision) {
    if (m_modelRequests.isEmpty())
        return;
    const auto generation = m_modelRequests.dequeue();
    if (generation != m_modelGeneration || !m_visualization->originalModel())
        return;
    m_modelRevision = revision;
    m_modelPending = false;
    m_visualization->setExpectedModelRevision(revision);
    emit stateChanged();
}

void WorkbenchModel::configurationAccepted(quint64 revision) {
    m_configuration = m_pendingConfiguration;
    QString savedError;
    if (!m_pendingIdentity.isEmpty()) {
        m_activeIdentity = m_pendingIdentity;
        m_pendingIdentity.clear();
        config::IniConfigStore store(config::ParameterStore::runtimePath());
        QString error;
        if (!store.writeValue("classic.baseline", "identity", m_activeIdentity, &error))
            savedError = error;
    }
    m_configurationRevision = revision;
    m_configurationPending = false;
    emit configurationChanged();
    emit configurationApplied(revision);
    emit stateChanged();
    notice(savedError.isEmpty() ? tr("解算配置已生效并保存到 INI")
                                : tr("基线已生效，身份保存失败：%1").arg(savedError));
}

bool WorkbenchModel::applyCalibration(const Calculation::CalculationConfiguration &configuration,
                                      QString *error) {
    if (m_configurationPending) {
        if (error)
            *error = tr("上一项解算配置仍在提交中");
        return false;
    }
    Calculation::StrainCalibration validation;
    validation.setCalibrationStrainData(configuration.strainCalibration);
    validation.setCalibrationChannelData(configuration.channelCalibration);
    if (!validation.checkPara()) {
        if (error)
            *error = validation.lastError();
        return false;
    }
    m_pendingConfiguration = configuration;
    m_configurationPending = true;
    m_result.reset();
    m_visualization->setResultRepository(nullptr);
    m_calculation->setConfiguration(configuration);
    emit stateChanged();
    return true;
}

bool WorkbenchModel::baseline(QString *error) {
    if (m_baselineClock.isValid() || m_configurationPending) {
        if (error)
            *error = tr("请等待当前置零或配置提交完成");
        return false;
    }
    if (!m_frame) {
        if (error)
            *error = tr("尚未接收到波长帧");
        return false;
    }
    config::IniConfigStore store(config::ParameterStore::runtimePath());
    config::ConfigSection sampling;
    if (!store.ensureModule("classic.baseline", {{"sampleCount", 20}, {"timeoutMs", 10000}},
                            error) ||
        !store.readModule("classic.baseline", &sampling, error))
        return false;
    bool countOk = false, timeoutOk = false;
    const int count = sampling.value("sampleCount").toInt(&countOk);
    const int timeout = sampling.value("timeoutMs").toInt(&timeoutOk);
    if (!countOk || count < 1 || count > 10000 || !timeoutOk || timeout < 100 ||
        timeout > 3600000) {
        if (error)
            *error = tr("置零配置无效：sampleCount应为1～10000，timeoutMs应为100～3600000");
        return false;
    }
    m_baselineTarget = count;
    m_baselineTimeout = timeout;
    m_baselineCount = 0;
    m_baselineRevision = 0;
    m_baselineSum.clear();
    m_baselineIdentity.clear();
    m_baselineClock.start();
    if (error)
        *error = tr("开始采样置零：等待%1个有效帧").arg(m_baselineTarget);
    return true;
}

void WorkbenchModel::calculationError(const Calculation::CalculationError &error) {
    if (error.operation == "configuration") {
        m_configurationPending = false;
        m_pendingIdentity.clear();
    }
    if (error.operation == "model" && !m_modelRequests.isEmpty()) {
        if (m_modelRequests.dequeue() == m_modelGeneration)
            m_modelPending = false;
    }
    emit stateChanged();
    notice(tr("解算失败（%1）：%2").arg(error.operation, error.message));
}

void WorkbenchModel::storageError(const storage::StorageError &error) {
    m_recording = false;
    m_storagePending = false;
    emit stateChanged();
    notice(tr("存储失败（%1）：%2").arg(error.operation, error.message));
}

void WorkbenchModel::storageStarted(const QString &session) {
    m_storagePending = false;
    m_recording = true;
    emit stateChanged();
    notice(tr("存储已启动：%1").arg(session));
}

void WorkbenchModel::storageStopped() {
    m_recording = false;
    m_storagePending = false;
    emit stateChanged();
    notice(tr("存储已停止"));
}
bool WorkbenchModel::startStorage(const QString &path, QString *error) {
    if (m_recording || m_storagePending) {
        if (error)
            *error = tr("已有存储会话正在运行");
        return false;
    }
    if (path.trimmed().isEmpty()) {
        if (error)
            *error = tr("请选择输出文件");
        return false;
    }
    storage::StorageConfiguration configuration;
    configuration.path = path;
    const auto suffix = QFileInfo(path).suffix().toLower();
    if (suffix != "csv" && suffix != "db" && suffix != "sqlite") {
        if (error)
            *error = tr("请使用 .csv、.sqlite 或 .db 扩展名");
        return false;
    }
    configuration.format =
        suffix == "csv" ? storage::StorageFormat::Csv : storage::StorageFormat::SQLite;
    configuration.dataTypes = storage::DataType::Wavelength |
                              storage::DataType::CalibratedHardware |
                              storage::DataType::PointDisplacement;
    configuration.allPoints = true;
    m_storagePending = true;
    emit stateChanged();
    m_calculation->storage().start(configuration);
    return true;
}
bool WorkbenchModel::sendSerialCommand(const QString &command, QString *error) {
    auto bytes = command.toUtf8();
    while (bytes.endsWith('\r') || bytes.endsWith('\n'))
        bytes.chop(1);
    if (bytes.trimmed().isEmpty()) {
        if (error)
            *error = tr("请输入串口发送指令");
        return false;
    }
    if (m_states[int(Channel::Serial)] != ConnectionState::Connected) {
        if (error)
            *error = tr("请先打开串口");
        return false;
    }
    bytes.append("\r\n");
    m_protocol->communication().send(Channel::Serial, bytes);
    return true;
}

bool WorkbenchModel::loadCalibration(QString *error) {
    Calculation::CalculationConfiguration c;
    return m_parameters.read(&c, true, error) && applyCalibration(c, error);
}
bool WorkbenchModel::setCalculationRunning(bool enabled, QString *error) {
    if (enabled && (m_configurationPending || !m_configurationRevision)) {
        if (error)
            *error = tr("请先完成置零或读取有效配置");
        return false;
    }
    m_solving = enabled;
    if (enabled) {
        const auto latest = m_protocol->dataModel()->latest();
        m_runStartFrameRevision = latest ? latest->revision : 0;
    }
    m_calculation->setRunning(enabled);
    if (!enabled) {
        m_visualization->setResultRepository(nullptr);
        m_result.reset();
    }
    emit stateChanged();
    return true;
}
bool WorkbenchModel::setDisplayState(DisplayState state, QString *error) {
    if (!originalModel()) {
        if (error)
            *error = tr("请先载入模型");
        return false;
    }
    m_running = state == DisplayState::Running;
    m_displayPaused = state == DisplayState::Paused;
    m_visualization->setResultRepository(m_running && !m_modelPending && !m_configurationPending &&
                                                 m_result
                                             ? m_calculation->dataModel()
                                             : nullptr);
    emit stateChanged();
    return true;
}
bool WorkbenchModel::setRecording(bool enabled, const QString &path, QString *error) {
    if (m_storagePending) {
        if (error)
            *error = tr("请等待保存操作完成");
        return false;
    }
    if (enabled)
        return startStorage(path, error);
    if (m_recording) {
        m_storagePending = true;
        emit stateChanged();
        m_calculation->storage().stop();
    }
    return true;
}
void WorkbenchModel::clearModel() {
    m_visualization->setResultRepository(nullptr);
    m_visualization->unloadModel();
    m_calculation->clearModel();
    m_result.reset();
    m_modelRevision = 0;
    m_modelPending = false;
    m_running = false;
    m_displayPaused = false;
    ++m_modelGeneration;
    emit stateChanged();
    emit dataChanged();
}
void WorkbenchModel::resetDeformation() {
    m_visualization->resetDeformation();
    setDisplayState(DisplayState::Stopped);
    emit dataChanged();
}

} // namespace application
} // namespace fosd
