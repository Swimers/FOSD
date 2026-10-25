#include "disp3dinfodockpanel.h"
#include "../../../../common/FormBinding.h"
#include "../../../../resources/UiResources.h"
#include "ui_disp3dinfodockpanel.h"
namespace fosd { namespace ui { namespace classic {
InfoPanelView::InfoPanelView(QWidget *parent) : QWidget(parent), m_ui(new Ui::Disp3DInfoDockPanel) {
    initializeFosdUiResources();
    m_ui->setupUi(this);
    m_binding = new FormBinding(PanelId::InfoPanel, this, this);
    connect(m_binding, &ViewEvents::actionTriggered, this, &InfoPanelView::relayControl);
}
InfoPanelView::~InfoPanelView() = default;
ViewEvents *InfoPanelView::events() const { return m_binding; }
QVariantMap InfoPanelView::values() const { return m_binding->values(); }
void InfoPanelView::applyFields(const QVariantMap &fields) { m_binding->applyFields(fields); }
void InfoPanelView::clearSensitiveFields() { m_binding->clearSensitiveFields(); }
void InfoPanelView::clearInputs() { m_binding->clearInputs(); }
void InfoPanelView::showStatus(const QString &message) { m_binding->showStatus(message); }
void InfoPanelView::relayControl(PanelId panel, const QString &action) {
    if (panel != PanelId::InfoPanel) return;
    if (action == "pushButton_6") { emit pushButton_6Requested(values()); return; }
    if (action == "radioButton") { emit radioButtonRequested(values()); return; }
    if (action == "radioButton_2") { emit radioButton_2Requested(values()); return; }
    if (action == "radioButton_3") { emit radioButton_3Requested(values()); return; }
    if (action == "radioButton_4") { emit radioButton_4Requested(values()); return; }
}
} } }
