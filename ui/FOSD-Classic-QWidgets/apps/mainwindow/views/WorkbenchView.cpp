#include "WorkbenchView.h"
#include "../../../common/ViewFactory.h"
#include "../../../controls/renderhostwidget.h"
#include <QButtonGroup>
#include <QDockWidget>
#include <QFile>
#include <QFileDialog>
#include <QHeaderView>
#include <QIcon>
#include <QLabel>
#include <QPushButton>
#include <QStackedWidget>
#include <QTableWidget>

namespace fosd {
namespace ui {
namespace classic {
WorkbenchView::WorkbenchView(QWidget *parent) : MainWindowView(parent) {
    setWindowTitle(tr("FOSD Monitor"));
    setWindowIcon(QIcon(":/icon/icon/favicon.ico"));
    // Capture/bind each form before nesting other forms in it. Values are also
    // filtered at runtime by fosdPanelRoot, so sibling fields never leak.
    for (int index = int(PanelId::Main); index <= int(PanelId::Account); ++index) {
        const auto panel = PanelId(index);
        QWidget *widget = panel == PanelId::Main ? this : createPanelView(panel, this);
        if (widget != this) {
            widget->setWindowFlag(Qt::Window, true);
            widget->hide();
        }
        m_panels.insert(index, widget);
    }
    m_pages = findChild<QStackedWidget *>("stackedWidget");
    for (auto panel : {PanelId::Connection, PanelId::Plot, PanelId::Visualization}) {
        auto *page = panelWidget(panel);
        page->setWindowFlags(Qt::Widget);
        m_pages->addWidget(page);
    }
    auto *navigation = new QButtonGroup(this);
    for (const auto &name : {"pbtn_JieTiaoYi", "pbtn_2DPlot", "pbtn_3DPlot"}) {
        auto *button = findChild<QPushButton *>(name);
        button->setCheckable(true);
        navigation->addButton(button);
    }
    navigation->setExclusive(true);
    selectPage(0);
    const QMap<int, QString> titles{{int(PanelId::ConnectionDebug), tr("通信诊断")},
                                    {int(PanelId::PlotSettings), tr("标定参数")},
                                    {int(PanelId::KeyPointEdit), tr("关键点编辑")},
                                    {int(PanelId::KeyPointSelect), tr("关键点选择")},
                                    {int(PanelId::ColorPicker), tr("模型颜色")},
                                    {int(PanelId::HardwareData), tr("硬件数据")},
                                    {int(PanelId::TrackingManager), tr("追踪点管理器")},
                                    {int(PanelId::TrackingData), tr("追踪点变形量")},
                                    {int(PanelId::Registration), tr("用户注册")},
                                    {int(PanelId::Recovery), tr("找回密码")},
                                    {int(PanelId::Account), tr("账户管理")}};
    for (auto title = titles.constBegin(); title != titles.constEnd(); ++title)
        panelWidget(PanelId(title.key()))->setWindowTitle(title.value());
    auto *visualization = qobject_cast<QMainWindow *>(panelWidget(PanelId::Visualization));
    const QVector<PanelId> docks{PanelId::ModelPanel, PanelId::ScenePanel, PanelId::InfoPanel};
    const QStringList dockTitles{tr("模型配置"), tr("场景与相机"), tr("模型信息")};
    for (int index = 0; index < docks.size(); ++index) {
        auto *dock = new QDockWidget(dockTitles[index], visualization);
        dock->setObjectName(QStringLiteral("fosdDock%1").arg(index));
        auto *page = panelWidget(docks[index]);
        page->setWindowFlags(Qt::Widget);
        dock->setWidget(page);
        visualization->addDockWidget(index == 2 ? Qt::RightDockWidgetArea : Qt::LeftDockWidgetArea,
                                     dock);
        m_docks.insert(int(docks[index]), dock);
    }
    for (auto *table : findChildren<QTableWidget *>()) {
        table->setEditTriggers(QAbstractItemView::NoEditTriggers);
        table->setSelectionBehavior(QAbstractItemView::SelectRows);
        table->horizontalHeader()->setStretchLastSection(true);
    }
    auto *data = panelWidget(PanelId::HardwareData)->findChild<QTableWidget *>("tableWidget");
    data->setColumnCount(4);
    data->setHorizontalHeaderLabels({tr("编号"), tr("应变"), tr("温度"), tr("自由度")});
    auto *tracking =
        panelWidget(PanelId::TrackingManager)->findChild<QTableWidget *>("tab_pointtable");
    tracking->setColumnCount(7);
    tracking->setHorizontalHeaderLabels(
        {tr("显示"), "ID", tr("名称"), "X (mm)", "Y (mm)", "Z (mm)", ""});
    auto *trackingData =
        panelWidget(PanelId::TrackingData)->findChild<QTableWidget *>("tab_datatable");
    trackingData->setColumnCount(10);
    trackingData->setHorizontalHeaderLabels({tr("显示"), "ID", tr("名称"), "X0 (mm)", "Y0 (mm)",
                                             "Z0 (mm)", "dX (mm)", "dY (mm)", "dZ (mm)", ""});
}

WorkbenchView::~WorkbenchView() = default;
IView *WorkbenchView::panelView(PanelId panel) const {
    return dynamic_cast<IView *>(panelWidget(panel));
}

QWidget *WorkbenchView::panelWidget(PanelId panel) const {
    return m_panels.value(int(panel), nullptr);
}

RenderHostWidget *WorkbenchView::renderHost() const {
    return panelWidget(PanelId::Visualization)->findChild<RenderHostWidget *>("vtk_openGL");
}

void WorkbenchView::selectPage(int index) {
    if (index < 0 || index >= m_pages->count())
        return;
    m_pages->setCurrentIndex(index);
    const QStringList names{"pbtn_JieTiaoYi", "pbtn_2DPlot", "pbtn_3DPlot"};
    for (int button = 0; button < names.size(); ++button) {
        auto *widget = findChild<QPushButton *>(names[button]);
        widget->setChecked(button == index);
        widget->setStyleSheet(button == index ? "background-color:#0078D7;color:white;"
                                              : QString());
    }
}

void WorkbenchView::showPanel(PanelId panel) {
    if (m_docks.contains(int(panel))) {
        selectPage(2);
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
        case WindowAction::Close:
            closePanel(panel);
            break;
    }
}

void WorkbenchView::applyTheme(bool dark) {
    QFile style(dark ? ":/fosd-ui/styles/dark.qss" : ":/fosd-ui/styles/light.qss");
    if (style.open(QIODevice::ReadOnly))
        setStyleSheet(QString::fromUtf8(style.readAll()));
}

QString WorkbenchView::chooseFile(bool save, const QString &title, const QString &filter) {
    return save ? QFileDialog::getSaveFileName(this, title, QString(), filter)
                : QFileDialog::getOpenFileName(this, title, QString(), filter);
}
} // namespace classic
} // namespace ui
} // namespace fosd
