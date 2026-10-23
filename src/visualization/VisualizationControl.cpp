#include "VisualizationControl.h"

#include "mesh/MeshManager.h"
#include "render/DeformationRenderer.h"
#include "render/ScalarColorMapper.h"
#include "marker/KeyPointManager.h"
#include "interaction/PointPicker.h"
#include "interaction/ModelInteractorStyle.h"
#include "scene/SceneEnvironment.h"
#include <vtkRenderer.h>
namespace fosd { namespace visualization {
VisualizationControl::VisualizationControl(QObject *parent) : QObject(parent) {}
VisualizationControl::~VisualizationControl() = default;
} }
