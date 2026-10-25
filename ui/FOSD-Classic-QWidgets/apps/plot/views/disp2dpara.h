#pragma once
#include <QWidget>
#include <memory>
#include "../../../common/IView.h"
namespace Ui { class disp2dpara; }
namespace fosd { namespace ui { namespace classic {
class FormBinding;
// UI-only wrapper for disp2dpara.ui; emits intent, never calls business Controls.
class PlotSettingsView : public QWidget, public IView {
    Q_OBJECT
public:
    explicit PlotSettingsView(QWidget *parent = nullptr);
    ~PlotSettingsView() override;
    ViewEvents *events() const override;
    QVariantMap values() const override;
    void applyFields(const QVariantMap &fields) override;
    void clearSensitiveFields() override;
    void clearInputs() override;
    void showStatus(const QString &message) override;
signals:
    void pbtn_SenerCancelRequested(const QVariantMap &fields);
    void pbtn_SenerSetRequested(const QVariantMap &fields);
private slots:
    void relayControl(fosd::ui::classic::PanelId panel, const QString &action);
private:
    std::unique_ptr<Ui::disp2dpara> m_ui;
    FormBinding *m_binding = nullptr;
};
} } }
