#include "FOSD-Classic-QWidgets/common/PanelPresenter.h"
#include <cmath>
#include <limits>
#include <algorithm>
#include <QSet>
using namespace fosd;
using namespace fosd::visualization;
#include "FOSD-Classic-QWidgets/features/visualization/presenters/ModelPanelPresenter.h"
#include "FOSD-Classic-QWidgets/application/IWorkbenchView.h"
#include <QMap>
namespace fosd {
namespace ui {
namespace classic {
ModelPanelPresenter::ModelPanelPresenter(PanelId panel, IView *view, IWorkbenchView *host,
                                         QObject *parent)
    : PanelPresenter(panel, view, host, parent) {
}

bool ModelPanelPresenter::localAction(const QString &action, const QVariantMap &fields) {
    if (action == "pbtn_KeyPointEdit")
        return showPanel(PanelId::KeyPointEdit);
    if (action == "pbtn_KeyPointSelect")
        return showPanel(PanelId::KeyPointSelect);
    return PanelPresenter::localAction(action, fields);
}

bool ModelPanelPresenter::prepareAction(const QString &action, QVariantMap &fields) {
    if (!m_host || action != "pbtn_ModelLoad")
        return true;
    const bool save = false;
    const QString path =
        m_host->chooseFile(save, tr("选择文件"), tr("STL 模型 (*.stl);;所有文件 (*)"));
    if (path.isEmpty())
        return false;
    fields["filePath"] = path;
    return true;
}

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

void ModelPanelPresenter::bindModel() {
    connect(m_model, &application::WorkbenchModel::dataChanged, this,
            &ModelPanelPresenter::refresh);
}
void ModelPanelPresenter::initialize() {
    if (m_panel == PanelId::ModelPanel)
        presentOwn(m_panel, {{"cbox_FPS", QVariantMap{{"index", 2}}}});
    refresh();
}
void ModelPanelPresenter::refresh() {
    if (!m_model)
        return;
    if (m_panel == PanelId::ModelPanel) {
        publishScalar();
        return;
    }
    publishMarkers();
}
bool ModelPanelPresenter::handleAction(const QString &action, const QVariantMap &fields,
                                       QString &message, QVariantMap &updates) {
    bool success = true;
    const auto &a = action;
    const auto &f = fields;
    auto &view = *m_model;
    if (m_panel != PanelId::ModelPanel)
        return markerAction(action, fields, message, updates);
    if (a == "pbtn_ModelLoad") {
        success = view.loadModel(text(f, "filePath"));
        if (success) {
            refresh();
        } else
            message = tr("模型载入失败，请查看主状态栏");
    } else if (a == "pbtn_ModelClear") {
        view.clearModel();
        refresh();
    } else if (a == "cbox_FPS") {
        QString rate = text(f, "cbox_FPS");
        rate.remove("Hz", Qt::CaseInsensitive);
        bool ok = false;
        const double fps = rate.toDouble(&ok);
        success = ok && std::isfinite(fps) && fps > 0 && fps <= 240;
        if (success)
            view.setfreshFPS(qMax(1, qRound(1000.0 / fps)));
        else
            message = tr("无效的刷新率");
    } else if (a == "spin_DeformationScale") {
        bool ok = false;
        const double scale = f.value(a).toDouble(&ok);
        success = ok && std::isfinite(scale) && scale >= 0;
        if (success)
            view.setDeformationScale(scale);
        else
            message = tr("变形倍率必须为有限非负数");
    } else if (a == "pbtn_ScalarSet" || a == "pbtn_ScalarClear" || a == "cbox_ScalarSelect" ||
               a == "pbtn_Normalize" || a == "le_MapRangeMin" || a == "le_MapRangeMax" ||
               a == "cbox_ScalarValue" || a == "cbox_RangeMode") {
        auto settings = view.scalarColorSettings();
        const int field = index(f, "cbox_ScalarSelect");
        if (field >= 0 && field <= 3)
            settings.field = static_cast<ScalarField>(field);
        if (a == "pbtn_ScalarSet")
            settings.enabled = true;
        if (a == "pbtn_ScalarClear")
            settings.enabled = false;
        if (a == "pbtn_Normalize")
            settings.mappingMode = settings.mappingMode == ScalarMappingMode::Normalized
                                       ? ScalarMappingMode::Physical
                                       : ScalarMappingMode::Normalized;
        if (a == "cbox_ScalarValue")
            settings.valueMode =
                index(f, a) == 0 ? ScalarValueMode::Signed : ScalarValueMode::Absolute;
        if (a == "cbox_RangeMode")
            settings.rangeMode =
                index(f, a) == 0 ? ColorRangeMode::Automatic : ColorRangeMode::Manual;
        if (a == "le_MapRangeMin" || a == "le_MapRangeMax") {
            bool low = false, high = false;
            settings.minimum = text(f, "le_MapRangeMin").toDouble(&low);
            settings.maximum = text(f, "le_MapRangeMax").toDouble(&high);
            if (!low || !high) {
                success = false;
                message = tr("映射上下限必须是数值");
                return success;
            }
            settings.rangeMode = ColorRangeMode::Manual;
        }
        success = view.setScalarColorSettings(settings);
        if (!success)
            message = tr("颜色映射范围或参数无效");
    } else
        return false;
    return success;
}
bool ModelPanelPresenter::markerAction(const QString &action, const QVariantMap &fields,
                                       QString &message, QVariantMap &updates) {
    bool success = false;
    auto &view = *m_model;
    const auto model = view.originalModel();
    success = false;
    message = tr("点集合设置失败，请检查点 ID 和输入参数");

    if (m_panel == PanelId::KeyPointEdit) {
        if (!model) {
            message = tr("请先载入模型");
            return success;
        }
        if (action == "pbtn_SelectAll") {
            success = view.selectAllKeyPoints();
        } else if (action == "pbtn_ClearAll") {
            success = view.setKeyPoints({});
        } else if (action == "pbtn_AddKeyPoint" || action == "pbtn_RemoveKeyPoint" || action == "pbtn_FindKeyPoint" || action == "cbox_IDKeyPoint") {
            vtkIdType id = -1;
            const auto field = action == "cbox_IDKeyPoint" ? "cbox_IDKeyPoint" : "edit_IDKeyPoint";
            if (!pointId(text(fields, field), &id) || !model->contains(id)) {
                message = tr("点 ID 不在模型中");
                return success;
            }
            if (action == "pbtn_AddKeyPoint") {
                success = view.addKeyPoint(id);
            } else if (action == "pbtn_RemoveKeyPoint") {
                success = view.removeKeyPoint(id);
            } else {
                // Querying a model vertex is independent of interactive selection membership.
                const auto xyz = model->value(id);
                updates = {{"lab_XPosition", QString::number(xyz[0], 'f', 2)},
                           {"lab_YPosition", QString::number(xyz[1], 'f', 2)},
                           {"lab_ZPosition", QString::number(xyz[2], 'f', 2)}};
                success = true;
            }
        } else if (action == "pbtn_AddBox" || action == "pbtn_RemoveBox") {
            std::array<double, 3> first, second;
            const char *axes[] = {"X", "Y", "Z"};
            for (int axis = 0; axis < 3; ++axis) {
                if (!coordinate(fields, QString("edit_Box1%1").arg(axes[axis]), &first[axis]) ||
                    !coordinate(fields, QString("edit_Box2%1").arg(axes[axis]), &second[axis])) {
                    message = tr("包围盒的六个坐标必须是有限数值（mm）");
                    return success;
                }
            }
            success = view.updateKeyPointsInBox(first, second, action == "pbtn_AddBox");
        } else
            return false;
    } else if (m_panel == PanelId::KeyPointSelect) {
        if (action == "pbtn_AddPoint") {
            DisplayedPointDefinition point;
            if (index(fields, "cbox_PointSource") == 1) {
                point.source = DisplayedPointSource::External;
                if (!pointId(text(fields, "edit_ExternalPointID"), &point.pointId) ||
                    !coordinate(fields, "edit_ExternalX", &point.positionMm[0]) ||
                    !coordinate(fields, "edit_ExternalY", &point.positionMm[1]) ||
                    !coordinate(fields, "edit_ExternalZ", &point.positionMm[2])) {
                    message = tr("模型外点需要非负整数 ID 和三个有限坐标（mm）");
                    return success;
                }
            } else if (!pointId(text(fields, "cbox_KeyPointID"), &point.pointId)) {
                message = tr("请选择关键点");
                return success;
            }
            point.label = text(fields, "edit_DisplayLabel");
            success = view.upsertDisplayedPoint(point);
        } else if (action == "pbtn_DeletePoint") {
            const auto ids = fields.value("tablew_PointPara.selectedIds").toList();
            if (ids.isEmpty()) {
                message = tr("请在表格中选中需要删除的显示点");
                return success;
            }
            QVector<vtkIdType> selected;
            for (const auto &id : ids)
                selected.append(vtkIdType(id.toLongLong()));
            success = view.removeDisplayedPoints(selected);
        } else if (action == "pbtn_ClearDisplayed")
            success = view.setDisplayedPoints({});
        else if (action == "pbtn_DisplayAllKeys") {
            success = view.displayAllKeyPoints();
        } else if (action == "cbox_KeyPointID") {
            vtkIdType id = -1;
            success = pointId(text(fields, action), &id) && model && model->contains(id);
            if (success) {
                const auto xyz = model->value(id);
                updates = {{"lab_XPos", QString::number(xyz[0], 'f', 2)},
                           {"lab_YPos", QString::number(xyz[1], 'f', 2)},
                           {"lab_ZPos", QString::number(xyz[2], 'f', 2)}};
            }
        } else if (action == "radioButton" || action == "radioButton_2") {
            m_originalKeyCoordinates = action == "radioButton_2";
            publishDisplayedPoints();
            success = true;
        } else if (action == "radioButton_3") {
            view.showDisplayedPoints(fields.value(action).toBool());
            success = true;
        } else if (action == "cbox_PointSource") {
            const bool external = index(fields, action) == 1;
            updates = {{"cbox_KeyPointID.enabled", !external},
                       {"edit_ExternalPointID.enabled", external},
                       {"edit_ExternalX.enabled", external},
                       {"edit_ExternalY.enabled", external},
                       {"edit_ExternalZ.enabled", external}};
            success = true;
        } else
            return false;
    } else
        return false;
    if (success)
        message = tr("点集合已更新");
    return success;
}
void ModelPanelPresenter::publishMarkers() {
    const auto model = m_model->originalModel();
    const auto keys = m_model->keyPoints();
    QStringList ids;
    for (const auto &key : keys)
        ids.append(QString::number(qlonglong(key.pointId)));
    const auto count = QString::number(model ? model->size() : 0);
    const auto keyCount = QString::number(keys.size());
    presentOwn(PanelId::KeyPointEdit, {{"cbox_IDKeyPoint", QVariantMap{{"items", ids}}},
                                       {"lab_NumPoint", count},
                                       {"lab_NumKeyPoint", keyCount}});
    presentOwn(PanelId::KeyPointSelect, {{"cbox_KeyPointID", QVariantMap{{"items", ids}}},
                                         {"lab_NumPoints", count},
                                         {"lab_NumKeyPoints", keyCount},
                                         {"radioButton", !m_originalKeyCoordinates},
                                         {"radioButton_2", m_originalKeyCoordinates}});
    publishDisplayedPoints();
}
void ModelPanelPresenter::publishDisplayedPoints() {
    QVariantList rows;
    const auto points = m_model->displayedPoints();
    const auto infos = m_model->displayedPointInfos();
    for (int i = 0; i < points.size(); ++i) {
        const auto &xyz =
            m_originalKeyCoordinates ? infos[i].originalPositionMm : infos[i].displacementMm;
        rows.append(QVariant(
            QVariantList{qlonglong(points[i].pointId), QString::number(xyz[0], 'f', 2),
                         QString::number(xyz[1], 'f', 2), QString::number(xyz[2], 'f', 2),
                         points[i].source == DisplayedPointSource::External ? tr("模型外固定点")
                                                                            : tr("模型关键点")}));
    }
    const QStringList headers =
        m_originalKeyCoordinates ? QStringList{"ID", "X0 (mm)", "Y0 (mm)", "Z0 (mm)", tr("来源")}
                                 : QStringList{"ID", "dX (mm)", "dY (mm)", "dZ (mm)", tr("来源")};
    presentOwn(PanelId::KeyPointSelect,
               {{"tablew_PointPara", QVariantMap{{"headers", headers}, {"rows", rows}}},
                {"lab_NumDisplayed", QString::number(points.size())},
                {"radioButton_3", m_model->displayedPointsVisible()}});
}
void ModelPanelPresenter::publishScalar() {
    const auto settings = m_model->scalarColorSettings();
    const bool manual = settings.rangeMode == ColorRangeMode::Manual &&
                        settings.mappingMode == ScalarMappingMode::Physical;
    presentScalar(
        {{"cbox_ScalarSelect", QVariantMap{{"index", int(settings.field)}}},
         {"cbox_ScalarValue",
          QVariantMap{{"index", settings.valueMode == ScalarValueMode::Signed ? 0 : 1}}},
         {"cbox_RangeMode",
          QVariantMap{{"index", settings.rangeMode == ColorRangeMode::Automatic ? 0 : 1}}},
         {"pbtn_Normalize", settings.mappingMode == ScalarMappingMode::Normalized},
         {"le_MapRangeMin", QString::number(settings.minimum, 'g', 10)},
         {"le_MapRangeMax", QString::number(settings.maximum, 'g', 10)},
         {"le_MapRangeMin.enabled", manual},
         {"le_MapRangeMax.enabled", manual}});
}

} // namespace classic
} // namespace ui
} // namespace fosd

namespace fosd {
namespace ui {
namespace classic {
void ModelPanelPresenter::presentScalar(const QVariantMap &fields) {
    if (fields != m_lastScalar) {
        m_lastScalar = fields;
        presentOwn(PanelId::ModelPanel, fields);
    }
}
} // namespace classic
} // namespace ui
} // namespace fosd
