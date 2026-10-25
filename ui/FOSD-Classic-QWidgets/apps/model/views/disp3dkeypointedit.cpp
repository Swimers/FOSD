#include "disp3dkeypointedit.h"
#include "../../../common/FormBinding.h"
#include "../../../resources/UiResources.h"
#include "ui_disp3dkeypointedit.h"
namespace fosd { namespace ui { namespace classic {
KeyPointEditView::KeyPointEditView(QWidget *parent) : QWidget(parent), m_ui(new Ui::disp3dkeypointedit) {
    initializeFosdUiResources();
    m_ui->setupUi(this);
    m_binding = new FormBinding(PanelId::KeyPointEdit, this, this);
    connect(m_binding, &ViewEvents::actionTriggered, this, &KeyPointEditView::relayControl);
}
KeyPointEditView::~KeyPointEditView() = default;
ViewEvents *KeyPointEditView::events() const { return m_binding; }
QVariantMap KeyPointEditView::values() const { return m_binding->values(); }
void KeyPointEditView::applyFields(const QVariantMap &fields) { m_binding->applyFields(fields); }
void KeyPointEditView::clearSensitiveFields() { m_binding->clearSensitiveFields(); }
void KeyPointEditView::clearInputs() { m_binding->clearInputs(); }
void KeyPointEditView::showStatus(const QString &message) { m_binding->showStatus(message); }
void KeyPointEditView::relayControl(PanelId panel, const QString &action) {
    if (panel != PanelId::KeyPointEdit) return;
    if (action == "cbox_IDKeyPoint") { emit cbox_IDKeyPointRequested(values()); return; }
    if (action == "pbtn_ClearAll") { emit pbtn_ClearAllRequested(values()); return; }
    if (action == "pbtn_ClearCube") { emit pbtn_ClearCubeRequested(values()); return; }
    if (action == "pbtn_DispCube") { emit pbtn_DispCubeRequested(values()); return; }
    if (action == "pbtn_FindKeyPoint") { emit pbtn_FindKeyPointRequested(values()); return; }
    if (action == "pbtn_SelectAll") { emit pbtn_SelectAllRequested(values()); return; }
    if (action == "pbtn_SelectCube") { emit pbtn_SelectCubeRequested(values()); return; }
    if (action == "pushButton") { emit pushButtonRequested(values()); return; }
}
} } }
