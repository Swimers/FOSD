#include "FOSD-Classic-QWidgets/features/visualization/views/docks/disp3dmodeldockpanel.h"
#include "FOSD-Classic-QWidgets/common/FormBinding.h"
#include "FOSD-Classic-QWidgets/common/UiResources.h"
#include "ui_disp3dmodeldockpanel.h"
namespace fosd {
namespace ui {
namespace classic {
ModelPanelView::ModelPanelView(QWidget *parent)
    : QWidget(parent), m_ui(new Ui::Disp3DModelDockPanel) {
    initializeFosdUiResources();
    m_ui->setupUi(this);
    m_binding = new FormBinding(PanelId::ModelPanel, this, this);
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

void ModelPanelView::showStatus(const QString &message) {
    m_binding->showStatus(message);
}

} // namespace classic
} // namespace ui
} // namespace fosd
