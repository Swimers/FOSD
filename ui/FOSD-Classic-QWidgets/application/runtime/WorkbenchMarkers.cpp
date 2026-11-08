#include "FOSD-Classic-QWidgets/application/runtime/WorkbenchRuntime.h"
#include <algorithm>
#include <cmath>
#include <limits>

namespace fosd {
namespace application {
using namespace ui::classic;
using namespace visualization;
namespace {
bool pointId(const QString &text, vtkIdType *id) {
    bool ok = false;
    const auto value = text.toLongLong(&ok);
    if (!ok || value < 0 || value > std::numeric_limits<vtkIdType>::max())
        return false;
    *id = vtkIdType(value);
    return true;
}

bool coordinate(const QVariantMap &fields, const QString &name, double *value) {
    bool ok = false;
    *value = fields.value(name).toString().toDouble(&ok);
    return ok && std::isfinite(*value);
}
} // namespace

bool WorkbenchRuntime::markerAction(const UiRequest &request, UiResponse *response) {
    const auto &action = request.action;
    const auto &fields = request.fields;
    auto &view = *m_visualization;
    const auto model = view.originalModel();
    response->success = false;
    response->message = tr("点集合设置失败，请检查点 ID 和输入参数");

    if (request.panel == PanelId::KeyPointEdit) {
        if (!model) {
            response->message = tr("请先载入模型");
            return true;
        }
        auto keys = view.keyPoints();
        if (action == "pbtn_SelectAll") {
            keys.clear();
            for (auto it = model->constBegin(); it != model->constEnd(); ++it) {
                KeyPointDefinition point;
                point.pointId = vtkIdType(it.key());
                keys.append(point);
            }
            response->success = view.setKeyPoints(keys);
        } else if (action == "pbtn_ClearAll")
            response->success = view.setKeyPoints({});
        else if (action == "pbtn_AddKeyPoint" || action == "pbtn_RemoveKeyPoint" ||
                 action == "pbtn_FindKeyPoint" || action == "cbox_IDKeyPoint") {
            vtkIdType id = -1;
            const auto field = action == "cbox_IDKeyPoint" ? "cbox_IDKeyPoint" : "edit_IDKeyPoint";
            if (!pointId(text(fields, field), &id) || !model->contains(id)) {
                response->message = tr("点 ID 不在模型中");
                return true;
            }
            if (action == "pbtn_AddKeyPoint") {
                bool exists = false;
                for (const auto &key : keys)
                    exists = exists || key.pointId == id;
                if (!exists) {
                    KeyPointDefinition point;
                    point.pointId = id;
                    keys.append(point);
                }
                response->success = view.setKeyPoints(keys);
            } else if (action == "pbtn_RemoveKeyPoint") {
                for (int i = keys.size() - 1; i >= 0; --i)
                    if (keys[i].pointId == id)
                        keys.removeAt(i);
                response->success = view.setKeyPoints(keys);
            } else {
                // Querying a model vertex is independent of interactive selection membership.
                const auto xyz = model->value(id);
                response->fields = {{"lab_XPosition", QString::number(xyz[0], 'f', 2)},
                                    {"lab_YPosition", QString::number(xyz[1], 'f', 2)},
                                    {"lab_ZPosition", QString::number(xyz[2], 'f', 2)}};
                response->success = true;
            }
        } else if (action == "pbtn_AddBox" || action == "pbtn_RemoveBox") {
            std::array<double, 3> first, second;
            const char *axes[] = {"X", "Y", "Z"};
            for (int axis = 0; axis < 3; ++axis) {
                if (!coordinate(fields, QString("edit_Box1%1").arg(axes[axis]), &first[axis]) ||
                    !coordinate(fields, QString("edit_Box2%1").arg(axes[axis]), &second[axis])) {
                    response->message = tr("包围盒的六个坐标必须是有限数值（mm）");
                    return true;
                }
            }
            QSet<vtkIdType> inside;
            for (auto it = model->constBegin(); it != model->constEnd(); ++it) {
                bool contained = true;
                for (int axis = 0; axis < 3; ++axis)
                    contained = contained &&
                                it.value()[axis] >= std::min(first[axis], second[axis]) &&
                                it.value()[axis] <= std::max(first[axis], second[axis]);
                if (contained)
                    inside.insert(vtkIdType(it.key()));
            }
            QVector<KeyPointDefinition> updated;
            for (const auto &key : keys) {
                if (action == "pbtn_AddBox" || !inside.contains(key.pointId))
                    updated.append(key);
                if (action == "pbtn_AddBox")
                    inside.remove(key.pointId);
            }
            if (action == "pbtn_AddBox")
                for (auto it = model->constBegin(); it != model->constEnd(); ++it)
                    if (inside.contains(vtkIdType(it.key()))) {
                        KeyPointDefinition point;
                        point.pointId = vtkIdType(it.key());
                        updated.append(point);
                    }
            response->success = view.setKeyPoints(updated);
        } else
            return false;
    } else if (request.panel == PanelId::KeyPointSelect) {
        auto displays = view.displayedPoints();
        if (action == "pbtn_AddPoint") {
            DisplayedPointDefinition point;
            if (index(fields, "cbox_PointSource") == 1) {
                point.source = DisplayedPointSource::External;
                if (!pointId(text(fields, "edit_ExternalPointID"), &point.pointId) ||
                    !coordinate(fields, "edit_ExternalX", &point.positionMm[0]) ||
                    !coordinate(fields, "edit_ExternalY", &point.positionMm[1]) ||
                    !coordinate(fields, "edit_ExternalZ", &point.positionMm[2])) {
                    response->message = tr("模型外点需要非负整数 ID 和三个有限坐标（mm）");
                    return true;
                }
            } else if (!pointId(text(fields, "cbox_KeyPointID"), &point.pointId)) {
                response->message = tr("请选择关键点");
                return true;
            }
            point.label = text(fields, "edit_DisplayLabel");
            bool replaced = false;
            for (auto &display : displays)
                if (display.pointId == point.pointId) {
                    display = point;
                    replaced = true;
                    break;
                }
            if (!replaced)
                displays.append(point);
            response->success = view.setDisplayedPoints(displays);
        } else if (action == "pbtn_DeletePoint") {
            const auto ids = fields.value("tablew_PointPara.selectedIds").toList();
            if (ids.isEmpty()) {
                response->message = tr("请在表格中选中需要删除的显示点");
                return true;
            }
            QVector<DisplayedPointDefinition> retained;
            for (const auto &point : displays)
                if (!ids.contains(QVariant(qlonglong(point.pointId))))
                    retained.append(point);
            response->success = view.setDisplayedPoints(retained);
        } else if (action == "pbtn_ClearDisplayed")
            response->success = view.setDisplayedPoints({});
        else if (action == "pbtn_DisplayAllKeys") {
            QSet<vtkIdType> ids;
            for (const auto &point : displays)
                ids.insert(point.pointId);
            for (const auto &key : view.keyPoints())
                if (!ids.contains(key.pointId)) {
                    DisplayedPointDefinition point;
                    point.pointId = key.pointId;
                    point.label = key.label;
                    displays.append(point);
                }
            response->success = view.setDisplayedPoints(displays);
        } else if (action == "cbox_KeyPointID") {
            vtkIdType id = -1;
            response->success = pointId(text(fields, action), &id) && model && model->contains(id);
            if (response->success) {
                const auto xyz = model->value(id);
                response->fields = {{"lab_XPos", QString::number(xyz[0], 'f', 2)},
                                    {"lab_YPos", QString::number(xyz[1], 'f', 2)},
                                    {"lab_ZPos", QString::number(xyz[2], 'f', 2)}};
            }
        } else if (action == "radioButton" || action == "radioButton_2") {
            m_originalKeyCoordinates = action == "radioButton_2";
            publishDisplayedPoints();
            response->success = true;
        } else if (action == "radioButton_3") {
            view.showDisplayedPoints(fields.value(action).toBool());
            response->success = true;
        } else if (action == "cbox_PointSource") {
            const bool external = index(fields, action) == 1;
            response->fields = {{"cbox_KeyPointID.enabled", !external},
                                {"edit_ExternalPointID.enabled", external},
                                {"edit_ExternalX.enabled", external},
                                {"edit_ExternalY.enabled", external},
                                {"edit_ExternalZ.enabled", external}};
            response->success = true;
        } else
            return false;
    } else
        return false;
    if (response->success)
        response->message = tr("点集合已更新");
    return true;
}

void WorkbenchRuntime::publishMarkers() {
    const auto model = m_visualization->originalModel();
    const auto keys = m_visualization->keyPoints();
    QStringList ids;
    for (const auto &key : keys)
        ids.append(QString::number(qlonglong(key.pointId)));
    const auto count = QString::number(model ? model->size() : 0);
    const auto keyCount = QString::number(keys.size());
    emit presentationChanged(PanelId::KeyPointEdit,
                             {{"cbox_IDKeyPoint", QVariantMap{{"items", ids}}},
                              {"lab_NumPoint", count},
                              {"lab_NumKeyPoint", keyCount}});
    emit presentationChanged(
        PanelId::KeyPointSelect,
        {{"cbox_KeyPointID", QVariantMap{{"items", ids}}},
         {"lab_NumPoints", count},
         {"lab_NumKeyPoints", keyCount},
         {"radioButton", !m_originalKeyCoordinates},
         {"radioButton_2", m_originalKeyCoordinates}});
    emit presentationChanged(PanelId::InfoPanel, {{"lab_NumKeyPoints", keyCount}});
    publishDisplayedPoints();
}

void WorkbenchRuntime::publishDisplayedPoints() {
    QVariantList rows;
    const auto points = m_visualization->displayedPoints();
    const auto infos = m_visualization->displayedPointInfos();
    for (int i = 0; i < points.size(); ++i) {
        const auto &xyz =
            m_originalKeyCoordinates ? infos[i].originalPositionMm : infos[i].displacementMm;
        rows.append(QVariant(QVariantList{qlonglong(points[i].pointId),
                                          QString::number(xyz[0], 'f', 2),
                                          QString::number(xyz[1], 'f', 2),
                                          QString::number(xyz[2], 'f', 2),
                                          points[i].source == DisplayedPointSource::External
                                              ? tr("模型外固定点")
                                              : tr("模型关键点")}));
    }
    const QStringList headers =
        m_originalKeyCoordinates ? QStringList{"ID", "X0 (mm)", "Y0 (mm)", "Z0 (mm)", tr("来源")}
                                 : QStringList{"ID", "dX (mm)", "dY (mm)", "dZ (mm)", tr("来源")};
    emit presentationChanged(
        PanelId::KeyPointSelect,
        {{"tablew_PointPara", QVariantMap{{"headers", headers}, {"rows", rows}}},
         {"lab_NumDisplayed", QString::number(points.size())},
         {"radioButton_3", m_visualization->displayedPointsVisible()}});
}
} // namespace application
} // namespace fosd
