#include "FOSD-Classic-QWidgets/features/mainwindow/views/oldmainwindow.h"
#include "FOSD-Classic-QWidgets/common/FormBinding.h"
#include "FOSD-Classic-QWidgets/common/UiResources.h"
#include "ui_oldmainwindow.h"
#include <QMouseEvent>
#include <QLayout>
#ifdef Q_OS_WIN
#include <qt_windows.h>
#include <windowsx.h>
#endif
namespace fosd {
namespace ui {
namespace classic {
MainWindowView::MainWindowView(QWidget *parent) : QMainWindow(parent), m_ui(new Ui::OldMainWindow) {
    initializeFosdUiResources();
    m_ui->setupUi(this);
    this->setWindowFlags(Qt::FramelessWindowHint);
    m_binding = new FormBinding(PanelId::Main, this, this);
#ifdef Q_OS_WIN
    m_resizeTimer.setInterval(33);
    m_resizeTimer.setTimerType(Qt::PreciseTimer);
    connect(&m_resizeTimer, &QTimer::timeout, this, &MainWindowView::flushResizeLayout);
#endif
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

#ifdef Q_OS_WIN
void MainWindowView::beginInteractiveResize() {
    if (m_interactiveResize || !layout() || !layout()->isEnabled() ||
        isMaximized() || isFullScreen())
        return;
    // Freeze only the root geometry propagation between frames, not painting
    // or acquisition. In particular, QCustomPlot renders synchronously on each
    // resize and QOpenGLWidget reallocates its FBO when its dimensions change.
    m_interactiveResize = true;
    m_lastLayoutSize = size();
    layout()->setEnabled(false);
    m_resizeTimer.start();
}

void MainWindowView::flushResizeLayout() {
    if (!m_interactiveResize || m_lastLayoutSize == size())
        return;
    layout()->setEnabled(true);
    layout()->setGeometry(rect());
    m_lastLayoutSize = size();
    layout()->setEnabled(false);
    update();
}

void MainWindowView::endInteractiveResize() {
    if (!m_interactiveResize)
        return;
    m_resizeTimer.stop();
    m_interactiveResize = false;
    layout()->setEnabled(true);
    layout()->activate();
    layout()->setGeometry(rect());
    update();
}

bool MainWindowView::nativeEvent(const QByteArray &eventType, void *message, long *result) {
    const auto *msg = static_cast<MSG *>(message);
    if (msg->message == WM_SIZING)
        beginInteractiveResize();
    else if (msg->message == WM_EXITSIZEMOVE || msg->message == WM_CANCELMODE ||
             (msg->message == WM_SHOWWINDOW && !msg->wParam))
        endInteractiveResize();
    if (msg->message == WM_NCHITTEST && !isMaximized() && !isFullScreen()) {
        RECT bounds;
        if (GetWindowRect(msg->hwnd, &bounds)) {
            // WM_NCHITTEST uses physical screen pixels. Derive the scale from
            // the actual window size so negative monitor coordinates and Qt
            // high-DPI scaling both work without a logical/physical mismatch.
            const int border = qMax(1, qRound(7.0 * (bounds.right - bounds.left) / qMax(1, width())));
            const int x = GET_X_LPARAM(msg->lParam);
            const int y = GET_Y_LPARAM(msg->lParam);
            const bool inside = x >= bounds.left && x < bounds.right &&
                                y >= bounds.top && y < bounds.bottom;
            if (inside) {
                const bool left = x < bounds.left + border;
                const bool right = x >= bounds.right - border;
                const bool top = y < bounds.top + border;
                const bool bottom = y >= bounds.bottom - border;
                const bool horizontal = minimumWidth() < maximumWidth();
                const bool vertical = minimumHeight() < maximumHeight();
                const int hit = top && vertical
                    ? (left && horizontal ? HTTOPLEFT : right && horizontal ? HTTOPRIGHT : HTTOP)
                    : bottom && vertical
                    ? (left && horizontal ? HTBOTTOMLEFT : right && horizontal ? HTBOTTOMRIGHT : HTBOTTOM)
                    : left && horizontal ? HTLEFT : right && horizontal ? HTRIGHT : HTCLIENT;
                if (hit != HTCLIENT) {
                    *result = hit;
                    return true;
                }
            }
        }
    }
    return QMainWindow::nativeEvent(eventType, message, result);
}
#endif

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
