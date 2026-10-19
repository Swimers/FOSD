#pragma once
#include "IStorageWriter.h"
#include <QObject>
#include <QPointer>
#include <QThread>

namespace fosd {
namespace Calculation { class CalculationControl; }
namespace storage {
namespace detail {
class StorageWorker final : public QObject {
    Q_OBJECT
public:
    explicit StorageWorker(WriterFactory factory);
public slots:
    void start(const fosd::storage::StorageConfiguration &configuration);
    void stop();
    void flush();
    void writeWavelengthFrame(const fosd::core::FrameSnapshotPtr &frame);
    void writeCalibratedData(const fosd::core::CalibratedDataPtr &data);
    void writeResult(const fosd::core::CalculationResultPtr &result);
signals:
    void started(const QString &sessionId);
    void stopped();
    void batchWritten(fosd::storage::DataType type, quint64 sourceRevision, int rows);
    void errorOccurred(const fosd::storage::StorageError &error);
private:
    void writeBatch(const StorageBatch &batch);
    void reportError(const QString &operation, const QString &message, quint64 revision = 0);
    void failWrite(const QString &message, quint64 revision);
    WriterFactory m_factory;
    std::unique_ptr<IStorageWriter> m_writer;
    StorageConfiguration m_configuration;
    QString m_session;
};
}

// Hosts its worker on the supplied CalculationControl thread; never creates a
// thread itself. Commands are asynchronous and serialized with calculations.
class StorageControl final : public QObject {
    Q_OBJECT
public:
    StorageControl(QThread &host, WriterFactory factory = WriterFactory(), QObject *parent = nullptr);
    ~StorageControl() override;
public slots:
    void start(const fosd::storage::StorageConfiguration &configuration);
    void stop();
    void flush();
    void writeWavelengthFrame(const fosd::core::FrameSnapshotPtr &frame);
signals:
    void started(const QString &sessionId);
    void stopped();
    void batchWritten(fosd::storage::DataType type, quint64 sourceRevision, int rows);
    void errorOccurred(const fosd::storage::StorageError &error);

    void requestStart(const fosd::storage::StorageConfiguration &configuration);
    void requestStop();
    void requestFlush();
    void requestWavelength(const fosd::core::FrameSnapshotPtr &frame);
    
private:
    friend class fosd::Calculation::CalculationControl;
    void shutdown();
    QPointer<detail::StorageWorker> m_worker;
};
} }
