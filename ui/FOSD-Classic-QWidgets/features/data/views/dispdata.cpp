#include "FOSD-Classic-QWidgets/features/data/views/dispdata.h"
#include "FOSD-Classic-QWidgets/common/FormBinding.h"
#include "FOSD-Classic-QWidgets/common/UiResources.h"
#include "ui_dispdata.h"
namespace fosd {
namespace ui {
namespace classic {
HardwareDataView::HardwareDataView(QWidget *parent) : QDialog(parent), m_ui(new Ui::dispdata) {
    initializeFosdUiResources();
    m_ui->setupUi(this);
    m_binding = new FormBinding(PanelId::HardwareData, this, this);
}

HardwareDataView::~HardwareDataView() = default;
ViewEvents *HardwareDataView::events() const {
    return m_binding;
}

QVariantMap HardwareDataView::values() const {
    return m_binding->values();
}

void HardwareDataView::applyFields(const QVariantMap &fields) {
    m_binding->applyFields(fields);
}

void HardwareDataView::clearSensitiveFields() {
    m_binding->clearSensitiveFields();
}

void HardwareDataView::showStatus(const QString &message) {
    m_binding->showStatus(message);
}

} // namespace classic
} // namespace ui
} // namespace fosd
