#pragma once
#include <QWidget>
#include <memory>
#include "../../../common/IView.h"
namespace Ui {
class UserRegister;
}

namespace fosd {
namespace ui {
namespace classic {
class FormBinding;
// UI-only wrapper for userregister.ui; emits intent, never calls business Controls.
class RegistrationView : public QWidget, public IView {
    Q_OBJECT
public:
    explicit RegistrationView(QWidget *parent = nullptr);
    ~RegistrationView() override;
    ViewEvents *events() const override;
    QVariantMap values() const override;
    void applyFields(const QVariantMap &fields) override;
    void clearSensitiveFields() override;
    void clearInputs() override;
    void showStatus(const QString &message) override;
signals:
    void cbox_SelectNewPriRequested(const QVariantMap &fields);
    void pbtn_RegisterClearRequested(const QVariantMap &fields);
    void pbtn_RegisterSetRequested(const QVariantMap &fields);
private slots:
    void relayControl(fosd::ui::classic::PanelId panel, const QString &action);

private:
    std::unique_ptr<Ui::UserRegister> m_ui;
    FormBinding *m_binding = nullptr;
};
} // namespace classic
} // namespace ui
} // namespace fosd
