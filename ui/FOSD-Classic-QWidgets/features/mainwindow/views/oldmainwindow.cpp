#include "FOSD-Classic-QWidgets/features/mainwindow/views/oldmainwindow.h"
#include "FOSD-Classic-QWidgets/common/FormBinding.h"
#include "FOSD-Classic-QWidgets/common/UiResources.h"
#include "ui_oldmainwindow.h"
namespace fosd {
namespace ui {
namespace classic {
MainWindowView::MainWindowView(QWidget *parent) : QMainWindow(parent), m_ui(new Ui::OldMainWindow) {
    initializeFosdUiResources();
    m_ui->setupUi(this);
    m_binding = new FormBinding(PanelId::Main, this, this);
}

MainWindowView::~MainWindowView() = default;
ViewEvents *MainWindowView::events() const {
    return m_binding;
}

QVariantMap MainWindowView::values() const {
    return m_binding->values();
}

void MainWindowView::applyFields(const QVariantMap &fields) {
    m_binding->applyFields(fields);
}

void MainWindowView::clearSensitiveFields() {
    m_binding->clearSensitiveFields();
}

void MainWindowView::showStatus(const QString &message) {
    m_binding->showStatus(message);
}

} // namespace classic
} // namespace ui
} // namespace fosd
