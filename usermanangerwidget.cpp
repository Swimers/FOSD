#include "usermanangerwidget.h"
#include "ui_usermanangerwidget.h"
#include <QDebug>
#include <QMessageBox>
#include <QSettings>

#pragma execution_character_set("utf-8")

UserManangerWidget::UserManangerWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::UserManangerWidget)
{
    ui->setupUi(this);


    setWindowFlags(Qt::MSWindowsFixedSizeDialogHint);

    ReadInit("username", m_userNameList);
    ReadInit("password", m_passwdList);

    ui->comboBox->addItems(m_userNameList);


}


UserManangerWidget::~UserManangerWidget()
{
    delete ui;

    if(this->settings){
        delete settings;
        this->settings = nullptr;
    }
}


//读账号密码
void UserManangerWidget::ReadInit(const QString &key, QStringList &value) {
    //value.clear();
    QString path = "login.ini";
    QSettings config(path, QSettings::IniFormat);
    QVariant variant = config.value(QString("config/") + key);
    value = variant.toStringList();
    qDebug() << "Read" << key << ":" << value;  // 调试信息
}

void UserManangerWidget::WriteInit(const QString &key, const QStringList &value) {
    QString path = "login.ini";
    QSettings config(path, QSettings::IniFormat);
    config.beginGroup("config");
    config.setValue(key, value);
    config.endGroup();
    qDebug() << "Write" << key << ":" << value;  // 调试信息
}


void UserManangerWidget::on_btn_log_clicked()
{
    QString username = ui->comboBox->currentText();
    QString oldPW = ui->edit_oldPW->text();
    QString newPW = ui->edit_newPW->text();



    int currentIndex = m_userNameList.indexOf(username);

    if(oldPW != m_passwdList.at(currentIndex))
    {
        QMessageBox::warning(this, "警告", "旧密码错误，无法更改!");
        return;
    }

    m_passwdList.replace(currentIndex,newPW);

    WriteInit("username", m_userNameList);
    WriteInit("password", m_passwdList);

    close();

}
