#pragma once
#include <QWidget>
#include <memory>
#include "../../../common/IView.h"
namespace Ui {
class disp3dkeypointedit;
}

namespace fosd {
namespace ui {
namespace classic {
class FormBinding;
// UI-only wrapper for disp3dkeypointedit.ui; emits intent, never calls business Controls.
class KeyPointEditView : public QWidget, public IView {
    Q_OBJECT
public:
    explicit KeyPointEditView(QWidget *parent = nullptr);
    ~KeyPointEditView() override;
    ViewEvents *events() const override;
    QVariantMap values() const override;
    void applyFields(const QVariantMap &fields) override;
    void clearSensitiveFields() override;
    void clearInputs() override;
    void showStatus(const QString &message) override;
signals:
    void cbox_IDKeyPointRequested(const QVariantMap &fields);
    void pbtn_ClearAllRequested(const QVariantMap &fields);
    void pbtn_FindKeyPointRequested(const QVariantMap &fields);
    void pbtn_SelectAllRequested(const QVariantMap &fields);
private slots:
    void relayControl(fosd::ui::classic::PanelId panel, const QString &action);

private:
    std::unique_ptr<Ui::disp3dkeypointedit> m_ui;
    FormBinding *m_binding = nullptr;
};
} // namespace classic
} // namespace ui
} // namespace fosd
