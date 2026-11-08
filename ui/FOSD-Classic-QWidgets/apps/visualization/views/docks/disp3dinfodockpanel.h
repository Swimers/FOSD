#pragma once
#include <QWidget>
#include <memory>
#include "../../../../common/IView.h"
namespace Ui {
class Disp3DInfoDockPanel;
}

namespace fosd {
namespace ui {
namespace classic {
class FormBinding;
// UI-only wrapper for disp3dinfodockpanel.ui; emits intent, never calls business Controls.
class InfoPanelView : public QWidget, public IView {
    Q_OBJECT
public:
    explicit InfoPanelView(QWidget *parent = nullptr);
    ~InfoPanelView() override;
    ViewEvents *events() const override;
    QVariantMap values() const override;
    void applyFields(const QVariantMap &fields) override;
    void clearSensitiveFields() override;
    void clearInputs() override;
    void showStatus(const QString &message) override;
signals:
    void modelAvailabilityChanged(bool available);
    void pushButton_6Requested(const QVariantMap &fields);
    void radioButtonRequested(const QVariantMap &fields);
    void radioButton_2Requested(const QVariantMap &fields);
    void radioButton_3Requested(const QVariantMap &fields);
    void radioButton_4Requested(const QVariantMap &fields);
private slots:
    void relayControl(fosd::ui::classic::PanelId panel, const QString &action);

private:
    std::unique_ptr<Ui::Disp3DInfoDockPanel> m_ui;
    FormBinding *m_binding = nullptr;
    bool m_modelAvailable = false;
};
} // namespace classic
} // namespace ui
} // namespace fosd
