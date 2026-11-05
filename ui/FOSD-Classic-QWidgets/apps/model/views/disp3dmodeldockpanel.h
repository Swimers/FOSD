#pragma once
#include <QWidget>
#include <memory>
#include "../../../common/IView.h"
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
    void clearInputs() override;
    void showStatus(const QString &message) override;
signals:
    void cbox_FPSRequested(const QVariantMap &fields);
    void cbox_ScalarSelectRequested(const QVariantMap &fields);
    void pbtn_KeyPointEditRequested(const QVariantMap &fields);
    void pbtn_KeyPointSelectRequested(const QVariantMap &fields);
    void pbtn_ModelClearRequested(const QVariantMap &fields);
    void pbtn_ModelLoadRequested(const QVariantMap &fields);
    void pbtn_NormalizeRequested(const QVariantMap &fields);
    void pbtn_ScalarClearRequested(const QVariantMap &fields);
    void pbtn_ScalarSetRequested(const QVariantMap &fields);
private slots:
    void relayControl(fosd::ui::classic::PanelId panel, const QString &action);

private:
    std::unique_ptr<Ui::Disp3DModelDockPanel> m_ui;
    FormBinding *m_binding = nullptr;
};
} // namespace classic
} // namespace ui
} // namespace fosd
