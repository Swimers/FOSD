#pragma once
#include <QWidget>
#include <memory>
#include "../../../common/IView.h"
namespace Ui { class disp2dwindow; }
namespace fosd { namespace ui { namespace classic {
class FormBinding;
// UI-only wrapper for disp2dwindow.ui; emits intent, never calls business Controls.
class PlotView : public QWidget, public IView {
    Q_OBJECT
public:
    explicit PlotView(QWidget *parent = nullptr);
    ~PlotView() override;
    ViewEvents *events() const override;
    QVariantMap values() const override;
    void applyFields(const QVariantMap &fields) override;
    void clearSensitiveFields() override;
    void clearInputs() override;
    void showStatus(const QString &message) override;
signals:
    void pbtn_ChangeBackGroundRequested(const QVariantMap &fields);
    void pbtn_ChangeOriginalCoordinatesRequested(const QVariantMap &fields);
    void pbtn_ObserveRequested(const QVariantMap &fields);
    void pbtn_ParaReviseRequested(const QVariantMap &fields);
private slots:
    void relayControl(fosd::ui::classic::PanelId panel, const QString &action);
private:
    std::unique_ptr<Ui::disp2dwindow> m_ui;
    FormBinding *m_binding = nullptr;
};
} } }
