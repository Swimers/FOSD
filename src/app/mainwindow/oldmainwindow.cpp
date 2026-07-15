#include "oldmainwindow.h"
#include "ui_oldmainwindow.h"
#include "login.h"

#include <QApplication>
#include <QFile>
#include <QGuiApplication>
#include <QMessageBox>
#include <QPushButton>
#include <QScreen>
#include <QStackedWidget>

static QButtonGroup btnGroup;

OldMainWindow::OldMainWindow(const QString& privilege, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::OldMainWindow)
{
    ui->setupUi(this);
    this->privilege = privilege;

    setWindowIcon(QIcon(":/icon/icon/xidian_logo.jpg"));
    setWindowFlags(Qt::FramelessWindowHint | windowFlags());

    InitOldMainWindow();
}

OldMainWindow::~OldMainWindow()
{
    delete ui;
}

void OldMainWindow::InitOldMainWindow()
{
    if (privilege == "1") {
        setWindowTitle("FOSD Monitor | Administrator");
    } else if (privilege == "2") {
        setWindowTitle("FOSD Monitor | Operator");
    } else if (privilege == "3") {
        setWindowTitle("FOSD Monitor | Guest");
    } else {
        QMessageBox::warning(this, "Warning", "Unknown privilege level.");
    }

    btnGroup.addButton(ui->pbtn_JieTiaoYi, 0);
    btnGroup.addButton(ui->pbtn_2DPlot, 1);
    btnGroup.addButton(ui->pbtn_3DPlot, 2);
    btnGroup.setExclusive(true);

    const int x = ui->stackedWidget->size().width();
    const int y = ui->stackedWidget->size().height();
    originalStyleSheets[0] = ui->pbtn_JieTiaoYi->styleSheet();
    originalStyleSheets[1] = ui->pbtn_2DPlot->styleSheet();
    originalStyleSheets[2] = ui->pbtn_3DPlot->styleSheet();

    m_connectwindow = new connectwindow();
    m_disp2dwindow = new disp2dwindow();
    m_disp3dwindow = new disp3dwindow();

    m_disp3dwindow->GetIFEPtr(m_disp2dwindow->SendIFEPtr());
    m_disp2dwindow->GetStrainPtr(m_connectwindow->SendStrainPtr());

    m_connectwindow->resize(x, y);
    m_disp2dwindow->resize(x, y);
    m_disp3dwindow->resize(x, y);
    ui->stackedWidget->addWidget(m_connectwindow);
    ui->stackedWidget->addWidget(m_disp2dwindow);
    ui->stackedWidget->addWidget(m_disp3dwindow);

    connect(&btnGroup,
            static_cast<void(QButtonGroup::*)(int)>(&QButtonGroup::buttonClicked),
            ui->stackedWidget,
            &QStackedWidget::setCurrentIndex);
    connect(&btnGroup,
            static_cast<void(QButtonGroup::*)(int)>(&QButtonGroup::buttonClicked),
            this,
            &OldMainWindow::onButtonGroupClicked);

    btnGroup.button(0)->setChecked(true);
    ui->stackedWidget->setCurrentIndex(0);
    onButtonGroupClicked(0);

    connect(ui->switch_Style, SIGNAL(toggled(bool)), this, SLOT(switchUiStyle(bool)));
    connect(this, SIGNAL(setUiBlacked()), m_connectwindow, SLOT(SwitchToB()));
    connect(this, SIGNAL(setUiWhited()), m_connectwindow, SLOT(SwitchToW()));
}

void OldMainWindow::mouseMoveEvent(QMouseEvent *e)
{
    if (!Flag_WindowFull && (e->buttons() & Qt::LeftButton)) {
        move(e->globalPos() - p);
    }
}

void OldMainWindow::mousePressEvent(QMouseEvent *e)
{
    if (!Flag_WindowFull && e->button() == Qt::LeftButton) {
        p = e->globalPos() - frameGeometry().topLeft();
    }
}

void OldMainWindow::onButtonGroupClicked(int id)
{
    for (int i = 0; i < 3; i++) {
        QPushButton* button = qobject_cast<QPushButton*>(btnGroup.button(i));
        if (!button) {
            continue;
        }

        if (i == id) {
            button->setStyleSheet("background-color: #0078D7; border-color: #005A9E; color: white;");
        } else {
            button->setStyleSheet(originalStyleSheets[i]);
        }
    }
}

void OldMainWindow::switchUiStyle(bool flag)
{
    Flag_Style = flag;
    emit Flag_Style ? setUiBlacked() : setUiWhited();

    QFile qssFile(Flag_Style ? "./QSS/MaterialDark.qss" : "./QSS/MacOS.qss");
    if (qssFile.open(QFile::ReadOnly)) {
        qApp->setStyleSheet(qssFile.readAll());
    }
}

void OldMainWindow::on_pbtn_Maximize_clicked()
{
    close();
}

void OldMainWindow::on_pbtn_TurnOff_clicked()
{
    loginwindow *m = loginwindow::getInstance();
    m->show();

    close();
    deleteLater();
}

void OldMainWindow::on_pbtn_Minimize_clicked()
{
    showMinimized();
}

void OldMainWindow::on_pbtn_Full_clicked()
{
    Flag_WindowFull = !Flag_WindowFull;
    if (Flag_WindowFull) {
        showFullScreen();
        return;
    }

    showNormal();
    QScreen *screen = QGuiApplication::primaryScreen();
    if (!screen) {
        return;
    }

    const QRect screenGeometry = screen->availableGeometry();
    const int x = screenGeometry.center().x() - width() / 2;
    const int y = screenGeometry.center().y() - height() / 2;
    move(x, y);
}
