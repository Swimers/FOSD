#pragma once
#include <QMainWindow>
#include <memory>
#include "FOSD-Classic-QWidgets/common/IView.h"
namespace Ui {
class OldMainWindow;
}

namespace fosd {
namespace ui {
namespace classic {
class FormBinding;
// UI-only wrapper for oldmainwindow.ui; emits intent, never calls business Controls.
class MainWindowView : public QMainWindow, public IView {
    Q_OBJECT
public:
    explicit MainWindowView(QWidget *parent = nullptr);
    ~MainWindowView() override;
    ViewEvents *events() const override;
    QVariantMap values() const override;
    void applyFields(const QVariantMap &fields) override;
    void clearSensitiveFields() override;
    void showStatus(const QString &message) override;

private:
    std::unique_ptr<Ui::OldMainWindow> m_ui;
    FormBinding *m_binding = nullptr;
};
} // namespace classic
} // namespace ui
} // namespace fosd
