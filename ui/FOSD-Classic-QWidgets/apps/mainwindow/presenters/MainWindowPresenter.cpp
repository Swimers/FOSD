#include "MainWindowPresenter.h"
#include "../contracts/IWorkbenchView.h"
#include <QMap>
namespace fosd { namespace ui { namespace classic {
MainWindowPresenter::MainWindowPresenter(PanelId panel, IView *view, IWorkbenchView *host, QObject *parent)
    : PanelPresenter(panel, view, host, parent) {}
bool MainWindowPresenter::localAction(const UiRequest &request) {
    const auto &action = request.action;
    if (m_host) {
        const QStringList navigation{"pbtn_JieTiaoYi", "pbtn_2DPlot", "pbtn_3DPlot"};
        if (navigation.contains(action)) { m_host->selectPage(navigation.indexOf(action)); return true; }
        if (action == "switch_Style") { m_host->applyTheme(request.fields.value("switch_Style").toBool()); return true; }
        if (action == "pbtn_TurnOff") return showPanel(PanelId::Login);
    }
    return PanelPresenter::localAction(request);
}
} } }
