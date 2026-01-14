#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QButtonGroup>
#include "logwidget.h"
#include "usermanangerwidget.h"

#pragma execution_character_set("utf-8")
MainWindow::MainWindow(const QString& privilege,QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->privilege = privilege;
    if(this->privilege == "1")
        this->setWindowTitle("实时监控系统(管理员)");
    else if(this->privilege == "2")
        this->setWindowTitle("实时监控系统");
    else if(this->privilege == "3")
        this->setWindowTitle("实时监控系统(游客)");
    else
        QMessageBox::warning(this, "警告", "未知的权限等级");

    //if(this->privilege != "1")
        //ui->tbtn_userInfo->setDisabled(true);


    //SetLogWindow();     //设置登录界面
    initializeWidget(); //初始化主界面
}

MainWindow::~MainWindow()
{
    if(this->demo) delete demo;
    if(this->disp2d) delete disp2d;
    if(this->disp3d) delete disp3d;
    delete ui;
}

void MainWindow::SetLogWindow()
{
    this->setFixedSize(this->width(),this->height());
}

void MainWindow::initializeWidget()
{
    btnGroup.addButton(ui->tbtn_Demo,0);
    btnGroup.addButton(ui->tbtn_Disp2D,1);
    btnGroup.addButton(ui->tbtn_Disp3D,2);

    int x,y;
    //获取stackedWidget的大小，保持页面与其大小一致
    x = ui->stackedWidget->size().width();
    y = ui->stackedWidget->size().height();

    demo = new DemoWindow(this->privilege,this);
    disp2d = new Disp2DWindow(this->privilege,this);
    disp2d->GetStrainPtr(demo->SendStrainPtr());        //将thread的strain指针传入ife中
    disp3d = new disp3dMainWindow(this->privilege,this);
    disp3d->GetIFEPtr(disp2d->SendIFEPtr());

    demo->resize(x,y);
    disp2d->resize(x,y);
    //disp3d->resize(x,y);
    disp3d->resize(x,y);

    ui->stackedWidget->addWidget(demo);
    ui->stackedWidget->addWidget(disp2d);
    ui->stackedWidget->addWidget(disp3d);
    //ui->stackedWidget->addWidget(disp3d);

    connect(&btnGroup,static_cast<void(QButtonGroup::*)(int)>(&QButtonGroup::buttonClicked),
            ui->stackedWidget,&QStackedWidget::setCurrentIndex);
    btnGroup.button(0)->setChecked(true);
    ui->stackedWidget->setCurrentIndex(0);
}

void MainWindow::on_tbtn_Exit_clicked()
{
    LogWidget* loginWidget = LogWidget::instance();
    loginWidget->func_init();
    loginWidget->loadcfg();
    loginWidget->show();
    this->close();
    this->deleteLater();
}


void MainWindow::on_tbtn_userInfo_clicked()
{
    if(privilege=="1"){
        //QString configFilePath = "login.ini"; // 配置文件路径

        // 打开配置文件
        //QDesktopServices::openUrl(QUrl::fromLocalFile(configFilePath));
        qDebug()<<"111";
        UserManangerWidget* usermananger = new UserManangerWidget();
        qDebug()<<"222";
        usermananger->show();
        qDebug()<<"333";
    }
    else {
        QMessageBox::warning(this, "警告", "你没有权限进行此操作");
        return;
    }


}
