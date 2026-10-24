#include "VtkValidationBackend.h"
#include "visualization/VisualizationControl.h"
#include <vtkActor.h>
#include <vtkPolyDataMapper.h>
#include <vtkProperty.h>
#include <vtkRenderer.h>
#include <vtkRendererCollection.h>
#include <vtkSphereSource.h>
#include <cmath>
namespace fosd { namespace vtk_test {
VtkValidationBackend::VtkValidationBackend()
    : m_protocol(new protocol::ProtocolControl),
      m_calculation(new Calculation::CalculationControl(m_protocol->dataModel())) {
    auto &connection = m_protocol->communication();
    connect(m_protocol.get(), &protocol::ProtocolControl::frameParsed,
            &m_calculation->storage(), &storage::StorageControl::writeWavelengthFrame);
    connect(&connection, &communication::ConnectControl::errorOccurred, this, &VtkValidationBackend::communicationError);
    connect(&connection, &communication::ConnectControl::decoderErrorOccurred, this, &VtkValidationBackend::decoderError);
    connect(&connection, &communication::ConnectControl::stateChanged, this, &VtkValidationBackend::connectionState);
    connect(m_protocol.get(), &protocol::ProtocolControl::errorOccurred, this, &VtkValidationBackend::protocolError);
    connect(m_calculation.get(), &Calculation::CalculationControl::errorOccurred, this, &VtkValidationBackend::calculationError);
    connect(m_calculation.get(), &Calculation::CalculationControl::modelAccepted, this, &VtkValidationBackend::modelAccepted);
    connect(&m_calculation->storage(), &storage::StorageControl::errorOccurred, this, &VtkValidationBackend::storageError);
    connect(&m_calculation->storage(), &storage::StorageControl::started, this, &VtkValidationBackend::storageStarted);
    connect(&m_calculation->storage(), &storage::StorageControl::stopped, this, &VtkValidationBackend::storageStopped);
    m_statistics.setInterval(500);
    connect(&m_statistics, &QTimer::timeout, this, &VtkValidationBackend::updateStatistics);
    m_statistics.start();
}
VtkValidationBackend::~VtkValidationBackend() {
    m_statistics.stop(); detach();
    // Stop ingress first; CalculationControl drains work and closes storage.
    m_protocol.reset(); m_calculation.reset();
}
bool VtkValidationBackend::attach(vtkRenderWindow *window, QString *error) {
    if (!window) { if (error) *error = QString::fromUtf8("渲染窗口为空"); return false; }
    if (!m_control) {
        // The exact frontend-owned pointer is handed to the current constructor.
        m_control.reset(new visualization::VisualizationControl(nullptr, window));
        m_control->setResultRepository(m_calculation->dataModel());
        connect(m_control.get(), &visualization::VisualizationControl::modelLoaded, this, &VtkValidationBackend::modelLoaded);
        connect(m_control.get(), &visualization::VisualizationControl::errorOccurred, this, &VtkValidationBackend::status);
    } else if (!m_control->attach(window)) {
        if (error) *error = QString::fromUtf8("VisualizationControl 绑定失败"); return false;
    }
    if (boundWindow() != window || !renderer(error)) {
        if (error) *error = QString::fromUtf8("渲染窗口指针或 Renderer 验证失败"); return false;
    }
    return true;
}
vtkRenderWindow *VtkValidationBackend::boundWindow() const {
    return m_control ? m_control->renderWindow() : nullptr;
}
vtkRenderer *VtkValidationBackend::renderer(QString *error) const {
    auto *window = boundWindow();
    auto *scene = window ? window->GetRenderers()->GetFirstRenderer() : nullptr;
    if (!scene && error) *error = QString::fromUtf8("请先绑定 VisualizationControl 的 Renderer");
    return scene;
}
void VtkValidationBackend::detach() {
    if (auto *scene = renderer(nullptr))
        if (m_demo) scene->RemoveActor(m_demo);
    m_demo = nullptr;
    if (m_control) m_control->detach();
}
bool VtkValidationBackend::showDemo(QString *error) {
    auto *scene = renderer(error); if (!scene) return false;
    if (!m_demo) {
        auto source = vtkSmartPointer<vtkSphereSource>::New();
        source->SetRadius(50); source->SetThetaResolution(48); source->SetPhiResolution(48);
        auto mapper = vtkSmartPointer<vtkPolyDataMapper>::New(); mapper->SetInputConnection(source->GetOutputPort());
        m_demo = vtkSmartPointer<vtkActor>::New(); m_demo->SetMapper(mapper);
        m_demo->GetProperty()->SetColor(0.95, 0.65, 0.15); scene->AddActor(m_demo);
    }
    scene->SetBackground(0.08, 0.12, 0.18); scene->ResetCamera();
    return render(error);
}
bool VtkValidationBackend::clearDemo(QString *error) {
    auto *scene = renderer(error); if (!scene) return false;
    if (m_demo) scene->RemoveActor(m_demo);
    m_demo = nullptr;
    return render(error);
}
bool VtkValidationBackend::resetCamera(QString *error) {
    if (!renderer(error)) return false;
    m_control->resetCamera(); return true;
}
bool VtkValidationBackend::render(QString *error) {
    if (!renderer(error)) return false;
    boundWindow()->Render(); return true;
}
void VtkValidationBackend::status(const QString &message) { m_status = message; emit statusChanged(message); }
void VtkValidationBackend::modelLoaded(const core::ModelDataPtr &model) {
    m_modelPending = true; m_calculation->setModel(*model);
    status(tr("模型已提交解算：%1 点，单位 mm").arg(model->size()));
}
void VtkValidationBackend::modelAccepted(quint64 revision) {
    if (!m_modelPending || !m_control) return;
    m_modelPending = false; m_control->setExpectedModelRevision(revision);
    status(tr("模型已就绪，版本 %1").arg(revision));
}
void VtkValidationBackend::communicationError(const communication::CommunicationError &e) { status("communication: " + e.message); }
void VtkValidationBackend::decoderError(const communication::DecoderError &e) { status("decoder: " + e.message); }
void VtkValidationBackend::protocolError(const protocol::ProtocolError &e) { status("protocol: " + e.message); }
void VtkValidationBackend::calculationError(const Calculation::CalculationError &e) {
    if (e.operation == "model") m_modelPending = false;
    status("calculation: " + e.message);
}
void VtkValidationBackend::storageError(const storage::StorageError &e) { status("storage: " + e.message); }
void VtkValidationBackend::storageStarted(const QString &session) { status(tr("存储已启动：%1").arg(session)); }
void VtkValidationBackend::storageStopped() { status(tr("存储已停止")); }
void VtkValidationBackend::connectionState(communication::Channel channel, communication::ConnectionState state) {
    status(tr("通信通道 %1，状态 %2").arg(int(channel)).arg(int(state)));
}
void VtkValidationBackend::updateStatistics() {
    const auto frame = m_protocol->dataModel()->latest();
    const auto result = m_calculation->dataModel()->latest();
    emit statusChanged(tr("接收帧版本 %1 | 解算结果版本 %2 | %3")
        .arg(frame ? frame->revision : 0).arg(result ? result->revision : 0).arg(m_status));
}
bool VtkValidationBackend::execute(const QString &action, const QVariantMap &p, QString *error) {
    if (error) error->clear();
    const int transport = p.value("transport").toInt();
    const auto channel = transport == 0 ? communication::Channel::Udp :
        transport == 1 ? communication::Channel::Tcp : communication::Channel::Serial;
    auto &connection = m_protocol->communication();
    if (action == "openButton") {
        if (channel == communication::Channel::Udp) {
            communication::UdpConfig config; config.bindAddress = QHostAddress(p.value("address").toString());
            config.localPort = quint16(p.value("port").toUInt());
            if (config.bindAddress.isNull()) { if (error) *error = tr("UDP 绑定地址无效"); return false; }
            connection.openUdp(config);
        } else if (channel == communication::Channel::Tcp) {
            communication::TcpConfig config; config.host = p.value("address").toString();
            config.port = quint16(p.value("port").toUInt()); connection.openTcp(config);
        } else {
            communication::SerialConfig config; config.portName = p.value("serial").toString();
            config.baudRate = p.value("baud").toInt(); connection.openSerial(config);
        }
    } else if (action == "closeButton") connection.close(channel);
    else if (action == "modelButton") {
        if (m_modelPending) { if (error) *error = tr("等待上次模型接收完成"); return false; }
        if (!m_control || !boundWindow()) return false;
        clearDemo(nullptr);
        if (!m_control->loadModel(p.value("path").toString())) { if (error) *error = m_status; return false; }
    } else if (action == "baselineButton") {
        const auto frame = m_protocol->dataModel()->latest();
        const auto wavelength = frame ? std::dynamic_pointer_cast<const core::WavelengthFrame>(frame->data) : nullptr;
        if (!wavelength) { if (error) *error = tr("尚未收到有效波长帧"); return false; }
        Calculation::CalculationConfiguration config;
        config.strainCalibration.A = p.value("A", 1.0).toDouble();
        config.strainCalibration.B = p.value("B", 0.0).toDouble();
        config.strainCalibration.calibrationFactor = p.value("factor").toDouble();
        for (const auto &record : wavelength->records) config.strainCalibration.originalWavelength.push_back(record.wavelength);
        for (const auto &text : p.value("indices").toString().split(',')) {
            bool ok = false; const int index = text.trimmed().toInt(&ok);
            if (!ok) { if (error) *error = tr("通道下标须为逗号分隔整数"); return false; }
            config.channelCalibration.channelIndices.push_back(index);
            config.channelCalibration.calibrationFactors.push_back(1.0);
        }
        Calculation::StrainCalibration validation;
        validation.setCalibrationStrainData(config.strainCalibration);
        validation.setCalibrationChannelData(config.channelCalibration);
        if (!validation.checkPara()) { if (error) *error = validation.lastError(); return false; }
        m_calculation->setConfiguration(config);
    } else if (action == "storageStartButton") {
        storage::StorageConfiguration config;
        config.path = p.value("path").toString();
        config.format = p.value("format").toInt() == 0 ? storage::StorageFormat::Csv : storage::StorageFormat::SQLite;
        config.dataTypes = storage::DataType::Wavelength | storage::DataType::CalibratedHardware | storage::DataType::PointDisplacement;
        config.allPoints = true; m_calculation->storage().start(config);
    } else if (action == "storageStopButton") m_calculation->storage().stop();
    else if (action == "scaleButton" && m_control) m_control->setDeformationScale(p.value("scale").toDouble());
    else if (action == "resetDeformationButton" && m_control) m_control->resetDeformation();
    else { if (error) *error = tr("未支持的操作"); return false; }
    return true;
}
} }

