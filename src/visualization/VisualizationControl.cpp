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
VisualizationControl::VisualizationControl(QObject *parent, vtkRenderWindow *renderWindowPtr)
    : QObject(parent), m_renderer(vtkSmartPointer<vtkRenderer>::New()), m_mesh(new MeshManager),
      m_deformation(new DeformationRenderer), m_colors(new ScalarColorMapper),
      m_keyPoints(new KeyPointManager), m_picker(new PointPicker),
      m_interactorStyle(vtkSmartPointer<ModelInteractorStyle>::New()),
      m_scene(new SceneEnvironment) {
    qRegisterMetaType<ScalarColorSettings>("fosd::visualization::ScalarColorSettings");
    // 子模块初始化
    m_picker->setRenderer(m_renderer.GetPointer());
    m_interactorStyle->setPointPicker(m_picker.get());
    m_scene->setRenderer(m_renderer.GetPointer());

    // 刷新
    connect(m_picker.get(), &PointPicker::pointPicked, this, &VisualizationControl::pointPicked);
    m_refreshTimer = new QTimer(this);
    m_refreshTimer->setInterval(33);
    connect(m_refreshTimer, &QTimer::timeout, this, &VisualizationControl::refreshResult);

    // 功能绑定
    if (renderWindowPtr)
        attach(renderWindowPtr);
}

VisualizationControl::~VisualizationControl() {
    detach();
}

bool VisualizationControl::attach(vtkRenderWindow *window) {
    if (!window) {
        emit errorOccurred(QStringLiteral("No rendering window is provided"));
        return false;
    }

    if (m_renderWindow == window) {
        vtkRenderWindowInteractor *interactor = window->GetInteractor();
        if (interactor)
            interactor->SetInteractorStyle(m_interactorStyle.GetPointer());
        return true;
    }

    detach();

    m_renderWindow = window;
    m_renderWindow->AddRenderer(m_renderer);

    // 绑定交互器
    vtkRenderWindowInteractor *interactor = m_renderWindow->GetInteractor();
    if (interactor)
        interactor->SetInteractorStyle(m_interactorStyle.GetPointer());

    return true;
}

