#pragma once
#include "visualization/VisualizationTypes.h"
#include <vtkSmartPointer.h>
#include <vtkRenderer.h>

class vtkActor;
class vtkBillboardTextActor3D;

namespace fosd {
namespace visualization {
class KeyPointManager final {
public:
    KeyPointManager();
    ~KeyPointManager();

    void setRenderer(vtkRenderer *renderer);

    void setKeyPoints(const QVector<KeyPointDefinition> &points);
    void applyResult(const core::CalculationResultPtr &result, double displayScale);
    void setVisible(bool visible);
    void clearSelection();
    void clear();

private:
    vtkRenderer *m_renderer = nullptr;

    QVector<KeyPointDefinition> m_definitions;
    QVector<vtkSmartPointer<vtkActor>> m_markers;
    QVector<vtkSmartPointer<vtkBillboardTextActor3D>> m_labels;
    bool m_visible = true;
};

} // namespace visualization
} // namespace fosd
