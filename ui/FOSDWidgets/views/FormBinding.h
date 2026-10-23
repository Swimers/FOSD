#pragma once
#include "../contracts/IPanelView.h"
#include "../contracts/UiTypes.h"
#include <QObject>
#include <QPointer>
class QWidget;
namespace fosd { namespace widgets {
// Widget adapter: collect input, emit user intent, and apply presentation data.
// It performs no navigation, file/device I/O, calibration or rendering.
class FormBinding final : public QObject, public IPanelView {
    Q_OBJECT
public:
    FormBinding(PanelId panel, QWidget *widget, QObject *parent = nullptr);
    QWidget *widget() const;
    QVariantMap values() const override;
    void applyFields(const QVariantMap &fields) override;
    void clearSensitiveFields() override;
    void clearInputs() override;
    void showStatus(const QString &message) override;
signals:
    void actionTriggered(fosd::widgets::PanelId panel, const QString &action);
private slots:
    void relay();
    void accepted();
    void rejected();
private:
    bool owns(const QObject *object) const;
    PanelId m_panel;
    QPointer<QWidget> m_widget;
};
} }
