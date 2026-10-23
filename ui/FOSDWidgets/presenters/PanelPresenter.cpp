#include "PanelPresenter.h"
#include <atomic>
namespace fosd { namespace widgets {
namespace { std::atomic<quint64> nextRequest{0}; }
PanelPresenter::PanelPresenter(PanelId panel, IPanelView *view, QObject *parent)
    : QObject(parent), m_panel(panel), m_view(view) {}
void PanelPresenter::execute(const QString &action) {
    if (action.isEmpty()) return;
    UiRequest request; request.id = ++nextRequest; request.panel = m_panel;
    request.action = action; request.fields = m_view->values();
    m_pending.insert(request.id, action);
    emit requestReady(request);
}
void PanelPresenter::finishLocal(quint64 id) { m_pending.remove(id); }
void PanelPresenter::cancelPending() { m_pending.clear(); m_view->clearSensitiveFields(); }
void PanelPresenter::complete(const UiResponse &response) {
    if (response.panel != m_panel || !m_pending.contains(response.id)) return;
    const auto action = m_pending.take(response.id);
    if (response.success) m_view->applyFields(response.fields);
    if (action == "btn_Login" || action == "pbtn_RegisterSet" || action == "pbtn_ForgetSet" || action == "pbtn_UserSet")
        m_view->clearSensitiveFields();
    const auto message = response.message.isEmpty()
        ? (response.success ? tr("操作完成") : tr("操作未完成")) : response.message;
    m_view->showStatus(message);
    emit statusChanged(message);
}
} }
