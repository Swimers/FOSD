#include "VisualizationPresenter.h"
#include "../../IWorkbenchView.h"
#include <QMap>
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
                                         {"actionTrackingPoints", PanelId::TrackingManager},
                                         {"actionTrackingPointData", PanelId::TrackingData},
                                         {"actionBackData", PanelId::HardwareData}};
    if (targets.contains(action))
        return showPanel(targets[action]);
    return PanelPresenter::localAction(request);
}
} // namespace classic
} // namespace ui
} // namespace fosd
