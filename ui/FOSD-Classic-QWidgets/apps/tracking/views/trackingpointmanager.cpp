#include "trackingpointmanager.h"
#include "../../../common/FormBinding.h"
#include "../../../resources/UiResources.h"
#include "ui_trackingpointmanager.h"
namespace fosd { namespace ui { namespace classic {
TrackingManagerView::TrackingManagerView(QWidget *parent) : QWidget(parent), m_ui(new Ui::trackingpointmanager) {
    initializeFosdUiResources();
    m_ui->setupUi(this);
    m_binding = new FormBinding(PanelId::TrackingManager, this, this);
    connect(m_binding, &ViewEvents::actionTriggered, this, &TrackingManagerView::relayControl);
}
TrackingManagerView::~TrackingManagerView() = default;
ViewEvents *TrackingManagerView::events() const { return m_binding; }
QVariantMap TrackingManagerView::values() const { return m_binding->values(); }
void TrackingManagerView::applyFields(const QVariantMap &fields) { m_binding->applyFields(fields); }
void TrackingManagerView::clearSensitiveFields() { m_binding->clearSensitiveFields(); }
void TrackingManagerView::clearInputs() { m_binding->clearInputs(); }
void TrackingManagerView::showStatus(const QString &message) { m_binding->showStatus(message); }
void TrackingManagerView::relayControl(PanelId panel, const QString &action) {
    if (panel != PanelId::TrackingManager) return;
    if (action == "pbtn_ClearAll") { emit pbtn_ClearAllRequested(values()); return; }
    if (action == "pbtn_LoadFile") { emit pbtn_LoadFileRequested(values()); return; }
    if (action == "pbtn_Refresh") { emit pbtn_RefreshRequested(values()); return; }
    if (action == "pbtn_SelectAll") { emit pbtn_SelectAllRequested(values()); return; }
    if (action == "pbtn_SelectNone") { emit pbtn_SelectNoneRequested(values()); return; }
    if (action == "accepted") { emit acceptedRequested(values()); return; }
    if (action == "rejected") { emit rejectedRequested(values()); return; }
}
} } }
