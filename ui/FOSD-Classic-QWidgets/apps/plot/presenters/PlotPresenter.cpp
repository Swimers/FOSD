#include "PlotPresenter.h"
#include "../../IWorkbenchView.h"
#include <QMap>
namespace fosd {
namespace ui {
namespace classic {
PlotPresenter::PlotPresenter(PanelId panel, IView *view, IWorkbenchView *host, QObject *parent)
    : PanelPresenter(panel, view, host, parent) {
}

bool PlotPresenter::localAction(const UiRequest &request) {
    const auto &action = request.action;
    if (action == "pbtn_ParaRevise")
        return showPanel(PanelId::PlotSettings);
    return PanelPresenter::localAction(request);
}
} // namespace classic
} // namespace ui
} // namespace fosd
