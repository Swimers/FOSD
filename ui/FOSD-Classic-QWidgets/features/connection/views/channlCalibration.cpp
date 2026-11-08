#include "FOSD-Classic-QWidgets/features/connection/views/channlCalibration.h"
#include "FOSD-Classic-QWidgets/common/FormBinding.h"
#include "FOSD-Classic-QWidgets/common/UiResources.h"
#include "ui_channlCalibration.h"
namespace fosd {
namespace ui {
namespace classic {
ChannelCalibrationView::ChannelCalibrationView(QWidget *parent) : QWidget(parent), m_ui(new Ui::channlCalibration) {
    initializeFosdUiResources();
    m_ui->setupUi(this);
    m_binding = new FormBinding(PanelId::ChannelCalibration, this, this);
    connect(m_binding, &ViewEvents::actionTriggered, this, &ChannelCalibrationView::relayControl);
}

ChannelCalibrationView::~ChannelCalibrationView() = default;
ViewEvents *ChannelCalibrationView::events() const {
    return m_binding;
}

QVariantMap ChannelCalibrationView::values() const {
    return m_binding->values();
}

void ChannelCalibrationView::applyFields(const QVariantMap &fields) {
    m_binding->applyFields(fields);
}

void ChannelCalibrationView::clearSensitiveFields() {
    m_binding->clearSensitiveFields();
}

void ChannelCalibrationView::clearInputs() {
    m_binding->clearInputs();
}

void ChannelCalibrationView::showStatus(const QString &message) {
    m_binding->showStatus(message);
}

void ChannelCalibrationView::relayControl(PanelId panel, const QString &action) {
    if (panel != PanelId::ChannelCalibration)
        return;
    if (action == "pbtn_SenerCancel") {
        emit pbtn_SenerCancelRequested(values());
        return;
    }
    if (action == "pbtn_SenerSet") {
        emit pbtn_SenerSetRequested(values());
        return;
    }
}
} // namespace classic
} // namespace ui
} // namespace fosd
