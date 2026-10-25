#include "AccountPresenter.h"
#include "../../mainwindow/contracts/IWorkbenchView.h"
#include <QMap>
namespace fosd {
namespace ui {
namespace classic {
AccountPresenter::AccountPresenter(PanelId panel, IView *view, IWorkbenchView *host,
                                   QObject *parent)
    : PanelPresenter(panel, view, host, parent) {
}

bool AccountPresenter::localAction(const UiRequest &request) {
    const auto &action = request.action;
    if (action == "btn_Register")
        return showPanel(PanelId::Registration);
    if (action == "btn_Forget")
        return showPanel(PanelId::Recovery);
    if (action == "btn_Clear" || action == "pbtn_RegisterClear" || action == "pbtn_ForgetClear") {
        m_view->clearInputs();
        return true;
    }
    return PanelPresenter::localAction(request);
}

bool AccountPresenter::isSensitiveAction(const QString &action) const {
    return action == "btn_Login" || action == "pbtn_RegisterSet" || action == "pbtn_ForgetSet" ||
           action == "pbtn_UserSet";
}
} // namespace classic
} // namespace ui
} // namespace fosd
