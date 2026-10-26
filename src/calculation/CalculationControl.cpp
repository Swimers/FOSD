#include "CalculationControl.h"
#include "HYYIFE.h"
#include "PlateIFE.h"
#include <QTimer>
#include <cmath>
#include <utility>

namespace fosd {
namespace Calculation {
namespace {
std::unique_ptr<IFrameIFE> createHYY() {
    return std::unique_ptr<IFrameIFE>(new HYYIFE);
}

bool validateModel(const core::ModelData &model, QString *error) {
    if (model.isEmpty()) {
        *error = QStringLiteral("Model must contain at least one point");
        return false;
    }
    for (auto point = model.constBegin(); point != model.constEnd(); ++point) {
        const auto &xyz = point.value();
        if (point.key() < 0 || xyz.size() != 3 || !std::isfinite(xyz[0]) ||
            !std::isfinite(xyz[1]) || !std::isfinite(xyz[2])) {
            *error = QStringLiteral(
                         "Model point %1 requires a nonnegative ID and three finite coordinates")
                         .arg(qlonglong(point.key()));
            return false;
        }
    }
    return true;
}
} // namespace
namespace detail {
CalculationWorker::CalculationWorker(core::FrameRepositoryPtr input,
                                     std::shared_ptr<core::CalculationRepository> output,
                                     IFEFactory factory)
    : m_input(std::move(input)), m_output(std::move(output)), m_factory(std::move(factory)) {
}

void CalculationWorker::initialize() {
    m_calibration = new StrainCalibration(this);
    if (m_input) {
        // Read the latest immutable snapshot directly, without a UI signal relay.
        QTimer *timer = new QTimer(this);
        timer->setInterval(10);
        connect(timer, &QTimer::timeout, this, &CalculationWorker::pollLatest);
        timer->start();
    }
}

void CalculationWorker::shutdown() {
    m_stopping = true;
    const auto timers = findChildren<QTimer *>();
    for (QTimer *timer : timers)
        timer->stop();
}

void CalculationWorker::configure(const CalculationConfiguration &configuration) {
    StrainCalibration validation;
    validation.setCalibrationStrainData(configuration.strainCalibration);
    validation.setCalibrationChannelData(configuration.channelCalibration);
    if (!validation.checkPara()) {
        reportError(QStringLiteral("configuration"), validation.lastError());
        return;
    }
    auto algorithm =
        configuration.algorithm == CalculationAlgorithm::Plate ? createPlateIFE() : m_factory();
    if (!algorithm) {
        reportError(QStringLiteral("configuration"),
                    QStringLiteral("IFE factory returned no algorithm"));
        return;
    }
    m_algorithm = std::move(algorithm);
    m_calibration->setCalibrationStrainData(configuration.strainCalibration);
    m_calibration->setCalibrationChannelData(configuration.channelCalibration);
    m_seenFrame.reset();
    emit configurationAccepted(++m_configurationRevision);
}

void CalculationWorker::setModel(const core::ModelData &model) {
    QString error;
    if (!validateModel(model, &error)) {
        reportError(QStringLiteral("model"), error);
        return;
    }
    m_model = std::make_shared<const core::ModelData>(model);
    m_seenFrame.reset();
    emit modelAccepted(++m_modelRevision);
}

void CalculationWorker::pollLatest() {
    if (m_stopping || !m_algorithm)
        return;
    const auto snapshot = m_input->latest();
    if (snapshot && snapshot != m_seenFrame)
        processFrame(snapshot);
}

void CalculationWorker::clearModel() {
    m_model.reset();
    m_output->clear();
    m_seenFrame.reset();
}

void CalculationWorker::processFrame(const core::FrameSnapshotPtr &snapshot) {
    if (m_stopping)
        return;
    const quint64 revision = snapshot ? snapshot->revision : 0;
    if (!m_algorithm) {
        reportError(QStringLiteral("solve"), QStringLiteral("Set a valid configuration first"),
                    revision);
        return;
    }
    m_seenFrame = snapshot;
    const auto wavelength =
        snapshot ? std::dynamic_pointer_cast<const core::WavelengthFrame>(snapshot->data) : nullptr;
    if (!wavelength || wavelength->records.isEmpty() ||
        int(wavelength->recordCount) != wavelength->records.size()) {
        reportError(QStringLiteral("frame"),
                    QStringLiteral("Expected a nonempty, consistent wavelength snapshot"),
                    revision);
        return;
    }
    Wavelength values;
    values.reserve(size_t(wavelength->records.size()));
    for (const auto &record : wavelength->records)
        values.push_back(double(record.wavelength));
    const Strain calibrated = m_calibration->applyCalibration(values);
    if (calibrated.empty()) {
        reportError(QStringLiteral("calibration"), m_calibration->lastError(), revision);
        return;
    }
    const Strain strains = m_calibration->rearrangeCalibration(calibrated);
    if (strains.empty()) {
        reportError(QStringLiteral("calibration"), m_calibration->lastError(), revision);
        return;
    }

    core::CalibratedData hardware;
    hardware.sourceFrame = snapshot;
    hardware.configurationRevision = m_configurationRevision;
    hardware.calibratedStrains = QVector<double>::fromStdVector(calibrated);
    hardware.selectedStrains = QVector<double>::fromStdVector(strains);
    hardware.sourceIndices =
        QVector<int>::fromStdVector(m_calibration->getCalibrationChannelData().channelIndices);
    emit calibratedReady(std::make_shared<const core::CalibratedData>(std::move(hardware)));

    // Hardware calibration does not require an imported geometric model.
    if (!m_model)
        return;
    const NodeDofs dofs = m_algorithm->calculateDofs(strains);
    core::CalculationResult result;
    result.sourceFrame = snapshot;
    result.algorithmId = m_algorithm->IFEId();
    result.modelRevision = m_modelRevision;
    result.configurationRevision = m_configurationRevision;
    result.strains = QVector<double>::fromStdVector(strains);
    result.nodeDofs = QVector<double>::fromStdVector(dofs);

    for (auto point = m_model->constBegin(); point != m_model->constEnd(); ++point) {
        const auto &xyz = point.value();
        Point3D position;
        position.x = xyz[0];
        position.y = xyz[1];
        position.z = xyz[2];
        const Displacement displacement = m_algorithm->calculateDisplacement(position, dofs);
        QVector<double> delta(3), deformed(3);
        for (int axis = 0; axis < 3; ++axis) {
            delta[axis] = displacement[size_t(axis)] * 1000.0;
            deformed[axis] = xyz[axis] + delta[axis];
            if (!std::isfinite(delta[axis]) || !std::isfinite(deformed[axis])) {
                reportError(QStringLiteral("interpolation"),
                            QStringLiteral("Nonfinite model result"), revision);
                return;
            }
        }
        result.displacementMap.insert(point.key(), delta);
        result.deformedModel.insert(point.key(), deformed);
    }
    // Publish only after every point succeeded. Never deform the previous result.
    const auto published = m_output->publish(std::move(result));
    emit resultReady(published);
}

void CalculationWorker::reportError(const QString &operation, const QString &message,
                                    quint64 revision) {
    CalculationError error;
    error.operation = operation;
    error.message = message;
    error.sourceRevision = revision;
    emit errorOccurred(error);
}
} // namespace detail

CalculationControl::CalculationControl(QObject *parent)
    : CalculationControl(core::FrameRepositoryPtr(), parent) {
}

CalculationControl::CalculationControl(core::FrameRepositoryPtr input, QObject *parent,
                                       IFEFactory factory, storage::WriterFactory writerFactory)
    : QObject(parent), m_output(std::make_shared<core::CalculationRepository>()),
      m_worker(new detail::CalculationWorker(
          std::move(input), m_output, factory ? std::move(factory) : IFEFactory(createHYY))) {
    qRegisterMetaType<CalculationConfiguration>("fosd::Calculation::CalculationConfiguration");
    qRegisterMetaType<CalculationError>("fosd::Calculation::CalculationError");
    qRegisterMetaType<core::PointMap>("fosd::core::PointMap");
    qRegisterMetaType<core::ModelData>("fosd::core::ModelData");
    qRegisterMetaType<core::FrameSnapshotPtr>("fosd::core::FrameSnapshotPtr");
    qRegisterMetaType<core::CalculationResultPtr>("fosd::core::CalculationResultPtr");

    m_storage.reset(new storage::StorageControl(m_thread, std::move(writerFactory)));
    m_worker->moveToThread(&m_thread);

    connect(&m_thread, &QThread::started, m_worker, &detail::CalculationWorker::initialize);
    connect(&m_thread, &QThread::finished, m_worker, &QObject::deleteLater);

    connect(this, &CalculationControl::requestConfigure, m_worker,
            &detail::CalculationWorker::configure, Qt::QueuedConnection);
    connect(this, &CalculationControl::requestSetModel, m_worker,
            &detail::CalculationWorker::setModel, Qt::QueuedConnection);
    connect(this, &CalculationControl::requestClearModel, m_worker,
            &detail::CalculationWorker::clearModel, Qt::QueuedConnection);
    connect(this, &CalculationControl::requestFrame, m_worker,
            &detail::CalculationWorker::processFrame, Qt::QueuedConnection);

    connect(m_worker, &detail::CalculationWorker::resultReady, this,
            &CalculationControl::resultReady);
    connect(m_worker, &detail::CalculationWorker::calibratedReady, this,
            &CalculationControl::calibratedReady);

    // Both receivers live on this same thread; no UI-thread relay or extra queue.
    connect(m_worker, &detail::CalculationWorker::calibratedReady, m_storage->m_worker.data(),
            &storage::detail::StorageWorker::writeCalibratedData, Qt::DirectConnection);
    connect(m_worker, &detail::CalculationWorker::resultReady, m_storage->m_worker.data(),
            &storage::detail::StorageWorker::writeResult, Qt::DirectConnection);

    connect(m_worker, &detail::CalculationWorker::errorOccurred, this,
            &CalculationControl::errorOccurred);
    connect(m_worker, &detail::CalculationWorker::modelAccepted, this,
            &CalculationControl::modelAccepted);
    connect(m_worker, &detail::CalculationWorker::configurationAccepted, this,
            &CalculationControl::configurationAccepted);

    m_thread.start();
}

CalculationControl::~CalculationControl() {
    // Drain previously submitted calculations before closing their writer.
    QMetaObject::invokeMethod(m_worker, "shutdown", Qt::BlockingQueuedConnection);
    m_storage->shutdown();
    m_thread.quit();
    m_thread.wait();
    m_storage.reset();
}

void CalculationControl::setConfiguration(const CalculationConfiguration &configuration) {
    emit requestConfigure(configuration);
}

void CalculationControl::setModel(const core::ModelData &model) {
    emit requestSetModel(model);
}

void CalculationControl::setRawPointMap(const core::PointMap &rawPointMap) {
    setModel(rawPointMap);
}

void CalculationControl::clearModel() {
    emit requestClearModel();
}

void CalculationControl::submitFrame(const core::FrameSnapshotPtr &snapshot) {
    m_storage->writeWavelengthFrame(snapshot);
    emit requestFrame(snapshot);
}
} // namespace Calculation
} // namespace fosd
