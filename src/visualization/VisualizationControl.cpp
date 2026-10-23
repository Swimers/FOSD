#include "VisualizationControl.h"

#include "mesh/MeshManager.h"
#include "render/DeformationRenderer.h"
#include "render/ScalarColorMapper.h"
#include "marker/KeyPointManager.h"
#include "interaction/PointPicker.h"
#include "interaction/ModelInteractorStyle.h"
#include "scene/SceneEnvironment.h"

#include <vtkRenderer.h>

namespace fosd { 
namespace visualization {

VisualizationControl::VisualizationControl(
    QObject *parent,
    vtkRenderWindow *renderWindowPtr)
    : QObject(parent),
      m_renderer(vtkSmartPointer<vtkRenderer>::New())
{
    if (renderWindowPtr)
        attach(renderWindowPtr);
    emit errorOccurred(QStringLiteral(""));
}

VisualizationControl::~VisualizationControl() = default;

bool VisualizationControl::attach(vtkRenderWindow *window)
{
    if(!window){
        emit errorOccurred(QStringLiteral("No rendering window is provided"));
        return false;
    }

    if(m_renderWindow == window)
        return true;
    
    detach();

    m_renderWindow = window;
    m_renderWindow->AddRenderer(m_renderer);
    return true;

}

void VisualizationControl::detach()
{
    if(m_renderWindow && m_renderer)
        m_renderWindow->RemoveRenderer(m_renderer);

    m_renderWindow = nullptr;
}





} 
}
