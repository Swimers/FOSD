#include "trackingpointdatatable.h"
#include "../../../common/FormBinding.h"
#include "../../../resources/UiResources.h"
#include "ui_trackingpointdatatable.h"
namespace fosd { namespace ui { namespace classic {
TrackingDataView::TrackingDataView(QWidget *parent) : QWidget(parent), m_ui(new Ui::trackingpointdatatable) {
    initializeFosdUiResources();
    m_ui->setupUi(this);
    m_binding = new FormBinding(PanelId::TrackingData, this, this);
    connect(m_binding, &ViewEvents::actionTriggered, this, &TrackingDataView::relayControl);
}
TrackingDataView::~TrackingDataView() = default;
ViewEvents *TrackingDataView::events() const { return m_binding; }
QVariantMap TrackingDataView::values() const { return m_binding->values(); }
void TrackingDataView::applyFields(const QVariantMap &fields) { m_binding->applyFields(fields); }
void TrackingDataView::clearSensitiveFields() { m_binding->clearSensitiveFields(); }
void TrackingDataView::clearInputs() { m_binding->clearInputs(); }
void TrackingDataView::showStatus(const QString &message) { m_binding->showStatus(message); }
void TrackingDataView::relayControl(PanelId panel, const QString &action) {
    if (panel != PanelId::TrackingData) return;
    if (action == "cbox_AutoRefresh") { emit cbox_AutoRefreshRequested(values()); return; }
    if (action == "pbtn_HideAll") { emit pbtn_HideAllRequested(values()); return; }
    if (action == "pbtn_Refresh") { emit pbtn_RefreshRequested(values()); return; }
    if (action == "pbtn_ShowAll") { emit pbtn_ShowAllRequested(values()); return; }
    if (action == "pbtn_ShowAllRows") { emit pbtn_ShowAllRowsRequested(values()); return; }
    if (action == "pbtn_ShowSelected") { emit pbtn_ShowSelectedRequested(values()); return; }
    if (action == "accepted") { emit acceptedRequested(values()); return; }
    if (action == "rejected") { emit rejectedRequested(values()); return; }
}
} } }
