#include "renderhostwidget.h"
#include <QVBoxLayout>
namespace fosd {
namespace ui {
namespace classic {
RenderHostWidget::RenderHostWidget(QWidget *parent) : QWidget(parent) {
    auto *box = new QVBoxLayout(this);
    box->setContentsMargins(0, 0, 0, 0);
    setToolTip(tr("三维显示宿主，等待可视化适配器接入"));
}

void RenderHostWidget::setSurface(QWidget *surface) {
    if (m_surface == surface)
        return;
    if (m_surface) {
        layout()->removeWidget(m_surface);
        m_surface->hide();
        m_surface->setParent(nullptr);
    }
    m_surface = surface;
    if (surface) {
        layout()->addWidget(surface);
        surface->show();
    }
}
} // namespace classic
} // namespace ui
} // namespace fosd
