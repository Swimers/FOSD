#pragma once
#include "visualization/VisualizationTypes.h"
#include <QObject>
#include <vtkSmartPointer.h>
class vtkPointPicker;
class vtkPointLocator;
class vtkRenderer;
class vtkPolyData;
class vtkActor;
namespace fosd {
namespace visualization {
class KeyPointManager;
class PointPicker final : public QObject {
    Q_OBJECT
public:
    explicit PointPicker(QObject *parent = nullptr);
    ~PointPicker() override;
    void setRenderer(vtkRenderer *renderer);
    void setKeyPointManager(KeyPointManager *manager);
    void setMesh(vtkPolyData *displayMesh, core::ModelDataPtr originalModel,
                 vtkActor *actor = nullptr);
    void setResult(core::CalculationResultPtr result);
    bool setSettings(const PickingSettings &settings, QString *error = nullptr);
    PickingSettings settings() const {
        return m_settings;
    }
    PickedPointInfo pick(int displayX, int displayY);
    PickedPointInfo selectPoint(vtkIdType id);
    PickedPointInfo selection() const;
    void refreshSelection();
    void cameraInteractionStarted();
    QVector<vtkIdType> nearbyPoints(const std::array<double, 3> &positionMm, double radiusMm) const;
    void clearSelection();
signals:
    void pointPicked(const fosd::visualization::PickedPointInfo &info);
    void selectionUpdated(const fosd::visualization::PickedPointInfo &info);

private:
    bool selectable(vtkIdType id) const;
    vtkRenderer *m_renderer = nullptr;
    vtkPolyData *m_displayMesh = nullptr;
    vtkActor *m_modelActor = nullptr;
    KeyPointManager *m_keyPoints = nullptr;
    vtkSmartPointer<vtkPointPicker> m_picker;
    vtkSmartPointer<vtkPointLocator> m_locator;
    core::ModelDataPtr m_originalModel;
    core::CalculationResultPtr m_result;
    PickingSettings m_settings;
    vtkIdType m_selectedId = -1;
};
} // namespace visualization
} // namespace fosd
