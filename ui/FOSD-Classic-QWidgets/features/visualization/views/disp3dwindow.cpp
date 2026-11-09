#include "FOSD-Classic-QWidgets/features/visualization/views/disp3dwindow.h"
#include "FOSD-Classic-QWidgets/common/FormBinding.h"
#include "FOSD-Classic-QWidgets/common/UiResources.h"
#include "ui_disp3dwindow.h"
#include "FOSD-Classic-QWidgets/common/CoordinateText.h"
namespace fosd {
namespace ui {
namespace classic {
VisualizationView::VisualizationView(QWidget *parent)
    : QMainWindow(parent), m_ui(new Ui::disp3dwindow) {
    initializeFosdUiResources();
    m_ui->setupUi(this);
    m_ui->statusBar_SelectedPoint->showMessage(selectedPointStatus());
    m_binding = new FormBinding(PanelId::Visualization, this, this);
}

VisualizationView::~VisualizationView() = default;
ViewEvents *VisualizationView::events() const {
    return m_binding;
}

QVariantMap VisualizationView::values() const {
    return m_binding->values();
}

void VisualizationView::applyFields(const QVariantMap &fields) {
    m_binding->applyFields(fields);
}

void VisualizationView::clearSensitiveFields() {
    m_binding->clearSensitiveFields();
}

void VisualizationView::showStatus(const QString &message) {
    m_binding->showStatus(message);
}

} // namespace classic
} // namespace ui
} // namespace fosd
