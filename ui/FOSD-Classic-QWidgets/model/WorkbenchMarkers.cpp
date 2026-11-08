#include "FOSD-Classic-QWidgets/model/WorkbenchModel.h"
#include <QSet>
#include <algorithm>
#include <cmath>
namespace fosd {
namespace application {
using namespace visualization;
bool WorkbenchModel::selectAllKeyPoints() {
    auto model = originalModel();
    if (!model)
        return false;
    QVector<KeyPointDefinition> keys;
    for (auto it = model->constBegin(); it != model->constEnd(); ++it) {
        KeyPointDefinition key;
        key.pointId = it.key();
        keys.append(key);
    }
    return setKeyPoints(keys);
}
bool WorkbenchModel::addKeyPoint(vtkIdType id) {
    auto keys = keyPoints();
    for (const auto &key : keys)
        if (key.pointId == id)
            return true;
    KeyPointDefinition key;
    key.pointId = id;
    keys.append(key);
    return setKeyPoints(keys);
}
bool WorkbenchModel::removeKeyPoint(vtkIdType id) {
    auto keys = keyPoints();
    for (int i = keys.size() - 1; i >= 0; --i)
        if (keys[i].pointId == id)
            keys.removeAt(i);
    return setKeyPoints(keys);
}
bool WorkbenchModel::updateKeyPointsInBox(const std::array<double, 3> &first,
                                          const std::array<double, 3> &second, bool add) {
    const auto model = originalModel();
    if (!model)
        return false;
    for (int axis = 0; axis < 3; ++axis)
        if (!std::isfinite(first[axis]) || !std::isfinite(second[axis]))
            return false;
    QSet<vtkIdType> inside;
    for (auto it = model->constBegin(); it != model->constEnd(); ++it) {
        bool contained = true;
        for (int axis = 0; axis < 3; ++axis)
            contained = contained && it.value()[axis] >= std::min(first[axis], second[axis]) &&
                        it.value()[axis] <= std::max(first[axis], second[axis]);
        if (contained)
            inside.insert(it.key());
    }
    QVector<KeyPointDefinition> keys;
    for (const auto &key : keyPoints()) {
        if (add || !inside.contains(key.pointId))
            keys.append(key);
        if (add)
            inside.remove(key.pointId);
    }
    if (add)
        for (auto it = model->constBegin(); it != model->constEnd(); ++it)
            if (inside.contains(it.key())) {
                KeyPointDefinition key;
                key.pointId = it.key();
                keys.append(key);
            }
    return setKeyPoints(keys);
}
bool WorkbenchModel::upsertDisplayedPoint(const DisplayedPointDefinition &point) {
    auto points = displayedPoints();
    for (auto &p : points)
        if (p.pointId == point.pointId) {
            p = point;
            return setDisplayedPoints(points);
        }
    points.append(point);
    return setDisplayedPoints(points);
}
bool WorkbenchModel::removeDisplayedPoints(const QVector<vtkIdType> &ids) {
    QVector<DisplayedPointDefinition> retained;
    for (const auto &point : displayedPoints())
        if (!ids.contains(point.pointId))
            retained.append(point);
    return setDisplayedPoints(retained);
}
bool WorkbenchModel::displayAllKeyPoints() {
    auto points = displayedPoints();
    QSet<vtkIdType> ids;
    for (const auto &point : points)
        ids.insert(point.pointId);
    for (const auto &key : keyPoints())
        if (!ids.contains(key.pointId)) {
            DisplayedPointDefinition point;
            point.pointId = key.pointId;
            point.label = key.label;
            points.append(point);
        }
    return setDisplayedPoints(points);
}
} // namespace application
} // namespace fosd
