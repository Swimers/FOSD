#pragma once
#include <QMainWindow>
#include <QPoint>
#include <memory>
#include "FOSD-Classic-QWidgets/common/IView.h"
class QMouseEvent;
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
    void showStatus(const QString &message) override;

protected:
    // 无边框窗口的拖动：空白标题带按下后跟随鼠标移动窗口。
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

private:
    // 判断窗口坐标是否落在标题带（与最小化/全屏/关闭按钮同一水平区域）。
    bool isTitleBarArea(const QPoint &pos) const;

    std::unique_ptr<Ui::OldMainWindow> m_ui;
    FormBinding *m_binding = nullptr;
    bool m_dragging = false;   // 是否处于拖动中
    QPoint m_dragOffset;       // 按下点相对窗口左上角的偏移
};
} // namespace classic
} // namespace ui
} // namespace fosd
