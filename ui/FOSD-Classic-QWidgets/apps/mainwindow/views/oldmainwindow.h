#pragma once
#include <QMainWindow>
#include <memory>
#include "../../../common/IView.h"
namespace Ui {
class OldMainWindow;
}

namespace fosd {
namespace ui {
namespace classic {
class FormBinding;
// UI-only wrapper for oldmainwindow.ui; emits intent, never calls business Controls.
class MainWindowView : public QMainWindow, public IView {
    Q_OBJECT
public:
    explicit MainWindowView(QWidget *parent = nullptr);
    ~MainWindowView() override;
    ViewEvents *events() const override;
    QVariantMap values() const override;
    void applyFields(const QVariantMap &fields) override;
    void clearSensitiveFields() override;
    void clearInputs() override;
    void showStatus(const QString &message) override;
signals:
    void pbtn_2DPlotRequested(const QVariantMap &fields);
    void pbtn_3DPlotRequested(const QVariantMap &fields);
    void pbtn_FullRequested(const QVariantMap &fields);
    void pbtn_JieTiaoYiRequested(const QVariantMap &fields);
    void pbtn_MaximizeRequested(const QVariantMap &fields);
    void pbtn_MinimizeRequested(const QVariantMap &fields);
    void pbtn_TurnOffRequested(const QVariantMap &fields);
    void switch_StyleRequested(const QVariantMap &fields);
private slots:
    void relayControl(fosd::ui::classic::PanelId panel, const QString &action);

private:
    std::unique_ptr<Ui::OldMainWindow> m_ui;
    FormBinding *m_binding = nullptr;
};
} // namespace classic
} // namespace ui
} // namespace fosd
