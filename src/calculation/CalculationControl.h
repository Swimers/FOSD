#pragma once
#include "config/CoreParameters.h"
#include "IFrameIFE.h"
#include "StrainCalibration.h"
#include "core/Datamodel/CalculationRepository.h"
#include "core/Datamodel/FrameRepository.h"
#include "storage/StorageControl.h"
#include <QObject>
#include <QThread>
#include <functional>

namespace fosd {
namespace Calculation {


struct CalculationError {
    QString operation;
    QString message;
    quint64 sourceRevision = 0;
};
using IFEFactory = std::function<std::unique_ptr<IFrameIFE>()>;

namespace detail {
class CalculationWorker final : public QObject {
    Q_OBJECT
public:
    CalculationWorker(core::FrameRepositoryPtr input,
                      std::shared_ptr<core::CalculationRepository> output, IFEFactory factory);
public slots:
    void initialize();
    void shutdown();
    void configure(const fosd::Calculation::CalculationConfiguration &configuration);
    void setModel(const fosd::core::ModelData &model);
    void clearModel();
    void processFrame(const fosd::core::FrameSnapshotPtr &snapshot);
private slots:
    void pollLatest();
signals:
    void calibratedReady(const fosd::core::CalibratedDataPtr &data);
    void resultReady(const fosd::core::CalculationResultPtr &result);
    void errorOccurred(const fosd::Calculation::CalculationError &error);
    void modelAccepted(quint64 revision);
    void configurationAccepted(quint64 revision);

private:
    void reportError(const QString &operation, const QString &message, quint64 revision = 0);
    core::FrameRepositoryPtr m_input;
    std::shared_ptr<core::CalculationRepository> m_output;
    IFEFactory m_factory;
    std::unique_ptr<IFrameIFE> m_algorithm;
    StrainCalibration *m_calibration = nullptr;
    core::ModelDataPtr m_model;
    core::FrameSnapshotPtr m_seenFrame;
    quint64 m_modelRevision = 0;
    quint64 m_configurationRevision = 0;
    bool m_stopping = false;
};
} // namespace detail

class CalculationControl final : public QObject {
    Q_OBJECT
public:
    explicit CalculationControl(QObject *parent = nullptr);
    // Polls the latest input every 10 ms on its own worker thread. Intermediate
    // frames may be skipped. Create/destroy the facade on the caller thread.
    CalculationControl(core::FrameRepositoryPtr input, QObject *parent = nullptr,
                       IFEFactory factory = IFEFactory(),
                       storage::WriterFactory writerFactory = storage::WriterFactory());
    ~CalculationControl() override;
    void setConfiguration(const CalculationConfiguration &configuration);
    void setModel(const core::ModelData &model);
    void clearModel();
    // Replay/manual input when constructed without a repository. Complete,
    // immutable snapshots only; do not combine replay with automatic polling.
    void submitFrame(const core::FrameSnapshotPtr &snapshot);
    core::CalculationRepositoryPtr dataModel() const {
        return m_output;
    }
    storage::StorageControl &storage() const {
        return *m_storage;
    }
public slots:
    // Import/replace the entire original point map. Re-submit edited points
    // to replace the model. Accepted asynchronously via modelAccepted(revision);
    // rejected input leaves the previous model intact and emits errorOccurred.
    void setRawPointMap(const fosd::core::PointMap &rawPointMap);
signals:
    void resultReady(const fosd::core::CalculationResultPtr &result);
    void calibratedReady(const fosd::core::CalibratedDataPtr &data);
    void errorOccurred(const fosd::Calculation::CalculationError &error);
    void modelAccepted(quint64 revision);
    void configurationAccepted(quint64 revision);
    void requestConfigure(const fosd::Calculation::CalculationConfiguration &configuration);
    void requestSetModel(const fosd::core::ModelData &model);
    void requestClearModel();
    void requestFrame(const fosd::core::FrameSnapshotPtr &snapshot);

private:
    std::shared_ptr<core::CalculationRepository> m_output;
    QThread m_thread;
    detail::CalculationWorker *m_worker = nullptr;
    std::unique_ptr<storage::StorageControl> m_storage;
};

} // namespace Calculation
} // namespace fosd
Q_DECLARE_METATYPE(fosd::Calculation::CalculationConfiguration)
Q_DECLARE_METATYPE(fosd::Calculation::CalculationError)
