#include "disp2dpara.h"
#include "../../../common/FormBinding.h"
#include "../../../resources/UiResources.h"
#include "ui_disp2dpara.h"
namespace fosd { namespace ui { namespace classic {
PlotSettingsView::PlotSettingsView(QWidget *parent) : QWidget(parent), m_ui(new Ui::disp2dpara) {
    initializeFosdUiResources();
    m_ui->setupUi(this);
    m_binding = new FormBinding(PanelId::PlotSettings, this, this);
    connect(m_binding, &ViewEvents::actionTriggered, this, &PlotSettingsView::relayControl);
}
PlotSettingsView::~PlotSettingsView() = default;
ViewEvents *PlotSettingsView::events() const { return m_binding; }
QVariantMap PlotSettingsView::values() const { return m_binding->values(); }
void PlotSettingsView::applyFields(const QVariantMap &fields) { m_binding->applyFields(fields); }
void PlotSettingsView::clearSensitiveFields() { m_binding->clearSensitiveFields(); }
void PlotSettingsView::clearInputs() { m_binding->clearInputs(); }
void PlotSettingsView::showStatus(const QString &message) { m_binding->showStatus(message); }
void PlotSettingsView::relayControl(PanelId panel, const QString &action) {
    if (panel != PanelId::PlotSettings) return;
    if (action == "pbtn_SenerCancel") { emit pbtn_SenerCancelRequested(values()); return; }
    if (action == "pbtn_SenerSet") { emit pbtn_SenerSetRequested(values()); return; }
}
} } }
