#pragma once
#include "visualization/VisualizationTypes.h"
#include <QObject>
#include <vtkSmartPointer.h>
class vtkPointPicker;
class vtkPointLocator;
class vtkRenderer;
class vtkPolyData;
namespace fosd { namespace visualization {
class PointPicker final : public QObject {
    Q_OBJECT
public:
    explicit PointPicker(QObject *parent = nullptr);
    ~PointPicker() override;
    void setRenderer(vtkRenderer *renderer);
    void setMesh(vtkPolyData *displayMesh, core::ModelDataPtr originalModel);
    PickedPointInfo pick(int displayX, int displayY);
    QVector<vtkIdType> nearbyPoints(const std::array<double, 3> &positionMm, double radiusMm) const;
    void clearSelection();
signals:
    void pointPicked(const fosd::visualization::PickedPointInfo &info);
private:
    vtkRenderer *m_renderer = nullptr; // Borrowed from Control.
    vtkPolyData *m_displayMesh = nullptr; // Borrowed from DeformationRenderer.
    vtkSmartPointer<vtkPointPicker> m_picker;
    vtkSmartPointer<vtkPointLocator> m_locator;
    core::ModelDataPtr m_originalModel;
};
} }
