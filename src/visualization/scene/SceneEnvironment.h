#pragma once
#include "visualization/VisualizationTypes.h"
#include <vtkSmartPointer.h>
class vtkRenderer;
class vtkAxesActor;
class vtkActor;
class vtkCamera;
namespace fosd {
namespace visualization {
class SceneEnvironment final {
public:
    SceneEnvironment();
    ~SceneEnvironment();
    void setRenderer(vtkRenderer *renderer);
    void showAxes(bool visible);
    void showGrid(bool visible);
    void setBackground(double red, double green, double blue);
    void setCameraPreset(CameraPreset preset);
    void setCameraLocked(bool locked);
    void resetCamera();
    void clear();

private:
    vtkRenderer *m_renderer = nullptr; // Borrowed from Control.
    vtkSmartPointer<vtkAxesActor> m_axes;
    vtkSmartPointer<vtkActor> m_grid;
    vtkSmartPointer<vtkCamera> m_camera;
};
} // namespace visualization
} // namespace fosd
