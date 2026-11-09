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
    void showStatus(const QString &message) override;

private:
    std::unique_ptr<Ui::Disp3DViewDockPanel> m_ui;
    FormBinding *m_binding = nullptr;
};
} // namespace classic
} // namespace ui
} // namespace fosd
