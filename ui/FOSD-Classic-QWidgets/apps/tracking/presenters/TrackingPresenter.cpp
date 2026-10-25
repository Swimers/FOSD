#include "TrackingPresenter.h"
#include "../../mainwindow/contracts/IWorkbenchView.h"
#include <QMap>
namespace fosd { namespace ui { namespace classic {
TrackingPresenter::TrackingPresenter(PanelId panel, IView *view, IWorkbenchView *host, QObject *parent)
    : PanelPresenter(panel, view, host, parent) {}
bool TrackingPresenter::localAction(const UiRequest &request) {
    const auto &action = request.action;
    return PanelPresenter::localAction(request);
}
bool TrackingPresenter::prepareRequest(UiRequest &request) {
    if (!m_host || !(request.action == "pbtn_LoadFile")) return true;
    const bool save = request.action == "pbtn_ModelExport";
    const QString path = m_host->chooseFile(save, tr("选择文件"), tr("数据文件 (*.csv *.txt);;所有文件 (*)"));
    if (path.isEmpty()) return false;
    request.fields["filePath"] = path; return true;
}
} } }
