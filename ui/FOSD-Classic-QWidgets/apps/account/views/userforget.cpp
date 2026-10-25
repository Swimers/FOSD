#include "userforget.h"
#include "../../../common/FormBinding.h"
#include "../../../resources/UiResources.h"
#include "ui_userforget.h"
namespace fosd {
namespace ui {
namespace classic {
RecoveryView::RecoveryView(QWidget *parent) : QWidget(parent), m_ui(new Ui::UserForget) {
    initializeFosdUiResources();
    m_ui->setupUi(this);
    m_binding = new FormBinding(PanelId::Recovery, this, this);
    connect(m_binding, &ViewEvents::actionTriggered, this, &RecoveryView::relayControl);
}

RecoveryView::~RecoveryView() = default;
ViewEvents *RecoveryView::events() const {
    return m_binding;
}

QVariantMap RecoveryView::values() const {
    return m_binding->values();
}

void RecoveryView::applyFields(const QVariantMap &fields) {
    m_binding->applyFields(fields);
}

void RecoveryView::clearSensitiveFields() {
    m_binding->clearSensitiveFields();
}

void RecoveryView::clearInputs() {
    m_binding->clearInputs();
}

void RecoveryView::showStatus(const QString &message) {
    m_binding->showStatus(message);
}

void RecoveryView::relayControl(PanelId panel, const QString &action) {
    if (panel != PanelId::Recovery)
        return;
    if (action == "cbox_SelectPri") {
        emit cbox_SelectPriRequested(values());
        return;
    }
    if (action == "pbtn_ForgetClear") {
        emit pbtn_ForgetClearRequested(values());
        return;
    }
    if (action == "pbtn_ForgetSet") {
        emit pbtn_ForgetSetRequested(values());
        return;
    }
}
} // namespace classic
} // namespace ui
} // namespace fosd
