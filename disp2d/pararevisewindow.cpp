#include "pararevisewindow.h"
#include "ui_pararevisewindow.h"
#include <QLineEdit>
#include <QIntValidator>
#include <QDoubleValidator>

ParaReviseWindow::ParaReviseWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ParaReviseWindow)
{
    ui->setupUi(this);

    // 禁止用户调整窗口大小
    setWindowFlags(windowFlags() | Qt::MSWindowsFixedSizeDialogHint);

    QIntValidator *intValidator = new QIntValidator(this);  //整数过滤
    QDoubleValidator *doubleValidator = new QDoubleValidator(this); //浮点数过滤

    // 设置整数验证器
    ui->lineEditOrder1->setValidator(intValidator);  // 只允许整数
    ui->lineEditOrder2->setValidator(intValidator);
    ui->lineEditOrder3->setValidator(intValidator);
    ui->lineEditOrder4->setValidator(intValidator);
    ui->lineEditOrder5->setValidator(intValidator);
    ui->lineEditOrder6->setValidator(intValidator);

    // 设置浮点数验证器.
    doubleValidator->setNotation(QDoubleValidator::StandardNotation);
    ui->lineEditPara1->setValidator(doubleValidator);
    ui->lineEditPara2->setValidator(doubleValidator);
    ui->lineEditPara3->setValidator(doubleValidator);
    ui->lineEditPara4->setValidator(doubleValidator);
    ui->lineEditPara5->setValidator(doubleValidator);
    ui->lineEditPara6->setValidator(doubleValidator);
}

ParaReviseWindow::~ParaReviseWindow()
{
    delete ui;
}

void ParaReviseWindow::init()
{
    ui->lineEditOrder1->setText(QString::number(ife->order[0]));
    ui->lineEditOrder2->setText(QString::number(ife->order[1]));
    ui->lineEditOrder3->setText(QString::number(ife->order[2]));
    ui->lineEditOrder4->setText(QString::number(ife->order[3]));
    ui->lineEditOrder5->setText(QString::number(ife->order[4]));
    ui->lineEditOrder6->setText(QString::number(ife->order[5]));

    ui->lineEditPara1->setText(QString::number(ife->para[0]));
    ui->lineEditPara2->setText(QString::number(ife->para[1]));
    ui->lineEditPara3->setText(QString::number(ife->para[2]));
    ui->lineEditPara4->setText(QString::number(ife->para[3]));
    ui->lineEditPara5->setText(QString::number(ife->para[4]));
    ui->lineEditPara6->setText(QString::number(ife->para[5]));

}

void ParaReviseWindow::on_btnOk_clicked()
{
    ife->order[0] = ui->lineEditOrder1->text().toInt();
    ife->order[1] = ui->lineEditOrder2->text().toInt();
    ife->order[2] = ui->lineEditOrder3->text().toInt();
    ife->order[3] = ui->lineEditOrder4->text().toInt();
    ife->order[4] = ui->lineEditOrder5->text().toInt();
    ife->order[5] = ui->lineEditOrder6->text().toInt();

    ife->para[0] = ui->lineEditPara1->text().toDouble();
    ife->para[1] = ui->lineEditPara2->text().toDouble();
    ife->para[2] = ui->lineEditPara3->text().toDouble();
    ife->para[3] = ui->lineEditPara4->text().toDouble();
    ife->para[4] = ui->lineEditPara5->text().toDouble();
    ife->para[5] = ui->lineEditPara6->text().toDouble();

    ife->saveConfig("ifepara.ini");
    this->close();
}

void ParaReviseWindow::on_btnCancel_clicked()
{
    this->close();
}
