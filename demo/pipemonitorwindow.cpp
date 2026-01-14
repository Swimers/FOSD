#include "pipemonitorwindow.h"
#include "ui_pipemonitorwindow.h"

#pragma execution_character_set("utf-8")

PipeMonitorWindow::PipeMonitorWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PipeMonitorWindow)
{
    ui->setupUi(this);

    ui->lineEditState->setReadOnly(true);
    ui->lineEditState->setText("正常");

    // 禁止用户调整窗口大小
    setWindowFlags(windowFlags() | Qt::MSWindowsFixedSizeDialogHint);
}

PipeMonitorWindow::~PipeMonitorWindow()
{
    delete ui;
}



void PipeMonitorWindow::on_btnState_clicked()
{
    if(isRunning == true) //按下停止
    {
        ui->btnState->setText("开始");
        isRunning = false;
        return;
    }

    //按下开始
    ui->btnState->setText("停止");
    isRunning = true;
}


void PipeMonitorWindow::on_btnClose_clicked()
{
    this->close();
}
