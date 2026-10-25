#include "SceneEnvironment.h"
#include <vtkActor.h>
#include <vtkAxesActor.h>
#include <vtkCamera.h>
namespace fosd {
namespace visualization {
SceneEnvironment::SceneEnvironment() = default;
SceneEnvironment::~SceneEnvironment() = default;

void SceneEnvironment::setRenderer(vtkRenderer *renderer) {
    m_renderer = renderer;
}

} // namespace visualization
} // namespace fosd
