#include "KeyPointManager.h"
#include "KeyPointCreator.h"
#include <vtkActor.h>
#include <vtkBillboardTextActor3D.h>
#include <vtkPolyData.h>
#include <vtkRenderer.h>
#include <QSet>
namespace fosd {
namespace visualization {
KeyPointManager::KeyPointManager() {
    // 初始化默认参数
    m_selectionStyle.radiusMm = 7;
    m_selectionStyle.color = {{0.0, 1.0, 1.0}};
    m_selectionStyle.labelColor = {{0.0, 1.0, 1.0}};
}

KeyPointManager::~KeyPointManager() {
    clear();
}

void KeyPointManager::add(const KeyPointActors &actors) {
    if (!m_renderer || !actors.sphere)
        return;
        
    m_renderer->AddViewProp(actors.sphere);
    m_renderer->AddViewProp(actors.label);
}

void KeyPointManager::remove(const KeyPointActors &actors) {
    if (!m_renderer || !actors.sphere)
        return;
    m_renderer->RemoveViewProp(actors.sphere);
    m_renderer->RemoveViewProp(actors.label);
}

void KeyPointManager::setRenderer(vtkRenderer *renderer) {
    if (m_renderer == renderer)
        return;
        
    for (const auto &actors : m_points)
        remove(actors);
    remove(m_selection);

    m_renderer = renderer;
    for (const auto &actors : m_points)
        add(actors);
    add(m_selection);
}

void KeyPointManager::setModel(core::ModelDataPtr model, vtkPolyData *displayMesh) {
    clear();
    m_model = std::move(model);
    m_displayMesh = displayMesh;
}

std::array<double, 3> KeyPointManager::position(vtkIdType id) const {
    std::array<double, 3> xyz{{0, 0, 0}};
    if (m_displayMesh && id >= 0 && id < m_displayMesh->GetNumberOfPoints())
        m_displayMesh->GetPoint(id, xyz.data());
    return xyz;
}

bool KeyPointManager::setKeyPoints(const QVector<KeyPointDefinition> &points, QString *error) {
    if (error)
        error->clear();
        
    QSet<vtkIdType> ids;
    for (const auto &point : points) {
        if (!m_model || !m_displayMesh || point.pointId < 0 ||
            point.pointId >= m_displayMesh->GetNumberOfPoints() ||
            !m_model->contains(point.pointId) || ids.contains(point.pointId)) {
            if (error)
                *error =
                    QStringLiteral("Key points require unique IDs present in the current model");
            return false;
        }
        ids.insert(point.pointId);
    }
    for (const auto &actors : m_points)
        remove(actors);
    m_points.clear();
    m_definitions = points;
    for (const auto &point : points) {
        auto actors =
            KeyPointCreator::create(point.pointId, position(point.pointId), point.label, m_style);
        m_points.insert(point.pointId, actors);
        add(actors);
    }
    updateVisibility();
    return true;
}

bool KeyPointManager::setStyle(const KeyPointStyle &style, QString *error) {
    if (!KeyPointCreator::validateStyle(style, error))
        return false;
    m_style = style;
    for (auto &actors : m_points)
        KeyPointCreator::configure(actors, m_style);
    updatePositions();
    updateVisibility();
    return true;
}

bool KeyPointManager::setSelectionStyle(const KeyPointStyle &style, QString *error) {
    if (!KeyPointCreator::validateStyle(style, error))
        return false;
    m_selectionStyle = style;
    KeyPointCreator::configure(m_selection, m_selectionStyle);
    updatePositions();
    updateVisibility();
    return true;
}

void KeyPointManager::updatePositions() {
    for (auto &actors : m_points)
        KeyPointCreator::setPosition(actors, position(actors.pointId), m_style);
    if (m_selection.pointId >= 0)
        KeyPointCreator::setPosition(m_selection, position(m_selection.pointId), m_selectionStyle);
}

void KeyPointManager::updateVisibility() {
    for (auto &actors : m_points) {
        const bool visible = m_visible && actors.pointId != m_selection.pointId;
        actors.sphere->SetVisibility(visible);
        actors.label->SetVisibility(visible && m_style.labelsVisible);
    }
    if (m_selection.sphere) {
        m_selection.sphere->VisibilityOn();
        m_selection.label->SetVisibility(m_selectionStyle.labelsVisible);
    }
}

void KeyPointManager::setVisible(bool visible) {
    m_visible = visible;
    updateVisibility();
}

bool KeyPointManager::setSelectedPoint(vtkIdType id) {
    if (!m_model || !m_displayMesh || id < 0 || id >= m_displayMesh->GetNumberOfPoints() ||
        !m_model->contains(id))
        return false;
    if (id == m_selection.pointId) {
        KeyPointCreator::setPosition(m_selection, position(id), m_selectionStyle);
        return true;
    }
    remove(m_selection);
    m_selection = KeyPointCreator::create(id, position(id), QString(), m_selectionStyle);
    add(m_selection);
    updateVisibility();
    return true;
}

void KeyPointManager::clearSelection() {
    remove(m_selection);
    m_selection = KeyPointActors();
    updateVisibility();
}

void KeyPointManager::clear() {

}
} // namespace visualization
} // namespace fosd
