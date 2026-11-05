#include "disp3dkeypointselect.h"
#include "../../../common/FormBinding.h"
#include "../../../resources/UiResources.h"
#include "ui_disp3dkeypointselect.h"
namespace fosd {
namespace ui {
namespace classic {
KeyPointSelectView::KeyPointSelectView(QWidget *parent)
    : QWidget(parent), m_ui(new Ui::disp3dkeypointselect) {
    initializeFosdUiResources();
    m_ui->setupUi(this);
    m_binding = new FormBinding(PanelId::KeyPointSelect, this, this);
    connect(m_binding, &ViewEvents::actionTriggered, this, &KeyPointSelectView::relayControl);
}

KeyPointSelectView::~KeyPointSelectView() = default;
ViewEvents *KeyPointSelectView::events() const {
    return m_binding;
}

QVariantMap KeyPointSelectView::values() const {
    return m_binding->values();
}

void KeyPointSelectView::applyFields(const QVariantMap &fields) {
    m_binding->applyFields(fields);
}

void KeyPointSelectView::clearSensitiveFields() {
    m_binding->clearSensitiveFields();
}

void KeyPointSelectView::clearInputs() {
    m_binding->clearInputs();
}

void KeyPointSelectView::showStatus(const QString &message) {
    m_binding->showStatus(message);
}

void KeyPointSelectView::relayControl(PanelId panel, const QString &action) {
    if (panel != PanelId::KeyPointSelect)
        return;
    if (action == "cbox_KeyPointID") {
        emit cbox_KeyPointIDRequested(values());
        return;
    }
    if (action == "pbtn_AddPoint") {
        emit pbtn_AddPointRequested(values());
        return;
    }
    if (action == "pbtn_DeletePoint") {
        emit pbtn_DeletePointRequested(values());
        return;
    }
    if (action == "pbtn_Exit") {
        emit pbtn_ExitRequested(values());
        return;
    }
    if (action == "radioButton") {
        emit radioButtonRequested(values());
        return;
    }
    if (action == "radioButton_2") {
        emit radioButton_2Requested(values());
        return;
    }
    if (action == "radioButton_3") {
        emit radioButton_3Requested(values());
        return;
    }
}
} // namespace classic
} // namespace ui
} // namespace fosd
