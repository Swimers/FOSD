#pragma once
#include <QWidget>
#include <memory>
#include "../../../common/IView.h"
namespace Ui { class trackingpointdatatable; }
namespace fosd { namespace ui { namespace classic {
class FormBinding;
// UI-only wrapper for trackingpointdatatable.ui; emits intent, never calls business Controls.
class TrackingDataView : public QWidget, public IView {
    Q_OBJECT
public:
    explicit TrackingDataView(QWidget *parent = nullptr);
    ~TrackingDataView() override;
    ViewEvents *events() const override;
    QVariantMap values() const override;
    void applyFields(const QVariantMap &fields) override;
    void clearSensitiveFields() override;
    void clearInputs() override;
    void showStatus(const QString &message) override;
signals:
    void cbox_AutoRefreshRequested(const QVariantMap &fields);
    void pbtn_HideAllRequested(const QVariantMap &fields);
    void pbtn_RefreshRequested(const QVariantMap &fields);
    void pbtn_ShowAllRequested(const QVariantMap &fields);
    void pbtn_ShowAllRowsRequested(const QVariantMap &fields);
    void pbtn_ShowSelectedRequested(const QVariantMap &fields);
    void acceptedRequested(const QVariantMap &fields);
    void rejectedRequested(const QVariantMap &fields);
private slots:
    void relayControl(fosd::ui::classic::PanelId panel, const QString &action);
private:
    std::unique_ptr<Ui::trackingpointdatatable> m_ui;
    FormBinding *m_binding = nullptr;
};
} } }
