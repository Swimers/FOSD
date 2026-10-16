#pragma once
#include "IFrameIFE.h"
#include "StrainCalibration.h"
#include "core/Datamodel/CalculationRepository.h"
#include "core/Datamodel/FrameRepository.h"
#include <QObject>
#include <QThread>
#include <functional>

namespace fosd{
namespace Calculation{

struct CalculationConfiguration {
    CalibrationStrainData strainCalibration;
    CalibrationChannelData channelCalibration;
    std::vector<int> freeDofIndices; // HYY matrix rows -> global DOFs, 20 unique indices.
    NodeDofs constrainedDofs;       // 22 entries, only unmapped entries are retained.
};
struct CalculationError {
    QString operation;
    QString message;
    quint64 sourceRevision = 0;
};
using IFEFactory = std::function<std::unique_ptr<IFrameIFE>(const CalculationConfiguration &)>;

namespace detail {
class CalculationWorker final : public QObject {
    Q_OBJECT
public:
    CalculationWorker(core::FrameRepositoryPtr input,
                      std::shared_ptr<core::CalculationRepository> output, IFEFactory factory);
public slots:
    void initialize();
    void configure(const fosd::Calculation::CalculationConfiguration &configuration);
    void setModel(const fosd::core::ModelData &model);
    void processFrame(const fosd::core::FrameSnapshotPtr &snapshot);
private slots:
    void pollLatest();
signals:
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
    NodeDofs m_probeDofs;
    quint64 m_modelRevision = 0;
    quint64 m_configurationRevision = 0;
};
}

class CalculationControl final : public QObject{
    Q_OBJECT
public:
    explicit CalculationControl(QObject *parent = nullptr);
    // Polls the latest input every 10 ms on its own worker thread. Intermediate
    // frames may be skipped. Create/destroy the facade on the caller thread.
    CalculationControl(core::FrameRepositoryPtr input, QObject *parent = nullptr,
                       IFEFactory factory = IFEFactory());
    ~CalculationControl() override;
    void setConfiguration(const CalculationConfiguration &configuration);
    void setModel(const core::ModelData &model);
    // Replay/manual input when constructed without a repository. Complete,
    // immutable snapshots only; do not combine replay with automatic polling.
    void submitFrame(const core::FrameSnapshotPtr &snapshot);
    core::CalculationRepositoryPtr dataModel() const { return m_output; }
signals:
    void resultReady(const fosd::core::CalculationResultPtr &result);
    void errorOccurred(const fosd::Calculation::CalculationError &error);
    void modelAccepted(quint64 revision);
    void configurationAccepted(quint64 revision);
    void requestConfigure(const fosd::Calculation::CalculationConfiguration &configuration);
    void requestSetModel(const fosd::core::ModelData &model);
    void requestFrame(const fosd::core::FrameSnapshotPtr &snapshot);
private:
    std::shared_ptr<core::CalculationRepository> m_output;
    QThread m_thread;
    detail::CalculationWorker *m_worker = nullptr;
};

} // namespace Calculation
} // namespace fosd
Q_DECLARE_METATYPE(fosd::Calculation::CalculationConfiguration)
Q_DECLARE_METATYPE(fosd::Calculation::CalculationError)
