#include "VisualizationPresenter.h"
#include "../../IWorkbenchView.h"
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

bool VisualizationPresenter::localAction(const UiRequest &request) {
    const auto &action = request.action;
    const QMap<QString, PanelId> targets{{"actionModelDock", PanelId::ModelPanel},
                                         {"actionViewDock", PanelId::ScenePanel},
                                         {"actionInforDock", PanelId::InfoPanel},
                                         {"actionBackData", PanelId::HardwareData}};
    if (targets.contains(action))
        return showPanel(targets[action]);
    return PanelPresenter::localAction(request);
}

bool VisualizationPresenter::prepareRequest(UiRequest &request) {
    if (request.panel == PanelId::ScenePanel &&
        (request.action == "pbtn_ModelColor" || request.action == "pbtn_BackgroundBottomColor" ||
         request.action == "pbtn_BackgroundTopColor")) {
        QColorDialog dialog(dynamic_cast<QWidget *>(m_view));
        Ui::SceneColorDialog form;
        form.setupUi(&dialog);
        dialog.setCurrentColor(QColor(request.fields.value(request.action).toString()));
        if (dialog.exec() != QDialog::Accepted)
            return false;
        request.fields["color"] = dialog.selectedColor().name();
    }
    return true;
}
} // namespace classic
} // namespace ui
} // namespace fosd
