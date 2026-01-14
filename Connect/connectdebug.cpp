#include "connectdebug.h"
#include "ui_connectdebug.h"

connectdebug::connectdebug(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::connectdebug)
{
    ui->setupUi(this);

    ui->edit_ConnectState->setReadOnly(true);
    ui->edit_ConnectState->setText("正常");

    // 禁止用户调整窗口大小
    setWindowFlags(windowFlags() | Qt::MSWindowsFixedSizeDialogHint);
}

connectdebug::~connectdebug()
{
    delete ui;
}

void connectdebug::on_pbtn_ConnectDebugStart_clicked()
{
    if(isRunning){
        ui->pbtn_ConnectDebugStart->setText(" 启动 ");
        isRunning = false;
    }
    else {
        ui->pbtn_ConnectDebugStart->setText(" 停止 ");
        isRunning = true;
    }

}

void connectdebug::on_pbtn_ConnectDebugClose_clicked()
{
    this->close();
    this->deleteLater();
}
