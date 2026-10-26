#include "TrackingPresenter.h"
#include "../../IWorkbenchView.h"
#include <QMap>
namespace fosd {
namespace ui {
namespace classic {
TrackingPresenter::TrackingPresenter(PanelId panel, IView *view, IWorkbenchView *host,
                                     QObject *parent)
    : PanelPresenter(panel, view, host, parent) {
}

bool TrackingPresenter::localAction(const UiRequest &request) {
    const auto &action = request.action;
    return PanelPresenter::localAction(request);
}

bool TrackingPresenter::prepareRequest(UiRequest &request) {
    Q_UNUSED(request);
    return true;
}
} // namespace classic
} // namespace ui
} // namespace fosd
