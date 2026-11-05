#include "disp2dwindow.h"
#include "../../../common/FormBinding.h"
#include "../../../resources/UiResources.h"
#include "ui_disp2dwindow.h"
namespace fosd {
namespace ui {
namespace classic {
PlotView::PlotView(QWidget *parent) : QWidget(parent), m_ui(new Ui::disp2dwindow) {
    initializeFosdUiResources();
    m_ui->setupUi(this);
    m_binding = new FormBinding(PanelId::Plot, this, this);
    connect(m_binding, &ViewEvents::actionTriggered, this, &PlotView::relayControl);
}

PlotView::~PlotView() = default;
ViewEvents *PlotView::events() const {
    return m_binding;
}

QVariantMap PlotView::values() const {
    return m_binding->values();
}

void PlotView::applyFields(const QVariantMap &fields) {
    m_binding->applyFields(fields);
}

void PlotView::clearSensitiveFields() {
    m_binding->clearSensitiveFields();
}

void PlotView::clearInputs() {
    m_binding->clearInputs();
}

void PlotView::showStatus(const QString &message) {
    m_binding->showStatus(message);
}

void PlotView::relayControl(PanelId panel, const QString &action) {
    if (panel != PanelId::Plot)
        return;
    if (action == "pbtn_Observe") {
        emit pbtn_ObserveRequested(values());
        return;
    }
    if (action == "pbtn_ParaRevise") {
        emit pbtn_ParaReviseRequested(values());
        return;
    }
}
} // namespace classic
} // namespace ui
} // namespace fosd
