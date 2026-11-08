#include "FOSD-Classic-QWidgets/common/PanelPresenter.h"
#include "FOSD-Classic-QWidgets/application/IWorkbenchView.h"
#include <QWidget>
#include <QAction>
namespace fosd {
namespace ui {
namespace classic {
PanelPresenter::PanelPresenter(PanelId panel, IView *view, IWorkbenchView *host, QObject *parent)
    : QObject(parent), m_panel(panel), m_view(view), m_host(host) {
    if (view->events())
        connect(view->events(), &ViewEvents::actionTriggered, this,
                [this](PanelId p, const QString &action) {
                    if (p == m_panel)
                        execute(action);
                });
}
void PanelPresenter::setModel(fosd::application::WorkbenchModel *model) {
    if (m_model == model)
        return;
    if (m_model)
        disconnect(m_model, nullptr, this, nullptr);
    m_model = model;
    if (model) {
        connect(model, &fosd::application::WorkbenchModel::stateChanged, this,
                &PanelPresenter::refresh);
        bindModel();
    }
}
void PanelPresenter::initialize() {
    refresh();
}
void PanelPresenter::present(PanelId panel, const QVariantMap &fields) {
    auto *view = m_host ? m_host->panelView(panel) : (panel == m_panel ? m_view : nullptr);
    if (view)
        view->applyFields(fields);
}
QString PanelPresenter::text(const QVariantMap &f, const QString &n) {
    auto v = f.value(n);
    return v.type() == QVariant::Map ? v.toMap().value("text").toString() : v.toString();
}
int PanelPresenter::index(const QVariantMap &f, const QString &n) {
    return f.value(n).toMap().value("index", -1).toInt();
}
void PanelPresenter::execute(const QString &action) {
    if (action.isEmpty())
        return;
    if (auto *root = dynamic_cast<QWidget *>(m_view)) {
        if (!root->isEnabled())
            return;
        auto *source = root->findChild<QObject *>(action);
        if (auto *w = qobject_cast<QWidget *>(source)) {
            if (!w->isEnabled())
                return;
        } else if (auto *a = qobject_cast<QAction *>(source)) {
            if (!a->isEnabled())
                return;
        }
    }
    QVariantMap fields = m_view->values();
    if (localAction(action, fields))
        return;
    if (!m_model) {
        const auto message = tr("业务服务尚未接入");
        m_view->showStatus(message);
        emit statusChanged(message);
        emit actionFinished(action, false, message);
        return;
    }
    if (!prepareAction(action, fields))
        return;
    QString message = tr("请求已提交");
    QVariantMap updates;
    const bool success = handleAction(action, fields, message, updates);
    if (!updates.isEmpty())
        m_view->applyFields(updates);
    m_view->showStatus(success ? message : tr("操作失败：%1").arg(message));
    emit statusChanged(message);
    emit actionFinished(action, success, message);
}
bool PanelPresenter::showPanel(PanelId panel) {
    if (!m_host)
        return false;
    m_host->showPanel(panel);
    return true;
}
bool PanelPresenter::localAction(const QString &action, const QVariantMap &fields) {
    if (!m_host)
        return false;
    Q_UNUSED(fields)
    if (action == "pbtn_Minimize" || action == "btn_Min") {
        m_host->windowAction(m_panel, WindowAction::Minimize);
        return true;
    }
    if (action == "pbtn_Full") {
        m_host->windowAction(m_panel, WindowAction::ToggleFullScreen);
        return true;
    }
    if (action == "pbtn_Maximize" || action == "btn_Close" || action == "pbtn_Exit" ||
        action == "rejected" || action == "pbtn_SenerCancel") {
        m_host->closePanel(m_panel);
        return true;
    }
    return false;
}
} // namespace classic
} // namespace ui
} // namespace fosd
