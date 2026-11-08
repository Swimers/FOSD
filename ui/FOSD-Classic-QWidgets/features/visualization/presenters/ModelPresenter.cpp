#include "FOSD-Classic-QWidgets/features/visualization/presenters/ModelPresenter.h"
#include "FOSD-Classic-QWidgets/application/IWorkbenchView.h"
#include <QMap>
namespace fosd {
namespace ui {
namespace classic {
ModelPresenter::ModelPresenter(PanelId panel, IView *view, IWorkbenchView *host, QObject *parent)
    : PanelPresenter(panel, view, host, parent) {
}

bool ModelPresenter::localAction(const UiRequest &request) {
    const auto &action = request.action;
    if (action == "pbtn_KeyPointEdit")
        return showPanel(PanelId::KeyPointEdit);
    if (action == "pbtn_KeyPointSelect")
        return showPanel(PanelId::KeyPointSelect);
    return PanelPresenter::localAction(request);
}

bool ModelPresenter::prepareRequest(UiRequest &request) {
    if (!m_host || request.action != "pbtn_ModelLoad")
        return true;
    const bool save = false;
    const QString path =
        m_host->chooseFile(save, tr("选择文件"), tr("STL 模型 (*.stl);;所有文件 (*)"));
    if (path.isEmpty())
        return false;
    request.fields["filePath"] = path;
    return true;
}
} // namespace classic
} // namespace ui
} // namespace fosd
