#include "dispdata.h"
#include "../../../common/FormBinding.h"
#include "../../../resources/UiResources.h"
#include "ui_dispdata.h"
namespace fosd { namespace ui { namespace classic {
HardwareDataView::HardwareDataView(QWidget *parent) : QDialog(parent), m_ui(new Ui::dispdata) {
    initializeFosdUiResources();
    m_ui->setupUi(this);
    m_binding = new FormBinding(PanelId::HardwareData, this, this);
    connect(m_binding, &ViewEvents::actionTriggered, this, &HardwareDataView::relayControl);
}
HardwareDataView::~HardwareDataView() = default;
ViewEvents *HardwareDataView::events() const { return m_binding; }
QVariantMap HardwareDataView::values() const { return m_binding->values(); }
void HardwareDataView::applyFields(const QVariantMap &fields) { m_binding->applyFields(fields); }
void HardwareDataView::clearSensitiveFields() { m_binding->clearSensitiveFields(); }
void HardwareDataView::clearInputs() { m_binding->clearInputs(); }
void HardwareDataView::showStatus(const QString &message) { m_binding->showStatus(message); }
void HardwareDataView::relayControl(PanelId panel, const QString &action) {
    if (panel != PanelId::HardwareData) return;
    if (action == "checkBox") { emit checkBoxRequested(values()); return; }
}
} } }
