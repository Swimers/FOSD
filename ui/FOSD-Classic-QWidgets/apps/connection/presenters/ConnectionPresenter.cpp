#include "ConnectionPresenter.h"
#include "../../mainwindow/contracts/IWorkbenchView.h"
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
    const QMap<QString, QString> clears{{"pbtn_SerialClear", "ptedit_SerialRec"},
                                        {"pbtn_UdpClear", "ptedit_UdpRec"},
                                        {"pbtn_TcpClear", "ptedit_TcpRec"}};
    if (clears.contains(action)) {
        m_view->applyFields({{clears[action], QString()}});
        return true;
    }
    if (action == "pbtn_SelectAllCH") {
        QVariantMap fields;
        for (int index = 1; index <= 16; ++index)
            fields[QStringLiteral("ckbox_CH%1").arg(index)] = true;
        m_view->applyFields(fields);
        return true;
    }
    return PanelPresenter::localAction(request);
}
} // namespace classic
} // namespace ui
} // namespace fosd
