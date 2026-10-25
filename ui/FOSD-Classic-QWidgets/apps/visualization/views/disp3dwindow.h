#pragma once
#include <QMainWindow>
#include <memory>
#include "../../../common/IView.h"
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
    void clearInputs() override;
    void showStatus(const QString &message) override;
signals:
    void actionBackDataRequested(const QVariantMap &fields);
    void actionInforDockRequested(const QVariantMap &fields);
    void actionModelDockRequested(const QVariantMap &fields);
    void actionPauseRequested(const QVariantMap &fields);
    void actionResetRequested(const QVariantMap &fields);
    void actionStartRequested(const QVariantMap &fields);
    void actionStopRequested(const QVariantMap &fields);
    void actionTrackingPointDataRequested(const QVariantMap &fields);
    void actionTrackingPointsRequested(const QVariantMap &fields);
    void actionViewDockRequested(const QVariantMap &fields);
private slots:
    void relayControl(fosd::ui::classic::PanelId panel, const QString &action);

private:
    std::unique_ptr<Ui::disp3dwindow> m_ui;
    FormBinding *m_binding = nullptr;
};
} // namespace classic
} // namespace ui
} // namespace fosd
