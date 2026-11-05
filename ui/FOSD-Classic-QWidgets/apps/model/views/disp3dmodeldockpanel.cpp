#include "disp3dmodeldockpanel.h"
#include "../../../common/FormBinding.h"
#include "../../../resources/UiResources.h"
#include "ui_disp3dmodeldockpanel.h"
namespace fosd {
namespace ui {
namespace classic {
ModelPanelView::ModelPanelView(QWidget *parent)
    : QWidget(parent), m_ui(new Ui::Disp3DModelDockPanel) {
    initializeFosdUiResources();
    m_ui->setupUi(this);
    m_binding = new FormBinding(PanelId::ModelPanel, this, this);
    connect(m_binding, &ViewEvents::actionTriggered, this, &ModelPanelView::relayControl);
}

ModelPanelView::~ModelPanelView() = default;
ViewEvents *ModelPanelView::events() const {
    return m_binding;
}

QVariantMap ModelPanelView::values() const {
    return m_binding->values();
}

void ModelPanelView::applyFields(const QVariantMap &fields) {
    m_binding->applyFields(fields);
}

void ModelPanelView::clearSensitiveFields() {
    m_binding->clearSensitiveFields();
}

void ModelPanelView::clearInputs() {
    m_binding->clearInputs();
}

void ModelPanelView::showStatus(const QString &message) {
    m_binding->showStatus(message);
}

void ModelPanelView::relayControl(PanelId panel, const QString &action) {
    if (panel != PanelId::ModelPanel)
        return;
    if (action == "cbox_FPS") {
        emit cbox_FPSRequested(values());
        return;
    }
    if (action == "cbox_ScalarSelect") {
        emit cbox_ScalarSelectRequested(values());
        return;
    }
    if (action == "pbtn_KeyPointEdit") {
        emit pbtn_KeyPointEditRequested(values());
        return;
    }
    if (action == "pbtn_KeyPointSelect") {
        emit pbtn_KeyPointSelectRequested(values());
        return;
    }
    if (action == "pbtn_ModelClear") {
        emit pbtn_ModelClearRequested(values());
        return;
    }
    if (action == "pbtn_ModelLoad") {
        emit pbtn_ModelLoadRequested(values());
        return;
    }
    if (action == "pbtn_Normalize") {
        emit pbtn_NormalizeRequested(values());
        return;
    }
    if (action == "pbtn_ScalarClear") {
        emit pbtn_ScalarClearRequested(values());
        return;
    }
    if (action == "pbtn_ScalarSet") {
        emit pbtn_ScalarSetRequested(values());
        return;
    }
}
} // namespace classic
} // namespace ui
} // namespace fosd
