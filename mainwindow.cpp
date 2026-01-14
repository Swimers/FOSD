#include "QButtonGroup"
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "login.h"

#include "switchcontrol.h"

static QButtonGroup btnGroup;

MainWindow::MainWindow(const QString& privilege,QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->privilege = privilege;

    // 使用绝对路径设置窗口图标
    setWindowIcon(QIcon(":/icon/Resource/icon/favicon.ico"));
    setWindowFlags(Qt::FramelessWindowHint | windowFlags());

    qDebug() << "MianThreadNum:" << QThread::currentThreadId();

    InitMainwindow();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::InitMainwindow()
{
    if(this->privilege == "1")
        this->setWindowTitle(" 实时监控系统|管理员| ");
    else if(this->privilege == "2")
        this->setWindowTitle(" 实时监控系统 ");
    else if(this->privilege == "3")
        this->setWindowTitle(" 实时监控系统|游客| ");
    else
        QMessageBox::warning(this, "警告", " 未知的权限等级 ");
    btnGroup.addButton(ui->pbtn_JieTiaoYi,0);
    btnGroup.addButton(ui->pbtn_2DPlot,1);
    btnGroup.addButton(ui->pbtn_3DPlot,2);
    btnGroup.setExclusive(true);
    int x,y;
    x = ui->stackedWidget->size().width();
    y = ui->stackedWidget->size().height();
    originalStyleSheets[0] = ui->pbtn_JieTiaoYi->styleSheet();
    originalStyleSheets[1] = ui->pbtn_2DPlot->styleSheet();
    originalStyleSheets[2] = ui->pbtn_3DPlot->styleSheet();
    m_connectwindow = new connectwindow();
    m_disp2dwindow  = new disp2dwindow();
    m_disp3dwindow  = new disp3dwindow();

    m_disp3dwindow->GetIFEPtr(m_disp2dwindow->SendIFEPtr());
    m_disp2dwindow->GetStrainPtr(m_connectwindow->SendStrainPtr());//将thread的strain指针传入ife中

    m_connectwindow->resize(x,y);
    m_disp2dwindow->resize(x,y);
    m_disp3dwindow->resize(x,y);
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
            &MainWindow::onButtonGroupClicked);
    btnGroup.button(0)->setChecked(true);
    ui->stackedWidget->setCurrentIndex(0);
    onButtonGroupClicked(0);

    connect(ui->switch_Style, SIGNAL(toggled(bool)), this, SLOT(switchUiStyle(bool)));
    connect(this, SIGNAL(setUiBlacked()), m_connectwindow, SLOT(SwitchToB()));
    connect(this, SIGNAL(setUiWhited()), m_connectwindow, SLOT(SwitchToW()));
<<<<<<< Updated upstream
=======
    connect(this, SIGNAL(setUiBlacked()), m_disp3dwindow, SLOT(SwitchToB()));
    connect(this, SIGNAL(setUiWhited()), m_disp3dwindow, SLOT(SwitchToW()));
>>>>>>> Stashed changes
}

void MainWindow::mouseMoveEvent(QMouseEvent *e)
{
    if(!Flag_WindowFull){
        if(e->buttons() & Qt::LeftButton){
            move(e->globalPos() - p);       //移到左上角
        }
    }
}

void MainWindow::mousePressEvent(QMouseEvent *e)
{
    if(!Flag_WindowFull){
        if(e->button() == Qt::LeftButton){
            p = e->globalPos() - this->frameGeometry().topLeft();   //求坐标差值:当前点击坐标-窗口左上角坐标
        }
    }
}

void MainWindow::onButtonGroupClicked(int id)
{
    for(int i = 0; i < 3; i++) {
        QPushButton* button = qobject_cast<QPushButton*>(btnGroup.button(i));
        if(button) {
            if(i == id) {
                button->setStyleSheet("background-color: #0078D7;border-color: #005A9E; color: white;");
            } else {
                button->setStyleSheet(originalStyleSheets[i]);
            }
        }
    }
}

void MainWindow::switchUiStyle(bool flag)
{
    Flag_Style = flag;
    if(!Flag_Style){
        //加载qss文件
        emit setUiWhited();
        QFile qssFile("./QSS/MacOS.qss");//1.
        if(qssFile.open(QFile::ReadOnly)){//2.
            qApp->setStyleSheet(qssFile.readAll());//3.
        }
        qssFile.close();//4.
    }
    if(Flag_Style){
        //加载qss文件
        emit setUiBlacked();
        QFile qssFile("./QSS/MaterialDark.qss");//1.
        if(qssFile.open(QFile::ReadOnly)){//2.
            qApp->setStyleSheet(qssFile.readAll());//3.
        }
        qssFile.close();//4.
    }
}

void MainWindow::on_pbtn_Maximize_clicked()
{
    close();
}

void MainWindow::on_pbtn_TurnOff_clicked()
{
    loginwindow * m = loginwindow::getInstance();
    m ->show();

    this->close();
    this->deleteLater();
}

void MainWindow::on_pbtn_UserManage_clicked()
{
    if(privilege=="1"){
        UserManage * m = UserManage::getInstance();
        m ->show();
    }
    else {
        QMessageBox::warning(this, "警告", "你没有权限进行此操作|");
        return;
    }
}

void MainWindow::on_pbtn_Minimize_clicked()
{
    showMinimized();
}

void MainWindow::on_pbtn_Full_clicked()
{
    Flag_WindowFull = !Flag_WindowFull;
    if(Flag_WindowFull){
        showFullScreen();
    }
    if(!Flag_WindowFull){
        showNormal();
        QScreen *screen = QGuiApplication::primaryScreen();
        if (!screen) return;
        QRect screenGeometry = screen->availableGeometry();
        int x = screenGeometry.center().x() - this->width() / 2;
        int y = screenGeometry.center().y() - this->height() / 2;
        this->move(x, y);
    }
}