void VisualizationControl::detach() {
    if (m_renderWindow) {
        vtkRenderWindowInteractor *interactor = m_renderWindow->GetInteractor();
        if (interactor && interactor->GetInteractorStyle() == m_interactorStyle.GetPointer())
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

    m_colors->clear(m_deformation->displayMesh(), m_deformation->mapper());
    m_colorResult.reset();
    m_customField = PointScalarField();
    m_deformation->setOriginalMesh(m_mesh->originalMesh());
    m_deformation->actor()->GetProperty()->SetColor(0.5, 0.65, 0.15);
    m_renderer->AddActor(m_deformation->actor());
    m_expectedModelRevision = 0;
    m_lastResultRevision = 0;
    resetCamera();
    emit modelLoaded(originalModel());
    return true;
}

void VisualizationControl::unloadModel() {
    if (m_deformation->actor())
        m_renderer->RemoveActor(m_deformation->actor());
    m_colors->clear(m_deformation->displayMesh(), m_deformation->mapper());
    m_colorResult.reset();
    m_customField = PointScalarField();
    m_deformation->clear();
    m_mesh->clear();
    m_expectedModelRevision = 0;
    m_lastResultRevision = 0;

    if (m_renderWindow)
        m_renderWindow->Render();
}

core::ModelDataPtr VisualizationControl::originalModel() const {
    return m_mesh->originalModel();
}

void VisualizationControl::setResultRepository(core::CalculationRepositoryPtr repository) {
    m_repository = std::move(repository);
    if (m_repository)
        m_refreshTimer->start();
    else
        m_refreshTimer->stop();
}

void VisualizationControl::refreshResult() {
    if (m_renderWindow && m_repository)
        applyResult(m_repository->latest());
}

void VisualizationControl::setExpectedModelRevision(quint64 revision) {
    if (revision != m_expectedModelRevision) {
        m_colorResult.reset();
        m_customField = PointScalarField();
        m_colors->clear(m_deformation->displayMesh(), m_deformation->mapper());
    }
    m_expectedModelRevision = revision;
    m_lastResultRevision = 0;
}

void VisualizationControl::applyResult(const core::CalculationResultPtr &result) {
    if (!result || !originalModel() || !m_expectedModelRevision ||
        result->modelRevision != m_expectedModelRevision ||
        result->revision <= m_lastResultRevision)
        return;

    if (result->displacementMap.size() != originalModel()->size()) {
        emit errorOccurred(QStringLiteral("Result point count does not match the model"));
        return;
    }

    for (auto point = originalModel()->constBegin(); point != originalModel()->constEnd();
         ++point) {
        const auto delta = result->displacementMap.value(point.key());
        if (delta.size() != 3 || !std::isfinite(delta[0]) || !std::isfinite(delta[1]) ||
            !std::isfinite(delta[2])) {
            emit errorOccurred(QStringLiteral("Invalid model displacement"));
            return;
        }
    }
    m_deformation->applyResult(result);
    m_colorResult = result;
    updateScalarColors();
    m_lastResultRevision = result->revision;
    if (m_renderWindow)
        m_renderWindow->Render();
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
    m_colorResult.reset();
    m_customField = PointScalarField();
    m_colors->clear(m_deformation->displayMesh(), m_deformation->mapper());
    if (m_renderWindow)
        m_renderWindow->Render();
}

void VisualizationControl::setfreshFPS(int Interval) {
    m_refreshTimer->stop();
    m_refreshTimer->setInterval(Interval);
    m_refreshTimer->start();
    if (m_renderWindow)
        m_renderWindow->Render();
}

void VisualizationControl::resetCamera() {
    m_renderer->ResetCamera();
    if (m_renderWindow)
        m_renderWindow->Render();
}

ScalarColorSettings VisualizationControl::scalarColorSettings() const {
    return m_colors->settings();
}

ScalarColorRange VisualizationControl::scalarColorRange() const {
    return m_colors->range();
}

bool VisualizationControl::setScalarColorSettings(const ScalarColorSettings &settings) {
    QString error;
    if (!m_colors->setSettings(settings, &error)) {
        emit errorOccurred(error);
        return false;
    }
    updateScalarColors();
    emit scalarColorSettingsChanged(settings);
    if (m_renderWindow)
        m_renderWindow->Render();
    return true;
}

void VisualizationControl::setScalarField(ScalarField field) {
    auto settings = scalarColorSettings();
    settings.field = field;
    setScalarColorSettings(settings);
}

void VisualizationControl::setColorRangeMode(ColorRangeMode mode) {
    auto settings = scalarColorSettings();
    settings.rangeMode = mode;
    setScalarColorSettings(settings);
}

void VisualizationControl::setColorRange(double minimum, double maximum) {
    auto settings = scalarColorSettings();
    settings.minimum = minimum;
    settings.maximum = maximum;
    setScalarColorSettings(settings);
}

void VisualizationControl::setScalarColoringEnabled(bool enabled) {
    auto settings = scalarColorSettings();
    settings.enabled = enabled;
    setScalarColorSettings(settings);
}

void VisualizationControl::setScalarMappingMode(ScalarMappingMode mode) {
    auto settings = scalarColorSettings();
    settings.mappingMode = mode;
    setScalarColorSettings(settings);
}

void VisualizationControl::setScalarValueMode(ScalarValueMode mode) {
    auto settings = scalarColorSettings();
    settings.valueMode = mode;
    setScalarColorSettings(settings);
}

bool VisualizationControl::setPointScalarField(const PointScalarField &field) {
    const auto model = originalModel();
    if (!model || !m_expectedModelRevision || field.modelRevision != m_expectedModelRevision ||
        field.id.isEmpty() || field.values.size() != model->size()) {
        emit errorOccurred(
            QStringLiteral("Point scalar field does not match the current model revision"));
        return false;
    }
    for (auto point = model->constBegin(); point != model->constEnd(); ++point) {
        const auto value = field.values.constFind(point.key());
        if (value == field.values.constEnd() || !std::isfinite(value.value())) {
            emit errorOccurred(QStringLiteral("Every model point requires a finite scalar"));
            return false;
        }
    }
    m_customField = field;
    auto settings = scalarColorSettings();
    settings.field = ScalarField::Custom;
    return setScalarColorSettings(settings);
}

void VisualizationControl::updateScalarColors() {
    auto *mesh = m_deformation->displayMesh();
    auto *mapper = m_deformation->mapper();
    if (!mesh || !mapper)
        return;
    const auto settings = m_colors->settings();
    if (!settings.enabled || (settings.field != ScalarField::Custom && !m_colorResult) ||
        (settings.field == ScalarField::Custom && m_customField.id.isEmpty())) {
        m_colors->clear(mesh, mapper);
        return;
    }
    QString error;
    const bool ok = settings.field == ScalarField::Custom
                        ? m_colors->applyField(m_customField, mesh, mapper, &error)
                        : m_colors->applyResult(m_colorResult, mesh, mapper, &error);
    if (!ok)
        emit errorOccurred(error);
}

} // namespace visualization
} // namespace fosd
