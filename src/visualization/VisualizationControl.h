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
// GUI-thread facade. Model loading and deformation display are implemented;
// Scene environment and interaction stay on the GUI thread.

class VisualizationControl final : public QObject {
    Q_OBJECT
public:
    explicit VisualizationControl(QObject *parent = nullptr,
                                  vtkRenderWindow *renderWindowPtr = nullptr);
    ~VisualizationControl() override;
    bool attach(vtkRenderWindow *window);
    void detach();
    vtkRenderWindow *renderWindow() const {
        return m_renderWindow;
    } // Borrowed, GUI thread only.

    bool loadModel(const QString &path);
    void unloadModel();
    core::ModelDataPtr originalModel() const;
    qint64 modelPolygonCount() const;

    void setResultRepository(core::CalculationRepositoryPtr repository);
    void applyResult(const core::CalculationResultPtr &result);
    void setExpectedModelRevision(quint64 revision);
    void setDeformationScale(double scale);
    void resetDeformation();
    void setfreshFPS(int Interval);

    void setScalarField(ScalarField field);
    void setColorRangeMode(ColorRangeMode mode);
    void setColorRange(double minimum, double maximum);
    bool setScalarColorSettings(const ScalarColorSettings &settings);
    ScalarColorSettings scalarColorSettings() const;
    ScalarColorRange scalarColorRange() const;
    void setScalarColoringEnabled(bool enabled);
    void setScalarMappingMode(ScalarMappingMode mode);
    void setScalarValueMode(ScalarValueMode mode);
    // For future nodal strain/temperature/etc. Hardware strain arrays must be
    // interpolated to model point IDs before passing through this generic port.
    bool setPointScalarField(const PointScalarField &field);
    bool setKeyPoints(const QVector<KeyPointDefinition> &points);
    QVector<KeyPointDefinition> keyPoints() const;
    bool setKeyPointStyle(const KeyPointStyle &style);
    bool setSelectionStyle(const KeyPointStyle &style);
    KeyPointStyle keyPointStyle() const;
    KeyPointStyle selectionStyle() const;
    void showKeyPoints(bool visible);
    bool setPickingSettings(const PickingSettings &settings);
    PickingSettings pickingSettings() const;
    PickedPointInfo selectPoint(vtkIdType pointId);
    PickedPointInfo selectedPoint() const;
    QVector<vtkIdType> nearbyPoints(const std::array<double, 3> &positionMm, double radiusMm) const;
    void clearSelection();
    void showAxes(bool visible);
    void showGrid(bool visible);
    bool showGrid(GridPlane plane, bool visible);
    bool setAxesSettings(const AxesSettings &settings);
    bool setGridSettings(const GridSettings &settings);
    bool setBackgroundSettings(const BackgroundSettings &settings);
    bool setWorldSettings(const WorldSettings &settings);
    bool setCameraSettings(const CameraSettings &settings);
    bool setCameraPose(const CameraPose &pose);
    AxesSettings axesSettings() const;
    GridSettings gridSettings() const;
    BackgroundSettings backgroundSettings() const;
    WorldSettings worldSettings() const;
    CameraSettings cameraSettings() const;
    CameraPose cameraPose() const;
    bool setBackground(double red, double green, double blue);
    void setCameraPreset(CameraPreset preset);
    void setCameraLocked(bool locked);
    void resetCamera();

signals:
    void modelLoaded(const fosd::core::ModelDataPtr &model);
    void pointPicked(const fosd::visualization::PickedPointInfo &info);
    void selectionUpdated(const fosd::visualization::PickedPointInfo &info);
    void errorOccurred(const QString &message);
    void scalarColorSettingsChanged(const fosd::visualization::ScalarColorSettings &settings);

private slots:
    void refreshResult();
    void handlePointPicked(const fosd::visualization::PickedPointInfo &info);

private:
    void updateScalarColors();
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
    core::CalculationResultPtr m_colorResult;
    PointScalarField m_customField;

    QTimer *m_refreshTimer = nullptr; // GUI polls the latest result at 30 Hz.
    quint64 m_expectedModelRevision = 0;
    quint64 m_lastResultRevision = 0;
};

} // namespace visualization
} // namespace fosd
