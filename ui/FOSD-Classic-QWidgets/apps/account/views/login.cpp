#include "login.h"
#include "../../../common/FormBinding.h"
#include "../../../resources/UiResources.h"
#include "ui_login.h"
namespace fosd {
namespace ui {
namespace classic {
LoginView::LoginView(QWidget *parent) : QMainWindow(parent), m_ui(new Ui::loginwindow) {
    initializeFosdUiResources();
    m_ui->setupUi(this);
    m_binding = new FormBinding(PanelId::Login, this, this);
    connect(m_binding, &ViewEvents::actionTriggered, this, &LoginView::relayControl);
}

LoginView::~LoginView() = default;
ViewEvents *LoginView::events() const {
    return m_binding;
}

QVariantMap LoginView::values() const {
    return m_binding->values();
}

void LoginView::applyFields(const QVariantMap &fields) {
    m_binding->applyFields(fields);
}

void LoginView::clearSensitiveFields() {
    m_binding->clearSensitiveFields();
}

void LoginView::clearInputs() {
    m_binding->clearInputs();
}

void LoginView::showStatus(const QString &message) {
    m_binding->showStatus(message);
}

void LoginView::relayControl(PanelId panel, const QString &action) {
    if (panel != PanelId::Login)
        return;
    if (action == "btn_Clear") {
        emit btn_ClearRequested(values());
        return;
    }
    if (action == "btn_Close") {
        emit btn_CloseRequested(values());
        return;
    }
    if (action == "btn_Forget") {
        emit btn_ForgetRequested(values());
        return;
    }
    if (action == "btn_Login") {
        emit btn_LoginRequested(values());
        return;
    }
    if (action == "btn_Min") {
        emit btn_MinRequested(values());
        return;
    }
    if (action == "btn_Register") {
        emit btn_RegisterRequested(values());
        return;
    }
    if (action == "cbox_Privilege") {
        emit cbox_PrivilegeRequested(values());
        return;
    }
    if (action == "check_RemName") {
        emit check_RemNameRequested(values());
        return;
    }
    if (action == "check_RemPass") {
        emit check_RemPassRequested(values());
        return;
    }
    if (action == "comboBox") {
        emit comboBoxRequested(values());
        return;
    }
}
} // namespace classic
} // namespace ui
} // namespace fosd
