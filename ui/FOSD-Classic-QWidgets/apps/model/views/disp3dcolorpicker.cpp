#include "disp3dcolorpicker.h"
#include "../../../common/FormBinding.h"
#include "../../../resources/UiResources.h"
#include "ui_disp3dcolorpicker.h"
namespace fosd { namespace ui { namespace classic {
ColorPickerView::ColorPickerView(QWidget *parent) : QDialog(parent), m_ui(new Ui::Disp3DColorPicker) {
    initializeFosdUiResources();
    m_ui->setupUi(this);
    m_binding = new FormBinding(PanelId::ColorPicker, this, this);
    connect(m_binding, &ViewEvents::actionTriggered, this, &ColorPickerView::relayControl);
}
ColorPickerView::~ColorPickerView() = default;
ViewEvents *ColorPickerView::events() const { return m_binding; }
QVariantMap ColorPickerView::values() const { return m_binding->values(); }
void ColorPickerView::applyFields(const QVariantMap &fields) { m_binding->applyFields(fields); }
void ColorPickerView::clearSensitiveFields() { m_binding->clearSensitiveFields(); }
void ColorPickerView::clearInputs() { m_binding->clearInputs(); }
void ColorPickerView::showStatus(const QString &message) { m_binding->showStatus(message); }
void ColorPickerView::relayControl(PanelId panel, const QString &action) {
    if (panel != PanelId::ColorPicker) return;
    if (action == "btnPick") { emit btnPickRequested(values()); return; }
    if (action == "sliderB") { emit sliderBRequested(values()); return; }
    if (action == "sliderG") { emit sliderGRequested(values()); return; }
    if (action == "sliderR") { emit sliderRRequested(values()); return; }
    if (action == "spinB") { emit spinBRequested(values()); return; }
    if (action == "spinG") { emit spinGRequested(values()); return; }
    if (action == "spinR") { emit spinRRequested(values()); return; }
    if (action == "accepted") { emit acceptedRequested(values()); return; }
    if (action == "rejected") { emit rejectedRequested(values()); return; }
}
} } }
