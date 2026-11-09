#pragma once
#include <QWidget>
#include <memory>
#include "FOSD-Classic-QWidgets/common/IView.h"
namespace Ui {
class channlCalibration;
}

namespace fosd {
namespace ui {
namespace classic {
class FormBinding;
// UI-only wrapper for channlCalibration.ui; emits intent, never calls business Controls.
class ChannelCalibrationView : public QWidget, public IView {
    Q_OBJECT
public:
    explicit ChannelCalibrationView(QWidget *parent = nullptr);
    ~ChannelCalibrationView() override;
    ViewEvents *events() const override;
    QVariantMap values() const override;
    void applyFields(const QVariantMap &fields) override;
    void clearSensitiveFields() override;
    void showStatus(const QString &message) override;

private:
    std::unique_ptr<Ui::channlCalibration> m_ui;
    FormBinding *m_binding = nullptr;
};
} // namespace classic
} // namespace ui
} // namespace fosd
