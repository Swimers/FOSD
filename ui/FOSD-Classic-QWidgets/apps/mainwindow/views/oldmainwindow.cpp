#include "oldmainwindow.h"
#include "../../../common/FormBinding.h"
#include "../../../resources/UiResources.h"
#include "ui_oldmainwindow.h"
namespace fosd {
namespace ui {
namespace classic {
MainWindowView::MainWindowView(QWidget *parent) : QMainWindow(parent), m_ui(new Ui::OldMainWindow) {
    initializeFosdUiResources();
    m_ui->setupUi(this);
    m_binding = new FormBinding(PanelId::Main, this, this);
    connect(m_binding, &ViewEvents::actionTriggered, this, &MainWindowView::relayControl);
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

void MainWindowView::clearInputs() {
    m_binding->clearInputs();
}

void MainWindowView::showStatus(const QString &message) {
    m_binding->showStatus(message);
}

void MainWindowView::relayControl(PanelId panel, const QString &action) {
    if (panel != PanelId::Main)
        return;
    if (action == "pbtn_3DPlot") {
        emit pbtn_3DPlotRequested(values());
        return;
    }
    if (action == "pbtn_Full") {
        emit pbtn_FullRequested(values());
        return;
    }
    if (action == "pbtn_JieTiaoYi") {
        emit pbtn_JieTiaoYiRequested(values());
        return;
    }
    if (action == "pbtn_Maximize") {
        emit pbtn_MaximizeRequested(values());
        return;
    }
    if (action == "pbtn_Minimize") {
        emit pbtn_MinimizeRequested(values());
        return;
    }
    if (action == "pbtn_TurnOff") {
        emit pbtn_TurnOffRequested(values());
        return;
    }
    if (action == "switch_Style") {
        emit switch_StyleRequested(values());
        return;
    }
}
} // namespace classic
} // namespace ui
} // namespace fosd
