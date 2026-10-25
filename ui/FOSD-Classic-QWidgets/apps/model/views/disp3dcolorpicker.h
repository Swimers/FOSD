#pragma once
#include <QDialog>
#include <memory>
#include "../../../common/IView.h"
namespace Ui {
class Disp3DColorPicker;
}

namespace fosd {
namespace ui {
namespace classic {
class FormBinding;
// UI-only wrapper for disp3dcolorpicker.ui; emits intent, never calls business Controls.
class ColorPickerView : public QDialog, public IView {
    Q_OBJECT
public:
    explicit ColorPickerView(QWidget *parent = nullptr);
    ~ColorPickerView() override;
    ViewEvents *events() const override;
    QVariantMap values() const override;
    void applyFields(const QVariantMap &fields) override;
    void clearSensitiveFields() override;
    void clearInputs() override;
    void showStatus(const QString &message) override;
signals:
    void btnPickRequested(const QVariantMap &fields);
    void sliderBRequested(const QVariantMap &fields);
    void sliderGRequested(const QVariantMap &fields);
    void sliderRRequested(const QVariantMap &fields);
    void spinBRequested(const QVariantMap &fields);
    void spinGRequested(const QVariantMap &fields);
    void spinRRequested(const QVariantMap &fields);
    void acceptedRequested(const QVariantMap &fields);
    void rejectedRequested(const QVariantMap &fields);
private slots:
    void relayControl(fosd::ui::classic::PanelId panel, const QString &action);

private:
    std::unique_ptr<Ui::Disp3DColorPicker> m_ui;
    FormBinding *m_binding = nullptr;
};
} // namespace classic
} // namespace ui
} // namespace fosd
