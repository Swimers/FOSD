#include "userregister.h"
#include "ui_userregister.h"
#include "login.h"

#include <QSettings>
#include <QMessageBox>
#include <QRegularExpression>
#include <QDebug>


UserRegister* UserRegister::m_UserRegister = nullptr;

UserRegister::UserRegister(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::UserRegister)
{
    ui->setupUi(this);

    this->setWindowTitle("用户注册");
    ui->edit_NewPassword->setEchoMode(QLineEdit::Password);
    ui->edit_NewPasswordS->setEchoMode(QLineEdit::Password);
}

UserRegister::~UserRegister()
{
    delete ui;
}

UserRegister *UserRegister::getInstance()
{
    if (!m_UserRegister) {
        m_UserRegister = new UserRegister();
    }
    return m_UserRegister;
}

void UserRegister::on_pbtn_RegisterSet_clicked()
{
    QString username = ui->edit_NewUsername->text().trimmed();
    QString password = ui->edit_NewPassword->text();
    QString confirmPassword = ui->edit_NewPasswordS->text();
    QString privilegeText = ui->cbox_SelectNewPri->currentText();
    int privilegeValue = ui->cbox_SelectNewPri->currentData().toInt();
    privilegeValue++;
    QString privilege = QString::number(privilegeValue);
    if (username.isEmpty()) {
        QMessageBox::warning(this, "注册失败", "用户名不能为空|");
        return;
    }
    if (password.isEmpty()) {
        QMessageBox::warning(this, "注册失败", "密码不能为空|");
        return;
    }
    if (username.isEmpty() || password.isEmpty()) {
        QMessageBox::warning(this, "注册失败", "用户名和密码不能为空");
        return;
    }
    if (password != confirmPassword) {
        QMessageBox::warning(this, "注册失败", "两次输入的密码不一致|");
        return;
    }
    QStringList usernames, passwords, privileges, isRemembered;
    ReadInit("username", usernames);
    ReadInit("password", passwords);
    ReadInit("privilege", privileges);
    ReadInit("isrembered", isRemembered);
    if (usernames.contains(username)) {
        QMessageBox::warning(this, "注册失败", "用户名已存在");
        return;
    }
    usernames.append(username);
    passwords.append(password);
    privileges.append(privilege);
    isRemembered.append("false"); // 默认不记住密码
    WriteInit("username", usernames);
    WriteInit("password", passwords);
    WriteInit("privilege", privileges);
    WriteInit("isrembered", isRemembered);
    QMessageBox::information(this, "注册成功", QString("用户 %1 注册成功，权限为：%2").arg(username).arg(privilegeText));
    ui->edit_NewUsername->clear();
    ui->edit_NewPassword->clear();
    ui->edit_NewPasswordS->clear();
    emit registerSuccess();
}

void UserRegister::ReadInit(const QString &key, QStringList &value)
{
    value.clear();
    QString path = "data/config/login.ini";
    QSettings config(path, QSettings::IniFormat);
    QVariant variant = config.value(QString("config/") + key);
    value = variant.toStringList();
    qDebug() << "Read" << key << ":" << value;  // 调试信息
}

void UserRegister::WriteInit(const QString &key, const QStringList &value)
{
    QString path = "data/config/login.ini";
    QSettings config(path, QSettings::IniFormat);
    config.beginGroup("config");
    config.setValue(key, value);
    config.endGroup();
    qDebug() << "Write" << key << ":" << value;  // 调试信息
}
