#include "VisualizationControl.h"
#include "config/ParameterStore.h"

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
    : QObject(parent), 
      m_renderer(vtkSmartPointer<vtkRenderer>::New()), 
      m_mesh(new MeshManager),
      m_deformation(new DeformationRenderer), 
      m_colors(new ScalarColorMapper),
      m_keyPoints(new KeyPointManager), 
      m_picker(new PointPicker),
      m_interactorStyle(vtkSmartPointer<ModelInteractorStyle>::New()),
      m_scene(new SceneEnvironment) {
    qRegisterMetaType<ScalarColorSettings>("fosd::visualization::ScalarColorSettings");
    // 子模块初始化
    m_picker->setRenderer(m_renderer.GetPointer());
    m_keyPoints->setRenderer(m_renderer.GetPointer());
    m_picker->setKeyPointManager(m_keyPoints.get());
    m_interactorStyle->setPointPicker(m_picker.get());
    m_interactorStyle->SetDefaultRenderer(m_renderer);
    m_scene->setRenderer(m_renderer.GetPointer());

    // 刷新
    connect(m_picker.get(), &PointPicker::pointPicked, this,
            &VisualizationControl::handlePointPicked);
    connect(m_picker.get(), &PointPicker::selectionUpdated, this,
            &VisualizationControl::selectionUpdated);
    m_refreshTimer = new QTimer(this);
    m_refreshTimer->setInterval(VisualizationRuntimeSettings().refreshIntervalMs);
    connect(m_refreshTimer, &QTimer::timeout, this, &VisualizationControl::refreshResult);

    config::ParameterStore parameters;
    QString parameterError;
    ScalarColorSettings saved0;
    if (!parameters.initialize(&saved0, &parameterError) || !m_colors->setSettings(saved0, &parameterError))
        QMetaObject::invokeMethod(this, "errorOccurred", Qt::QueuedConnection, Q_ARG(QString, parameterError));
    AxesSettings saved1;
    if (!parameters.initialize(&saved1, &parameterError) || !m_scene->setAxesSettings(saved1, &parameterError))
        QMetaObject::invokeMethod(this, "errorOccurred", Qt::QueuedConnection, Q_ARG(QString, parameterError));
    GridSettings saved2;
    if (!parameters.initialize(&saved2, &parameterError) || !m_scene->setGridSettings(saved2, &parameterError))
        QMetaObject::invokeMethod(this, "errorOccurred", Qt::QueuedConnection, Q_ARG(QString, parameterError));
    BackgroundSettings saved3;
    if (!parameters.initialize(&saved3, &parameterError) || !m_scene->setBackgroundSettings(saved3, &parameterError))
        QMetaObject::invokeMethod(this, "errorOccurred", Qt::QueuedConnection, Q_ARG(QString, parameterError));
    WorldSettings saved4;
    if (!parameters.initialize(&saved4, &parameterError) || !m_scene->setWorldSettings(saved4, &parameterError))
        QMetaObject::invokeMethod(this, "errorOccurred", Qt::QueuedConnection, Q_ARG(QString, parameterError));
    CameraSettings saved5;
    if (!parameters.initialize(&saved5, &parameterError) || !m_scene->setCameraSettings(saved5, &parameterError))
        QMetaObject::invokeMethod(this, "errorOccurred", Qt::QueuedConnection, Q_ARG(QString, parameterError));
    KeyPointStyle saved6;
    if (!parameters.initialize(&saved6, &parameterError) || !m_keyPoints->setStyle(saved6, &parameterError))
        QMetaObject::invokeMethod(this, "errorOccurred", Qt::QueuedConnection, Q_ARG(QString, parameterError));
    SelectionStyleSettings saved7;
    if (!parameters.initialize(&saved7, &parameterError) || !m_keyPoints->setSelectionStyle(saved7, &parameterError))
        QMetaObject::invokeMethod(this, "errorOccurred", Qt::QueuedConnection, Q_ARG(QString, parameterError));
    PickingSettings saved8;
    if (!parameters.initialize(&saved8, &parameterError) || !m_picker->setSettings(saved8, &parameterError))
        QMetaObject::invokeMethod(this, "errorOccurred", Qt::QueuedConnection, Q_ARG(QString, parameterError));
    VisualizationRuntimeSettings runtime;
    if (parameters.initialize(&runtime, &parameterError)) {
        m_deformation->setDeformationScale(runtime.deformationScale);
        m_refreshTimer->setInterval(runtime.refreshIntervalMs);
    } else QMetaObject::invokeMethod(this, "errorOccurred", Qt::QueuedConnection, Q_ARG(QString, parameterError));
    m_interactorStyle->setCameraLocked(m_scene->cameraSettings().locked);
    m_interactorStyle->setClickTolerancePixels(m_picker->settings().clickTolerancePixels);

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
    // 是在替换网格之前，主动切断其它模块对旧 VTK 对象的裸指针引用
    m_picker->setMesh(nullptr, {});
    m_keyPoints->setModel({}, nullptr);

    m_colorResult.reset();
    m_customField = PointScalarField();
    m_deformation->setOriginalMesh(m_mesh->originalMesh());
    m_deformation->actor()->GetProperty()->SetColor(0.5, 0.65, 0.15);
    m_renderer->AddActor(m_deformation->actor());
    m_keyPoints->setModel(originalModel(), m_deformation->displayMesh());
    m_scene->setModelActor(m_deformation->actor());
    m_picker->setMesh(m_deformation->displayMesh(), originalModel(), m_deformation->actor());
    m_expectedModelRevision = 0;
    m_lastResultRevision = 0;
    resetCamera();
    emit modelLoaded(originalModel());
    return true;
}

