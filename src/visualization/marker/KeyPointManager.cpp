#include "KeyPointManager.h"
#include "KeyPointCreator.h"
#include <vtkActor.h>
#include <vtkBillboardTextActor3D.h>
#include <vtkPolyData.h>
#include <vtkRenderer.h>
#include <QSet>
#include <cmath>
namespace fosd {
namespace visualization {
KeyPointManager::KeyPointManager() {
    // 初始化默认参数
    m_selectionStyle = SelectionStyleSettings();
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

    for (const auto &actors : m_displayedActors)
        remove(actors);
    remove(m_selectedActors);

    m_renderer = renderer;
    for (const auto &actors : m_displayedActors)
        add(actors);
    add(m_selectedActors);
}

void KeyPointManager::setModel(core::ModelDataPtr model, vtkPolyData *displayMesh) {
    clear();
    m_model = std::move(model);
    m_displayMesh = displayMesh;
    if (m_model && m_displayMesh) {
        m_keyDefinitions.reserve(m_model->size());
        for (auto it = m_model->constBegin(); it != m_model->constEnd(); ++it) {
            const auto id = vtkIdType(it.key());
            if (validModelPoint(id)) {
                KeyPointDefinition point;
                point.pointId = id;
                m_keyDefinitions.append(point);
                m_keyIds.insert(id);
            }
        }
    }
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
        if (!validModelPoint(point.pointId) || ids.contains(point.pointId)) {
            if (error)
                *error =
                    QStringLiteral("Key points require unique IDs present in the current model");
            return false;
        }
        ids.insert(point.pointId);
    }
    m_keyDefinitions = points;
    m_keyIds = ids;
    // Reconcile dependent groups only after the new membership has been validated.
    for (int i = m_displayedDefinitions.size() - 1; i >= 0; --i) {
        const auto &point = m_displayedDefinitions[i];
        if (point.source == DisplayedPointSource::Model && !contains(point.pointId)) {
            remove(m_displayedActors.take(point.pointId));
            m_displayedDefinitions.removeAt(i);
        }
    }
    if (m_selectedActors.pointId >= 0 && !contains(m_selectedActors.pointId))
        clearSelection();
    updateVisibility();
    return true;
}

bool KeyPointManager::validModelPoint(vtkIdType id) const {
    return m_model && m_displayMesh && id >= 0 && id < m_displayMesh->GetNumberOfPoints() &&
           m_model->contains(id) && m_model->value(id).size() == 3;
}

std::array<double, 3> KeyPointManager::position(const DisplayedPointDefinition &point) const {
    return point.source == DisplayedPointSource::External ? point.positionMm
                                                          : position(point.pointId);
}

bool KeyPointManager::setDisplayedPoints(const QVector<DisplayedPointDefinition> &points,
                                         QString *error) {
    if (error)
        error->clear();
    QSet<vtkIdType> ids;
    for (const auto &point : points) {
        bool valid = point.pointId >= 0 && !ids.contains(point.pointId);
        if (point.source == DisplayedPointSource::Model)
            valid = valid && validModelPoint(point.pointId) && contains(point.pointId);
        else if (point.source == DisplayedPointSource::External) {
            valid = valid && (!m_model || !m_model->contains(point.pointId));
            for (double coordinate : point.positionMm)
                valid = valid && std::isfinite(coordinate);
        } else
            valid = false;
        if (!valid) {
            if (error)
                *error = QStringLiteral(
                    "Displayed points require unique nonnegative IDs, key-point membership for "
                    "model vertices, and finite external coordinates");
            return false;
        }
        ids.insert(point.pointId);
    }

    // Reuse surviving actors so editing one display does not recreate every sphere and label.
    for (auto it = m_displayedActors.begin(); it != m_displayedActors.end();) {
        if (!ids.contains(it.key())) {
            remove(it.value());
            it = m_displayedActors.erase(it);
        } else
            ++it;
    }
    m_displayedDefinitions = points;
    for (const auto &point : points) {
        if (!m_displayedActors.contains(point.pointId)) {
            auto actors = KeyPointCreator::create(point.pointId, position(point), point.label,
                                                  m_displayedStyle);
            m_displayedActors.insert(point.pointId, actors);
            add(actors);
        } else
            m_displayedActors[point.pointId].text =
                point.label.isEmpty() ? QString::number(qlonglong(point.pointId)) : point.label;
    }
    updatePositions();
    updateVisibility();
    return true;
}

bool KeyPointManager::setStyle(const KeyPointStyle &style, QString *error) {
    if (!KeyPointCreator::validateStyle(style, error))
        return false;
    m_displayedStyle = style;
    for (auto &actors : m_displayedActors)
        KeyPointCreator::configure(actors, m_displayedStyle);
    updatePositions();
    updateVisibility();
    return true;
}

bool KeyPointManager::setSelectionStyle(const KeyPointStyle &style, QString *error) {
    if (!KeyPointCreator::validateStyle(style, error))
        return false;
    m_selectionStyle = style;
    KeyPointCreator::configure(m_selectedActors, m_selectionStyle);
    updatePositions();
    updateVisibility();
    return true;
}

void KeyPointManager::updatePositions() {
    for (const auto &point : m_displayedDefinitions)
        KeyPointCreator::setPosition(m_displayedActors[point.pointId], position(point),
                                     m_displayedStyle);
    if (m_selectedActors.pointId >= 0)
        KeyPointCreator::setPosition(m_selectedActors, position(m_selectedActors.pointId),
                                     m_selectionStyle);
}

void KeyPointManager::updateVisibility() {
    for (auto &actors : m_displayedActors) {
        const bool visible = m_visible;
        actors.sphere->SetVisibility(visible);
        actors.label->SetVisibility(visible && m_displayedStyle.labelsVisible);
    }
    if (m_selectedActors.sphere) {
        m_selectedActors.sphere->VisibilityOn();
        m_selectedActors.label->SetVisibility(m_selectionStyle.labelsVisible);
    }
}

void KeyPointManager::setVisible(bool visible) {
    m_visible = visible;
    updateVisibility();
}

bool KeyPointManager::setSelectedPoint(vtkIdType id) {
    if (!validModelPoint(id) || !contains(id))
        return false;
    if (id == m_selectedActors.pointId) {
        KeyPointCreator::setPosition(m_selectedActors, position(id), m_selectionStyle);
        return true;
    }
    remove(m_selectedActors);
    m_selectedActors = KeyPointCreator::create(id, position(id), QString(), m_selectionStyle);
    add(m_selectedActors);
    updateVisibility();
    return true;
}

void KeyPointManager::clearSelection() {
    remove(m_selectedActors);
    m_selectedActors = KeyPointActors();
    updateVisibility();
}

void KeyPointManager::clear() {
    // 先从渲染器摘除所有标记，再清空内部状态。
    // setModel 与析构都依赖本函数，不能留下指向旧 displayMesh 的残留 actor。
    for (const auto &actors : m_displayedActors)
        remove(actors);
    remove(m_selectedActors);

    m_displayedActors.clear();
    m_keyDefinitions.clear();
    m_keyIds.clear();
    m_displayedDefinitions.clear();
    m_selectedActors = KeyPointActors();
    m_model.reset();
    m_displayMesh = nullptr;
}
} // namespace visualization
} // namespace fosd
