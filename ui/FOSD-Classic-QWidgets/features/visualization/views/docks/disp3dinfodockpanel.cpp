#include "FOSD-Classic-QWidgets/features/visualization/views/docks/disp3dinfodockpanel.h"
#include "FOSD-Classic-QWidgets/common/FormBinding.h"
#include "FOSD-Classic-QWidgets/common/UiResources.h"
#include "ui_disp3dinfodockpanel.h"
namespace fosd {
namespace ui {
namespace classic {
InfoPanelView::InfoPanelView(QWidget *parent) : QWidget(parent), m_ui(new Ui::Disp3DInfoDockPanel) {
    initializeFosdUiResources();
    m_ui->setupUi(this);
    m_binding = new FormBinding(PanelId::InfoPanel, this, this);
}

InfoPanelView::~InfoPanelView() = default;
ViewEvents *InfoPanelView::events() const {
    return m_binding;
}

QVariantMap InfoPanelView::values() const {
    return m_binding->values();
}

void InfoPanelView::applyFields(const QVariantMap &fields) {
    m_binding->applyFields(fields);

}

void InfoPanelView::clearSensitiveFields() {
    m_binding->clearSensitiveFields();
}

void InfoPanelView::showStatus(const QString &message) {
    m_binding->showStatus(message);
}

} // namespace classic
} // namespace ui
} // namespace fosd
