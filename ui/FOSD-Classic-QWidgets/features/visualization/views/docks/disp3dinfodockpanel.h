#pragma once
#include <QWidget>
#include <memory>
#include "FOSD-Classic-QWidgets/common/IView.h"
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
    void showStatus(const QString &message) override;

private:
    std::unique_ptr<Ui::Disp3DInfoDockPanel> m_ui;
    FormBinding *m_binding = nullptr;
};
} // namespace classic
} // namespace ui
} // namespace fosd
