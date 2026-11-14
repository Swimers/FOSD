#include "FOSD-Classic-QWidgets/application/WorkbenchView.h"
#include "FOSD-Classic-QWidgets/controls/renderhostwidget.h"
#include <QButtonGroup>
#include <QDockWidget>
#include <QFile>
#include <QFileDialog>
#include <QHeaderView>
#include <QIcon>
#include <QLabel>
#include <QPushButton>
#include <QStackedWidget>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QComboBox>
#include "FOSD-Classic-QWidgets/common/ViewEvents.h"
#include "FOSD-Classic-QWidgets/features/connection/views/connectwindow.h"
#include "FOSD-Classic-QWidgets/features/connection/views/channlCalibration.h"
#include "FOSD-Classic-QWidgets/features/data/views/dispdata.h"
#include "FOSD-Classic-QWidgets/features/visualization/views/disp3dwindow.h"
#include "FOSD-Classic-QWidgets/features/visualization/views/docks/disp3dinfodockpanel.h"
#include "FOSD-Classic-QWidgets/features/visualization/views/docks/disp3dmodeldockpanel.h"
#include "FOSD-Classic-QWidgets/features/visualization/views/docks/disp3dviewdockpanel.h"
#include "FOSD-Classic-QWidgets/features/visualization/views/keypoints/disp3dkeypointedit.h"
#include "FOSD-Classic-QWidgets/features/visualization/views/keypoints/disp3dkeypointselect.h"
#include <QTableWidget>
#include <QPalette>
#include <QAction>
#include <QGuiApplication>
#include <QScreen>
#include <QCursor>
#include <QTabWidget>

