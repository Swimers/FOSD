#include "usermanage.h"
#include "../../../common/FormBinding.h"
#include "../../../resources/UiResources.h"
#include "ui_usermanage.h"
namespace fosd { namespace ui { namespace classic {
AccountView::AccountView(QWidget *parent) : QWidget(parent), m_ui(new Ui::UserManage) {
    initializeFosdUiResources();
    m_ui->setupUi(this);
    m_binding = new FormBinding(PanelId::Account, this, this);
    connect(m_binding, &ViewEvents::actionTriggered, this, &AccountView::relayControl);
}
AccountView::~AccountView() = default;
ViewEvents *AccountView::events() const { return m_binding; }
QVariantMap AccountView::values() const { return m_binding->values(); }
void AccountView::applyFields(const QVariantMap &fields) { m_binding->applyFields(fields); }
void AccountView::clearSensitiveFields() { m_binding->clearSensitiveFields(); }
void AccountView::clearInputs() { m_binding->clearInputs(); }
void AccountView::showStatus(const QString &message) { m_binding->showStatus(message); }
void AccountView::relayControl(PanelId panel, const QString &action) {
    if (panel != PanelId::Account) return;
    if (action == "cbox_MUsername") { emit cbox_MUsernameRequested(values()); return; }
    if (action == "pbtn_UserSet") { emit pbtn_UserSetRequested(values()); return; }
}
} } }
