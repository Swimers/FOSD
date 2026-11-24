#include "FOSD-Classic-QWidgets/controls/renderhostwidget.h"
#include <QVBoxLayout>
#include <QEvent>
namespace fosd {
namespace ui {
namespace classic {
RenderHostWidget::RenderHostWidget(QWidget *parent) : QWidget(parent) {
    auto *box = new QVBoxLayout(this);
    box->setContentsMargins(0, 0, 0, 0);
    setToolTip(tr("请先导入模型"));
}

void RenderHostWidget::setSurface(QWidget *surface) {
    if (m_surface == surface)
        return;
    if (m_surface) {
        m_surface->removeEventFilter(this);
        for (auto *widget : m_surface->findChildren<QWidget *>())
            widget->removeEventFilter(this);
        layout()->removeWidget(m_surface);
        m_surface->hide();
        m_surface->setParent(nullptr);
    }
    m_surface = surface;
    if (surface) {
        surface->installEventFilter(this);
        for (auto *widget : surface->findChildren<QWidget *>())
            widget->installEventFilter(this);
        surface->setEnabled(m_modelAvailable);
        layout()->addWidget(surface);
        surface->show();
    }
}

void RenderHostWidget::setModelAvailable(bool available) {
    m_modelAvailable = available;
    if (m_surface) {
        if (!available)
            m_surface->clearFocus();
        m_surface->setEnabled(available);
    }
    setToolTip(available ? tr("拖动旋转、滚轮缩放、点击选择模型点") : tr("请先导入模型"));
}

bool RenderHostWidget::eventFilter(QObject *watched, QEvent *event) {
    if (!m_modelAvailable) {
        // QVTK forwards input before QWidget's default enabled-state handling.
        // Block input explicitly, including direct event delivery, but allow
        // painting and resize/GL initialization to continue normally.
        switch (event->type()) {
        case QEvent::MouseButtonPress: case QEvent::MouseButtonRelease:
        case QEvent::MouseButtonDblClick: case QEvent::MouseMove:
        case QEvent::Wheel: case QEvent::KeyPress: case QEvent::KeyRelease:
        case QEvent::ShortcutOverride: case QEvent::ContextMenu:
        case QEvent::TouchBegin: case QEvent::TouchUpdate:
        case QEvent::TouchEnd: case QEvent::TouchCancel:
        case QEvent::Gesture: case QEvent::GestureOverride: case QEvent::NativeGesture:
        case QEvent::TabletPress: case QEvent::TabletMove: case QEvent::TabletRelease:
            event->accept();
            return true;
        default:
            break;
        }
    }
    return QWidget::eventFilter(watched, event);
}
} // namespace classic
} // namespace ui
} // namespace fosd
