#pragma once
#include <QWidget>
#include <memory>
#include "../../../common/IView.h"
namespace Ui {
class trackingpointmanager;
}

namespace fosd {
namespace ui {
namespace classic {
class FormBinding;
// UI-only wrapper for trackingpointmanager.ui; emits intent, never calls business Controls.
class TrackingManagerView : public QWidget, public IView {
    Q_OBJECT
public:
    explicit TrackingManagerView(QWidget *parent = nullptr);
    ~TrackingManagerView() override;
    ViewEvents *events() const override;
    QVariantMap values() const override;
    void applyFields(const QVariantMap &fields) override;
    void clearSensitiveFields() override;
    void clearInputs() override;
    void showStatus(const QString &message) override;
signals:
    void pbtn_ClearAllRequested(const QVariantMap &fields);
    void pbtn_LoadFileRequested(const QVariantMap &fields);
    void pbtn_RefreshRequested(const QVariantMap &fields);
    void pbtn_SelectAllRequested(const QVariantMap &fields);
    void pbtn_SelectNoneRequested(const QVariantMap &fields);
    void acceptedRequested(const QVariantMap &fields);
    void rejectedRequested(const QVariantMap &fields);
private slots:
    void relayControl(fosd::ui::classic::PanelId panel, const QString &action);

private:
    std::unique_ptr<Ui::trackingpointmanager> m_ui;
    FormBinding *m_binding = nullptr;
};
} // namespace classic
} // namespace ui
} // namespace fosd
