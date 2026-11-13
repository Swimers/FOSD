#include "FOSD-Classic-QWidgets/features/visualization/views/docks/disp3dviewdockpanel.h"
#include "FOSD-Classic-QWidgets/common/FormBinding.h"
#include "FOSD-Classic-QWidgets/common/UiResources.h"
#include "ui_disp3dviewdockpanel.h"
#include <QColor>
namespace fosd {
namespace ui {
namespace classic {
ScenePanelView::ScenePanelView(QWidget *parent)
    : QWidget(parent), m_ui(new Ui::Disp3DViewDockPanel) {
    initializeFosdUiResources();
    m_ui->setupUi(this);
    m_binding = new FormBinding(PanelId::ScenePanel, this, this);
}

ScenePanelView::~ScenePanelView() = default;
ViewEvents *ScenePanelView::events() const {
    return m_binding;
}

QVariantMap ScenePanelView::values() const {
    auto fields = m_binding->values();
    for (auto *button : {m_ui->pbtn_ModelColor, m_ui->pbtn_BackgroundBottomColor, m_ui->pbtn_BackgroundTopColor})
        fields[button->objectName()] = button->text();
    return fields;
}

void ScenePanelView::applyFields(const QVariantMap &fields) {
    m_binding->applyFields(fields);
    for (auto *button : {m_ui->pbtn_ModelColor, m_ui->pbtn_BackgroundBottomColor, m_ui->pbtn_BackgroundTopColor}) {
        const QColor color(button->text());
        if (color.isValid())
            button->setStyleSheet(QString("QPushButton:enabled { background-color:%1; color:%2; border:1px solid palette(mid); border-radius:4px; padding:4px; } ")
                .arg(color.name(), color.lightness() < 128 ? "white" : "black"));
    }
}

void ScenePanelView::clearSensitiveFields() {
    m_binding->clearSensitiveFields();
}

void ScenePanelView::showStatus(const QString &message) {
    m_binding->showStatus(message);
}

} // namespace classic
} // namespace ui
} // namespace fosd
