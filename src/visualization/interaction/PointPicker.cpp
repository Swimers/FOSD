#include "PointPicker.h"
#include "visualization/marker/KeyPointManager.h"
#include <vtkPointPicker.h>
#include <vtkPointLocator.h>
#include <vtkIdList.h>
#include <vtkPolyData.h>
#include <vtkActor.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <algorithm>
#include <cmath>
namespace fosd {
namespace visualization {
PointPicker::PointPicker(QObject *parent)
    : QObject(parent), m_picker(vtkSmartPointer<vtkPointPicker>::New()),
      m_locator(vtkSmartPointer<vtkPointLocator>::New()) {
    qRegisterMetaType<PickedPointInfo>("fosd::visualization::PickedPointInfo");
    m_picker->PickFromListOn();
    m_picker->SetTolerance(m_settings.tolerance);
}

PointPicker::~PointPicker() = default;
void PointPicker::setRenderer(vtkRenderer *renderer) {
    m_renderer = renderer;
}

void PointPicker::setKeyPointManager(KeyPointManager *manager) {
    clearSelection();
    m_keyPoints = manager;
}

void PointPicker::setMesh(vtkPolyData *mesh, core::ModelDataPtr originalModel, vtkActor *actor) {
    clearSelection();
    m_result.reset();
    m_picker->InitializePickList();
    m_displayMesh = mesh;
    m_originalModel = std::move(originalModel);
    m_modelActor = actor;
    if (actor)
        m_picker->AddPickList(actor);
    m_locator->SetDataSet(mesh);
    m_locator->FreeSearchStructure();
}

void PointPicker::setResult(core::CalculationResultPtr result) {
    m_result = std::move(result);
}

bool PointPicker::setSettings(const PickingSettings &settings, QString *error) {
    if (error)
        error->clear();
    if (!std::isfinite(settings.tolerance) || settings.tolerance <= 0 || settings.tolerance > 1 ||
        settings.clickTolerancePixels < 0 || settings.clickTolerancePixels > 100) {
        if (error)
            *error =
                QStringLiteral("Picking tolerance must be in (0,1], click threshold in [0,100]");
        return false;
    }
    m_settings = settings;
    m_picker->SetTolerance(settings.tolerance);
    if (!settings.enabled)
        clearSelection();
    else
        refreshSelection();
    return true;
}

bool PointPicker::selectable(vtkIdType id) const {
    return m_settings.enabled && m_displayMesh && m_originalModel && id >= 0 &&
           id < m_displayMesh->GetNumberOfPoints() && m_originalModel->contains(id) &&
           m_originalModel->value(id).size() == 3 &&
           (!m_settings.onlyKeyPoints || (m_keyPoints && m_keyPoints->contains(id)));
}

PickedPointInfo PointPicker::selection() const {
    PickedPointInfo info;
    if (!selectable(m_selectedId))
        return info;
    info.pointId = m_selectedId;
    const auto raw = m_originalModel->value(m_selectedId);
    for (int axis = 0; axis < 3; ++axis)
        info.originalPositionMm[axis] = raw[axis];
    m_displayMesh->GetPoint(m_selectedId, info.displayPositionMm.data());
    if (m_result) {
        const auto displacement = m_result->displacementMap.value(m_selectedId);
        if (displacement.size() == 3)
            for (int axis = 0; axis < 3; ++axis)
                info.displacementMm[axis] = displacement[axis];
    }
    info.valid = true;
    return info;
}

PickedPointInfo PointPicker::selectPoint(vtkIdType id) {
    if (!selectable(id)) {
        const bool hadSelection = m_selectedId >= 0;
        clearSelection();
        if (!hadSelection)
            emit pointPicked(PickedPointInfo());
        return {};
    }
    m_selectedId = id;
    if (m_keyPoints)
        m_keyPoints->setSelectedPoint(id);
    const auto info = selection();
    emit pointPicked(info);
    return info;
}

PickedPointInfo PointPicker::pick(int x, int y) {
    if (!m_settings.enabled)
        return {};
    if (!m_renderer || !m_renderer->GetRenderWindow() || !m_modelActor || !m_displayMesh ||
        !m_modelActor->GetVisibility() || x < 0 || y < 0)
        return selectPoint(-1);
    const int *size = m_renderer->GetRenderWindow()->GetSize();
    if (x >= size[0] || y >= size[1])
        return selectPoint(-1);
    m_picker->Pick(x, y, 0, m_renderer);
    if (m_picker->GetActor() != m_modelActor || m_picker->GetDataSet() != m_displayMesh)
        return selectPoint(-1);
    return selectPoint(m_picker->GetPointId());
}

void PointPicker::refreshSelection() {
    if (m_selectedId < 0)
        return;
    if (!selectable(m_selectedId)) {
        clearSelection();
        return;
    }
    if (m_keyPoints)
        m_keyPoints->setSelectedPoint(m_selectedId);
    emit selectionUpdated(selection());
}

void PointPicker::cameraInteractionStarted() {
    if (m_settings.clearOnCameraInteraction)
        clearSelection();
}

QVector<vtkIdType> PointPicker::nearbyPoints(const std::array<double, 3> &position,
                                             double radius) const {
    QVector<vtkIdType> points;
    if (!m_displayMesh || !m_originalModel || !m_displayMesh->GetNumberOfPoints() ||
        !std::isfinite(radius) || radius < 0)
        return points;
    for (double value : position)
        if (!std::isfinite(value))
            return points;
    auto ids = vtkSmartPointer<vtkIdList>::New();
    m_locator->BuildLocator();
    m_locator->FindPointsWithinRadius(radius, position.data(), ids);
    for (vtkIdType index = 0; index < ids->GetNumberOfIds(); ++index)
        points.append(ids->GetId(index));
    std::sort(points.begin(), points.end());
    return points;
}

void PointPicker::clearSelection() {
    const bool changed = m_selectedId >= 0;
    m_selectedId = -1;
    
    if (m_keyPoints)
        m_keyPoints->clearSelection();
    if (changed)
        emit pointPicked(PickedPointInfo());
}
} // namespace visualization
} // namespace fosd
