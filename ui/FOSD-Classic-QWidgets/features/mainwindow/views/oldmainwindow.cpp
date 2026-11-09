#include "FOSD-Classic-QWidgets/features/mainwindow/views/oldmainwindow.h"
#include "FOSD-Classic-QWidgets/common/FormBinding.h"
#include "FOSD-Classic-QWidgets/common/UiResources.h"
#include "ui_oldmainwindow.h"
#include <QMouseEvent>
namespace fosd {
namespace ui {
namespace classic {
MainWindowView::MainWindowView(QWidget *parent) : QMainWindow(parent), m_ui(new Ui::OldMainWindow) {
    initializeFosdUiResources();
    m_ui->setupUi(this);
    this->setWindowFlags(Qt::FramelessWindowHint);
    m_binding = new FormBinding(PanelId::Main, this, this);
}

MainWindowView::~MainWindowView() = default;
ViewEvents *MainWindowView::events() const {
    return m_binding;
}

QVariantMap MainWindowView::values() const {
    return m_binding->values();
}

void MainWindowView::applyFields(const QVariantMap &fields) {
    m_binding->applyFields(fields);
}

void MainWindowView::clearSensitiveFields() {
    m_binding->clearSensitiveFields();
}

void MainWindowView::showStatus(const QString &message) {
    m_binding->showStatus(message);
}

bool MainWindowView::isTitleBarArea(const QPoint &pos) const {
    QWidget *bar = m_ui->pbtn_Minimize->parentWidget();
    if (!bar || !m_ui->horizontalLayout_3)
        return false;
    const QRect band = m_ui->horizontalLayout_3->geometry().adjusted(0, -6, 0, 6);
    return band.translated(bar->mapTo(this, QPoint(0, 0))).contains(pos);
}

void MainWindowView::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton && !isMaximized() && !isFullScreen() &&
        isTitleBarArea(event->pos())) {
        m_dragging = true;
        m_dragOffset = event->globalPos() - pos();
        event->accept();
        return;
    }
    QMainWindow::mousePressEvent(event);
}

void MainWindowView::mouseMoveEvent(QMouseEvent *event) {
    if (m_dragging && (event->buttons() & Qt::LeftButton)) {
        move(event->globalPos() - m_dragOffset);
        event->accept();
        return;
    }
    m_dragging = false;
    QMainWindow::mouseMoveEvent(event);
}

void MainWindowView::mouseReleaseEvent(QMouseEvent *event) {
    if (m_dragging && event->button() == Qt::LeftButton) {
        m_dragging = false;
        event->accept();
        return;
    }
    QMainWindow::mouseReleaseEvent(event);
}

} // namespace classic
} // namespace ui
} // namespace fosd
