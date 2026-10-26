#pragma once
#include "KeyPointCreator.h"
class vtkRenderer;
class vtkPolyData;
namespace fosd {
namespace visualization {
class KeyPointManager final {
public:
    KeyPointManager();
    ~KeyPointManager();
    void setRenderer(vtkRenderer *renderer);
    void setModel(core::ModelDataPtr model, vtkPolyData *displayMesh);
    bool setKeyPoints(const QVector<KeyPointDefinition> &points, QString *error = nullptr);
    QVector<KeyPointDefinition> keyPoints() const {
        return m_definitions;
    }
    bool contains(vtkIdType id) const {
        return m_points.contains(id);
    }
    bool setStyle(const KeyPointStyle &style, QString *error = nullptr);
    bool setSelectionStyle(const KeyPointStyle &style, QString *error = nullptr);
    KeyPointStyle style() const {
        return m_style;
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
    vtkRenderer *m_renderer = nullptr; // Borrowed; Control outlives this manager.
    vtkPolyData *m_displayMesh = nullptr;
    core::ModelDataPtr m_model;
    QVector<KeyPointDefinition> m_definitions;
    QMap<vtkIdType, KeyPointActors> m_points;
    KeyPointActors m_selection;
    KeyPointStyle m_style;
    KeyPointStyle m_selectionStyle;
    bool m_visible = true;
};
} // namespace visualization
} // namespace fosd
