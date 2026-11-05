#include "VtkValidationBackend.h"
#include "visualization/VisualizationControl.h"
#include <vtkActor.h>
#include <vtkPolyDataMapper.h>
#include <vtkProperty.h>
#include <vtkRenderer.h>
#include <vtkRendererCollection.h>
#include <vtkSphereSource.h>
#include <vtkCamera.h>
#include <cmath>
#include <limits>
namespace fosd {
namespace vtk_test {
namespace {
bool sceneColor(const QString &text, std::array<double, 3> *color, QString *error) {
    const auto parts = text.split(',');
    if (parts.size() != 3) {
        if (error)
            *error = QStringLiteral("RGB 需要三个逗号分隔的 0～1 数值");
        return false;
    }
    for (int i = 0; i < 3; ++i) {
        bool ok = false;
        (*color)[i] = parts[i].trimmed().toDouble(&ok);
        if (!ok || !std::isfinite((*color)[i]) || (*color)[i] < 0 || (*color)[i] > 1) {
            if (error)
                *error = QStringLiteral("RGB 分量必须为 0～1 的有限数值");
            return false;
        }
    }
    return true;
}
} // namespace
VtkValidationBackend::VtkValidationBackend() : m_protocol(new protocol::ProtocolControl) {
    createCalculation();
    auto &connection = m_protocol->communication();
    connect(&connection, &communication::ConnectControl::errorOccurred, this,
            &VtkValidationBackend::communicationError);
    connect(&connection, &communication::ConnectControl::decoderErrorOccurred, this,
            &VtkValidationBackend::decoderError);
    connect(&connection, &communication::ConnectControl::stateChanged, this,
            &VtkValidationBackend::connectionState);
    connect(m_protocol.get(), &protocol::ProtocolControl::errorOccurred, this,
            &VtkValidationBackend::protocolError);
    m_statistics.setInterval(500);
    connect(&m_statistics, &QTimer::timeout, this, &VtkValidationBackend::updateStatistics);
    m_statistics.start();
}

VtkValidationBackend::~VtkValidationBackend() {
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
        connect(m_control.get(), &visualization::VisualizationControl::pointPicked, this,
                &VtkValidationBackend::presentSelection);
        connect(m_control.get(), &visualization::VisualizationControl::selectionUpdated, this,
                &VtkValidationBackend::presentSelection);
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
    m_control->setBackground(0.08, 0.12, 0.18);
    m_control->resetCamera();
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
}

void VtkValidationBackend::configurationAccepted(quint64) {
    if (sender() != m_calculation.get())
        return;
}



void VtkValidationBackend::communicationError(const communication::CommunicationError &e) {

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

}

void VtkValidationBackend::updateStatistics() {
    const auto frame = m_protocol->dataModel()->latest();
    const auto result = m_calculation->dataModel()->latest();
    emit statusChanged(tr("接收帧版本 %1 | 解算结果版本 %2 | %3 | %4")
                           .arg(frame ? frame->revision : 0)
                           .arg(result ? result->revision : 0)
                           .arg("HYY IFE")
                           .arg(m_status));
}

void VtkValidationBackend::createCalculation() {
    if (m_calculation)
        return;
    m_modelPending = false;
    if (m_control) {
        m_control->setResultRepository({});
        m_control->unloadModel();
    }
    m_calculation.reset(); // Drains the previous calculation/storage worker.
    m_calculation.reset(new Calculation::CalculationControl(m_protocol->dataModel()));
    connect(m_protocol.get(), &protocol::ProtocolControl::frameParsed, &m_calculation->storage(), &storage::StorageControl::writeWavelengthFrame);
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







bool VtkValidationBackend::execute(const QString &action, const QVariantMap &p, QString *error) {
    if (error)
        error->clear();
    const int transport = p.value("transport").toInt();
    const auto channel = transport == 0 ? communication::Channel::Udp
                                        : transport == 1 ? communication::Channel::Tcp
                                                         : communication::Channel::Serial;
    auto &connection = m_protocol->communication();
    if (action == "openButton") {
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
    } else if (action == "sceneApplyButton") {
        if (!m_control || !boundWindow())
            return false;
        auto background = m_control->backgroundSettings();
        background.gradient = p.value("gradientBackground").toBool();
        if (!sceneColor(p.value("backgroundBottom").toString(), &background.bottomColor, error) ||
            !sceneColor(p.value("backgroundTop").toString(), &background.topColor, error))
            return false;
        auto axes = m_control->axesSettings();
        axes.visible = p.value("axesVisible").toBool();
        axes.lengthMm = p.value("axesLength").toDouble();
        auto grid = m_control->gridSettings();
        grid.visible = {
            {p.value("gridXZ").toBool(), p.value("gridXY").toBool(), p.value("gridYZ").toBool()}};
        grid.halfExtentMm = p.value("gridExtent").toDouble();
        grid.divisions = p.value("gridDivisions").toInt();
        if (!m_control->setAxesSettings(axes) || !m_control->setGridSettings(grid) ||
            !m_control->setBackgroundSettings(background)) {
            if (error)
                *error = m_status;
            return false;
        }
    } else if (action == "cameraViewButton" && m_control) {
        const int view = p.value("cameraView").toInt();
        if (view < 0 || view > 6) {
            if (error)
                *error = tr("相机视图索引无效");
            return false;
        }
        m_control->setCameraPreset(visualization::CameraPreset(view));
    } else if (action == "cameraApplyButton" && m_control) {
        auto settings = m_control->cameraSettings();
        settings.projection = visualization::CameraProjection(p.value("cameraProjection").toInt());
        settings.viewAngleDegrees = p.value("cameraAngle").toDouble();
        settings.parallelScaleMm = p.value("cameraScale").toDouble();
        settings.locked = p.value("cameraLocked").toBool();
        if (!m_control->setCameraSettings(settings)) {
            if (error)
                *error = m_status;
            return false;
        }
    } else if (action == "keyPointsApplyButton") {
        if (!m_control || !boundWindow()) {
            if (error)
                *error = tr("请先绑定渲染窗口");
            return false;
        }
        QVector<visualization::KeyPointDefinition> points;
        for (const auto &value :
             p.value("keyPointIds").toString().split(',', QString::SkipEmptyParts)) {
            bool ok = false;
            const qlonglong id = value.trimmed().toLongLong(&ok);
            if (!ok || id < 0 || id > std::numeric_limits<vtkIdType>::max()) {
                if (error)
                    *error = tr("关键点 ID 必须为非负整数");
                return false;
            }
            visualization::KeyPointDefinition definition;
            definition.pointId = vtkIdType(id);
            points.append(definition);
        }
        auto style = m_control->keyPointStyle();
        style.radiusMm = p.value("keyPointRadius", 5.0).toDouble();
        style.labelsVisible = p.value("keyPointLabels", true).toBool();
        if (!m_control->setKeyPoints(points) || !m_control->setKeyPointStyle(style)) {
            if (error)
                *error = m_status;
            return false;
        }
        m_control->showKeyPoints(p.value("keyPointsVisible", true).toBool());
        auto picking = m_control->pickingSettings();
        picking.enabled = p.value("pickingEnabled", true).toBool();
        picking.onlyKeyPoints = p.value("pickOnlyKeyPoints").toBool();
        if (!m_control->setPickingSettings(picking)) {
            if (error)
                *error = m_status;
            return false;
        }
        m_control->setCameraLocked(p.value("cameraLocked").toBool());
    } else if (action == "clearSelectionButton" && m_control) {
        m_control->clearSelection();
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

void VtkValidationBackend::presentSelection(const visualization::PickedPointInfo &info) {
    if (!info.valid) {
        emit selectionTextChanged(tr("未选中模型点"));
        return;
    }
    emit selectionTextChanged(
        tr("ID %1 | 原始 mm (%2, %3, %4) | 实际位移 mm (%5, %6, %7) | 显示 mm (%8, %9, %10)")
            .arg(qlonglong(info.pointId))
            .arg(info.originalPositionMm[0], 0, 'f', 3)
            .arg(info.originalPositionMm[1], 0, 'f', 3)
            .arg(info.originalPositionMm[2], 0, 'f', 3)
            .arg(info.displacementMm[0], 0, 'f', 6)
            .arg(info.displacementMm[1], 0, 'f', 6)
            .arg(info.displacementMm[2], 0, 'f', 6)
            .arg(info.displayPositionMm[0], 0, 'f', 3)
            .arg(info.displayPositionMm[1], 0, 'f', 3)
            .arg(info.displayPositionMm[2], 0, 'f', 3));
}
} // namespace vtk_test
} // namespace fosd
