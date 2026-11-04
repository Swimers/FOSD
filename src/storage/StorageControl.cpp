#include "StorageControl.h"
#include "config/ParameterStore.h"
#include "StorageRecords.h"
#include <QSet>
#include <QUuid>
#include <utility>

namespace fosd {
namespace storage {
namespace {
bool validateConfiguration(const StorageConfiguration &config, QString *error) {
    const int flags = int(config.dataTypes);
    const int supported = int(DataType::Wavelength) | int(DataType::CalibratedHardware) |
                          int(DataType::PointDisplacement);
    if (config.path.trimmed().isEmpty() || !flags || (flags & ~supported) ||
        (config.format != StorageFormat::Csv && config.format != StorageFormat::SQLite)) {
        *error =
            QStringLiteral("A file path, supported format and nonempty data types are required");
        return false;
    }
    if (config.dataTypes.testFlag(DataType::PointDisplacement)) {
        if (config.allPoints && !config.pointIds.isEmpty()) {
            *error = QStringLiteral("Choose either allPoints or explicit pointIds");
            return false;
        }
        if (!config.allPoints && config.pointIds.isEmpty()) {
            *error = QStringLiteral("Select displacement pointIds or enable allPoints");
            return false;
        }
        QSet<PointId> unique;
        for (PointId id : config.pointIds) {
            if (id < 0 || unique.contains(id)) {
                *error = QStringLiteral("Point IDs must be nonnegative and unique");
                return false;
            }
            unique.insert(id);
        }
    }
    return true;
}
} // namespace
namespace detail {
StorageWorker::StorageWorker(WriterFactory factory) : m_factory(std::move(factory)) {
}

void StorageWorker::start(const StorageConfiguration &configuration) {
    if (m_writer) {
        reportError(QStringLiteral("start"),
                    QStringLiteral("Stop the active session before reconfiguring"));
        return;
    }
    m_configuration = configuration;
    QString error;
    if (!validateConfiguration(configuration, &error)) {
        reportError(QStringLiteral("start"), error);
        return;
    }
    auto writer = m_factory(configuration.format);
    if (!writer || writer->format() != configuration.format) {
        reportError(QStringLiteral("start"),
                    QStringLiteral("Writer factory returned no matching backend"));
        return;
    }
    if (!writer->open(configuration.path, &error)) {
        reportError(QStringLiteral("open"), error);
        return;
    }
    if (!config::ParameterStore().write(configuration, true, &error)) {
        writer->close(); reportError("configuration", error); return;
    }
    m_writer = std::move(writer);
    m_session = QUuid::createUuid().toString(QUuid::WithoutBraces);
    emit started(m_session);
}

void StorageWorker::stop() {
    if (!m_writer)
        return;
    QString error;
    if (!m_writer->flush(&error))
        reportError(QStringLiteral("flush"), error);
    m_writer->close();
    m_writer.reset();
    m_session.clear();
    emit stopped();
}

void StorageWorker::flush() {
    if (!m_writer)
        return;
    QString error;
    if (!m_writer->flush(&error)) {
        reportError(QStringLiteral("flush"), error);
        m_writer->close();
        m_writer.reset();
        m_session.clear();
        emit stopped();
    }
}

void StorageWorker::writeWavelengthFrame(const core::FrameSnapshotPtr &frame) {
    if (!m_writer || !m_configuration.dataTypes.testFlag(DataType::Wavelength))
        return;
    StorageBatch batch;
    QString error;
    if (!wavelengthRecords(m_session, frame, &batch, &error)) {
        failWrite(error, frame ? frame->revision : 0);
        return;
    }
    writeBatch(batch);
}

void StorageWorker::writeCalibratedData(const core::CalibratedDataPtr &data) {
    if (!m_writer || !m_configuration.dataTypes.testFlag(DataType::CalibratedHardware))
        return;
    StorageBatch batch;
    QString error;
    if (!calibratedRecords(m_session, data, &batch, &error)) {
        failWrite(error, data && data->sourceFrame ? data->sourceFrame->revision : 0);
        return;
    }
    writeBatch(batch);
}

void StorageWorker::writeResult(const core::CalculationResultPtr &result) {
    if (!m_writer || !m_configuration.dataTypes.testFlag(DataType::PointDisplacement))
        return;
    StorageBatch batch;
    QString error;
    if (!displacementRecords(m_session, result, m_configuration, &batch, &error)) {
        failWrite(error, result && result->sourceFrame ? result->sourceFrame->revision : 0);
        return;
    }
    writeBatch(batch);
}

void StorageWorker::writeBatch(const StorageBatch &batch) {
    QString error;
    if (!m_writer->write(batch.records, &error)) {
        failWrite(error, batch.sourceRevision);
        return;
    }
    emit batchWritten(batch.type, batch.sourceRevision, batch.records.size());
}

void StorageWorker::failWrite(const QString &message, quint64 revision) {
    reportError(QStringLiteral("write"), message, revision);
    m_writer->close();
    m_writer.reset();
    m_session.clear();
    emit stopped();
}

void StorageWorker::reportError(const QString &operation, const QString &message,
                                quint64 revision) {
    StorageError error;
    error.operation = operation;
    error.message = message;
    error.path = m_configuration.path;
    error.sourceRevision = revision;
    emit errorOccurred(error);
}
} // namespace detail

StorageControl::StorageControl(QThread &host, WriterFactory factory, QObject *parent)
    : QObject(parent), m_worker(new detail::StorageWorker(
                           factory ? std::move(factory) : WriterFactory(createStorageWriter))) {
    qRegisterMetaType<StorageConfiguration>("fosd::storage::StorageConfiguration");
    qRegisterMetaType<StorageError>("fosd::storage::StorageError");
    qRegisterMetaType<DataType>("fosd::storage::DataType");
    qRegisterMetaType<core::FrameSnapshotPtr>("fosd::core::FrameSnapshotPtr");
    qRegisterMetaType<core::CalibratedDataPtr>("fosd::core::CalibratedDataPtr");
    qRegisterMetaType<core::CalculationResultPtr>("fosd::core::CalculationResultPtr");

    StorageConfiguration saved;
    QString message;
    if (!config::ParameterStore().initialize(&saved, &message)) {
        StorageError error; error.operation = "configuration"; error.message = message;
        QMetaObject::invokeMethod(this, "errorOccurred", Qt::QueuedConnection, Q_ARG(fosd::storage::StorageError, error));
    }
    m_worker->moveToThread(&host);

    connect(&host, &QThread::finished, m_worker.data(), &QObject::deleteLater);

    connect(this, &StorageControl::requestStart, m_worker.data(), &detail::StorageWorker::start,
            Qt::QueuedConnection);
    connect(this, &StorageControl::requestStop, m_worker.data(), &detail::StorageWorker::stop,
            Qt::QueuedConnection);
    connect(this, &StorageControl::requestFlush, m_worker.data(), &detail::StorageWorker::flush,
            Qt::QueuedConnection);
    connect(this, &StorageControl::requestWavelength, m_worker.data(),
            &detail::StorageWorker::writeWavelengthFrame, Qt::QueuedConnection);

    connect(m_worker.data(), &detail::StorageWorker::started, this, &StorageControl::started);
    connect(m_worker.data(), &detail::StorageWorker::stopped, this, &StorageControl::stopped);
    connect(m_worker.data(), &detail::StorageWorker::batchWritten, this,
            &StorageControl::batchWritten);
    connect(m_worker.data(), &detail::StorageWorker::errorOccurred, this,
            &StorageControl::errorOccurred);
}

StorageControl::~StorageControl() {
    shutdown();
}

void StorageControl::shutdown() {
    if (!m_worker || !m_worker->thread()->isRunning())
        return;
    if (QThread::currentThread() == m_worker->thread())
        m_worker->stop();
    else
        QMetaObject::invokeMethod(m_worker.data(), "stop", Qt::BlockingQueuedConnection);
}

void StorageControl::start(const StorageConfiguration &configuration) {
    emit requestStart(configuration);
}

void StorageControl::stop() {
    emit requestStop();
}

void StorageControl::flush() {
    emit requestFlush();
}

void StorageControl::writeWavelengthFrame(const core::FrameSnapshotPtr &frame) {
    emit requestWavelength(frame);
}

} // namespace storage
} // namespace fosd
