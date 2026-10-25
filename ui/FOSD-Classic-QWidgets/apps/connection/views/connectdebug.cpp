#include "connectdebug.h"
#include "../../../common/FormBinding.h"
#include "../../../resources/UiResources.h"
#include "ui_connectdebug.h"
namespace fosd { namespace ui { namespace classic {
ConnectionDebugView::ConnectionDebugView(QWidget *parent) : QWidget(parent), m_ui(new Ui::connectdebug) {
    initializeFosdUiResources();
    m_ui->setupUi(this);
    m_binding = new FormBinding(PanelId::ConnectionDebug, this, this);
    connect(m_binding, &ViewEvents::actionTriggered, this, &ConnectionDebugView::relayControl);
}
ConnectionDebugView::~ConnectionDebugView() = default;
ViewEvents *ConnectionDebugView::events() const { return m_binding; }
QVariantMap ConnectionDebugView::values() const { return m_binding->values(); }
void ConnectionDebugView::applyFields(const QVariantMap &fields) { m_binding->applyFields(fields); }
void ConnectionDebugView::clearSensitiveFields() { m_binding->clearSensitiveFields(); }
void ConnectionDebugView::clearInputs() { m_binding->clearInputs(); }
void ConnectionDebugView::showStatus(const QString &message) { m_binding->showStatus(message); }
void ConnectionDebugView::relayControl(PanelId panel, const QString &action) {
    if (panel != PanelId::ConnectionDebug) return;
    if (action == "pbtn_ConnectDebugClose") { emit pbtn_ConnectDebugCloseRequested(values()); return; }
    if (action == "pbtn_ConnectDebugStart") { emit pbtn_ConnectDebugStartRequested(values()); return; }
}
} } }
