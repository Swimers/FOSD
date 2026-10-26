#include "SceneEnvironment.h"
#include <vtkActor.h>
#include <vtkAxesActor.h>
#include <vtkCamera.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkProp3D.h>
#include <vtkCaptionActor2D.h>
#include <vtkTextActor.h>
#include <vtkTextProperty.h>
#include <vtkPoints.h>
#include <vtkCellArray.h>
#include <vtkPolyData.h>
#include <vtkPolyDataMapper.h>
#include <vtkProperty.h>
#include <vtkPlaneSource.h>
#include <vtkTexturedSphereSource.h>
#include <vtkTexture.h>
#include <vtkImageData.h>
#include <vtkCallbackCommand.h>
#include <vtkCommand.h>
#include <QImage>
#include <algorithm>
#include <cmath>
#include <cstring>
namespace fosd {
namespace visualization {
namespace {
bool fail(QString *error, const QString &message) {
    if (error)
        *error = message;
    return false;
}

bool finite(const std::array<double, 3> &v) {
    return std::isfinite(v[0]) && std::isfinite(v[1]) && std::isfinite(v[2]);
}

bool rgb(const std::array<double, 3> &v) {
    return finite(v) && v[0] >= 0 && v[0] <= 1 && v[1] >= 0 && v[1] <= 1 && v[2] >= 0 && v[2] <= 1;
}

void backgroundProp(vtkActor *actor) {
    actor->PickableOff();
    actor->UseBoundsOff();
    actor->GetProperty()->LightingOff();
}

vtkSmartPointer<vtkActor> gridActor(int plane, const GridSettings &s) {
    auto points = vtkSmartPointer<vtkPoints>::New();
    points->SetDataTypeToDouble();
    auto lines = vtkSmartPointer<vtkCellArray>::New();
    const int first = plane == 2 ? 1 : 0, second = plane == 1 ? 1 : 2;
    for (int i = -s.divisions; i <= s.divisions; ++i) {
        const double offset = s.halfExtentMm * double(i) / s.divisions;
        for (int direction = 0; direction < 2; ++direction) {
            auto a = s.originMm, b = s.originMm;
            const int parallel = direction == 0 ? first : second,
                      fixed = direction == 0 ? second : first;
            a[parallel] -= s.halfExtentMm;
            b[parallel] += s.halfExtentMm;
            a[fixed] += offset;
            b[fixed] += offset;
            const vtkIdType ids[2] = {points->InsertNextPoint(a.data()),
                                      points->InsertNextPoint(b.data())};
            lines->InsertNextCell(2, ids);
        }
    }
    auto data = vtkSmartPointer<vtkPolyData>::New();
    data->SetPoints(points);
    data->SetLines(lines);
    auto mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    mapper->SetInputData(data);
    mapper->ScalarVisibilityOff();
    auto actor = vtkSmartPointer<vtkActor>::New();
    actor->SetMapper(mapper);
    backgroundProp(actor);
    actor->GetProperty()->SetColor(s.color[0], s.color[1], s.color[2]);
    actor->GetProperty()->SetOpacity(s.opacity);
    actor->GetProperty()->SetLineWidth(s.lineWidth);
    return actor;
}

bool texture(const QString &path, bool repeat, vtkSmartPointer<vtkTexture> *output,
             QString *error) {
    if (path.isEmpty()) {
        *output = nullptr;
        return true;
    }
    QImage image(path);
    if (image.isNull())
        return fail(error, QStringLiteral("Cannot decode scene texture: %1").arg(path));
    image = image.convertToFormat(QImage::Format_RGBA8888);
    auto pixels = vtkSmartPointer<vtkImageData>::New();
    pixels->SetDimensions(image.width(), image.height(), 1);
    pixels->AllocateScalars(VTK_UNSIGNED_CHAR, 4);
    for (int y = 0; y < image.height(); ++y)
        std::memcpy(pixels->GetScalarPointer(0, y, 0), image.constScanLine(image.height() - 1 - y),
                    size_t(image.width()) * 4);
    *output = vtkSmartPointer<vtkTexture>::New();
    (*output)->SetInputData(pixels);
    (*output)->InterpolateOn();
    (*output)->SetRepeat(repeat);
    return true;
}

bool validBounds(const double b[6]) {
    for (int i = 0; i < 6; ++i)
        if (!std::isfinite(b[i]))
            return false;
    return b[0] <= b[1] && b[2] <= b[3] && b[4] <= b[5];
}
} // namespace
SceneEnvironment::SceneEnvironment()
    : m_renderObserver(vtkSmartPointer<vtkCallbackCommand>::New()) {
    m_renderObserver->SetClientData(this);
    m_renderObserver->SetCallback(&SceneEnvironment::onRender);
}

SceneEnvironment::~SceneEnvironment() {
    clear();
}

void SceneEnvironment::onRender(vtkObject *, unsigned long, void *data, void *) {
    static_cast<SceneEnvironment *>(data)->updateClippingRange();
}

void SceneEnvironment::detachProps() {
    if (!m_renderer)
        return;
    if (m_axes)
        m_renderer->RemoveViewProp(m_axes);
    for (auto &grid : m_grids)
        if (grid)
            m_renderer->RemoveViewProp(grid);
    if (m_ground)
        m_renderer->RemoveViewProp(m_ground);
    if (m_sky)
        m_renderer->RemoveViewProp(m_sky);
}

void SceneEnvironment::setRenderer(vtkRenderer *renderer) {
    if (m_renderer == renderer)
        return;
    if (m_renderer) {
        m_cameraSettings = cameraSettings();
        m_savedPose = cameraPose();
        m_hasSavedPose = true;
        detachProps();
        if (m_observerTag)
            m_renderer->RemoveObserver(m_observerTag);
    }
    m_renderer = renderer;
    m_observerTag = 0;
    if (!renderer)
        return;
    m_observerTag = renderer->AddObserver(vtkCommand::StartEvent, m_renderObserver);
    setBackgroundSettings(m_backgroundSettings);
    setCameraSettings(m_cameraSettings);
    if (m_hasSavedPose)
        setCameraPose(m_savedPose);
    if ((m_worldSettings.groundVisible || m_worldSettings.skyVisible) && !m_ground)
        setWorldSettings(m_worldSettings);
    syncProps();
}

void SceneEnvironment::setModelActor(vtkProp3D *model) {
    m_model = model;
}

void SceneEnvironment::configureAxes() {
    if (!m_axes)
        m_axes = vtkSmartPointer<vtkAxesActor>::New();
    const auto &s = m_axesSettings;
    m_axes->PickableOff();
    m_axes->UseBoundsOff();
    m_axes->SetPosition(s.originMm[0], s.originMm[1], s.originMm[2]);
    m_axes->SetTotalLength(s.lengthMm, s.lengthMm, s.lengthMm);
    m_axes->SetAxisLabels(s.labelsVisible);
    m_axes->SetNormalizedShaftLength(0.95, 0.95, 0.95);
    m_axes->SetNormalizedTipLength(0.05, 0.05, 0.05);
    vtkCaptionActor2D *captions[3] = {m_axes->GetXAxisCaptionActor2D(),
                                      m_axes->GetYAxisCaptionActor2D(),
                                      m_axes->GetZAxisCaptionActor2D()};
    for (int i = 0; i < 3; ++i) {
        captions[i]->GetCaptionTextProperty()->SetColor(i == 0 ? 1 : 0, i == 1 ? 1 : 0,
                                                        i == 2 ? 1 : 0);
        captions[i]->GetCaptionTextProperty()->SetFontSize(s.fontSize);
        captions[i]->GetTextActor()->SetTextScaleModeToNone();
    }
}

void SceneEnvironment::syncProps() {
    if (!m_renderer)
        return;
    if (m_axesSettings.visible && !m_axes)
        configureAxes();
    if (m_axes) {
        if (m_axesSettings.visible)
            m_renderer->AddViewProp(m_axes);
        else
            m_renderer->RemoveViewProp(m_axes);
    }
    for (int i = 0; i < 3; ++i) {
        if (m_gridSettings.visible[i] && !m_grids[i])
            m_grids[i] = gridActor(i, m_gridSettings);
        if (m_grids[i]) {
            if (m_gridSettings.visible[i])
                m_renderer->AddViewProp(m_grids[i]);
            else
                m_renderer->RemoveViewProp(m_grids[i]);
        }
    }
    if (m_ground) {
        if (m_worldSettings.groundVisible)
            m_renderer->AddViewProp(m_ground);
        else
            m_renderer->RemoveViewProp(m_ground);
    }
    if (m_sky) {
        if (m_worldSettings.skyVisible)
            m_renderer->AddViewProp(m_sky);
        else
            m_renderer->RemoveViewProp(m_sky);
    }
}

bool SceneEnvironment::setAxesSettings(const AxesSettings &s, QString *error) {
    if (error)
        error->clear();
    if (!finite(s.originMm) || !std::isfinite(s.lengthMm) || s.lengthMm <= 0 || s.fontSize < 1 ||
        s.fontSize > 256)
        return fail(
            error,
            QStringLiteral("Axes require a positive length, finite origin and font size 1..256"));
    for (double v : s.originMm)
        if (!std::isfinite(v + s.lengthMm))
            return fail(error, QStringLiteral("Axes bounds overflow"));
    m_axesSettings = s;
    configureAxes();
    syncProps();
    return true;
}

bool SceneEnvironment::setGridSettings(const GridSettings &s, QString *error) {
    if (error)
        error->clear();
    if (!finite(s.originMm) || !rgb(s.color) || !std::isfinite(s.halfExtentMm) ||
        s.halfExtentMm <= 0 || s.divisions < 1 || s.divisions > 500 || !std::isfinite(s.opacity) ||
        s.opacity < 0 || s.opacity > 1 || !std::isfinite(s.lineWidth) || s.lineWidth <= 0 ||
        s.lineWidth > 20)
        return fail(
            error,
            QStringLiteral("Invalid grid extent, divisions (1..500), RGB, opacity or line width"));
    for (double v : s.originMm)
        if (!std::isfinite(v + s.halfExtentMm) || !std::isfinite(v - s.halfExtentMm))
            return fail(error, QStringLiteral("Grid bounds overflow"));
    const bool rebuild = s.halfExtentMm != m_gridSettings.halfExtentMm ||
                         s.divisions != m_gridSettings.divisions ||
                         s.originMm != m_gridSettings.originMm;
    m_gridSettings = s;
    for (int i = 0; i < 3; ++i) {
        if (rebuild && m_grids[i]) {
            if (m_renderer)
                m_renderer->RemoveViewProp(m_grids[i]);
            m_grids[i] = nullptr;
        }
        if (m_grids[i]) {
            m_grids[i]->GetProperty()->SetColor(s.color[0], s.color[1], s.color[2]);
            m_grids[i]->GetProperty()->SetOpacity(s.opacity);
            m_grids[i]->GetProperty()->SetLineWidth(s.lineWidth);
        }
    }
    syncProps();
    return true;
}

bool SceneEnvironment::setBackgroundSettings(const BackgroundSettings &s, QString *error) {
    if (error)
        error->clear();
    if (!rgb(s.bottomColor) || !rgb(s.topColor))
        return fail(error, QStringLiteral("Background RGB components must be in [0,1]"));
    m_backgroundSettings = s;
    if (m_renderer) {
        m_renderer->SetBackground(s.bottomColor[0], s.bottomColor[1], s.bottomColor[2]);
        m_renderer->SetBackground2(s.topColor[0], s.topColor[1], s.topColor[2]);
        m_renderer->SetGradientBackground(s.gradient);
    }
    return true;
}

bool SceneEnvironment::setWorldSettings(const WorldSettings &s, QString *error) {
    if (error)
        error->clear();
    if (!finite(s.centerMm) || !rgb(s.groundColor) || !rgb(s.skyColor) ||
        !std::isfinite(s.groundHalfExtentMm) || s.groundHalfExtentMm <= 0 ||
        !std::isfinite(s.groundYmm) || !std::isfinite(s.skyRadiusMm) || s.skyRadiusMm <= 0)
        return fail(error, QStringLiteral("Invalid ground/sky size, position or RGB"));
    for (double v : s.centerMm)
        if (!std::isfinite(v + s.skyRadiusMm) || !std::isfinite(v - s.skyRadiusMm) ||
            !std::isfinite(v + s.groundHalfExtentMm) || !std::isfinite(v - s.groundHalfExtentMm) ||
            !std::isfinite(v + s.groundYmm))
            return fail(error, QStringLiteral("World bounds overflow"));
    vtkSmartPointer<vtkTexture> groundTexture, skyTexture;
    if (m_ground && s.groundTexturePath == m_worldSettings.groundTexturePath)
        groundTexture = m_ground->GetTexture();
    else if (!texture(s.groundTexturePath, true, &groundTexture, error))
        return false;
    if (m_sky && s.skyTexturePath == m_worldSettings.skyTexturePath)
        skyTexture = m_sky->GetTexture();
    else if (!texture(s.skyTexturePath, false, &skyTexture, error))
        return false;
    auto plane = vtkSmartPointer<vtkPlaneSource>::New();
    const double r = s.groundHalfExtentMm;
    plane->SetOrigin(-r, s.groundYmm, -r);
    plane->SetPoint1(r, s.groundYmm, -r);
    plane->SetPoint2(-r, s.groundYmm, r);
    auto groundMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    groundMapper->SetInputConnection(plane->GetOutputPort());
    auto ground = vtkSmartPointer<vtkActor>::New();
    ground->SetMapper(groundMapper);
    backgroundProp(ground);
    ground->SetPosition(s.centerMm[0], s.centerMm[1], s.centerMm[2]);
    ground->SetTexture(groundTexture);
    ground->GetProperty()->SetColor(groundTexture ? 1 : s.groundColor[0],
                                    groundTexture ? 1 : s.groundColor[1],
                                    groundTexture ? 1 : s.groundColor[2]);
    auto sphere = vtkSmartPointer<vtkTexturedSphereSource>::New();
    sphere->SetRadius(s.skyRadiusMm);
    sphere->SetThetaResolution(48);
    sphere->SetPhiResolution(32);
    auto skyMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    skyMapper->SetInputConnection(sphere->GetOutputPort());
    auto sky = vtkSmartPointer<vtkActor>::New();
    sky->SetMapper(skyMapper);
    backgroundProp(sky);
    sky->SetTexture(skyTexture);
    sky->SetPosition(s.centerMm[0], s.centerMm[1], s.centerMm[2]);
    sky->GetProperty()->FrontfaceCullingOn();
    sky->GetProperty()->SetColor(skyTexture ? 1 : s.skyColor[0], skyTexture ? 1 : s.skyColor[1],
                                 skyTexture ? 1 : s.skyColor[2]);
    if (m_renderer) {
        if (m_ground)
            m_renderer->RemoveViewProp(m_ground);
        if (m_sky)
            m_renderer->RemoveViewProp(m_sky);
    }
    m_ground = ground;
    m_sky = sky;
    m_worldSettings = s;
    syncProps();
    updateClippingRange();
    return true;
}

vtkCamera *SceneEnvironment::camera() const {
    return m_renderer ? m_renderer->GetActiveCamera() : nullptr;
}

CameraSettings SceneEnvironment::cameraSettings() const {
    auto settings = m_cameraSettings;
    if (auto *c = camera()) {
        settings.projection =
            c->GetParallelProjection() ? CameraProjection::Parallel : CameraProjection::Perspective;
        settings.viewAngleDegrees = c->GetViewAngle();
        settings.parallelScaleMm = c->GetParallelScale();
    }
    return settings;
}

CameraPose SceneEnvironment::cameraPose() const {
    CameraPose pose = m_savedPose;
    if (auto *c = camera()) {
        c->GetPosition(pose.positionMm.data());
        c->GetFocalPoint(pose.focalPointMm.data());
        c->GetViewUp(pose.viewUp.data());
    }
    return pose;
}

bool SceneEnvironment::setCameraSettings(const CameraSettings &s, QString *error) {
    if (error)
        error->clear();
    if ((s.projection != CameraProjection::Perspective &&
         s.projection != CameraProjection::Parallel) ||
        !std::isfinite(s.viewAngleDegrees) || s.viewAngleDegrees < 1 || s.viewAngleDegrees > 170 ||
        !std::isfinite(s.parallelScaleMm) || s.parallelScaleMm <= 0)
        return fail(
            error,
            QStringLiteral(
                "Camera requires valid projection, angle 1..170 and positive parallel scale"));
    m_cameraSettings = s;
    if (auto *c = camera()) {
        c->SetParallelProjection(s.projection == CameraProjection::Parallel);
        c->SetViewAngle(s.viewAngleDegrees);
        c->UseHorizontalViewAngleOff();
        c->SetParallelScale(s.parallelScaleMm);
    }
    return true;
}

bool SceneEnvironment::setCameraPose(const CameraPose &pose, QString *error) {
    if (error)
        error->clear();
    if (!finite(pose.positionMm) || !finite(pose.focalPointMm) || !finite(pose.viewUp))
        return fail(error, QStringLiteral("Camera pose must be finite"));
    std::array<double, 3> d;
    for (int i = 0; i < 3; ++i)
        d[i] = pose.positionMm[i] - pose.focalPointMm[i];
    const double length = std::hypot(std::hypot(d[0], d[1]), d[2]),
                 upLength = std::hypot(std::hypot(pose.viewUp[0], pose.viewUp[1]), pose.viewUp[2]);
    if (!std::isfinite(length) || length < 1e-12 || !std::isfinite(upLength) || upLength < 1e-12)
        return fail(error, QStringLiteral("Camera position/focal point or view-up is degenerate"));
    double dot = 0;
    for (int i = 0; i < 3; ++i)
        dot += d[i] / length * (pose.viewUp[i] / upLength);
    if (std::abs(dot) > 1 - 1e-10)
        return fail(error,
                    QStringLiteral("Camera view-up must not be parallel to viewing direction"));
    m_savedPose = pose;
    m_hasSavedPose = true;
    if (auto *c = camera()) {
        c->SetPosition(pose.positionMm.data());
        c->SetFocalPoint(pose.focalPointMm.data());
        c->SetViewUp(pose.viewUp.data());
        c->OrthogonalizeViewUp();
        updateClippingRange();
    }
    return true;
}

bool SceneEnvironment::modelBounds(double bounds[6]) const {
    if (!m_renderer)
        return false;
    if (m_model) {
        m_model->GetBounds(bounds);
    } else
        m_renderer->ComputeVisiblePropBounds(bounds);
    return validBounds(bounds);
}

bool SceneEnvironment::resetCamera() {
    double b[6];
    if (!camera() || !modelBounds(b))
        return false;
    auto *c = camera();
    double center[3], span[3];
    for (int i = 0; i < 3; ++i) {
        center[i] = b[2 * i] * 0.5 + b[2 * i + 1] * 0.5;
        span[i] = b[2 * i + 1] - b[2 * i];
    }
    const double radius = std::max(1e-3, 0.525 * std::hypot(std::hypot(span[0], span[1]), span[2]));
    if (!std::isfinite(radius))
        return false;
    double direction[3];
    for (int i = 0; i < 3; ++i)
        direction[i] = c->GetPosition()[i] - c->GetFocalPoint()[i];
    double distance = std::hypot(std::hypot(direction[0], direction[1]), direction[2]);
    if (distance < 1e-12 || !std::isfinite(distance)) {
        direction[0] = 0;
        direction[1] = 0;
        direction[2] = 1;
        distance = 1;
    }
    for (double &v : direction)
        v /= distance;
    double aspect = 1;
    if (m_renderer->GetRenderWindow()) {
        const int *size = m_renderer->GetRenderWindow()->GetSize();
        const double *vp = m_renderer->GetViewport();
        if (size[1] > 0 && vp[3] > vp[1])
            aspect = double(size[0]) * (vp[2] - vp[0]) / (size[1] * (vp[3] - vp[1]));
    }
    if (!std::isfinite(aspect) || aspect <= 0)
        aspect = 1;
    const double halfAngle = c->GetViewAngle() * 3.141592653589793 / 360;
    const double fittingAngle = std::min(halfAngle, std::atan(std::tan(halfAngle) * aspect));
    distance = c->GetParallelProjection() ? radius * 2 : radius / std::sin(fittingAngle);
    c->SetParallelScale(radius / std::min(1.0, aspect));
    c->SetFocalPoint(center);
    c->SetPosition(center[0] + direction[0] * distance, center[1] + direction[1] * distance,
                   center[2] + direction[2] * distance);
    updateClippingRange();
    return true;
}

bool SceneEnvironment::setCameraPreset(CameraPreset preset, QString *error) {
    if (error)
        error->clear();
    double direction[3] = {0, 0, 0}, up[3] = {0, 0, 1};
    switch (preset) {
        case CameraPreset::Isometric:
            direction[0] = -1;
            direction[1] = -1;
            direction[2] = 1;
            break;
        case CameraPreset::Front:
            direction[1] = -1;
            break;
        case CameraPreset::Back:
            direction[1] = 1;
            break;
        case CameraPreset::Left:
            direction[0] = -1;
            break;
        case CameraPreset::Right:
            direction[0] = 1;
            break;
        case CameraPreset::Top:
            direction[2] = 1;
            up[1] = 1;
            up[2] = 0;
            break;
        case CameraPreset::Bottom:
            direction[2] = -1;
            up[1] = 1;
            up[2] = 0;
            break;
        default:
            return fail(error, QStringLiteral("Unknown camera preset"));
    }
    if (auto *c = camera()) {
        double b[6], center[3] = {0, 0, 0};
        if (modelBounds(b))
            for (int i = 0; i < 3; ++i)
                center[i] = b[2 * i] * 0.5 + b[2 * i + 1] * 0.5;
        c->SetFocalPoint(center);
        c->SetPosition(center[0] + direction[0], center[1] + direction[1],
                       center[2] + direction[2]);
        c->SetViewUp(up);
        resetCamera();
    } else {
        CameraPose pose;
        pose.positionMm = {{direction[0], direction[1], direction[2]}};
        pose.viewUp = {{up[0], up[1], up[2]}};
        setCameraPose(pose);
    }
    return true;
}

void SceneEnvironment::updateClippingRange() {
    double b[6];
    if (!m_renderer)
        return;
    bool hasBounds = modelBounds(b);
    double modelNear = 0, modelFar = 0;
    if (hasBounds) {
        m_renderer->ResetCameraClippingRange(b);
        modelNear = camera()->GetClippingRange()[0];
        modelFar = camera()->GetClippingRange()[1];
    }
    vtkProp3D *environment[6] = {m_axes.GetPointer(),     m_grids[0].GetPointer(),
                                 m_grids[1].GetPointer(), m_grids[2].GetPointer(),
                                 m_ground.GetPointer(),   m_sky.GetPointer()};
    const bool visible[6] = {m_axesSettings.visible,        m_gridSettings.visible[0],
                             m_gridSettings.visible[1],     m_gridSettings.visible[2],
                             m_worldSettings.groundVisible, m_worldSettings.skyVisible};
    bool extended = false;
    for (int j = 0; j < 6; ++j)
        if (visible[j] && environment[j]) {
            double env[6];
            environment[j]->GetBounds(env);
            if (validBounds(env)) {
                extended = true;
                if (!hasBounds) {
                    for (int i = 0; i < 6; ++i)
                        b[i] = env[i];
                    hasBounds = true;
                } else
                    for (int i = 0; i < 3; ++i) {
                        b[2 * i] = std::min(b[2 * i], env[2 * i]);
                        b[2 * i + 1] = std::max(b[2 * i + 1], env[2 * i + 1]);
                    }
            }
        }
    if (hasBounds && extended) {
        m_renderer->ResetCameraClippingRange(b);
        // Extend the far plane without clipping a small foreground model.
        if (modelNear > 0)
            camera()->SetClippingRange(std::min(modelNear, camera()->GetClippingRange()[0]),
                                       std::max(modelFar, camera()->GetClippingRange()[1]));
    }
}

void SceneEnvironment::showAxes(bool visible) {
    auto s = m_axesSettings;
    s.visible = visible;
    setAxesSettings(s);
}

void SceneEnvironment::showGrid(bool visible) {
    auto s = m_gridSettings;
    s.visible = {{visible, visible, visible}};
    setGridSettings(s);
}

bool SceneEnvironment::showGrid(GridPlane plane, bool visible, QString *error) {
    const int index = int(plane);
    if (index < 0 || index > 2)
        return fail(error, QStringLiteral("Unknown grid plane"));
    auto s = m_gridSettings;
    s.visible[index] = visible;
    return setGridSettings(s, error);
}

bool SceneEnvironment::setBackground(double r, double g, double b, QString *error) {
    auto s = m_backgroundSettings;
    s.gradient = false;
    s.bottomColor = {{r, g, b}};
    return setBackgroundSettings(s, error);
}

void SceneEnvironment::setCameraLocked(bool locked) {
    m_cameraSettings.locked = locked;
}

void SceneEnvironment::clear() {
    setRenderer(nullptr);
    m_model = nullptr;
    m_axes = nullptr;
    for (auto &grid : m_grids)
        grid = nullptr;
    m_ground = nullptr;
    m_sky = nullptr;
}
} // namespace visualization
} // namespace fosd
