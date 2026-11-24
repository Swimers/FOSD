#pragma once
#include <QDialog>
#include <memory>
#include "FOSD-Classic-QWidgets/common/IView.h"
namespace Ui {
class dispdata;
}

namespace fosd {
namespace ui {
namespace classic {
class FormBinding;
// UI-only wrapper for dispdata.ui; emits intent, never calls business Controls.
class HardwareDataView : public QDialog, public IView {
    Q_OBJECT
public:
    explicit HardwareDataView(QWidget *parent = nullptr);
    ~HardwareDataView() override;
    ViewEvents *events() const override;
    QVariantMap values() const override;
    void applyFields(const QVariantMap &fields) override;
    void clearSensitiveFields() override;
    void showStatus(const QString &message) override;

private:
    std::unique_ptr<Ui::dispdata> m_ui;
    FormBinding *m_binding = nullptr;
};
} // namespace classic
} // namespace ui
} // namespace fosd
