#include "FOSD-Classic-QWidgets/application/runtime/WorkbenchRuntime.h"
#include "FOSD-Classic-QWidgets/common/CoordinateText.h"
#include <QFile>
#include <QNetworkInterface>
#include <QSerialPortInfo>
#include <cmath>

namespace fosd {
namespace application {
using namespace ui::classic;
WorkbenchRuntime::WorkbenchRuntime(QObject *parent)
    : WorkbenchServices(parent), m_protocol(new protocol::ProtocolControl),
      m_calculation(new Calculation::CalculationControl(m_protocol->dataModel())),
      m_visualization(new visualization::VisualizationControl) {
    m_historyClock.start();
    qRegisterMetaType<PanelId>("fosd::ui::classic::PanelId");
    qRegisterMetaType<UiResponse>("fosd::ui::classic::UiResponse");
    auto &communication = m_protocol->communication();
    connect(&communication, &communication::ConnectControl::stateChanged, this,
            &WorkbenchRuntime::transportState);
    connect(&communication, &communication::ConnectControl::rawDataReceived, this,
            &WorkbenchRuntime::rawReceived);
    connect(&communication, &communication::ConnectControl::requestSend, this,
            &WorkbenchRuntime::rawSent);
    connect(&communication, &communication::ConnectControl::requestOpenSerial, this,
            [this](const communication::SerialConfig &value) { m_logSerial = value; });
    connect(&communication, &communication::ConnectControl::requestOpenUdp, this,
            [this](const communication::UdpConfig &value) { m_logUdp = value; });
    connect(&communication, &communication::ConnectControl::requestOpenTcp, this,
            [this](const communication::TcpConfig &value) { m_logTcp = value; m_logTcpPeer = {}; });
    QString logConfigError;
    m_parameters.read(&m_logSerial, false, &logConfigError);
    m_parameters.read(&m_logUdp, false, &logConfigError);
    m_parameters.read(&m_logTcp, false, &logConfigError);
    connect(&communication, &communication::ConnectControl::errorOccurred, this,
            &WorkbenchRuntime::transportError);
    connect(&communication, &communication::ConnectControl::decoderErrorOccurred, this,
            &WorkbenchRuntime::decoderError);
    connect(m_protocol.get(), &protocol::ProtocolControl::errorOccurred, this,
            &WorkbenchRuntime::protocolError);
    connect(m_protocol.get(), &protocol::ProtocolControl::frameParsed, this,
            &WorkbenchRuntime::frameReceived);
    connect(m_protocol.get(), &protocol::ProtocolControl::frameParsed, &m_calculation->storage(),
            &storage::StorageControl::writeWavelengthFrame);
    connect(m_calculation.get(), &Calculation::CalculationControl::calibratedReady, this,
            &WorkbenchRuntime::hardwareReady);
    connect(m_calculation.get(), &Calculation::CalculationControl::resultReady, this,
            &WorkbenchRuntime::resultReady);
    connect(m_calculation.get(), &Calculation::CalculationControl::modelAccepted, this,
            &WorkbenchRuntime::modelAccepted);
    connect(m_calculation.get(), &Calculation::CalculationControl::configurationAccepted, this,
            &WorkbenchRuntime::configurationAccepted);
    connect(m_calculation.get(), &Calculation::CalculationControl::errorOccurred, this,
            &WorkbenchRuntime::calculationError);
    auto &storage = m_calculation->storage();
    connect(&storage, &storage::StorageControl::errorOccurred, this,
            &WorkbenchRuntime::storageError);
    connect(&storage, &storage::StorageControl::started, this, &WorkbenchRuntime::storageStarted);
    connect(&storage, &storage::StorageControl::stopped, this, &WorkbenchRuntime::storageStopped);
    connect(m_visualization.get(), &visualization::VisualizationControl::modelLoaded, this,
            &WorkbenchRuntime::modelLoaded);
    connect(m_visualization.get(), &visualization::VisualizationControl::selectionUpdated, this,
            &WorkbenchRuntime::selectionChanged);
    connect(m_visualization.get(), &visualization::VisualizationControl::pointPicked, this,
            &WorkbenchRuntime::selectionChanged);
    connect(m_visualization.get(), &visualization::VisualizationControl::keyPointsChanged, this,
            &WorkbenchRuntime::publishMarkers);
    connect(m_visualization.get(), &visualization::VisualizationControl::displayedPointsChanged, this,
            &WorkbenchRuntime::publishDisplayedPoints);
    connect(m_visualization.get(), &visualization::VisualizationControl::displayedPointsUpdated, this,
            &WorkbenchRuntime::publishDisplayedPoints);
    connect(m_visualization.get(), &visualization::VisualizationControl::errorOccurred, this,
            &WorkbenchRuntime::notice);
    connect(m_visualization.get(), &visualization::VisualizationControl::scalarColorSettingsChanged,
            this, &WorkbenchRuntime::publishScalar);
    m_visualization->setResultRepository(m_calculation->dataModel());
    m_timer.setInterval(100);
    connect(&m_timer, &QTimer::timeout, this, &WorkbenchRuntime::publishLatest);
    m_timer.start();
    m_receiveLogCleanupTimer.setParent(this);
    m_receiveLogCleanupTimer.setObjectName("receiveLogCleanupTimer");
    m_receiveLogCleanupTimer.setInterval(60000);
    connect(&m_receiveLogCleanupTimer, &QTimer::timeout, this, &WorkbenchRuntime::clearReceiveLogs);
    m_receiveLogCleanupTimer.start();
    m_configuration.strainCalibration.A = 1.0;
    QString error;
    Calculation::CalculationConfiguration saved;
    if (m_parameters.read(&saved, true, &error)) {
        config::IniConfigStore store(config::ParameterStore::runtimePath()); QVariant identity;
        if(store.readValue("classic.baseline","identity",&identity)) m_activeIdentity=identity.toString();
        configure(saved, &error);
    }
}

WorkbenchRuntime::~WorkbenchRuntime() {
    m_timer.stop();
    m_visualization->detach();
    // Stop the polling/writer thread before destroying its input repository.
    m_calculation.reset();
    m_protocol.reset();
}

bool WorkbenchRuntime::attach(vtkRenderWindow *window) {
    return m_visualization->attach(window);
}

QString WorkbenchRuntime::text(const QVariantMap &fields, const QString &name) {
    const auto value = fields.value(name);
    return value.type() == QVariant::Map ? value.toMap().value("text").toString()
                                         : value.toString();
}

int WorkbenchRuntime::index(const QVariantMap &fields, const QString &name) {
    return fields.value(name).toMap().value("index", -1).toInt();
}

bool WorkbenchRuntime::handleRequest(const UiRequest &request, UiResponse *response) {
    if (!response)
        return false;
    *response = UiResponse();
    response->id = request.id;
    response->panel = request.panel;
    const bool handled = connectionAction(request, response) ||
                         visualizationAction(request, response) || 
                         dataAction(request, response);
    if (handled && !response->success && !response->fields.isEmpty())
        emit presentationChanged(request.panel, response->fields);
    return handled;
}

void WorkbenchRuntime::initialize() {
    initializePorts();
    communication::SerialConfig serial;
    QString loadError;
    if (m_parameters.read(&serial, false, &loadError)) {
        const QMap<int, QString> parities{{QSerialPort::NoParity,"None"},{QSerialPort::OddParity,"Odd"},
            {QSerialPort::EvenParity,"Even"},{QSerialPort::MarkParity,"Mark"},{QSerialPort::SpaceParity,"Space"}};
        const QMap<int, QString> flows{{QSerialPort::NoFlowControl,"None"},{QSerialPort::HardwareControl,"Hardware"},{QSerialPort::SoftwareControl,"Software"}};
        emit presentationChanged(PanelId::Connection,
            {{"cbox_SerialBaudChoose",QVariantMap{{"text",QString::number(serial.baudRate)}}},
             {"edit_SerialDataBits",QString::number(int(serial.dataBits))},
             {"edit_SerialParity",parities.value(serial.parity)},
             {"edit_SerialStopBits",serial.stopBits == QSerialPort::OneAndHalfStop ? "1.5" : serial.stopBits == QSerialPort::TwoStop ? "2" : "1"},
             {"edit_SerialFlowControl",flows.value(serial.flowControl)}});
    }

    config::IniConfigStore store(config::ParameterStore::runtimePath()); QVariant channels;
    QVariantMap commands;
    for (int i = 0; i < 10; ++i) {
        QVariant command;
        if (store.readValue("classic.serialCommands", QString::number(i), &command))
            commands[QStringLiteral("edit_SerialCommand%1").arg(i)] = command;
    }
    if (!commands.isEmpty())
        emit presentationChanged(PanelId::Connection, commands);
    if(store.readValue("classic.display","channels",&channels)) {
        m_filterChannels=true; m_channels.clear(); for(const auto &s:channels.toStringList()) if(s.toInt()>=1 && s.toInt()<=16) m_channels.insert(s.toInt());
    }
    QVariant networkMode;
    int mode = 0;
    if (store.readValue("classic.connection", "networkMode", &networkMode) &&
        networkMode.toInt() >= 0 && networkMode.toInt() <= 2)
        mode = networkMode.toInt();
    publishNetworkConfiguration(mode);
    QVariantMap fields;
    for (int channel = 1; channel <= 16; ++channel)
        fields[QStringLiteral("ckbox_CH%1").arg(channel)] = !m_filterChannels || m_channels.contains(channel);
    emit presentationChanged(PanelId::Connection, fields);
    emit presentationChanged(PanelId::ModelPanel,
                             {{"cbox_ScalarSelect", QVariantMap{{"index", 3}}}});
    emit presentationChanged(PanelId::HardwareData, {{"checkBox", true}});
    emit presentationChanged(PanelId::KeyPointSelect,
                             {{"radioButton", true}, {"radioButton_3", true}});

    m_visualization->setfreshFPS(40);
    emit presentationChanged(PanelId::ModelPanel, {{"cbox_FPS", QVariantMap{{"index", 2}}}});
    const auto axes = m_visualization->axesSettings();
    const auto grid = m_visualization->gridSettings();
    emit presentationChanged(
        PanelId::ScenePanel,
        {{"cbox_AxisAble", axes.visible},
         {"hsd_AxisSize", qRound(100 * std::sqrt(axes.lengthMm / 50000.0))},
         {"cbox_GridAble", grid.visible[0] || grid.visible[1] || grid.visible[2]},
         {"cbox_XOZGrid", grid.visible[0]},
         {"cbox_XOYGrid", grid.visible[1]},
         {"cbox_ZOYGrid", grid.visible[2]},
         {"hsd_GridSize", qRound(grid.halfExtentMm / 150.0)},
         {"cbox_CareamLockAble", m_visualization->cameraSettings().locked}});
    publishCalibration();
    publishRunState();
    publishModel();
    publishScalar();
    publishAppearance();
    notice(tr("就绪：连接设备，采样置零后加载模型。"));
}

void WorkbenchRuntime::initializePorts(const QString &serialPort, const QString &localAddress) {
    QStringList ports, labels;
    QVariantList data;
    for (const auto &port : QSerialPortInfo::availablePorts()) {
        ports.append(port.portName());
        labels.append(port.portName() + ":" + (port.description().isEmpty() ? tr("串口设备") : port.description()));
        data.append(port.portName());
    }
    communication::SerialConfig saved;
    QString error;
    m_parameters.read(&saved, false, &error);
    const auto preferred = serialPort.isEmpty() ? saved.portName : serialPort;
    int selected = ports.indexOf(preferred);
    if (selected < 0) selected = ports.isEmpty() ? -1 : 0;
    emit presentationChanged(PanelId::Connection,
        {{"cbox_SerialNameChoose",QVariantMap{{"items",labels},{"data",data},{"index",selected}}}});
    initializeLocalAddresses(localAddress);
}

void WorkbenchRuntime::initializeLocalAddresses(const QString &localAddress) {
    QStringList addresses{QStringLiteral("0.0.0.0"), QStringLiteral("127.0.0.1")};
    for (const auto &address : QNetworkInterface::allAddresses())
        if (address.protocol() == QAbstractSocket::IPv4Protocol && !addresses.contains(address.toString()))
            addresses.append(address.toString());
    const int localIndex = addresses.indexOf(localAddress);
    emit presentationChanged(PanelId::Connection, {{"cbox_LocakIP",QVariantMap{{"items",addresses},{"index",localIndex < 0 ? 0 : localIndex}}}});
}

void WorkbenchRuntime::publishNetworkConfiguration(int mode) {
    m_networkChannel = mode == 0 ? communication::Channel::Udp : communication::Channel::Tcp;
    m_tcpRole = mode == 1 ? communication::TcpRole::Server : communication::TcpRole::Client;
    QVariantMap fields{{"cbox_CommuteChoose",QVariantMap{{"index",mode}}},
        {"cbox_LocakIP.enabled",mode != 2},{"edit_LocalPort.enabled",mode != 2},
        {"edit_TargetIP.enabled",mode != 1},{"edit_TargetPort.enabled",mode != 1}};
    QString error;
    if (mode == 0) {
        communication::UdpConfig saved;
        if (m_parameters.read(&saved, false, &error)) {
            fields["cbox_LocakIP"] = QVariantMap{{"text",saved.bindAddress.toString()}};
            fields["edit_LocalPort"] = QString::number(saved.localPort);
            if (saved.defaultPeer.isValid()) {
                fields["edit_TargetIP"] = saved.defaultPeer.address.toString();
                fields["edit_TargetPort"] = QString::number(saved.defaultPeer.port);
            }
        }
    } else {
        communication::TcpConfig saved;
        if (m_parameters.read(&saved, false, &error)) {
            fields["cbox_LocakIP"] = QVariantMap{{"text",saved.listenAddress.toString()}};
            fields["edit_TargetIP"] = saved.host;
            if (saved.port)
                fields[mode == 1 ? "edit_LocalPort" : "edit_TargetPort"] = QString::number(saved.port);
        }
    }
    emit presentationChanged(PanelId::Connection, fields);
}

void WorkbenchRuntime::frameReceived(const core::FrameSnapshotPtr &frame) {
    m_frame = frame;
    if (frame && frame->source.transport == core::TransportKind::Udp) {
        m_udpLogFrame = frame;
        m_udpLogPending = true;
    }
    auto wavelengths = frame ? std::dynamic_pointer_cast<const core::WavelengthFrame>(frame->data) : nullptr;
    QString layout;
    if(wavelengths) for(const auto &r:wavelengths->records) layout += QString("/%1:%2").arg(r.channel).arg(r.sensor);
    if(!m_activeIdentity.isEmpty() && layout != m_activeIdentity && !m_baselineClock.isValid()) {
        m_calculation->setRunning(false); m_solving=false; m_visualization->setResultRepository(nullptr); notice(tr("传感器布局改变，请重新置零并启动解算"));
    }
    if (!m_baselineClock.isValid()) return;
    auto data = frame ? std::dynamic_pointer_cast<const core::WavelengthFrame>(frame->data) : nullptr;
    QString identity = frame ? frame->protocolId + frame->source.address + QString::number(frame->source.port) + QString::number(int(frame->source.transport)) : QString();
    if (data) for (const auto &r : data->records) identity += QString("/%1:%2").arg(r.channel).arg(r.sensor);
    if (!data || data->records.isEmpty() || (m_baselineCount && identity != m_baselineIdentity)) {
        m_baselineClock.invalidate(); notice(tr("置零失败：来源或传感器布局改变")); return;
    }
    if (frame->revision == m_baselineRevision) return;
    for (const auto &r : data->records) if (!std::isfinite(r.wavelength) || r.wavelength <= 0) {
        m_baselineClock.invalidate(); notice(tr("置零失败：无效波长")); return;
    }
    if (!m_baselineCount) { m_baselineIdentity = identity; m_baselineSum.fill(0, data->records.size()); }
    m_baselineRevision = frame->revision;
    for (int i=0;i<data->records.size();++i) m_baselineSum[i] += data->records[i].wavelength;
    ++m_baselineCount;
    notice(tr("置零采样 %1/%2").arg(m_baselineCount).arg(m_baselineTarget));
    if (m_baselineCount == m_baselineTarget) {
        m_baselineClock.invalidate();
        auto c = m_configuration;
        c.strainCalibration.originalWavelength.clear();
        for (double sum : m_baselineSum) c.strainCalibration.originalWavelength.push_back(sum / m_baselineTarget);
        if (c.channelCalibration.channelIndices.empty()) for (int i=0;i<qMin(12,m_baselineSum.size());++i) {
            c.channelCalibration.channelIndices.push_back(i); c.channelCalibration.calibrationFactors.push_back(1.0);
        }
        QString error;
        m_pendingIdentity=layout;
        if (!configure(c, &error)) { m_pendingIdentity.clear(); notice(error); }
    }
}

void WorkbenchRuntime::hardwareReady(const core::CalibratedDataPtr &data) {
    if (m_solving && !m_configurationPending && data && data->sourceFrame &&
        data->sourceFrame->revision > m_runStartFrameRevision && data->configurationRevision == m_configurationRevision)
        m_hardware = data;
}

void WorkbenchRuntime::resultReady(const core::CalculationResultPtr &result) {
    if (m_solving && !m_modelPending && !m_configurationPending && result && result->sourceFrame &&
        result->sourceFrame->revision > m_runStartFrameRevision &&
        result->modelRevision == m_modelRevision &&
        result->configurationRevision == m_configurationRevision) {
        m_result = result;
        if (m_running)
            m_visualization->setResultRepository(m_calculation->dataModel());
    }
}

void WorkbenchRuntime::modelLoaded(const core::ModelDataPtr &model) {
    m_running = false;
    m_displayPaused = false;
    publishRunState();
    m_result.reset();
    m_modelPending = true;
    m_modelRequests.enqueue(++m_modelGeneration);
    // Until the worker acknowledges the replacement, do not apply a stale model result.
    m_visualization->setResultRepository(nullptr);
    m_calculation->setModel(*model);
    publishModel();
}

void WorkbenchRuntime::modelAccepted(quint64 revision) {
    if (m_modelRequests.isEmpty())
        return;
    const auto generation = m_modelRequests.dequeue();
    if (generation != m_modelGeneration || !m_visualization->originalModel())
        return;
    m_modelRevision = revision;
    m_modelPending = false;
    m_visualization->setExpectedModelRevision(revision);
}

void WorkbenchRuntime::configurationAccepted(quint64 revision) {
    m_configuration = m_pendingConfiguration;
    publishCalibration();
    QString savedError;
    if(!m_pendingIdentity.isEmpty()) {
        m_activeIdentity=m_pendingIdentity; m_pendingIdentity.clear();
        config::IniConfigStore store(config::ParameterStore::runtimePath()); QString error;
        if(!store.writeValue("classic.baseline","identity",m_activeIdentity,&error)) savedError=error;
    }
    m_configurationRevision = revision;
    m_configurationPending = false;
    notice(savedError.isEmpty() ? tr("解算配置已生效并保存到 INI") : tr("基线已生效，身份保存失败：%1").arg(savedError));
}

bool WorkbenchRuntime::configure(const Calculation::CalculationConfiguration &configuration,
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
    publishCalibration();
    return true;
}

bool WorkbenchRuntime::baseline(QString *error) {
    if (m_baselineClock.isValid() || m_configurationPending) { if(error) *error=tr("请等待当前置零或配置提交完成"); return false; }
    if (!m_frame) { if(error) *error=tr("尚未接收到波长帧"); return false; }
    config::IniConfigStore store(config::ParameterStore::runtimePath());
    config::ConfigSection sampling;
    if (!store.ensureModule("classic.baseline", {{"sampleCount",20},{"timeoutMs",10000}}, error) ||
        !store.readModule("classic.baseline", &sampling, error)) return false;
    bool countOk=false, timeoutOk=false;
    const int count=sampling.value("sampleCount").toInt(&countOk);
    const int timeout=sampling.value("timeoutMs").toInt(&timeoutOk);
    if(!countOk || count<1 || count>10000 || !timeoutOk || timeout<100 || timeout>3600000) {
        if(error) *error=tr("置零配置无效：sampleCount应为1～10000，timeoutMs应为100～3600000");
        return false;
    }
    m_baselineTarget=count; m_baselineTimeout=timeout;
    m_baselineCount=0; m_baselineRevision=0; m_baselineSum.clear(); m_baselineIdentity.clear(); m_baselineClock.start();
    if(error) *error=tr("开始采样置零：等待%1个有效帧").arg(m_baselineTarget);
    return true;
}

void WorkbenchRuntime::notice(const QString &message) {
    if (message == m_lastNotice)
        return;
    m_lastNotice = message;
    emit presentationChanged(PanelId::Main, {{"_status", message}});
}

void WorkbenchRuntime::transportError(const communication::CommunicationError &error) {
    notice(tr("通信失败（%1）：%2").arg(error.operation, error.message));
}

void WorkbenchRuntime::decoderError(const communication::DecoderError &error) {
    if (error.channel == communication::Channel::Udp) {
        m_udpLogStatus = error.message;
        m_udpLogPending = true;
    }
    notice(tr("拆包 %1：%2").arg(error.operation, error.message));
}

void WorkbenchRuntime::protocolError(const protocol::ProtocolError &error) {
    if (error.channel == communication::Channel::Udp) {
        m_udpLogFrame.reset();
        m_udpLogStatus = error.message;
        m_udpLogPending = true;
    }
    notice(tr("协议 %1：%2").arg(error.protocolId, error.message));
}

void WorkbenchRuntime::calculationError(const Calculation::CalculationError &error) {
    if (error.operation == "configuration")
        { m_configurationPending = false; m_pendingIdentity.clear(); }
    if (error.operation == "model" && !m_modelRequests.isEmpty()) {
        if (m_modelRequests.dequeue() == m_modelGeneration)
            m_modelPending = false;
    }
    notice(tr("解算失败（%1）：%2").arg(error.operation, error.message));
}

void WorkbenchRuntime::storageError(const storage::StorageError &error) {
    m_recording = false;
    m_storagePending = false;
    emit presentationChanged(PanelId::Connection, {{"pbtn_SaveDynamic", false},{"pbtn_SaveDynamic.enabled",true}});
    notice(tr("存储失败（%1）：%2").arg(error.operation, error.message));
}

void WorkbenchRuntime::storageStarted(const QString &session) {
    m_storagePending = false;
    m_recording = true;
    emit presentationChanged(PanelId::Connection, {{"pbtn_SaveDynamic", true},{"pbtn_SaveDynamic.enabled",true}});
    notice(tr("存储已启动：%1").arg(session));
}

void WorkbenchRuntime::storageStopped() {
    m_recording = false;
    m_storagePending = false;
    emit presentationChanged(PanelId::Connection, {{"pbtn_SaveDynamic", false},{"pbtn_SaveDynamic.enabled",true}});
    notice(tr("存储已停止"));
}

void WorkbenchRuntime::selectionChanged(const visualization::PickedPointInfo &info) {
    m_selectedPoint = info.valid ? info.pointId : -1;
    emit presentationChanged(PanelId::Visualization,
        {{"statusBar_SelectedPoint", selectedPointStatus(
            info.valid ? QString::number(qlonglong(info.pointId)) : QString(),
            info.originalPositionMm, info.displayPositionMm)}});
    if (info.valid) {
        emit presentationChanged(PanelId::KeyPointEdit,
            {{"edit_IDKeyPoint", QString::number(qlonglong(info.pointId))},
             {"lab_XPosition", QString::number(info.originalPositionMm[0], 'f', 2)},
             {"lab_YPosition", QString::number(info.originalPositionMm[1], 'f', 2)},
             {"lab_ZPosition", QString::number(info.originalPositionMm[2], 'f', 2)}});
    } else {
        emit presentationChanged(PanelId::KeyPointEdit,
            {{"lab_XPosition", QString()}, {"lab_YPosition", QString()}, {"lab_ZPosition", QString()}});
    }
}

void WorkbenchRuntime::publishRunState() {
    const bool model = bool(m_visualization->originalModel());
    emit presentationChanged(
        PanelId::Visualization,
        {{"actionStart.enabled", model && !m_running},
         {"actionPause.enabled", model && m_running},
         {"actionStop.enabled", model && (m_running || m_displayPaused)},
         {"actionReset.enabled", model},
         {"_status", m_running ? tr("实时变形显示") : tr("显示未启动或已暂停；通信、解算和存储继续运行")}});
}
} // namespace application
} // namespace fosd
