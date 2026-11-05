#include "disp3dviewdockpanel.h"
#include "../../../../common/FormBinding.h"
#include "../../../../resources/UiResources.h"
#include "ui_disp3dviewdockpanel.h"
namespace fosd {
namespace ui {
namespace classic {
ScenePanelView::ScenePanelView(QWidget *parent)
    : QWidget(parent), m_ui(new Ui::Disp3DViewDockPanel) {
    initializeFosdUiResources();
    m_ui->setupUi(this);
    m_binding = new FormBinding(PanelId::ScenePanel, this, this);
    connect(m_binding, &ViewEvents::actionTriggered, this, &ScenePanelView::relayControl);
}

ScenePanelView::~ScenePanelView() = default;
ViewEvents *ScenePanelView::events() const {
    return m_binding;
}

QVariantMap ScenePanelView::values() const {
    return m_binding->values();
}

void ScenePanelView::applyFields(const QVariantMap &fields) {
    m_binding->applyFields(fields);
}

void ScenePanelView::clearSensitiveFields() {
    m_binding->clearSensitiveFields();
}

void ScenePanelView::clearInputs() {
    m_binding->clearInputs();
}

void ScenePanelView::showStatus(const QString &message) {
    m_binding->showStatus(message);
}

void ScenePanelView::relayControl(PanelId panel, const QString &action) {
    if (panel != PanelId::ScenePanel)
        return;
    if (action == "cbox_AxisAble") {
        emit cbox_AxisAbleRequested(values());
        return;
    }
    if (action == "cbox_CareamLockAble") {
        emit cbox_CareamLockAbleRequested(values());
        return;
    }
    if (action == "cbox_GridAble") {
        emit cbox_GridAbleRequested(values());
        return;
    }
    if (action == "cbox_SelectView") {
        emit cbox_SelectViewRequested(values());
        return;
    }
    if (action == "cbox_XOYGrid") {
        emit cbox_XOYGridRequested(values());
        return;
    }
    if (action == "cbox_XOZGrid") {
        emit cbox_XOZGridRequested(values());
        return;
    }
    if (action == "cbox_ZOYGrid") {
        emit cbox_ZOYGridRequested(values());
        return;
    }
    if (action == "hsd_AxisSize") {
        emit hsd_AxisSizeRequested(values());
        return;
    }
    if (action == "hsd_GridSize") {
        emit hsd_GridSizeRequested(values());
        return;
    }
    if (action == "pbtn_SwitchView") {
        emit pbtn_SwitchViewRequested(values());
        return;
    }
}
} // namespace classic
} // namespace ui
} // namespace fosd
