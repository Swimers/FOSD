#include "PanelPresenter.h"
#include "../apps/mainwindow/contracts/IWorkbenchView.h"
#include <atomic>
namespace fosd {
namespace ui {
namespace classic {
namespace {
std::atomic<quint64> nextRequest{0};
}

PanelPresenter::PanelPresenter(PanelId panel, IView *view, IWorkbenchView *host, QObject *parent)
    : QObject(parent), m_panel(panel), m_view(view), m_host(host) {
    qRegisterMetaType<PanelId>("fosd::ui::classic::PanelId");
    qRegisterMetaType<UiRequest>("fosd::ui::classic::UiRequest");
    qRegisterMetaType<UiResponse>("fosd::ui::classic::UiResponse");
    if (view->events())
        connect(view->events(), &ViewEvents::actionTriggered, this,
                &PanelPresenter::actionTriggered);
}

void PanelPresenter::setBackend(IBackend *backend) {
    if (m_backend == backend)
        return;
    if (m_backend) {
        disconnect(this, nullptr, m_backend, nullptr);
        disconnect(m_backend, nullptr, this, nullptr);
    }
    cancelPending();
    m_backend = backend;
    if (backend) {
        connect(this, &PanelPresenter::requestReady, backend, &IBackend::execute,
                Qt::QueuedConnection);
        connect(backend, &IBackend::completed, this, &PanelPresenter::receiveResponse,
                Qt::QueuedConnection);
        connect(backend, &IBackend::presentationChanged, this, &PanelPresenter::present,
                Qt::QueuedConnection);
        connect(backend, &QObject::destroyed, this, &PanelPresenter::backendDestroyed);
    }
}

void PanelPresenter::cancelPending() {
    m_pending.clear();
    m_view->clearSensitiveFields();
}

void PanelPresenter::actionTriggered(PanelId panel, const QString &action) {
    if (panel == m_panel)
        execute(action);
}

void PanelPresenter::execute(const QString &action) {
    if (action.isEmpty())
        return;
    UiRequest request;
    request.id = ++nextRequest;
    request.panel = m_panel;
    request.action = action;
    request.fields = m_view->values();
    if (localAction(request))
        return;
    m_pending.insert(request.id, action);
    if (!m_backend || !m_backend->supports(m_panel)) {
        UiResponse response;
        response.id = request.id;
        response.panel = m_panel;
        response.message = tr("该功能的后端适配器尚未接入");
        complete(response);
        return;
    }
    if (!prepareRequest(request)) {
        m_pending.remove(request.id);
        return;
    }
    emit requestReady(request);
}

void PanelPresenter::receiveResponse(const UiResponse &response) {
    if (sender() == m_backend.data())
        complete(response);
}

void PanelPresenter::complete(const UiResponse &response) {
    if (response.panel != m_panel || !m_pending.contains(response.id))
        return;
    const auto action = m_pending.take(response.id);
    if (response.success)
        m_view->applyFields(response.fields);
    if (isSensitiveAction(action))
        m_view->clearSensitiveFields();
    const auto message = response.message.isEmpty()
                             ? (response.success ? tr("操作完成") : tr("操作未完成"))
                             : response.message;
    m_view->showStatus(message);
    emit statusChanged(message);
}

void PanelPresenter::present(PanelId panel, const QVariantMap &fields) {
    if (sender() == m_backend.data() && panel == m_panel)
        m_view->applyFields(fields);
}

void PanelPresenter::backendDestroyed() {
    if (m_backend)
        return; // A queued destruction from a replaced adapter is stale.
    cancelPending();
    m_view->showStatus(tr("后端适配器已断开"));
    emit statusChanged(tr("后端适配器已断开"));
}

bool PanelPresenter::showPanel(PanelId panel) {
    if (!m_host)
        return false;
    m_host->showPanel(panel);
    return true;
}

bool PanelPresenter::localAction(const UiRequest &request) {
    if (!m_host)
        return false;
    const auto &action = request.action;
    if (action == "pbtn_Minimize" || action == "btn_Min") {
        m_host->windowAction(m_panel, WindowAction::Minimize);
        return true;
    }
    if (action == "pbtn_Full") {
        m_host->windowAction(m_panel, WindowAction::ToggleFullScreen);
        return true;
    }
    if (action == "pbtn_Maximize" || action == "btn_Close" || action == "pbtn_Exit" ||
        action == "pbtn_ConnectDebugClose" || action == "rejected" ||
        action == "pbtn_SenerCancel") {
        m_host->closePanel(m_panel);
        return true;
    }
    return false;
}

bool PanelPresenter::prepareRequest(UiRequest &) {
    return true;
}

bool PanelPresenter::isSensitiveAction(const QString &) const {
    return false;
}
} // namespace classic
} // namespace ui
} // namespace fosd
