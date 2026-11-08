#pragma once
#include <QWidget>
#include <memory>
#include "FOSD-Classic-QWidgets/common/IView.h"
namespace Ui {
class Disp3DViewDockPanel;
}

namespace fosd {
namespace ui {
namespace classic {
class FormBinding;
// UI-only wrapper for disp3dviewdockpanel.ui; emits intent, never calls business Controls.
class ScenePanelView : public QWidget, public IView {
    Q_OBJECT
public:
    explicit ScenePanelView(QWidget *parent = nullptr);
    ~ScenePanelView() override;
    ViewEvents *events() const override;
    QVariantMap values() const override;
    void applyFields(const QVariantMap &fields) override;
    void clearSensitiveFields() override;
    void clearInputs() override;
    void showStatus(const QString &message) override;
signals:
    void cbox_AxisAbleRequested(const QVariantMap &fields);
    void cbox_CareamLockAbleRequested(const QVariantMap &fields);
    void cbox_GridAbleRequested(const QVariantMap &fields);
    void cbox_SelectViewRequested(const QVariantMap &fields);
    void cbox_XOYGridRequested(const QVariantMap &fields);
    void cbox_XOZGridRequested(const QVariantMap &fields);
    void cbox_ZOYGridRequested(const QVariantMap &fields);
    void hsd_AxisSizeRequested(const QVariantMap &fields);
    void hsd_GridSizeRequested(const QVariantMap &fields);
    void pbtn_SwitchViewRequested(const QVariantMap &fields);
private slots:
    void relayControl(fosd::ui::classic::PanelId panel, const QString &action);

private:
    std::unique_ptr<Ui::Disp3DViewDockPanel> m_ui;
    FormBinding *m_binding = nullptr;
};
} // namespace classic
} // namespace ui
} // namespace fosd
