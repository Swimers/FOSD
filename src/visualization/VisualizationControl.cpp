#include "VisualizationControl.h"

#include "mesh/MeshManager.h"
#include "render/DeformationRenderer.h"
#include "render/ScalarColorMapper.h"
#include "marker/KeyPointManager.h"
#include "interaction/PointPicker.h"
#include "interaction/ModelInteractorStyle.h"
#include "scene/SceneEnvironment.h"

#include <vtkRenderer.h>
#include <vtkRenderWindowInteractor.h>

namespace fosd { 
namespace visualization {

VisualizationControl::VisualizationControl(
    QObject *parent,
    vtkRenderWindow *renderWindowPtr)
    : QObject(parent),
      m_renderer(vtkSmartPointer<vtkRenderer>::New()),
      m_mesh(new MeshManager),
      m_deformation(new DeformationRenderer),
      m_colors(new ScalarColorMapper),
      m_keyPoints(new KeyPointManager),
      m_picker(new PointPicker),
      m_interactorStyle(vtkSmartPointer<ModelInteractorStyle>::New()),
      m_scene(new SceneEnvironment)
{
    m_picker->setRenderer(m_renderer.GetPointer());
    m_interactorStyle->setPointPicker(m_picker.get());
    m_scene->setRenderer(m_renderer.GetPointer());

    connect(m_picker.get(), &PointPicker::pointPicked,
            this, &VisualizationControl::pointPicked);

    if (renderWindowPtr)
        attach(renderWindowPtr);
}

VisualizationControl::~VisualizationControl()
{
    detach();
}

bool VisualizationControl::attach(vtkRenderWindow *window)
{
    if(!window){
        emit errorOccurred(QStringLiteral("No rendering window is provided"));
        return false;
    }

    if(m_renderWindow == window) {
        vtkRenderWindowInteractor *interactor = window->GetInteractor();
        if(interactor)
            interactor->SetInteractorStyle(m_interactorStyle.GetPointer());
        return true;
    }
    
    detach();

    m_renderWindow = window;
    m_renderWindow->AddRenderer(m_renderer);

    vtkRenderWindowInteractor *interactor = m_renderWindow->GetInteractor();
    if(interactor)
        interactor->SetInteractorStyle(m_interactorStyle.GetPointer());

    return true;

}

void VisualizationControl::detach()
{
    if(m_renderWindow) {
        vtkRenderWindowInteractor *interactor = m_renderWindow->GetInteractor();
        if(interactor && interactor->GetInteractorStyle() == m_interactorStyle.GetPointer())
            interactor->SetInteractorStyle(nullptr);

        m_renderWindow->RemoveRenderer(m_renderer);
    }

    m_renderWindow = nullptr;
}





} 
}
