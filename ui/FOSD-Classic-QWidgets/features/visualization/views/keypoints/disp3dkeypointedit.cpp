#include "FOSD-Classic-QWidgets/features/visualization/views/keypoints/disp3dkeypointedit.h"
#include "FOSD-Classic-QWidgets/common/FormBinding.h"
#include "FOSD-Classic-QWidgets/common/UiResources.h"
#include "ui_disp3dkeypointedit.h"
namespace fosd {
namespace ui {
namespace classic {
KeyPointEditView::KeyPointEditView(QWidget *parent)
    : QWidget(parent), m_ui(new Ui::disp3dkeypointedit) {
    initializeFosdUiResources();
    m_ui->setupUi(this);
    m_binding = new FormBinding(PanelId::KeyPointEdit, this, this);
}

KeyPointEditView::~KeyPointEditView() = default;
ViewEvents *KeyPointEditView::events() const {
    return m_binding;
}

QVariantMap KeyPointEditView::values() const {
    return m_binding->values();
}

void KeyPointEditView::applyFields(const QVariantMap &fields) {
    m_binding->applyFields(fields);
}

void KeyPointEditView::clearSensitiveFields() {
    m_binding->clearSensitiveFields();
}

void KeyPointEditView::showStatus(const QString &message) {
    m_binding->showStatus(message);
}

} // namespace classic
} // namespace ui
} // namespace fosd