void VisualizationControl::unloadModel() {
    m_scene->setModelActor(nullptr);
    m_picker->setMesh(nullptr, {});
    m_keyPoints->setModel({}, nullptr);
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

qint64 VisualizationControl::modelPolygonCount() const {
    return m_mesh->polygonCount();
}

void VisualizationControl::setResultRepository(core::CalculationRepositoryPtr repository) {
    if (m_repository == repository)
        return;
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
        m_picker->clearSelection();
        m_picker->setResult({});
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
    m_picker->setResult(result);
    m_keyPoints->updatePositions();
    m_picker->refreshSelection();
    m_colorResult = result;
    updateScalarColors();
    m_lastResultRevision = result->revision;
    if (m_renderWindow)
        m_renderWindow->Render();
}

void VisualizationControl::setDeformationScale(double scale) {
    VisualizationRuntimeSettings saved;
    QString error;
    config::ParameterStore store;
    if (!store.initialize(&saved, &error)) { emit errorOccurred(error); return; }
    saved.deformationScale = scale;
    if (!store.write(saved, true, &error)) { emit errorOccurred(error); return; }

    if (!std::isfinite(scale))
        return;
    m_deformation->setDeformationScale(scale);
    m_keyPoints->updatePositions();
    m_picker->refreshSelection();
    if (m_renderWindow)
        m_renderWindow->Render();
}

void VisualizationControl::resetDeformation() {
    m_lastResultRevision = 0;
    m_deformation->resetDeformation();
    m_picker->setResult({});
    m_keyPoints->updatePositions();
    m_picker->refreshSelection();
    m_colorResult.reset();
    m_customField = PointScalarField();
    m_colors->clear(m_deformation->displayMesh(), m_deformation->mapper());
    if (m_renderWindow)
        m_renderWindow->Render();
}

void VisualizationControl::setfreshFPS(int Interval) {
    VisualizationRuntimeSettings saved;
    QString error;
    config::ParameterStore store;
    if (!store.initialize(&saved, &error)) { emit errorOccurred(error); return; }
    saved.refreshIntervalMs = Interval;
    if (!store.write(saved, true, &error)) { emit errorOccurred(error); return; }

    m_refreshTimer->stop();
    m_refreshTimer->setInterval(Interval);
    m_refreshTimer->start();
    if (m_renderWindow)
        m_renderWindow->Render();
}

void VisualizationControl::resetCamera() {
    m_scene->resetCamera();
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
    const auto previous = m_colors->settings();
    QString error;
    if (!m_colors->setSettings(settings, &error)) {
        emit errorOccurred(error);
        return false;
    }
    if (!config::ParameterStore().write(settings, true, &error)) {
        m_colors->setSettings(previous, nullptr); emit errorOccurred(error); return false;
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

bool VisualizationControl::setKeyPoints(const QVector<KeyPointDefinition> &points) {
    QString error;
    if (!m_keyPoints->setKeyPoints(points, &error)) {
        emit errorOccurred(error);
        return false;
    }
    m_picker->refreshSelection();
    if (m_renderWindow)
        m_renderWindow->Render();
    return true;
}

QVector<KeyPointDefinition> VisualizationControl::keyPoints() const {
    return m_keyPoints->keyPoints();
}

KeyPointStyle VisualizationControl::keyPointStyle() const {
    return m_keyPoints->style();
}

KeyPointStyle VisualizationControl::selectionStyle() const {
    return m_keyPoints->selectionStyle();
}

bool VisualizationControl::setKeyPointStyle(const KeyPointStyle &style) {
    const auto previous = m_keyPoints->style();
    QString error;
    if (!m_keyPoints->setStyle(style, &error)) {
        emit errorOccurred(error);
        return false;
    }
    if (!config::ParameterStore().write(style, true, &error)) {
        m_keyPoints->setStyle(previous, nullptr); emit errorOccurred(error); return false;
    }
    if (m_renderWindow)
        m_renderWindow->Render();
    return true;
}

bool VisualizationControl::setSelectionStyle(const KeyPointStyle &style) {
    const auto previous = m_keyPoints->selectionStyle();
    QString error;
    if (!m_keyPoints->setSelectionStyle(style, &error)) {
        emit errorOccurred(error);
        return false;
    }
    SelectionStyleSettings saved; static_cast<KeyPointStyle &>(saved) = style;
    if (!config::ParameterStore().write(saved, true, &error)) {
        m_keyPoints->setSelectionStyle(previous, nullptr); emit errorOccurred(error); return false;
    }
    if (m_renderWindow)
        m_renderWindow->Render();
    return true;
}

void VisualizationControl::showKeyPoints(bool visible) {
    m_keyPoints->setVisible(visible);
    if (m_renderWindow)
        m_renderWindow->Render();
}

bool VisualizationControl::setPickingSettings(const PickingSettings &settings) {
    const auto previous = m_picker->settings();
    QString error;
    if (!m_picker->setSettings(settings, &error)) {
        emit errorOccurred(error);
        return false;
    }
    if (!config::ParameterStore().write(settings, true, &error)) {
        m_picker->setSettings(previous, nullptr); emit errorOccurred(error); return false;
    }
    m_interactorStyle->setClickTolerancePixels(settings.clickTolerancePixels);
    return true;
}

PickingSettings VisualizationControl::pickingSettings() const {
    return m_picker->settings();
}

PickedPointInfo VisualizationControl::selectPoint(vtkIdType pointId) {
    return m_picker->selectPoint(pointId);
}

PickedPointInfo VisualizationControl::selectedPoint() const {
    return m_picker->selection();
}

QVector<vtkIdType> VisualizationControl::nearbyPoints(const std::array<double, 3> &position,
                                                      double radius) const {
    return m_picker->nearbyPoints(position, radius);
}

void VisualizationControl::clearSelection() {
    m_picker->clearSelection();
}

void VisualizationControl::setCameraLocked(bool locked) {
    auto settings = cameraSettings();
    settings.locked = locked;
    setCameraSettings(settings);
}

void VisualizationControl::handlePointPicked(const PickedPointInfo &info) {
    emit pointPicked(info);
    if (m_renderWindow)
        m_renderWindow->Render();
}

AxesSettings VisualizationControl::axesSettings() const {
    return m_scene->axesSettings();
}

GridSettings VisualizationControl::gridSettings() const {
    return m_scene->gridSettings();
}

BackgroundSettings VisualizationControl::backgroundSettings() const {
    return m_scene->backgroundSettings();
}

WorldSettings VisualizationControl::worldSettings() const {
    return m_scene->worldSettings();
}

CameraSettings VisualizationControl::cameraSettings() const {
    return m_scene->cameraSettings();
}

CameraPose VisualizationControl::cameraPose() const {
    return m_scene->cameraPose();
}

bool VisualizationControl::setAxesSettings(const AxesSettings &settings) {
    const auto previous = m_scene->axesSettings();
    QString error;
    if (!m_scene->setAxesSettings(settings, &error)) {
        emit errorOccurred(error);
        return false;
    }
    if (!config::ParameterStore().write(settings, true, &error)) {
        m_scene->setAxesSettings(previous, nullptr); emit errorOccurred(error); return false;
    }
    if (m_renderWindow)
        m_renderWindow->Render();
    return true;
}

bool VisualizationControl::setGridSettings(const GridSettings &settings) {
    const auto previous = m_scene->gridSettings();
    QString error;
    if (!m_scene->setGridSettings(settings, &error)) {
        emit errorOccurred(error);
        return false;
    }
    if (!config::ParameterStore().write(settings, true, &error)) {
        m_scene->setGridSettings(previous, nullptr); emit errorOccurred(error); return false;
    }
    if (m_renderWindow)
        m_renderWindow->Render();
    return true;
}

bool VisualizationControl::setBackgroundSettings(const BackgroundSettings &settings) {
    const auto previous = m_scene->backgroundSettings();
    QString error;
    if (!m_scene->setBackgroundSettings(settings, &error)) {
        emit errorOccurred(error);
        return false;
    }
    if (!config::ParameterStore().write(settings, true, &error)) {
        m_scene->setBackgroundSettings(previous, nullptr); emit errorOccurred(error); return false;
    }
    if (m_renderWindow)
        m_renderWindow->Render();
    return true;
}

bool VisualizationControl::setWorldSettings(const WorldSettings &settings) {
    const auto previous = m_scene->worldSettings();
    QString error;
    if (!m_scene->setWorldSettings(settings, &error)) {
        emit errorOccurred(error);
        return false;
    }
    if (!config::ParameterStore().write(settings, true, &error)) {
        m_scene->setWorldSettings(previous, nullptr); emit errorOccurred(error); return false;
    }
    if (m_renderWindow)
        m_renderWindow->Render();
    return true;
}

bool VisualizationControl::setCameraSettings(const CameraSettings &settings) {
    const auto previous = m_scene->cameraSettings();
    QString error;
    if (!m_scene->setCameraSettings(settings, &error)) {
        emit errorOccurred(error);
        return false;
    }
    if (!config::ParameterStore().write(settings, true, &error)) {
        m_scene->setCameraSettings(previous, nullptr); emit errorOccurred(error); return false;
    }
    m_interactorStyle->setCameraLocked(settings.locked);
    if (m_renderWindow)
        m_renderWindow->Render();
    return true;
}

bool VisualizationControl::setCameraPose(const CameraPose &pose) {
    QString error;
    if (!m_scene->setCameraPose(pose, &error)) {
        emit errorOccurred(error);
        return false;
    }
    if (m_renderWindow)
        m_renderWindow->Render();
    return true;
}

void VisualizationControl::setCameraPreset(CameraPreset preset) {
    QString error;
    if (!m_scene->setCameraPreset(preset, &error)) {
        emit errorOccurred(error);
        return;
    }
    if (m_renderWindow)
        m_renderWindow->Render();
}

void VisualizationControl::showAxes(bool visible) {
    auto settings = axesSettings();
    settings.visible = visible;
    setAxesSettings(settings);
}

void VisualizationControl::showGrid(bool visible) {
    auto settings = gridSettings();
    settings.visible = {{visible, visible, visible}};
    setGridSettings(settings);
}

bool VisualizationControl::showGrid(GridPlane plane, bool visible) {
    const int index = int(plane);
    if (index < 0 || index >= 3) { emit errorOccurred("Unknown grid plane"); return false; }
    auto settings = gridSettings();
    settings.visible[size_t(index)] = visible;
    return setGridSettings(settings);
}

bool VisualizationControl::setBackground(double r, double g, double b) {
    auto settings = backgroundSettings();
    settings.gradient = false;
    settings.bottomColor = {{r, g, b}};
    return setBackgroundSettings(settings);
}

} // namespace visualization
} // namespace fosd
