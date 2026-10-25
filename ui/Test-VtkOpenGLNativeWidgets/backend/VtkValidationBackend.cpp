#include "VtkValidationBackend.h"
#include "visualization/VisualizationControl.h"
#include "plate-simulation/PlateIFE.h"
#include <vtkActor.h>
#include <vtkPolyDataMapper.h>
#include <vtkProperty.h>
#include <vtkRenderer.h>
#include <vtkRendererCollection.h>
#include <vtkSphereSource.h>
#include <vtkCamera.h>
#include <cmath>
namespace fosd {
namespace vtk_test {
VtkValidationBackend::VtkValidationBackend() : m_protocol(new protocol::ProtocolControl) {
    createCalculation(false);
    auto &connection = m_protocol->communication();
    connect(&connection, &communication::ConnectControl::errorOccurred, this,
            &VtkValidationBackend::communicationError);
    connect(&connection, &communication::ConnectControl::decoderErrorOccurred, this,
            &VtkValidationBackend::decoderError);
    connect(&connection, &communication::ConnectControl::stateChanged, this,
            &VtkValidationBackend::connectionState);
    connect(m_protocol.get(), &protocol::ProtocolControl::errorOccurred, this,
            &VtkValidationBackend::protocolError);
    connect(&m_plateSender, &simulation::PlateUdpSender::errorOccurred, this,
            &VtkValidationBackend::status);
    m_statistics.setInterval(500);
    connect(&m_statistics, &QTimer::timeout, this, &VtkValidationBackend::updateStatistics);
    m_statistics.start();
}

VtkValidationBackend::~VtkValidationBackend() {
    m_plateSender.stop();
    m_statistics.stop();
    detach();
    // Stop ingress first; CalculationControl drains work and closes storage.
    m_protocol.reset();
    m_calculation.reset();
}

bool VtkValidationBackend::attach(vtkRenderWindow *window, QString *error) {
    if (!window) {
        if (error)
            *error = QString::fromUtf8("渲染窗口为空");
        return false;
    }
    if (!m_control) {
        // The exact frontend-owned pointer is handed to the current constructor.
        m_control.reset(new visualization::VisualizationControl(nullptr, window));
        m_control->setResultRepository(m_calculation->dataModel());
        connect(m_control.get(), &visualization::VisualizationControl::modelLoaded, this,
                &VtkValidationBackend::modelLoaded);
        connect(m_control.get(), &visualization::VisualizationControl::errorOccurred, this,
                &VtkValidationBackend::status);
    } else if (!m_control->attach(window)) {
        if (error)
            *error = QString::fromUtf8("VisualizationControl 绑定失败");
        return false;
    }
    if (boundWindow() != window || !renderer(error)) {
        if (error)
            *error = QString::fromUtf8("渲染窗口指针或 Renderer 验证失败");
        return false;
    }
    return true;
}

vtkRenderWindow *VtkValidationBackend::boundWindow() const {
    return m_control ? m_control->renderWindow() : nullptr;
}

vtkRenderer *VtkValidationBackend::renderer(QString *error) const {
    auto *window = boundWindow();
    auto *scene = window ? window->GetRenderers()->GetFirstRenderer() : nullptr;
    if (!scene && error)
        *error = QString::fromUtf8("请先绑定 VisualizationControl 的 Renderer");
    return scene;
}

void VtkValidationBackend::detach() {
    if (auto *scene = renderer(nullptr))
        if (m_demo)
            scene->RemoveActor(m_demo);
    m_demo = nullptr;
    if (m_control)
        m_control->detach();
}

bool VtkValidationBackend::showDemo(QString *error) {
    auto *scene = renderer(error);
    if (!scene)
        return false;
    if (!m_demo) {
        auto source = vtkSmartPointer<vtkSphereSource>::New();
        source->SetRadius(50);
        source->SetThetaResolution(48);
        source->SetPhiResolution(48);
        auto mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
        mapper->SetInputConnection(source->GetOutputPort());
        m_demo = vtkSmartPointer<vtkActor>::New();
        m_demo->SetMapper(mapper);
        m_demo->GetProperty()->SetColor(0.95, 0.65, 0.15);
        scene->AddActor(m_demo);
    }
    scene->SetBackground(0.08, 0.12, 0.18);
    scene->ResetCamera();
    return render(error);
}

bool VtkValidationBackend::clearDemo(QString *error) {
    auto *scene = renderer(error);
    if (!scene)
        return false;
    if (m_demo)
        scene->RemoveActor(m_demo);
    m_demo = nullptr;
    return render(error);
}

bool VtkValidationBackend::resetCamera(QString *error) {
    if (!renderer(error))
        return false;
    m_control->resetCamera();
    return true;
}

bool VtkValidationBackend::render(QString *error) {
    if (!renderer(error))
        return false;
    boundWindow()->Render();
    return true;
}

void VtkValidationBackend::status(const QString &message) {
    m_status = message;
    emit statusChanged(message);
}

void VtkValidationBackend::modelLoaded(const core::ModelDataPtr &model) {
    m_modelPending = true;
    m_calculation->setModel(*model);
    status(tr("模型已提交解算：%1 点，单位 mm").arg(model->size()));
}

void VtkValidationBackend::modelAccepted(quint64 revision) {
    if (sender() != m_calculation.get() || !m_modelPending || !m_control)
        return;
    m_modelPending = false;
    m_control->setExpectedModelRevision(revision);
    status(tr("模型已就绪，版本 %1").arg(revision));
    startPlateSenderWhenReady();
}

void VtkValidationBackend::configurationAccepted(quint64) {
    if (sender() != m_calculation.get())
        return;
    m_plateConfigurationReady = true;
    startPlateSenderWhenReady();
}

void VtkValidationBackend::processPlateFrame(const core::FrameSnapshotPtr &frame) {
    // Consume newly received demo packets only. Do not reinterpret an old
    // repository snapshot from a previous device using the demo calibration.
    if (!m_plateMode || m_platePending || !m_plateUdpReady || !m_plateConfigurationReady ||
        m_modelPending || !frame || frame->source.transport != core::TransportKind::Udp ||
        frame->protocolId != "mp.wavelength")
        return;
    const auto data = std::dynamic_pointer_cast<const core::WavelengthFrame>(frame->data);
    if (!data || data->records.size() != simulation::PlateSpec::sensors)
        return;
    m_calculation->submitFrame(frame);
}

void VtkValidationBackend::communicationError(const communication::CommunicationError &e) {
    if (m_plateMode && e.channel == communication::Channel::Udp) {
        m_platePending = false;
        m_plateSender.stop();
    }
    status("communication: " + e.message);
}

void VtkValidationBackend::decoderError(const communication::DecoderError &e) {
    status("decoder: " + e.message);
}

void VtkValidationBackend::protocolError(const protocol::ProtocolError &e) {
    status("protocol: " + e.message);
}

void VtkValidationBackend::calculationError(const Calculation::CalculationError &e) {
    if (e.operation == "model")
        m_modelPending = false;
    status("calculation: " + e.message);
}

void VtkValidationBackend::storageError(const storage::StorageError &e) {
    status("storage: " + e.message);
}

void VtkValidationBackend::storageStarted(const QString &session) {
    status(tr("存储已启动：%1").arg(session));
}

void VtkValidationBackend::storageStopped() {
    status(tr("存储已停止"));
}

void VtkValidationBackend::connectionState(communication::Channel channel,
                                           communication::ConnectionState state) {
    status(tr("通信通道 %1，状态 %2").arg(int(channel)).arg(int(state)));
    if (m_plateMode && channel == communication::Channel::Udp) {
        m_plateUdpReady = state == communication::ConnectionState::Ready;
        if (!m_plateUdpReady)
            m_plateSender.stop();
        startPlateSenderWhenReady();
    }
}

void VtkValidationBackend::updateStatistics() {
    const auto frame = m_protocol->dataModel()->latest();
    const auto result = m_calculation->dataModel()->latest();
    emit statusChanged(tr("接收帧版本 %1 | 解算结果版本 %2 | %3 | %4")
                           .arg(frame ? frame->revision : 0)
                           .arg(result ? result->revision : 0)
                           .arg(m_plateMode ? "Plate IFE" : "HYY IFE")
                           .arg(m_status));
}

void VtkValidationBackend::createCalculation(bool plate) {
    if (m_calculation && m_plateMode == plate)
        return;
    m_plateSender.stop();
    m_modelPending = false;
    disconnect(m_plateFrameConnection);
    if (m_control) {
        m_control->setResultRepository({});
        m_control->unloadModel();
    }
    m_calculation.reset(); // Drains the previous calculation/storage worker.
    m_calculation.reset(new Calculation::CalculationControl(
        plate ? core::FrameRepositoryPtr() : m_protocol->dataModel(), nullptr,
        plate ? Calculation::IFEFactory(simulation::createPlateIFE) : Calculation::IFEFactory()));
    m_plateMode = plate;
    if (plate)
        m_plateFrameConnection = connect(m_protocol.get(), &protocol::ProtocolControl::frameParsed,
                                         this, &VtkValidationBackend::processPlateFrame);
    else
        connect(m_protocol.get(), &protocol::ProtocolControl::frameParsed,
                &m_calculation->storage(), &storage::StorageControl::writeWavelengthFrame);
    connect(m_calculation.get(), &Calculation::CalculationControl::errorOccurred, this,
            &VtkValidationBackend::calculationError);
    connect(m_calculation.get(), &Calculation::CalculationControl::modelAccepted, this,
            &VtkValidationBackend::modelAccepted);
    connect(m_calculation.get(), &Calculation::CalculationControl::configurationAccepted, this,
            &VtkValidationBackend::configurationAccepted);
    connect(&m_calculation->storage(), &storage::StorageControl::errorOccurred, this,
            &VtkValidationBackend::storageError);
    connect(&m_calculation->storage(), &storage::StorageControl::started, this,
            &VtkValidationBackend::storageStarted);
    connect(&m_calculation->storage(), &storage::StorageControl::stopped, this,
            &VtkValidationBackend::storageStopped);
    if (m_control)
        m_control->setResultRepository(m_calculation->dataModel());
}

bool VtkValidationBackend::setupPlate(const QVariantMap &p, bool send, QString *error) {
    if (m_platePending || m_modelPending) {
        if (error)
            *error = tr("请等待模型初始化完成");
        return false;
    }
    if (!m_control || !boundWindow()) {
        if (error)
            *error = tr("请先绑定渲染窗口");
        return false;
    }
    const int port = p.value("port", 9000).toInt();
    simulation::PlateSenderSettings settings;
    settings.rateHz = p.value("plateRate", 50.0).toDouble();
    settings.motion.amplitudeMm = p.value("plateAmplitude", 15.0).toDouble();
    settings.motion.frequencyHz = p.value("plateFrequency", 0.35).toDouble();
    if (port < 1 || port > 65535 || !std::isfinite(settings.rateHz) || settings.rateHz < 1 ||
        settings.rateHz > 200 || !std::isfinite(settings.motion.amplitudeMm) ||
        settings.motion.amplitudeMm < 0 || settings.motion.amplitudeMm > 100 ||
        !std::isfinite(settings.motion.frequencyHz) || settings.motion.frequencyHz < 0 ||
        settings.motion.frequencyHz > 10) {
        if (error)
            *error =
                tr("仿真参数无效：端口 1～65535，采样 1～200 Hz，幅值 0～100 mm，频率 0～10 Hz");
        return false;
    }
    settings.port = quint16(port);
    stopPlate();
    createCalculation(true);
    clearDemo(nullptr);
    m_plateSettings = settings;
    m_plateSendingRequested = send;
    m_plateUdpReady = false;
    m_plateConfigurationReady = false;
    m_protocol->communication().close(communication::Channel::Udp);
    m_calculation->setConfiguration(simulation::defaultCalibration());
    if (!m_control->loadModel(simulation::defaultModelPath())) {
        if (error)
            *error = m_status;
        return false;
    }
    auto colors = m_control->scalarColorSettings();
    colors.enabled = true;
    colors.field = visualization::ScalarField::Magnitude;
    colors.mappingMode = visualization::ScalarMappingMode::Normalized;
    colors.valueMode = visualization::ScalarValueMode::Signed;
    m_control->setScalarColorSettings(colors);
    m_control->setDeformationScale(1.0);
    auto *scene = renderer(nullptr);
    scene->SetBackground(0.08, 0.12, 0.18);
    scene->GetActiveCamera()->SetPosition(1600, -900, 1000);
    scene->GetActiveCamera()->SetFocalPoint(500, 0, 0);
    scene->GetActiveCamera()->SetViewUp(0, 0, 1);
    m_control->resetCamera();
    communication::UdpConfig config;
    config.bindAddress = QHostAddress::LocalHost;
    config.localPort = settings.port;
    m_platePending = true;
    m_protocol->communication().openUdp(config);
    status(tr("板模型与默认标定已提交，等待模型、标定和 UDP 就绪"));
    return true;
}

void VtkValidationBackend::startPlateSenderWhenReady() {
    if (!m_platePending || !m_plateUdpReady || !m_plateConfigurationReady || m_modelPending)
        return;
    m_platePending = false;
    if (m_plateSendingRequested) {
        QString error;
        if (!m_plateSender.start(m_plateSettings, &error)) {
            status(error);
            return;
        }
        status(tr("板仿真运行：127.0.0.1:%1，%2 Hz，幅值 %3 mm")
                   .arg(m_plateSettings.port)
                   .arg(m_plateSettings.rateHz)
                   .arg(m_plateSettings.motion.amplitudeMm));
    } else
        status(tr("板模型就绪，等待外部 UDP 发送器，端口 %1").arg(m_plateSettings.port));
}

void VtkValidationBackend::stopPlate() {
    m_platePending = false;
    m_plateSendingRequested = false;
    m_plateSender.stop();
    m_plateUdpReady = false;
    if (m_plateMode)
        m_protocol->communication().close(communication::Channel::Udp);
}

bool VtkValidationBackend::execute(const QString &action, const QVariantMap &p, QString *error) {
    if (error)
        error->clear();
    const int transport = p.value("transport").toInt();
    const auto channel = transport == 0 ? communication::Channel::Udp
                                        : transport == 1 ? communication::Channel::Tcp
                                                         : communication::Channel::Serial;
    auto &connection = m_protocol->communication();
    if (action == "plateStartButton" || action == "plateSetupButton") {
        return setupPlate(p, action == "plateStartButton", error);
    } else if (action == "plateStopButton") {
        stopPlate();
        status(tr("板仿真已停止，保留最后一帧"));
    } else if (action == "plateExitButton") {
        stopPlate();
        createCalculation(false);
        status(tr("已恢复 HYY，请重新导入模型和设置标定"));
    } else if (action == "openButton") {
        if (channel == communication::Channel::Udp) {
            communication::UdpConfig config;
            config.bindAddress = QHostAddress(p.value("address").toString());
            config.localPort = quint16(p.value("port").toUInt());
            if (config.bindAddress.isNull()) {
                if (error)
                    *error = tr("UDP 绑定地址无效");
                return false;
            }
            connection.openUdp(config);
        } else if (channel == communication::Channel::Tcp) {
            communication::TcpConfig config;
            config.host = p.value("address").toString();
            config.port = quint16(p.value("port").toUInt());
            connection.openTcp(config);
        } else {
            communication::SerialConfig config;
            config.portName = p.value("serial").toString();
            config.baudRate = p.value("baud").toInt();
            connection.openSerial(config);
        }
    } else if (action == "scalarApplyButton") {
        if (!m_control || !boundWindow()) {
            if (error)
                *error = tr("请先绑定渲染窗口");
            return false;
        }
        auto settings = m_control->scalarColorSettings();
        settings.enabled = p.value("scalarEnabled").toBool();
        settings.field = visualization::ScalarField(p.value("scalarField").toInt());
        settings.mappingMode = visualization::ScalarMappingMode(p.value("scalarMapping").toInt());
        settings.valueMode = p.value("scalarAbsolute").toBool()
                                 ? visualization::ScalarValueMode::Absolute
                                 : visualization::ScalarValueMode::Signed;
        settings.rangeMode = visualization::ColorRangeMode(p.value("scalarRangeMode").toInt());
        settings.minimum = p.value("scalarMinimum").toDouble();
        settings.maximum = p.value("scalarMaximum").toDouble();
        if (!m_control->setScalarColorSettings(settings)) {
            if (error)
                *error = m_status;
            return false;
        }
    } else if (action == "closeButton")
        connection.close(channel);
    else if (action == "modelButton") {
        if (m_modelPending) {
            if (error)
                *error = tr("等待上次模型接收完成");
            return false;
        }
        if (!m_control || !boundWindow())
            return false;
        clearDemo(nullptr);
        if (!m_control->loadModel(p.value("path").toString())) {
            if (error)
                *error = m_status;
            return false;
        }
    } else if (action == "baselineButton") {
        const auto frame = m_protocol->dataModel()->latest();
        const auto wavelength =
            frame ? std::dynamic_pointer_cast<const core::WavelengthFrame>(frame->data) : nullptr;
        if (!wavelength) {
            if (error)
                *error = tr("尚未收到有效波长帧");
            return false;
        }
        Calculation::CalculationConfiguration config;
        config.strainCalibration.A = p.value("A", 1.0).toDouble();
        config.strainCalibration.B = p.value("B", 0.0).toDouble();
        config.strainCalibration.calibrationFactor = p.value("factor").toDouble();
        for (const auto &record : wavelength->records)
            config.strainCalibration.originalWavelength.push_back(record.wavelength);
        for (const auto &text : p.value("indices").toString().split(',')) {
            bool ok = false;
            const int index = text.trimmed().toInt(&ok);
            if (!ok) {
                if (error)
                    *error = tr("通道下标须为逗号分隔整数");
                return false;
            }
            config.channelCalibration.channelIndices.push_back(index);
            config.channelCalibration.calibrationFactors.push_back(1.0);
        }
        Calculation::StrainCalibration validation;
        validation.setCalibrationStrainData(config.strainCalibration);
        validation.setCalibrationChannelData(config.channelCalibration);
        if (!validation.checkPara()) {
            if (error)
                *error = validation.lastError();
            return false;
        }
        m_calculation->setConfiguration(config);
    } else if (action == "storageStartButton") {
        storage::StorageConfiguration config;
        config.path = p.value("path").toString();
        config.format = p.value("format").toInt() == 0 ? storage::StorageFormat::Csv
                                                       : storage::StorageFormat::SQLite;
        config.dataTypes = storage::DataType::Wavelength | storage::DataType::CalibratedHardware |
                           storage::DataType::PointDisplacement;
        config.allPoints = true;
        m_calculation->storage().start(config);
    } else if (action == "storageStopButton")
        m_calculation->storage().stop();
    else if (action == "scaleButton" && m_control)
        m_control->setDeformationScale(p.value("scale").toDouble());
    else if (action == "resetDeformationButton" && m_control)
        m_control->resetDeformation();
    else {
        if (error)
            *error = tr("未支持的操作");
        return false;
    }
    return true;
}
} // namespace vtk_test
} // namespace fosd
