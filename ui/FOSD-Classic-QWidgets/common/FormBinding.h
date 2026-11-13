#pragma once
#include "FOSD-Classic-QWidgets/common/ViewEvents.h"
#include "FOSD-Classic-QWidgets/common/UiTypes.h"
#include <QObject>
#include <QPointer>
class QWidget;
namespace fosd {
namespace ui {
namespace classic {
// Widget adapter: collect input, emit user intent, and apply presentation data.
// It performs no navigation, file/device I/O, calibration or rendering.
class FormBinding final : public ViewEvents {
    Q_OBJECT
public:
    // 绑定一个面板根控件，并为其中所有命名控件接上用户操作信号。
    FormBinding(PanelId panel, QWidget *widget, QObject *parent = nullptr);

    // 采集本面板的输入，生成“控件名 -> 值”的字段表；只读控件与子面板控件被排除。
    QVariantMap values() const;

    // 把字段表写回控件：支持 "_status"、".enabled" 后缀，以及下拉框、表格等结构值。
    void applyFields(const QVariantMap &fields);

    // 清空本面板内所有非明文回显的输入框（密码类字段）。
    void clearSensitiveFields();

    // 显示操作结果：记录到面板根控件的 presenterStatus 属性，重要消息另外弹提示。
    void showStatus(const QString &message);

private slots:
    // 用户操作回调：把触发控件的 objectName 作为动作名发给 Presenter。
    void relay();

    // QDialogButtonBox 的“确定”，以动作名 "accepted" 上报。
    void accepted();

    // QDialogButtonBox 的“取消”，以动作名 "rejected" 上报。
    void rejected();

private:
    // 判断对象是否属于本面板：沿 parent 上溯，遇到其它面板根即视为不属于。
    bool owns(const QObject *object) const;

    PanelId m_panel;            // 本绑定代表的面板
    QPointer<QWidget> m_widget;  // 面板根控件；控件销毁后自动置空
};
} // namespace classic
} // namespace ui
} // namespace fosd
