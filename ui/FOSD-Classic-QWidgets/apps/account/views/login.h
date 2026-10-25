#pragma once
#include <QMainWindow>
#include <memory>
#include "../../../common/IView.h"
namespace Ui {
class loginwindow;
}

namespace fosd {
namespace ui {
namespace classic {
class FormBinding;
// UI-only wrapper for login.ui; emits intent, never calls business Controls.
class LoginView : public QMainWindow, public IView {
    Q_OBJECT
public:
    explicit LoginView(QWidget *parent = nullptr);
    ~LoginView() override;
    ViewEvents *events() const override;
    QVariantMap values() const override;
    void applyFields(const QVariantMap &fields) override;
    void clearSensitiveFields() override;
    void clearInputs() override;
    void showStatus(const QString &message) override;
signals:
    void btn_ClearRequested(const QVariantMap &fields);
    void btn_CloseRequested(const QVariantMap &fields);
    void btn_ForgetRequested(const QVariantMap &fields);
    void btn_LoginRequested(const QVariantMap &fields);
    void btn_MinRequested(const QVariantMap &fields);
    void btn_RegisterRequested(const QVariantMap &fields);
    void cbox_PrivilegeRequested(const QVariantMap &fields);
    void check_RemNameRequested(const QVariantMap &fields);
    void check_RemPassRequested(const QVariantMap &fields);
    void comboBoxRequested(const QVariantMap &fields);
private slots:
    void relayControl(fosd::ui::classic::PanelId panel, const QString &action);

private:
    std::unique_ptr<Ui::loginwindow> m_ui;
    FormBinding *m_binding = nullptr;
};
} // namespace classic
} // namespace ui
} // namespace fosd
