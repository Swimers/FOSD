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
#include <vtkPolyData.h>
#include <vtkProperty.h>
#include <QTimer>
#include <cmath>

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
    m_refreshTimer = new QTimer(this);
    m_refreshTimer->setInterval(33);
    connect(m_refreshTimer, &QTimer::timeout, this, &VisualizationControl::refreshResult);

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

bool VisualizationControl::loadModel(const QString &path) {
    QString error;
    if (!m_mesh->loadModel(path, &error)) { 
        emit errorOccurred(error); 
        return false; 
    }

    if (m_deformation->actor()) 
        m_renderer->RemoveActor(m_deformation->actor());

    m_deformation->setOriginalMesh(m_mesh->originalMesh());
    m_deformation->actor()->GetProperty()->SetColor(0.5, 0.65, 0.15);
    m_renderer->AddActor(m_deformation->actor());
    m_expectedModelRevision = 0; m_lastResultRevision = 0;
    resetCamera(); emit modelLoaded(originalModel());
    return true;
}
void VisualizationControl::unloadModel() {
    if (m_deformation->actor()) m_renderer->RemoveActor(m_deformation->actor());
    m_deformation->clear(); m_mesh->clear(); m_expectedModelRevision = 0; m_lastResultRevision = 0;
    if (m_renderWindow) m_renderWindow->Render();
}
core::ModelDataPtr VisualizationControl::originalModel() const { return m_mesh->originalModel(); }
void VisualizationControl::setResultRepository(core::CalculationRepositoryPtr repository) {
    m_repository = std::move(repository);
    if (m_repository) m_refreshTimer->start(); else m_refreshTimer->stop();
}
void VisualizationControl::refreshResult() {
    if (m_renderWindow && m_repository) applyResult(m_repository->latest());
}
void VisualizationControl::setExpectedModelRevision(quint64 revision) {
    m_expectedModelRevision = revision; m_lastResultRevision = 0;
}
void VisualizationControl::applyResult(const core::CalculationResultPtr &result) {
    if (!result || !originalModel() || !m_expectedModelRevision ||
        result->modelRevision != m_expectedModelRevision || result->revision <= m_lastResultRevision) return;
    if (result->displacementMap.size() != originalModel()->size()) {
        emit errorOccurred(QStringLiteral("Result point count does not match the model")); return;
    }
    for (auto point = originalModel()->constBegin(); point != originalModel()->constEnd(); ++point) {
        const auto delta = result->displacementMap.value(point.key());
        if (delta.size() != 3 || !std::isfinite(delta[0]) || !std::isfinite(delta[1]) || !std::isfinite(delta[2])) {
            emit errorOccurred(QStringLiteral("Invalid model displacement")); return;
        }
    }
    m_deformation->applyResult(result); m_lastResultRevision = result->revision;
    if (m_renderWindow) m_renderWindow->Render();
}
void VisualizationControl::setDeformationScale(double scale) {
    if (!std::isfinite(scale)) 
        return;
    m_deformation->setDeformationScale(scale); 
    if (m_renderWindow) 
        m_renderWindow->Render();
}
void VisualizationControl::resetDeformation() {
    m_deformation->resetDeformation(); 
    if (m_renderWindow) 
        m_renderWindow->Render();
}
void VisualizationControl::resetCamera() {
    m_renderer->ResetCamera(); 
    if (m_renderWindow) 
        m_renderWindow->Render();
}





} 
}
