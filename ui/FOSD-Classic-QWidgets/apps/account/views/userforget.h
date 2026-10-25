#pragma once
#include <QWidget>
#include <memory>
#include "../../../common/IView.h"
namespace Ui {
class UserForget;
}

namespace fosd {
namespace ui {
namespace classic {
class FormBinding;
// UI-only wrapper for userforget.ui; emits intent, never calls business Controls.
class RecoveryView : public QWidget, public IView {
    Q_OBJECT
public:
    explicit RecoveryView(QWidget *parent = nullptr);
    ~RecoveryView() override;
    ViewEvents *events() const override;
    QVariantMap values() const override;
    void applyFields(const QVariantMap &fields) override;
    void clearSensitiveFields() override;
    void clearInputs() override;
    void showStatus(const QString &message) override;
signals:
    void cbox_SelectPriRequested(const QVariantMap &fields);
    void pbtn_ForgetClearRequested(const QVariantMap &fields);
    void pbtn_ForgetSetRequested(const QVariantMap &fields);
private slots:
    void relayControl(fosd::ui::classic::PanelId panel, const QString &action);

private:
    std::unique_ptr<Ui::UserForget> m_ui;
    FormBinding *m_binding = nullptr;
};
} // namespace classic
} // namespace ui
} // namespace fosd
