#pragma once
#include <QWidget>
#include <memory>
#include "../../../common/IView.h"
namespace Ui {
class connectdebug;
}

namespace fosd {
namespace ui {
namespace classic {
class FormBinding;
// UI-only wrapper for connectdebug.ui; emits intent, never calls business Controls.
class ConnectionDebugView : public QWidget, public IView {
    Q_OBJECT
public:
    explicit ConnectionDebugView(QWidget *parent = nullptr);
    ~ConnectionDebugView() override;
    ViewEvents *events() const override;
    QVariantMap values() const override;
    void applyFields(const QVariantMap &fields) override;
    void clearSensitiveFields() override;
    void clearInputs() override;
    void showStatus(const QString &message) override;
signals:
    void pbtn_ConnectDebugCloseRequested(const QVariantMap &fields);
    void pbtn_ConnectDebugStartRequested(const QVariantMap &fields);
private slots:
    void relayControl(fosd::ui::classic::PanelId panel, const QString &action);

private:
    std::unique_ptr<Ui::connectdebug> m_ui;
    FormBinding *m_binding = nullptr;
};
} // namespace classic
} // namespace ui
} // namespace fosd
