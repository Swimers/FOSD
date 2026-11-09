#pragma once
#include <QMainWindow>
#include <memory>
#include "FOSD-Classic-QWidgets/common/IView.h"
namespace Ui {
class disp3dwindow;
}

namespace fosd {
namespace ui {
namespace classic {
class FormBinding;
// UI-only wrapper for disp3dwindow.ui; emits intent, never calls business Controls.
class VisualizationView : public QMainWindow, public IView {
    Q_OBJECT
public:
    explicit VisualizationView(QWidget *parent = nullptr);
    ~VisualizationView() override;
    ViewEvents *events() const override;
    QVariantMap values() const override;
    void applyFields(const QVariantMap &fields) override;
    void clearSensitiveFields() override;
    void showStatus(const QString &message) override;

private:
    std::unique_ptr<Ui::disp3dwindow> m_ui;
    FormBinding *m_binding = nullptr;
};
} // namespace classic
} // namespace ui
} // namespace fosd
