#include "disp2dpara.h"
#include "ui_disp2dpara.h"

#include <QLineEdit>
#include <QIntValidator>
#include <QDoubleValidator>

disp2dpara::disp2dpara(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::disp2dpara)
{
    setWindowFlags(Qt::FramelessWindowHint | windowFlags());        //去窗口边框

    ui->setupUi(this);
    setWindowFlags(windowFlags() | Qt::MSWindowsFixedSizeDialogHint);

    QIntValidator *intValidator = new QIntValidator(this);  //整数过滤
    QDoubleValidator *doubleValidator = new QDoubleValidator(this); //浮点数过滤
    ui->edit_Sensor1Order->setValidator(intValidator);
    ui->edit_Sensor2Order->setValidator(intValidator);
    ui->edit_Sensor3Order->setValidator(intValidator);
    ui->edit_Sensor4Order->setValidator(intValidator);
    ui->edit_Sensor5Order->setValidator(intValidator);
    ui->edit_Sensor6Order->setValidator(intValidator);
    ui->edit_Sensor7Order->setValidator(intValidator);
    ui->edit_Sensor8Order->setValidator(intValidator);
    ui->edit_Sensor9Order->setValidator(intValidator);
    ui->edit_Sensor10Order->setValidator(intValidator);
    ui->edit_Sensor11Order->setValidator(intValidator);
    ui->edit_Sensor12Order->setValidator(intValidator);

    doubleValidator->setNotation(QDoubleValidator::StandardNotation);
    ui->edit_Sensor1Para->setValidator(doubleValidator);
    ui->edit_Sensor2Para->setValidator(doubleValidator);
    ui->edit_Sensor3Para->setValidator(doubleValidator);
    ui->edit_Sensor4Para->setValidator(doubleValidator);
    ui->edit_Sensor5Para->setValidator(doubleValidator);
    ui->edit_Sensor6Para->setValidator(doubleValidator);
    ui->edit_Sensor7Para->setValidator(doubleValidator);
    ui->edit_Sensor8Para->setValidator(doubleValidator);
    ui->edit_Sensor9Para->setValidator(doubleValidator);
    ui->edit_Sensor10Para->setValidator(doubleValidator);
    ui->edit_Sensor11Para->setValidator(doubleValidator);
    ui->edit_Sensor12Para->setValidator(doubleValidator);

    ui->edit_Xaxis->setValidator(doubleValidator);
    ui->edit_Yaxis->setValidator(doubleValidator);
    ui->edit_Zaxis->setValidator(doubleValidator);
}

disp2dpara::~disp2dpara()
{
    delete ui;
}

void disp2dpara::init()
{
    ui->edit_Sensor1Order->setText(QString::number(ife->order[0]));
    ui->edit_Sensor2Order->setText(QString::number(ife->order[1]));
    ui->edit_Sensor3Order->setText(QString::number(ife->order[2]));
    ui->edit_Sensor4Order->setText(QString::number(ife->order[3]));
    ui->edit_Sensor5Order->setText(QString::number(ife->order[4]));
    ui->edit_Sensor6Order->setText(QString::number(ife->order[5]));
    ui->edit_Sensor7Order->setText(QString::number(ife->order[6]));
    ui->edit_Sensor8Order->setText(QString::number(ife->order[7]));
    ui->edit_Sensor9Order->setText(QString::number(ife->order[8]));
    ui->edit_Sensor10Order->setText(QString::number(ife->order[9]));
    ui->edit_Sensor11Order->setText(QString::number(ife->order[10]));
    ui->edit_Sensor12Order->setText(QString::number(ife->order[11]));

    ui->edit_Sensor1Para->setText(QString::number(ife->para[0]));
    ui->edit_Sensor2Para->setText(QString::number(ife->para[1]));
    ui->edit_Sensor3Para->setText(QString::number(ife->para[2]));
    ui->edit_Sensor4Para->setText(QString::number(ife->para[3]));
    ui->edit_Sensor5Para->setText(QString::number(ife->para[4]));
    ui->edit_Sensor6Para->setText(QString::number(ife->para[5]));
    ui->edit_Sensor7Para->setText(QString::number(ife->para[6]));
    ui->edit_Sensor8Para->setText(QString::number(ife->para[7]));
    ui->edit_Sensor9Para->setText(QString::number(ife->para[8]));
    ui->edit_Sensor10Para->setText(QString::number(ife->para[9]));
    ui->edit_Sensor11Para->setText(QString::number(ife->para[10]));
    ui->edit_Sensor12Para->setText(QString::number(ife->para[11]));

    ui->edit_Xaxis->setText(QString::number(ife->axis[0]));
    ui->edit_Yaxis->setText(QString::number(ife->axis[1]));
    ui->edit_Zaxis->setText(QString::number(ife->axis[2]));
}


void disp2dpara::on_pbtn_SenerSet_clicked()
{
    ife->order[0] = ui->edit_Sensor1Order->text().toInt();
    ife->order[1] = ui->edit_Sensor2Order->text().toInt();
    ife->order[2] = ui->edit_Sensor3Order->text().toInt();
    ife->order[3] = ui->edit_Sensor4Order->text().toInt();
    ife->order[4] = ui->edit_Sensor5Order->text().toInt();
    ife->order[5] = ui->edit_Sensor6Order->text().toInt();
    ife->order[6] = ui->edit_Sensor7Order->text().toInt();
    ife->order[7] = ui->edit_Sensor8Order->text().toInt();
    ife->order[8] = ui->edit_Sensor9Order->text().toInt();
    ife->order[9] = ui->edit_Sensor10Order->text().toInt();
    ife->order[10] = ui->edit_Sensor11Order->text().toInt();
    ife->order[11] = ui->edit_Sensor12Order->text().toInt();

    ife->para[0] = ui->edit_Sensor1Para->text().toDouble();
    ife->para[1] = ui->edit_Sensor2Para->text().toDouble();
    ife->para[2] = ui->edit_Sensor3Para->text().toDouble();
    ife->para[3] = ui->edit_Sensor4Para->text().toDouble();
    ife->para[4] = ui->edit_Sensor5Para->text().toDouble();
    ife->para[5] = ui->edit_Sensor6Para->text().toDouble();
    ife->para[6] = ui->edit_Sensor7Para->text().toDouble();
    ife->para[7] = ui->edit_Sensor8Para->text().toDouble();
    ife->para[8] = ui->edit_Sensor9Para->text().toDouble();
    ife->para[9] = ui->edit_Sensor10Para->text().toDouble();
    ife->para[10] = ui->edit_Sensor11Para->text().toDouble();
    ife->para[11] = ui->edit_Sensor12Para->text().toDouble();

    ife->axis[0] = ui->edit_Xaxis->text().toDouble();
    ife->axis[1] = ui->edit_Yaxis->text().toDouble();
    ife->axis[2] = ui->edit_Zaxis->text().toDouble();

    ife->saveConfig("ifepara.ini");
    this->close();
    emit DataSeted();
}

void disp2dpara::on_pbtn_SenerCancel_clicked()
{
    this->close();
}
