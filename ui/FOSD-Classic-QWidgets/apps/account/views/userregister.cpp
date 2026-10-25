#include "userregister.h"
#include "../../../common/FormBinding.h"
#include "../../../resources/UiResources.h"
#include "ui_userregister.h"
namespace fosd {
namespace ui {
namespace classic {
RegistrationView::RegistrationView(QWidget *parent) : QWidget(parent), m_ui(new Ui::UserRegister) {
    initializeFosdUiResources();
    m_ui->setupUi(this);
    m_binding = new FormBinding(PanelId::Registration, this, this);
    connect(m_binding, &ViewEvents::actionTriggered, this, &RegistrationView::relayControl);
}

RegistrationView::~RegistrationView() = default;
ViewEvents *RegistrationView::events() const {
    return m_binding;
}

QVariantMap RegistrationView::values() const {
    return m_binding->values();
}

void RegistrationView::applyFields(const QVariantMap &fields) {
    m_binding->applyFields(fields);
}

void RegistrationView::clearSensitiveFields() {
    m_binding->clearSensitiveFields();
}

void RegistrationView::clearInputs() {
    m_binding->clearInputs();
}

void RegistrationView::showStatus(const QString &message) {
    m_binding->showStatus(message);
}

void RegistrationView::relayControl(PanelId panel, const QString &action) {
    if (panel != PanelId::Registration)
        return;
    if (action == "cbox_SelectNewPri") {
        emit cbox_SelectNewPriRequested(values());
        return;
    }
    if (action == "pbtn_RegisterClear") {
        emit pbtn_RegisterClearRequested(values());
        return;
    }
    if (action == "pbtn_RegisterSet") {
        emit pbtn_RegisterSetRequested(values());
        return;
    }
}
} // namespace classic
} // namespace ui
} // namespace fosd
