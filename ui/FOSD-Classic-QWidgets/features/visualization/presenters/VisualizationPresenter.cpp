#include "FOSD-Classic-QWidgets/common/PanelPresenter.h"
#include "FOSD-Classic-QWidgets/common/CoordinateText.h"
#include <cmath>
using namespace fosd;
using namespace fosd::visualization;
#include "FOSD-Classic-QWidgets/features/visualization/presenters/VisualizationPresenter.h"
#include "FOSD-Classic-QWidgets/application/IWorkbenchView.h"
#include <QMap>
#include <QColorDialog>
#include "ui_sceneColorDialog.h"
namespace fosd {
namespace ui {
namespace classic {
VisualizationPresenter::VisualizationPresenter(PanelId panel, IView *view, IWorkbenchView *host,
                                               QObject *parent)
    : PanelPresenter(panel, view, host, parent) {
}

bool VisualizationPresenter::localAction(const QString &action, const QVariantMap &fields) {
    const QMap<QString, PanelId> targets{{"actionModelDock", PanelId::ModelPanel},
                                         {"actionViewDock", PanelId::ScenePanel},
                                         {"actionInforDock", PanelId::InfoPanel},
                                         {"actionBackData", PanelId::HardwareData}};
    if (targets.contains(action))
        return showPanel(targets[action]);
    return PanelPresenter::localAction(action, fields);
}

bool VisualizationPresenter::prepareAction(const QString &action, QVariantMap &fields) {
    if (m_panel == PanelId::ScenePanel &&
        (action == "pbtn_ModelColor" || action == "pbtn_BackgroundBottomColor" ||
         action == "pbtn_BackgroundTopColor")) {
        QColorDialog dialog(dynamic_cast<QWidget *>(m_view));
        Ui::SceneColorDialog form;
        form.setupUi(&dialog);
        dialog.setCurrentColor(QColor(fields.value(action).toString()));
        if (dialog.exec() != QDialog::Accepted)
            return false;
        fields["color"] = dialog.selectedColor().name();
    }
    return true;
}
bool VisualizationPresenter::handleAction(const QString &action, const QVariantMap &fields,
                                          QString &message, QVariantMap &updates) {
    bool success = true;
    const auto &a = action;
    const auto &f = fields;
    auto &view = *m_model;
    if (m_panel == PanelId::Visualization) {
        if (a != "actionStart" && a != "actionPause" && a != "actionStop" && a != "actionReset")
            return false;
        if (a == "actionReset") {
            view.resetDeformation();
            present(PanelId::InfoPanel, {{"lab_MaxDeformation", QStringLiteral("0 mm")}});
        } else
            success = view.setDisplayState(
                a == "actionStart" ? application::DisplayState::Running
                                   : a == "actionPause" ? application::DisplayState::Paused
                                                        : application::DisplayState::Stopped,
                &message);
        if (success)
            message = a == "actionStart" ? tr("实时变形显示已启动")
                                         : tr("变形显示已暂停；通信、解算和存储继续运行");
        return success;
    }
    if (m_panel != PanelId::ScenePanel)
        return false;
    if (a == "cbox_BackgroundMode" || a == "pbtn_BackgroundBottomColor" ||
        a == "pbtn_BackgroundTopColor" || a == "pbtn_ModelColor") {
        auto background = view.backgroundSettings();
        if (a == "cbox_BackgroundMode") {
            const int mode = index(f, a);
            success = mode == 0 || mode == 1;
            background.gradient = mode == 1;
        } else {
            const QColor color(text(f, "color"));
            success = color.isValid();
            if (success) {
                const std::array<double, 3> rgb{{color.redF(), color.greenF(), color.blueF()}};
                if (a == "pbtn_ModelColor")
                    success = view.setModelColor(rgb);
                else if (a == "pbtn_BackgroundTopColor")
                    background.topColor = rgb;
                else
                    background.bottomColor = rgb;
            }
        }
        if (success && a != "pbtn_ModelColor")
            success = view.setBackgroundSettings(background);
        if (!success)
            message = tr("颜色或背景设置未能保存");
        publishAppearance();
    } else if (a == "cbox_AxisAble")
        view.showAxes(f.value(a).toBool());
    else if (a == "hsd_AxisSize") {
        auto settings = view.axesSettings();
        const double scale = f.value(a).toDouble() / 100.0;
        settings.lengthMm = qMax(0.001, 50000.0 * scale * scale);
        success = view.setAxesSettings(settings);
    } else if (a == "cbox_GridAble") {
        const bool visible = f.value(a).toBool();
        view.showGrid(visible);
        updates = {{"cbox_XOZGrid", visible}, {"cbox_XOYGrid", visible}, {"cbox_ZOYGrid", visible}};
    } else if (a == "cbox_XOZGrid" || a == "cbox_XOYGrid" || a == "cbox_ZOYGrid") {
        const auto plane = a == "cbox_XOZGrid"
                               ? GridPlane::XZ
                               : a == "cbox_XOYGrid" ? GridPlane::XY : GridPlane::YZ;
        success = view.showGrid(plane, f.value(a).toBool());
        const auto settings = view.gridSettings();
        updates["cbox_GridAble"] =
            settings.visible[0] || settings.visible[1] || settings.visible[2];
    } else if (a == "hsd_GridSize") {
        auto settings = view.gridSettings();
        const double scale = f.value(a).toDouble() / 20.0;
        settings.halfExtentMm = qMax(1.0, 3000.0 * scale);
        settings.divisions = qBound(1, qRound(100.0 * scale), 500);
        success = view.setGridSettings(settings);
    } else if (a == "cbox_CareamLockAble")
        view.setCameraLocked(f.value(a).toBool());
    else if (a == "pbtn_SwitchView" || a == "cbox_SelectView") {
        const CameraPreset presets[] = {CameraPreset::Front,  CameraPreset::Right,
                                        CameraPreset::Left,   CameraPreset::Top,
                                        CameraPreset::Bottom, CameraPreset::Back};
        const int selected = index(f, "cbox_SelectView");
        if (selected < 0 || selected >= 6) {
            success = false;
            message = tr("请选择有效视角");
        } else
            view.setCameraPreset(presets[selected]);
    } else
        return false;

    return success;
}
void VisualizationPresenter::bindModel() {
    connect(m_model, &application::WorkbenchModel::dataChanged, this,
            &VisualizationPresenter::refresh);
}
void VisualizationPresenter::initialize() {
    if (!m_model)
        return;
    if (m_panel == PanelId::ScenePanel) {
        const auto axes = m_model->axesSettings();
        const auto grid = m_model->gridSettings();
        present(PanelId::ScenePanel,
                {{"cbox_AxisAble", axes.visible},
                 {"hsd_AxisSize", qRound(100 * std::sqrt(axes.lengthMm / 50000.0))},
                 {"cbox_GridAble", grid.visible[0] || grid.visible[1] || grid.visible[2]},
                 {"cbox_XOZGrid", grid.visible[0]},
                 {"cbox_XOYGrid", grid.visible[1]},
                 {"cbox_ZOYGrid", grid.visible[2]},
                 {"hsd_GridSize", qRound(grid.halfExtentMm / 150.0)},
                 {"cbox_CareamLockAble", m_model->cameraSettings().locked}});
        publishAppearance();
    }
    refresh();
}
void VisualizationPresenter::refresh() {
    if (!m_model)
        return;
    auto s = m_model->state();
    if (m_panel == PanelId::Visualization) {
        const bool running = s.display == application::DisplayState::Running;
        present(
            PanelId::Visualization,
            {{"actionStart.enabled", s.hasModel && !running},
             {"actionPause.enabled", s.hasModel && running},
             {"actionStop.enabled", s.hasModel && s.display != application::DisplayState::Stopped},
             {"actionReset.enabled", s.hasModel}});
        selectionChanged(m_model->selectedPoint());
    } else if (m_panel == PanelId::InfoPanel) {
        present(PanelId::InfoPanel,
                {{"lab_NumPoints",
                  QString::number(m_model->originalModel() ? m_model->originalModel()->size() : 0)},
                 {"lab_NumPolys", QString::number(m_model->modelPolygonCount())},
                 {"lab_NumKeyPoints", QString::number(m_model->keyPoints().size())}});
        poll();
    }
}
void VisualizationPresenter::poll() {
    if (!m_model || m_panel != PanelId::InfoPanel)
        return;
    const auto result = m_model->result();
    if (!result) {
        m_presentedResult = 0;
        present(PanelId::InfoPanel, {{"lab_MaxDeformation", QStringLiteral("--")}});
        return;
    }
    if (result->revision == m_presentedResult)
        return;
    double maximum = 0;
    for (const auto &d : result->displacementMap)
        if (d.size() == 3)
            maximum = qMax(maximum, std::sqrt(d[0] * d[0] + d[1] * d[1] + d[2] * d[2]));
    if (m_model->state().display == application::DisplayState::Running)
        present(PanelId::InfoPanel,
                {{"lab_MaxDeformation", QString::number(maximum, 'g', 8) + " mm"}});
    m_presentedResult = result->revision;
}
void VisualizationPresenter::selectionChanged(const visualization::PickedPointInfo &info) {
    present(PanelId::Visualization,
            {{"statusBar_SelectedPoint",
              selectedPointStatus(info.valid ? QString::number(qlonglong(info.pointId)) : QString(),
                                  info.originalPositionMm, info.displayPositionMm)}});
    if (info.valid) {
        present(PanelId::KeyPointEdit,
                {{"edit_IDKeyPoint", QString::number(qlonglong(info.pointId))},
                 {"lab_XPosition", QString::number(info.originalPositionMm[0], 'f', 2)},
                 {"lab_YPosition", QString::number(info.originalPositionMm[1], 'f', 2)},
                 {"lab_ZPosition", QString::number(info.originalPositionMm[2], 'f', 2)}});
    } else {
        present(PanelId::KeyPointEdit, {{"lab_XPosition", QString()},
                                        {"lab_YPosition", QString()},
                                        {"lab_ZPosition", QString()}});
    }
}
void VisualizationPresenter::publishAppearance() {
    const auto background = m_model->backgroundSettings();
    auto colorName = [](const std::array<double, 3> &rgb) {
        return QColor::fromRgbF(rgb[0], rgb[1], rgb[2]).name();
    };
    present(PanelId::ScenePanel,
            {{"pbtn_ModelColor", colorName(m_model->modelColor())},
             {"pbtn_BackgroundBottomColor", colorName(background.bottomColor)},
             {"pbtn_BackgroundTopColor", colorName(background.topColor)},
             {"cbox_BackgroundMode", QVariantMap{{"index", background.gradient ? 1 : 0}}},
             {"pbtn_BackgroundTopColor.enabled", background.gradient},
             {"label_BackgroundTop.enabled", background.gradient}});
}
} // namespace classic
} // namespace ui
} // namespace fosd
