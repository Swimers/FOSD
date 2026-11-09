#pragma once
#include <QWidget>
#include <memory>
#include "FOSD-Classic-QWidgets/common/IView.h"
namespace Ui {
class Disp3DModelDockPanel;
}

namespace fosd {
namespace ui {
namespace classic {
class FormBinding;
// UI-only wrapper for disp3dmodeldockpanel.ui; emits intent, never calls business Controls.
class ModelPanelView : public QWidget, public IView {
    Q_OBJECT
public:
    explicit ModelPanelView(QWidget *parent = nullptr);
    ~ModelPanelView() override;
    ViewEvents *events() const override;
    QVariantMap values() const override;
    void applyFields(const QVariantMap &fields) override;
    void clearSensitiveFields() override;
    void showStatus(const QString &message) override;

private:
    std::unique_ptr<Ui::Disp3DModelDockPanel> m_ui;
    FormBinding *m_binding = nullptr;
};
} // namespace classic
} // namespace ui
} // namespace fosd
