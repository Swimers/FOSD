#include "ConnectionPresenter.h"
#include "../../IWorkbenchView.h"
#include <QMap>
namespace fosd {
namespace ui {
namespace classic {
ConnectionPresenter::ConnectionPresenter(PanelId panel, IView *view, IWorkbenchView *host,
                                         QObject *parent)
    : PanelPresenter(panel, view, host, parent) {
}

bool ConnectionPresenter::localAction(const UiRequest &request) {
    const auto &action = request.action;
    if (action == "pbtn_Diagnosis")
        return showPanel(PanelId::ConnectionDebug);
    if (action.startsWith("ckbox_CH"))
        return true; // Staged locally until pbtn_SelectCH.
    if (action == "pbtn_SelectAllCH") {
        QVariantMap fields;
        for (int index = 1; index <= 16; ++index)
            fields[QStringLiteral("ckbox_CH%1").arg(index)] = true;
        m_view->applyFields(fields);
        return true;
    }
    return PanelPresenter::localAction(request);
}

bool ConnectionPresenter::prepareRequest(UiRequest &request) {
    if (!m_host)
        return true;
    const bool recording = request.action == "pbtn_SaveDynamic";
    if (request.action != "pbtn_SaveStatic" && !recording)
        return true;
    if (recording && !request.fields.value("pbtn_SaveDynamic").toBool())
        return true;
    const auto path =
        m_host->chooseFile(true, tr("选择存储文件"), tr("CSV (*.csv);;SQLite (*.sqlite *.db)"));
    if (path.isEmpty()) {
        if (recording)
            m_view->applyFields({{"pbtn_SaveDynamic", false}});
        return false;
    }
    request.fields["filePath"] = path;
    return true;
}
} // namespace classic
} // namespace ui
} // namespace fosd
