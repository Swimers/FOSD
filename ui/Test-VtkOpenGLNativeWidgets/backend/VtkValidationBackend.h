#pragma once
#include "IVtkValidationBackend.h"
#include <memory>
#include <vtkSmartPointer.h>
#include "protocol/ProtocolControl.h"
#include "calculation/CalculationControl.h"
#include <QTimer>
#include "plate-simulation/PlateUdpSender.h"
class vtkActor;
class vtkRenderer;
namespace fosd {
namespace visualization {
class VisualizationControl;
}

namespace vtk_test {
// App-layer validation adapter, not part of the production visualization API.
class VtkValidationBackend final : public QObject, public IVtkValidationBackend {
    Q_OBJECT
public:
    VtkValidationBackend();
    ~VtkValidationBackend() override;
    bool attach(vtkRenderWindow *window, QString *error) override;
    void detach() override;
    vtkRenderWindow *boundWindow() const override;
    bool showDemo(QString *error) override;
    bool clearDemo(QString *error) override;
    bool resetCamera(QString *error) override;
    bool render(QString *error) override;
    bool execute(const QString &action, const QVariantMap &parameters, QString *error) override;
    visualization::VisualizationControl *visualization() const {
        return m_control.get();
    }
    core::FrameSnapshotPtr latestFrame() const {
        return m_protocol->dataModel()->latest();
    }
    core::CalculationResultPtr latestResult() const {
        return m_calculation->dataModel()->latest();
    }
signals:
    void statusChanged(const QString &message);
private slots:
    void modelLoaded(const fosd::core::ModelDataPtr &model);
    void modelAccepted(quint64 revision);
    void configurationAccepted(quint64 revision);
    void processPlateFrame(const fosd::core::FrameSnapshotPtr &frame);
    void communicationError(const fosd::communication::CommunicationError &error);
    void decoderError(const fosd::communication::DecoderError &error);
    void protocolError(const fosd::protocol::ProtocolError &error);
    void calculationError(const fosd::Calculation::CalculationError &error);
    void storageError(const fosd::storage::StorageError &error);
    void storageStarted(const QString &session);
    void storageStopped();
    void connectionState(fosd::communication::Channel channel,
                         fosd::communication::ConnectionState state);
    void updateStatistics();

private:
    void createCalculation(bool plate);
    bool setupPlate(const QVariantMap &parameters, bool send, QString *error);
    void startPlateSenderWhenReady();
    void stopPlate();
    void status(const QString &message);
    vtkRenderer *renderer(QString *error) const;
    std::unique_ptr<visualization::VisualizationControl> m_control;
    vtkSmartPointer<vtkActor> m_demo;
    std::unique_ptr<protocol::ProtocolControl> m_protocol;
    std::unique_ptr<Calculation::CalculationControl> m_calculation;
    QTimer m_statistics;
    QString m_status;
    bool m_modelPending = false;
    bool m_plateMode = false;
    bool m_platePending = false;
    bool m_plateSendingRequested = false;
    bool m_plateUdpReady = false;
    bool m_plateConfigurationReady = false;
    simulation::PlateSenderSettings m_plateSettings;
    simulation::PlateUdpSender m_plateSender;
    QMetaObject::Connection m_plateFrameConnection;
};
} // namespace vtk_test
} // namespace fosd
