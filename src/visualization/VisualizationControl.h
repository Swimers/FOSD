#pragma once

#include "VisualizationTypes.h"
#include "core/Datamodel/CalculationRepository.h"

#include <QObject>
#include <memory>

#include <vtkSmartPointer.h>
#include <vtkRenderWindow.h>

class QTimer;
class vtkRenderer;
class vtkRenderWindow;
namespace fosd { 
namespace visualization {

class MeshManager;
class DeformationRenderer;
class ScalarColorMapper;
class KeyPointManager;
class PointPicker;
class ModelInteractorStyle;
class SceneEnvironment;
// GUI-thread facade. Business APIs are declarations only at this stage.

class VisualizationControl final : public QObject {
    Q_OBJECT
public:
    explicit VisualizationControl(QObject *parent = nullptr, vtkRenderWindow *renderWindowPtr = nullptr);
    ~VisualizationControl() override;
    bool attach(vtkRenderWindow *window);
    void detach();

    bool loadModel(const QString &path);
    void unloadModel();
    core::ModelDataPtr originalModel() const;

    void setResultRepository(core::CalculationRepositoryPtr repository);
    void applyResult(const core::CalculationResultPtr &result);
    void setExpectedModelRevision(quint64 revision);
    void setDeformationScale(double scale);
    void resetDeformation();

    void setScalarField(ScalarField field);
    void setColorRangeMode(ColorRangeMode mode);
    void setColorRange(double minimum, double maximum);
    void setKeyPoints(const QVector<KeyPointDefinition> &points);
    void showKeyPoints(bool visible);
    void clearSelection();
    void showAxes(bool visible);
    void showGrid(bool visible);
    void setCameraPreset(CameraPreset preset);
    void setCameraLocked(bool locked);
    void resetCamera();

signals:
    void modelLoaded(const fosd::core::ModelDataPtr &model);
    void pointPicked(const fosd::visualization::PickedPointInfo &info);
    void errorOccurred(const QString &message);

private:
    vtkRenderWindow *m_renderWindow = nullptr; // Borrowed; detach before host destroys it.
    vtkSmartPointer<vtkRenderer> m_renderer;
    std::unique_ptr<MeshManager> m_mesh;
    std::unique_ptr<DeformationRenderer> m_deformation;
    std::unique_ptr<ScalarColorMapper> m_colors;
    std::unique_ptr<KeyPointManager> m_keyPoints;
    std::unique_ptr<PointPicker> m_picker;
    vtkSmartPointer<ModelInteractorStyle> m_interactorStyle;
    std::unique_ptr<SceneEnvironment> m_scene;
    core::CalculationRepositoryPtr m_repository;
    QTimer *m_refreshTimer = nullptr;   // Reserved only; no timer is created yet.
    quint64 m_expectedModelRevision = 0;
    quint64 m_lastResultRevision = 0;

};

}
}
