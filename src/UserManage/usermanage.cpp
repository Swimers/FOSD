#include "usermanage.h"
#include "ui_usermanage.h"
#include <QDebug>
#include <QMessageBox>
#include <QSettings>

#include "QString"

UserManage* UserManage::m_UserManage = nullptr;

UserManage::UserManage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::UserManage)
{
    ui->setupUi(this);
    this->setWindowTitle("用户管理界面");

    setWindowFlags(Qt::MSWindowsFixedSizeDialogHint);
    ReadInit("username", m_userNameList);
    ReadInit("password", m_passwdList);
    ui->cbox_MUsername->addItems(m_userNameList);
}

UserManage::~UserManage()
{
    delete ui;
    if(this->settings){
        delete settings;
        this->settings = nullptr;
    }
}

UserManage* UserManage::getInstance()
{
    if (!m_UserManage) {
        m_UserManage = new UserManage();
    }
    return m_UserManage;
}

void UserManage::ReadInit(const QString &key, QStringList &value) {
    //value.clear();
    QString path = "login.ini";
    QSettings config(path, QSettings::IniFormat);
    QVariant variant = config.value(QString("config/") + key);
    value = variant.toStringList();
    qDebug() << "Read" << key << ":" << value;  // 调试信息
}

void UserManage::WriteInit(const QString &key, const QStringList &value) {
    QString path = "login.ini";
    QSettings config(path, QSettings::IniFormat);
    config.beginGroup("config");
    config.setValue(key, value);
    config.endGroup();
    qDebug() << "Write" << key << ":" << value;  // 调试信息
}

void UserManage::on_pbtn_UserSet_clicked()
{
    QString username = ui->cbox_MUsername->currentText();
    QString oldPW = ui->edit_OldPassword->text();
    QString newPW = ui->edit_NewPassword->text();
    int currentIndex = m_userNameList.indexOf(username);
    if(oldPW != m_passwdList.at(currentIndex)){
        QMessageBox::warning(this, "警告", "旧密码错误，无法更改!");
        return;
    }
    if (ui->edit_NewPassword->text().isEmpty()) {
        QMessageBox::warning(this, "注册失败", "密码不能为空");
        return;
    }
    m_passwdList.replace(currentIndex,newPW);
    WriteInit("username", m_userNameList);
    WriteInit("password", m_passwdList);
    close();
}
