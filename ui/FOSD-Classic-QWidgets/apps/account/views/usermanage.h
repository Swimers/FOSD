#pragma once
#include <QWidget>
#include <memory>
#include "../../../common/IView.h"
namespace Ui {
class UserManage;
}

namespace fosd {
namespace ui {
namespace classic {
class FormBinding;
// UI-only wrapper for usermanage.ui; emits intent, never calls business Controls.
class AccountView : public QWidget, public IView {
    Q_OBJECT
public:
    explicit AccountView(QWidget *parent = nullptr);
    ~AccountView() override;
    ViewEvents *events() const override;
    QVariantMap values() const override;
    void applyFields(const QVariantMap &fields) override;
    void clearSensitiveFields() override;
    void clearInputs() override;
    void showStatus(const QString &message) override;
signals:
    void cbox_MUsernameRequested(const QVariantMap &fields);
    void pbtn_UserSetRequested(const QVariantMap &fields);
private slots:
    void relayControl(fosd::ui::classic::PanelId panel, const QString &action);

private:
    std::unique_ptr<Ui::UserManage> m_ui;
    FormBinding *m_binding = nullptr;
};
} // namespace classic
} // namespace ui
} // namespace fosd
