#include "WorkbenchRuntime.h"
#include "calculation/PlateIFE.h"
#include <QFile>
#include <QNetworkInterface>
#include <QSerialPortInfo>
#include <cmath>

// Referencing the resource initializer also retains it when linking this static library.
static void initializeWorkbenchResources() {
    Q_INIT_RESOURCE(workbench);
}

namespace fosd {
namespace application {
using namespace ui::classic;
WorkbenchRuntime::WorkbenchRuntime(QObject *parent)
    : WorkbenchServices(parent), m_protocol(new protocol::ProtocolControl),
      m_calculation(new Calculation::CalculationControl(m_protocol->dataModel())),
      m_visualization(new visualization::VisualizationControl) {
    initializeWorkbenchResources();
    qRegisterMetaType<PanelId>("fosd::ui::classic::PanelId");
    qRegisterMetaType<UiResponse>("fosd::ui::classic::UiResponse");
    auto &communication = m_protocol->communication();
    connect(&communication, &communication::ConnectControl::stateChanged, this,
            &WorkbenchRuntime::transportState);
    connect(&communication, &communication::ConnectControl::rawDataReceived, this,
            &WorkbenchRuntime::rawReceived);
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
    connect(m_visualization.get(), &visualization::VisualizationControl::errorOccurred, this,
            &WorkbenchRuntime::notice);
    connect(m_visualization.get(), &visualization::VisualizationControl::scalarColorSettingsChanged,
            this, &WorkbenchRuntime::publishScalar);
    m_visualization->setResultRepository(m_calculation->dataModel());
    m_timer.setInterval(100);
    connect(&m_timer, &QTimer::timeout, this, &WorkbenchRuntime::publishLatest);
    m_timer.start();
    m_configuration.strainCalibration.A = 1.0;
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
                         visualizationAction(request, response) || dataAction(request, response);
    if (handled && !response->success && !response->fields.isEmpty())
        emit presentationChanged(request.panel, response->fields);
    return handled;
}

void WorkbenchRuntime::initialize() {
    initializePorts();
    QStringList addresses{QStringLiteral("0.0.0.0"), QStringLiteral("127.0.0.1")};
    for (const auto &address : QNetworkInterface::allAddresses())
        if (address.protocol() == QAbstractSocket::IPv4Protocol &&
            !addresses.contains(address.toString()))
            addresses.append(address.toString());
    QVariantMap fields{{"cbox_LocakIP", QVariantMap{{"items", addresses}, {"index", 0}}}};
    for (int channel = 1; channel <= 16; ++channel)
        fields[QStringLiteral("ckbox_CH%1").arg(channel)] = true;
    emit presentationChanged(PanelId::Connection, fields);
    emit presentationChanged(PanelId::ModelPanel,
                             {{"cbox_ScalarSelect", QVariantMap{{"index", 3}}}});
    emit presentationChanged(PanelId::HardwareData, {{"checkBox", true}});
    emit presentationChanged(PanelId::KeyPointSelect,
                             {{"radioButton", true}, {"radioButton_3", true}});
    emit presentationChanged(PanelId::Connection,
                             {{"textEdit_StrainTopPara", tr("阈值业务尚未实现")},
                              {"textEdit_StrainTopPara.enabled", false},
                              {"textEdit_StrainLowPara", tr("阈值业务尚未实现")},
                              {"textEdit_StrainLowPara.enabled", false}});
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
    notice(tr("就绪：先接收波长并置零配置，或在三维工具栏选择板模型 / Plate IFE。"));
}

void WorkbenchRuntime::initializePorts() {
    QStringList ports;
    for (const auto &port : QSerialPortInfo::availablePorts())
        ports.append(port.portName());
    emit presentationChanged(
        PanelId::Connection,
        {{"cbox_SerialNameChoose",
          QVariantMap{{"items", ports}, {"index", ports.isEmpty() ? -1 : 0}}}});
}

void WorkbenchRuntime::frameReceived(const core::FrameSnapshotPtr &frame) {
    m_frame = frame;
}

void WorkbenchRuntime::hardwareReady(const core::CalibratedDataPtr &data) {
    if (!m_configurationPending && data && data->configurationRevision == m_configurationRevision)
        m_hardware = data;
}

void WorkbenchRuntime::resultReady(const core::CalculationResultPtr &result) {
    if (!m_modelPending && !m_configurationPending && result &&
        result->modelRevision == m_modelRevision &&
        result->configurationRevision == m_configurationRevision) {
        m_result = result;
        if (m_running)
            m_visualization->setResultRepository(m_calculation->dataModel());
    }
}

void WorkbenchRuntime::modelLoaded(const core::ModelDataPtr &model) {
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
    m_configurationRevision = revision;
    m_configurationPending = false;
    notice(tr("解算配置已生效"));
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
    m_configuration = configuration;
    m_configurationPending = true;
    m_result.reset();
    m_visualization->setResultRepository(nullptr);
    m_calculation->setConfiguration(configuration);
    publishCalibration();
    return true;
}

bool WorkbenchRuntime::baseline(QString *error) {
    const auto frame =
        m_frame ? std::dynamic_pointer_cast<const core::WavelengthFrame>(m_frame->data) : nullptr;
    if (!frame || frame->records.isEmpty()) {
        if (error)
            *error = tr("尚未接收到波长快报");
        return false;
    }
    auto configuration = m_configuration;
    configuration.strainCalibration.originalWavelength.clear();
    for (const auto &record : frame->records)
        configuration.strainCalibration.originalWavelength.push_back(record.wavelength);
    if (configuration.channelCalibration.channelIndices.empty()) {
        for (int i = 0; i < qMin(12, frame->records.size()); ++i) {
            configuration.channelCalibration.channelIndices.push_back(i);
            configuration.channelCalibration.calibrationFactors.push_back(1.0);
        }
    }
    return configure(configuration, error);
}

bool WorkbenchRuntime::loadPlate(QString *error) {
    if (m_configurationPending) {
        if (error)
            *error = tr("请等待上一项配置完成");
        return false;
    }
    const auto path = m_assets.filePath("plate.stl");
    if (!QFile::exists(path) && !QFile::copy(":/workbench/plate.stl", path)) {
        if (error)
            *error = tr("无法提取内置板模型");
        return false;
    }
    Calculation::CalculationConfiguration configuration;
    configuration.algorithm = Calculation::CalculationAlgorithm::Plate;
    configuration.strainCalibration.A = 1.0;
    configuration.strainCalibration.calibrationFactor = 1250.0;
    for (int i = 0; i < Calculation::PlateSpec::sensors; ++i) {
        configuration.strainCalibration.originalWavelength.push_back(1550.0 + 0.5 * i);
        configuration.channelCalibration.channelIndices.push_back(i);
        configuration.channelCalibration.calibrationFactors.push_back(1.0);
    }
    if (!m_visualization->loadModel(path)) {
        if (error)
            *error = tr("板模型载入失败");
        return false;
    }
    m_modelPath = path;
    if (!configure(configuration, error))
        return false;
    auto colors = m_visualization->scalarColorSettings();
    colors.enabled = true;
    colors.field = visualization::ScalarField::Magnitude;
    colors.rangeMode = visualization::ColorRangeMode::Automatic;
    m_visualization->setScalarColorSettings(colors);
    m_running = true;
    publishModel();
    publishRunState();
    notice(tr("Plate IFE 已选择；请在通信页打开 UDP 并用 plate-udp-sender 发送仿真波长。"));
    return true;
}

void WorkbenchRuntime::notice(const QString &message) {
    if (message == m_lastNotice)
        return;
    m_lastNotice = message;
    emit presentationChanged(PanelId::Main, {{"_status", message}});
}

void WorkbenchRuntime::transportError(const communication::CommunicationError &error) {
    notice(tr("通信 %1：%2").arg(error.operation, error.message));
}

void WorkbenchRuntime::decoderError(const communication::DecoderError &error) {
    notice(tr("拆包 %1：%2").arg(error.operation, error.message));
}

void WorkbenchRuntime::protocolError(const protocol::ProtocolError &error) {
    notice(tr("协议 %1：%2").arg(error.protocolId, error.message));
}

void WorkbenchRuntime::calculationError(const Calculation::CalculationError &error) {
    if (error.operation == "configuration")
        m_configurationPending = false;
    if (error.operation == "model" && !m_modelRequests.isEmpty()) {
        if (m_modelRequests.dequeue() == m_modelGeneration)
            m_modelPending = false;
    }
    notice(tr("解算 %1：%2").arg(error.operation, error.message));
}

void WorkbenchRuntime::storageError(const storage::StorageError &error) {
    m_recording = false;
    m_storagePending = false;
    emit presentationChanged(PanelId::Connection, {{"pbtn_SaveDynamic", false}});
    notice(tr("存储 %1：%2").arg(error.operation, error.message));
}

void WorkbenchRuntime::storageStarted(const QString &session) {
    m_storagePending = false;
    m_recording = true;
    notice(tr("存储已启动：%1").arg(session));
}

void WorkbenchRuntime::storageStopped() {
    m_recording = false;
    m_storagePending = false;
    emit presentationChanged(PanelId::Connection, {{"pbtn_SaveDynamic", false}});
    notice(tr("存储已停止"));
}

void WorkbenchRuntime::selectionChanged(const visualization::PickedPointInfo &info) {
    if (info.valid && info.pointId != m_selectedPoint) {
        m_selectedPoint = info.pointId;
        emit presentationChanged(PanelId::KeyPointEdit,
                                 {{"edit_IDKeyPoint", QString::number(qlonglong(info.pointId))}});
    } else if (!info.valid) {
        m_selectedPoint = -1;
        emit presentationChanged(PanelId::Visualization, {{"lab_KeyPointID", QString()},
                                                          {"lab_KeyPointRawPos", QString()},
                                                          {"lab_KeyPointDisplace", QString()},
                                                          {"lab_KeyPointNewPos", QString()}});
        emit presentationChanged(PanelId::KeyPointEdit, {{"lab_XPosition", QString()},
                                                         {"lab_YPosition", QString()},
                                                         {"lab_ZPosition", QString()}});
        emit presentationChanged(
            PanelId::KeyPointSelect,
            {{"lab_XPos", QString()}, {"lab_YPos", QString()}, {"lab_ZPos", QString()}});
    }
}

void WorkbenchRuntime::publishRunState() {
    emit presentationChanged(
        PanelId::Visualization,
        {{"_status", m_running ? tr("实时变形显示") : tr("显示已暂停；通信、解算和存储继续运行")}});
}
} // namespace application
} // namespace fosd
