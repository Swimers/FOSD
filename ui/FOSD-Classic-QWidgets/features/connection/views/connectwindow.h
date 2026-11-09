#pragma once
#include <QWidget>
#include <memory>
#include <QPoint>
#include "FOSD-Classic-QWidgets/common/IView.h"
namespace Ui {
class connectwindow;
}

namespace fosd {
namespace ui {
namespace classic {
class FormBinding;
// UI-only wrapper for connectwindow.ui; emits intent, never calls business Controls.
class ConnectionView : public QWidget, public IView {
    Q_OBJECT
public:
    explicit ConnectionView(QWidget *parent = nullptr);
    ~ConnectionView() override;
    ViewEvents *events() const override;
    QVariantMap values() const override;
    void applyFields(const QVariantMap &fields) override;
    void clearSensitiveFields() override;
    void showStatus(const QString &message) override;
    void setSpectrumTheme(bool dark);


private:
    void initializeSpectrum();
    void refreshSpectrumSelection();
    void resetSpectrumView();
    std::unique_ptr<Ui::connectwindow> m_ui;
    FormBinding *m_binding = nullptr;
    QString m_selectedCurve;
    double m_selectedTime = 0;
    bool m_hasSpectrumSelection = false;
};
} // namespace classic
} // namespace ui
} // namespace fosd
