#pragma once
#include "ViewEvents.h"
#include "UiTypes.h"
#include <QObject>
#include <QPointer>
class QWidget;
namespace fosd { namespace ui { namespace classic {
// Widget adapter: collect input, emit user intent, and apply presentation data.
// It performs no navigation, file/device I/O, calibration or rendering.
class FormBinding final : public ViewEvents {
    Q_OBJECT
public:
    FormBinding(PanelId panel, QWidget *widget, QObject *parent = nullptr);
    QWidget *widget() const;
    QVariantMap values() const;
    void applyFields(const QVariantMap &fields);
    void clearSensitiveFields();
    void clearInputs();
    void showStatus(const QString &message);
private slots:
    void relay();
    void accepted();
    void rejected();
private:
    bool owns(const QObject *object) const;
    PanelId m_panel;
    QPointer<QWidget> m_widget;
};
} } }