namespace fosd {
namespace ui {
namespace classic {
namespace {
// PanelId → 具体 View 的映射。组装知识只属于组装根，原 common/ViewFactory 已并入此处。
QWidget *createPanelView(PanelId panel, QWidget *parent) {
    switch (panel) {
        case PanelId::Connection:
            return new ConnectionView(parent);
        case PanelId::ChannelCalibration:
            return new ChannelCalibrationView(parent);
        case PanelId::Visualization:
            return new VisualizationView(parent);
        case PanelId::InfoPanel:
            return new InfoPanelView(parent);
        case PanelId::ScenePanel:
            return new ScenePanelView(parent);
        case PanelId::ModelPanel:
            return new ModelPanelView(parent);
        case PanelId::KeyPointEdit:
            return new KeyPointEditView(parent);
        case PanelId::KeyPointSelect:
            return new KeyPointSelectView(parent);
        case PanelId::HardwareData:
            return new HardwareDataView(parent);
        case PanelId::Main:
            // 主窗口即 WorkbenchView 自身，调用处直接复用 this，不会走到这里。
            return nullptr;
    }
    return nullptr;
}
} // namespace

WorkbenchView::WorkbenchView(QWidget *parent) : MainWindowView(parent) {
    setWindowTitle(tr("FOSD Monitor"));
    setWindowIcon(QIcon(":/icon/icon/favicon.ico"));

    // Capture/bind each form before nesting other forms in it. Values are also
    // filtered at runtime by fosdPanelRoot, so sibling fields never leak.
    for (int index = int(PanelId::Main); index <= int(PanelId::HardwareData); ++index) {
        const auto panel = PanelId(index);
        QWidget *widget = panel == PanelId::Main ? this : createPanelView(panel, this);

        if (widget != this) {
            widget->setWindowFlag(Qt::Window, true);
            widget->hide();
        }
        m_panels.insert(index, widget);
    }

    m_pages = findChild<QStackedWidget *>("stackedWidget");
    for (auto panel : {PanelId::Connection, PanelId::Visualization}) {
        auto *page = panelWidget(panel);
        page->setWindowFlags(Qt::Widget);
        m_pages->addWidget(page);
    }

    auto *navigation = new QButtonGroup(this);
    for (const auto &name : {"pbtn_JieTiaoYi", "pbtn_3DPlot"}) {
        auto *button = findChild<QPushButton *>(name);
        button->setCheckable(true);
        navigation->addButton(button);
    }
    navigation->setExclusive(true);
    selectPage(0);
    const QMap<int, QString> titles{{int(PanelId::ChannelCalibration), tr("通道与系数标定")},
                                    {int(PanelId::KeyPointEdit), tr("关键点编辑")},
                                    {int(PanelId::KeyPointSelect), tr("关键点选择")},
                                    {int(PanelId::HardwareData), tr("硬件数据")}};
    for (auto title = titles.constBegin(); title != titles.constEnd(); ++title)
        panelWidget(PanelId(title.key()))->setWindowTitle(title.value());

    auto *visualization = qobject_cast<QMainWindow *>(
        panelWidget(PanelId::Visualization)); // Visualization 转化为 QMainWindow
    const QVector<PanelId> docks{PanelId::ModelPanel, PanelId::ScenePanel, PanelId::InfoPanel};
    const QStringList dockTitles{tr("模型配置"), tr("场景与相机"), tr("模型信息")};
    for (int index = 0; index < docks.size(); ++index) {
        auto *dock = new QDockWidget(dockTitles[index], visualization);
        dock->setObjectName(QStringLiteral("fosdDock%1").arg(index));
        auto *page = panelWidget(docks[index]);
        page->setWindowFlags(Qt::Widget);
        dock->setWidget(page);
        visualization->addDockWidget(Qt::LeftDockWidgetArea, dock);
        m_docks.insert(int(docks[index]), dock);
    }
    auto *modelDock = m_docks.value(int(PanelId::ModelPanel));
    for (auto panel : {PanelId::ScenePanel, PanelId::InfoPanel})
        visualization->tabifyDockWidget(modelDock, m_docks.value(int(panel)));
    visualization->setTabPosition(Qt::LeftDockWidgetArea, QTabWidget::North);
    visualization->resizeDocks({modelDock}, {280}, Qt::Horizontal);
    modelDock->raise();

    for (auto *table : findChildren<QTableWidget *>()) {
        table->setEditTriggers(QAbstractItemView::NoEditTriggers);
        table->setSelectionBehavior(QAbstractItemView::SelectRows);
        table->horizontalHeader()->setStretchLastSection(true);
    }
    auto *data = panelWidget(PanelId::HardwareData)->findChild<QTableWidget *>("tableWidget");
    data->setColumnCount(3);
    data->setHorizontalHeaderLabels({tr("IFE 输入"), tr("波长源序号"), tr("标定应变")});

    setModelAvailable(false);
    applyFields({{"switch_Style", true}});
    applyTheme(true);
    initializeWindowGeometry();
}

WorkbenchView::~WorkbenchView() = default;

void WorkbenchView::initializeWindowGeometry() {
    // Calculate in logical pixels, using the display under the pointer and
    // excluding taskbars. This is only the initial size, not an aspect lock.
    auto *screen = QGuiApplication::screenAt(QCursor::pos());
    if (!screen)
        screen = QGuiApplication::primaryScreen();
    if (!screen)
        return;
    ensurePolished();
    layout()->activate();
    const QRect available = screen->availableGeometry();
    const QSize minimum = minimumSize().expandedTo(minimumSizeHint());
    const int minimumUnits = qMax((minimum.width() + 15) / 16, (minimum.height() + 8) / 9);
    const int screenUnits = qMin(available.width() / 16, available.height() / 9);
    const int preferredUnits = qMin(int(available.width() * 0.94) / 16,
                                   int(available.height() * 0.94) / 9);
    // Drop the outer breathing space if it would violate the form minimum.
    // If the display cannot fit the form at all, Qt's minimum size wins.
    const int units = qMax(1, qMin(screenUnits, qMax(preferredUnits, minimumUnits)));
    resize(16 * units, 9 * units);
    move(available.left() + qMax(0, (available.width() - width()) / 2),
         available.top() + qMax(0, (available.height() - height()) / 2));
}

void WorkbenchView::setModelAvailable(bool available) {
    // Gate parent containers so child-specific states (e.g. gradient top color)
    // continue to follow their own rules while model availability changes.
    for (auto panel : {PanelId::ScenePanel, PanelId::KeyPointEdit, PanelId::KeyPointSelect})
        panelWidget(panel)->setEnabled(available);
    auto *model = panelWidget(PanelId::ModelPanel);
    model->findChild<QWidget *>("widget_ModelControls")->setEnabled(available);
    model->findChild<QPushButton *>("pbtn_ModelClear")->setEnabled(available);
}
IView *WorkbenchView::panelView(PanelId panel) const {
    return dynamic_cast<IView *>(panelWidget(panel));
}

QWidget *WorkbenchView::panelWidget(PanelId panel) const { //
    return m_panels.value(int(panel), nullptr);
}

RenderHostWidget *WorkbenchView::renderHost() const {
    return panelWidget(PanelId::Visualization)->findChild<RenderHostWidget *>("vtk_openGL");
}

void WorkbenchView::selectPage(int index) {
    if (index < 0 || index >= m_pages->count())
        return;
    m_pages->setCurrentIndex(index);
    const QStringList names{"pbtn_JieTiaoYi", "pbtn_3DPlot"};
    for (int button = 0; button < names.size(); ++button) {
        auto *widget = findChild<QPushButton *>(names[button]);
        widget->setChecked(button == index);
    }
}

void WorkbenchView::showPanel(PanelId panel) {
    if (m_docks.contains(int(panel))) {
        selectPage(1);
        auto *dock = m_docks.value(int(panel));
        dock->show();
        dock->raise();
    } else if (auto *widget = panelWidget(panel)) {
        widget->show();
        widget->raise();
        widget->activateWindow();
    }
}

void WorkbenchView::closePanel(PanelId panel) {
    if (auto *view = panelView(panel))
        view->clearSensitiveFields();
    if (auto *widget = panelWidget(panel))
        widget->close();
}

void WorkbenchView::windowAction(PanelId panel, WindowAction action) {
    QWidget *widget = panelWidget(panel);
    if (!widget)
        return;
    switch (action) {
        case WindowAction::Minimize:
            widget->showMinimized();
            break;
        case WindowAction::ToggleFullScreen:
            if (widget->isFullScreen())
                widget->showNormal();
            else
                widget->showFullScreen();
            break;
    }
}

void WorkbenchView::applyTheme(bool dark) {
    // Native subcontrols and popup windows must follow the stylesheet as well.
    QPalette colors;
    colors.setColor(QPalette::Window, QColor(dark ? "#171e2b" : "#f1f4f8"));
    colors.setColor(QPalette::WindowText, QColor(dark ? "#e6edf7" : "#243247"));
    colors.setColor(QPalette::Base, QColor(dark ? "#182231" : "#ffffff"));
    colors.setColor(QPalette::AlternateBase, QColor(dark ? "#202a3a" : "#edf1f6"));
    colors.setColor(QPalette::Text, colors.color(QPalette::WindowText));
    colors.setColor(QPalette::Button, QColor(dark ? "#29364a" : "#f6f8fb"));
    colors.setColor(QPalette::ButtonText, colors.color(QPalette::WindowText));
    colors.setColor(QPalette::Highlight, QColor(dark ? "#357cca" : "#2563aa"));
    colors.setColor(QPalette::HighlightedText, Qt::white);
    colors.setColor(QPalette::ToolTipBase, QColor(dark ? "#202a3a" : "#ffffff"));
    colors.setColor(QPalette::ToolTipText, colors.color(QPalette::WindowText));
    for (auto role : {QPalette::WindowText, QPalette::Text, QPalette::ButtonText})
        colors.setColor(QPalette::Disabled, role, QColor(dark ? "#8392a8" : "#7b899c"));
    setPalette(colors);
    const QString mode = dark ? QStringLiteral("dark") : QStringLiteral("light");
    const QMap<QString, QString> toolbarIcons{{"actionModelDock", "cube"}, {"actionInforDock", "info"},
        {"actionViewDock", "scene"}, {"actionStart", "play"}, {"actionPause", "pause"},
        {"actionStop", "stop"}, {"actionReset", "reset"}};
    for (auto it = toolbarIcons.constBegin(); it != toolbarIcons.constEnd(); ++it)
        panelWidget(PanelId::Visualization)->findChild<QAction *>(it.key())->setIcon(
            QIcon(QString(":/fosd-ui/icons/%1-%2.svg").arg(it.value(), mode)));
    for (const auto &name : {QStringLiteral("pbtn_JieTiaoYi"), QStringLiteral("pbtn_3DPlot")}) {
        const QString symbol = name == "pbtn_JieTiaoYi" ? QStringLiteral("wave") : QStringLiteral("cube");
        QIcon icon;
        icon.addFile(QString(":/fosd-ui/icons/%1-%2.svg").arg(symbol, mode), QSize(), QIcon::Normal, QIcon::Off);
        icon.addFile(QString(":/fosd-ui/icons/%1-active.svg").arg(symbol), QSize(), QIcon::Normal, QIcon::On);
        findChild<QPushButton *>(name)->setIcon(icon);
    }
    QFile style(dark ? ":/fosd-ui/styles/dark.qss" : ":/fosd-ui/styles/light.qss");
    if (style.open(QIODevice::ReadOnly))
        setStyleSheet(QString::fromUtf8(style.readAll()));
    if (auto *connection = qobject_cast<ConnectionView *>(panelWidget(PanelId::Connection)))
        connection->setSpectrumTheme(dark);
}

QString WorkbenchView::chooseFile(bool save, const QString &title, const QString &filter) {
    return save ? QFileDialog::getSaveFileName(this, title, QString(), filter)
                : QFileDialog::getOpenFileName(this, title, QString(), filter);
}
} // namespace classic
} // namespace ui
} // namespace fosd
