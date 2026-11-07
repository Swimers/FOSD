#pragma once
#include "visualization/VisualizationTypes.h"
#include <QSet>
class vtkRenderer;
class vtkPolyData;
namespace fosd {
namespace visualization {
class KeyPointManager final {
public:
    KeyPointManager();
    ~KeyPointManager();

    void setRenderer(vtkRenderer *renderer);
    // Replacing the model resets all groups; every model vertex initially is a key point.
    void setModel(core::ModelDataPtr model, vtkPolyData *displayMesh);

    // Key points are membership data only. Removing a key also removes its displays/selection.
    bool setKeyPoints(const QVector<KeyPointDefinition> &points, QString *error = nullptr);
    QVector<KeyPointDefinition> keyPoints() const {
        return m_keyDefinitions;
    }
    bool contains(vtkIdType id) const {
        return m_keyIds.contains(id);
    }

    bool setDisplayedPoints(const QVector<DisplayedPointDefinition> &points,
                            QString *error = nullptr);
    QVector<DisplayedPointDefinition> displayedPoints() const {
        return m_displayedDefinitions;
    }
    bool visible() const {
        return m_visible;
    }

    bool setStyle(const KeyPointStyle &style, QString *error = nullptr);
    bool setSelectionStyle(const KeyPointStyle &style, QString *error = nullptr);
    KeyPointStyle style() const {
        return m_displayedStyle;
    }
    KeyPointStyle selectionStyle() const {
        return m_selectionStyle;
    }
    void updatePositions();
    void setVisible(bool visible);

    bool setSelectedPoint(vtkIdType id);
    void clearSelection();
    void clear();

private:
    void add(const KeyPointActors &actors);
    void remove(const KeyPointActors &actors);
    void updateVisibility();
    std::array<double, 3> position(vtkIdType id) const;
    std::array<double, 3> position(const DisplayedPointDefinition &point) const;
    bool validModelPoint(vtkIdType id) const;

    vtkRenderer *m_renderer = nullptr; // Borrowed; Control outlives this manager.
    vtkPolyData *m_displayMesh = nullptr;
    core::ModelDataPtr m_model;
    QVector<KeyPointDefinition> m_keyDefinitions;
    QSet<vtkIdType> m_keyIds;
    QVector<DisplayedPointDefinition> m_displayedDefinitions;

    KeyPointStyle m_displayedStyle;
    QMap<vtkIdType, KeyPointActors> m_displayedActors;

    KeyPointActors m_selectedActors;
    KeyPointStyle m_selectionStyle;
    bool m_visible = true;
};
} // namespace visualization
} // namespace fosd
