#include "disp3dwindow.h"
#include "../../../common/FormBinding.h"
#include "../../../resources/UiResources.h"
#include "ui_disp3dwindow.h"
namespace fosd {
namespace ui {
namespace classic {
VisualizationView::VisualizationView(QWidget *parent)
    : QMainWindow(parent), m_ui(new Ui::disp3dwindow) {
    initializeFosdUiResources();
    m_ui->setupUi(this);
    m_binding = new FormBinding(PanelId::Visualization, this, this);
    connect(m_binding, &ViewEvents::actionTriggered, this, &VisualizationView::relayControl);
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

void VisualizationView::clearInputs() {
    m_binding->clearInputs();
}

void VisualizationView::showStatus(const QString &message) {
    m_binding->showStatus(message);
}

void VisualizationView::relayControl(PanelId panel, const QString &action) {
    if (panel != PanelId::Visualization)
        return;
    if (action == "actionBackData") {
        emit actionBackDataRequested(values());
        return;
    }
    if (action == "actionInforDock") {
        emit actionInforDockRequested(values());
        return;
    }
    if (action == "actionModelDock") {
        emit actionModelDockRequested(values());
        return;
    }
    if (action == "actionPause") {
        emit actionPauseRequested(values());
        return;
    }
    if (action == "actionReset") {
        emit actionResetRequested(values());
        return;
    }
    if (action == "actionStart") {
        emit actionStartRequested(values());
        return;
    }
    if (action == "actionStop") {
        emit actionStopRequested(values());
        return;
    }
    if (action == "actionTrackingPointData") {
        emit actionTrackingPointDataRequested(values());
        return;
    }
    if (action == "actionTrackingPoints") {
        emit actionTrackingPointsRequested(values());
        return;
    }
    if (action == "actionViewDock") {
        emit actionViewDockRequested(values());
        return;
    }
}
} // namespace classic
} // namespace ui
} // namespace fosd
