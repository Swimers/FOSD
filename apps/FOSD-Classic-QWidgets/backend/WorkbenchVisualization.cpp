#include "WorkbenchRuntime.h"
#include <cmath>
namespace fosd {
namespace application {
using namespace ui::classic;
using namespace visualization;
bool WorkbenchRuntime::visualizationAction(const UiRequest &request, UiResponse *response) {
    const auto panel = request.panel;
    if (panel != PanelId::Visualization && panel != PanelId::ModelPanel &&
        panel != PanelId::ScenePanel && panel != PanelId::KeyPointEdit &&
        panel != PanelId::KeyPointSelect)
        return false;
    const auto &a = request.action;
    const auto &f = request.fields;
    auto &view = *m_visualization;
    response->success = true;
    response->message = tr("设置已应用");
    if (panel == PanelId::Visualization) {
        if (a == "actionPlateValidation")
            response->success = loadPlate(&response->message);
        else if (a == "actionHYY") {
            auto configuration = m_configuration;
            if (configuration.algorithm == Calculation::CalculationAlgorithm::Plate)
                configuration.strainCalibration.calibrationFactor = 0.00075;
            configuration.algorithm = Calculation::CalculationAlgorithm::HYY;
            response->success = configure(configuration, &response->message);
            if (response->success)
                response->message = tr("HYY 配置已提交；实际设备请重新置零并检查标定参数和模型");
        } else if (a == "actionStart" || a == "actionPause" || a == "actionStop" ||
                   a == "actionReset") {
            m_running = a == "actionStart";
            view.setResultRepository(m_running && !m_modelPending && !m_configurationPending &&
                                             m_result
                                         ? m_calculation->dataModel()
                                         : nullptr);
            if (a == "actionReset")
                view.resetDeformation();
            response->message = m_running ? tr("实时变形显示已启动")
                                          : tr("变形显示已暂停；通信、解算和存储继续运行");
            publishRunState();
        } else
            return false;
    } else if (panel == PanelId::ModelPanel) {
        if (a == "pbtn_ModelLoad") {
            response->success = view.loadModel(text(f, "filePath"));
            if (response->success) {
                m_modelPath = text(f, "filePath");
                publishModel();
            } else
                response->message = tr("模型载入失败，请查看主状态栏");
        } else if (a == "pbtn_ModelClear") {
            view.setResultRepository(nullptr);
            view.unloadModel();
            m_calculation->clearModel();
            m_modelPath.clear();
            m_result.reset();
            m_modelRevision = 0;
            m_modelPending = false;
            ++m_modelGeneration;
            publishModel();
        } else if (a == "cbox_FPS") {
            QString rate = text(f, "cbox_FPS");
            rate.remove("Hz", Qt::CaseInsensitive);
            bool ok = false;
            const double fps = rate.toDouble(&ok);
            response->success = ok && std::isfinite(fps) && fps > 0 && fps <= 240;
            if (response->success)
                view.setfreshFPS(qMax(1, qRound(1000.0 / fps)));
            else
                response->message = tr("无效的刷新率");
        } else if (a == "spin_DeformationScale") {
            bool ok = false;
            const double scale = f.value(a).toDouble(&ok);
            response->success = ok && std::isfinite(scale) && scale >= 0;
            if (response->success)
                view.setDeformationScale(scale);
            else
                response->message = tr("变形倍率必须为有限非负数");
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
                    response->success = false;
                    response->message = tr("映射上下限必须是数值");
                    return true;
                }
                settings.rangeMode = ColorRangeMode::Manual;
            }
            response->success = view.setScalarColorSettings(settings);
            if (!response->success)
                response->message = tr("颜色映射范围或参数无效");
        } else
            return false;
    } else if (panel == PanelId::ScenePanel) {
        if (a == "cbox_AxisAble")
            view.showAxes(f.value(a).toBool());
        else if (a == "hsd_AxisSize") {
            auto settings = view.axesSettings();
            const double scale = f.value(a).toDouble() / 100.0;
            settings.lengthMm = qMax(0.001, 50000.0 * scale * scale);
            response->success = view.setAxesSettings(settings);
        } else if (a == "cbox_GridAble") {
            const bool visible = f.value(a).toBool();
            view.showGrid(visible);
            response->fields = {
                {"cbox_XOZGrid", visible}, {"cbox_XOYGrid", visible}, {"cbox_ZOYGrid", visible}};
        } else if (a == "cbox_XOZGrid" || a == "cbox_XOYGrid" || a == "cbox_ZOYGrid") {
            const auto plane = a == "cbox_XOZGrid"
                                   ? GridPlane::XZ
                                   : a == "cbox_XOYGrid" ? GridPlane::XY : GridPlane::YZ;
            response->success = view.showGrid(plane, f.value(a).toBool());
            const auto settings = view.gridSettings();
            response->fields["cbox_GridAble"] =
                settings.visible[0] || settings.visible[1] || settings.visible[2];
        } else if (a == "hsd_GridSize") {
            auto settings = view.gridSettings();
            const double scale = f.value(a).toDouble() / 20.0;
            settings.halfExtentMm = qMax(1.0, 3000.0 * scale);
            settings.divisions = qBound(1, qRound(100.0 * scale), 500);
            response->success = view.setGridSettings(settings);
        } else if (a == "cbox_CareamLockAble")
            view.setCameraLocked(f.value(a).toBool());
        else if (a == "pbtn_SwitchView" || a == "cbox_SelectView") {
            const CameraPreset presets[] = {CameraPreset::Front,  CameraPreset::Right,
                                            CameraPreset::Left,   CameraPreset::Top,
                                            CameraPreset::Bottom, CameraPreset::Back};
            const int selected = index(f, "cbox_SelectView");
            if (selected < 0 || selected >= 6) {
                response->success = false;
                response->message = tr("请选择有效视角");
            } else
                view.setCameraPreset(presets[selected]);
        } else
            return false;
    } else {
        const QStringList supported{"pbtn_SelectAll",   "pbtn_ClearAll",   "pbtn_FindKeyPoint",
                                    "cbox_IDKeyPoint",  "cbox_KeyPointID", "pbtn_AddPoint",
                                    "pbtn_DeletePoint", "radioButton",     "radioButton_2",
                                    "radioButton_3"};
        if (!supported.contains(a))
            return false;
        auto points = view.keyPoints();
        const auto model = view.originalModel();
        if (!model) {
            response->success = false;
            response->message = tr("请先载入模型");
            return true;
        }
        if (a == "pbtn_SelectAll") {
            points.clear();
            for (auto it = model->constBegin(); it != model->constEnd(); ++it) {
                KeyPointDefinition point;
                point.pointId = vtkIdType(it.key());
                points.append(point);
            }
            response->success = view.setKeyPoints(points);
        } else if (a == "pbtn_ClearAll")
            response->success = view.setKeyPoints({});
        else if (a == "pbtn_FindKeyPoint" || a == "cbox_IDKeyPoint" || a == "cbox_KeyPointID" ||
                 a == "pbtn_AddPoint") {
            const auto name = panel == PanelId::KeyPointSelect
                                  ? "cbox_KeyPointID"
                                  : a == "cbox_IDKeyPoint" ? "cbox_IDKeyPoint" : "edit_IDKeyPoint";
            bool ok = false;
            const auto id = text(f, name).toLongLong(&ok);
            response->success = ok && model->contains(id);
            if (response->success) {
                view.selectPoint(vtkIdType(id));
                if (a == "pbtn_AddPoint") {
                    bool exists = false;
                    for (const auto &point : points)
                        if (point.pointId == id)
                            exists = true;
                    if (!exists) {
                        KeyPointDefinition point;
                        point.pointId = vtkIdType(id);
                        points.append(point);
                    }
                    response->success = view.setKeyPoints(points);
                }
            } else
                response->message = tr("点 ID 不在模型中");
        } else if (a == "pbtn_DeletePoint") {
            const auto rows = f.value("tablew_PointPara.selectedRows").toList();
            QVector<KeyPointDefinition> retained;
            for (int i = 0; i < points.size(); ++i)
                if (!rows.contains(QVariant(i)))
                    retained.append(points[i]);
            response->success = view.setKeyPoints(retained);
        } else if (a == "radioButton" || a == "radioButton_2") {
            m_originalKeyCoordinates = a == "radioButton_2";
        } else if (a == "radioButton_3")
            view.showKeyPoints(f.value(a).toBool());
        else
            return false;
        publishModel();
    }
    return true;
}

void WorkbenchRuntime::publishScalar() {
    const auto settings = m_visualization->scalarColorSettings();
    const bool manual = settings.rangeMode == ColorRangeMode::Manual &&
                        settings.mappingMode == ScalarMappingMode::Physical;
    emit presentationChanged(
        PanelId::ModelPanel,
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
} // namespace application
} // namespace fosd
