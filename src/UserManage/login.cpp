#include "login.h"
#include "ui_login.h"
#include "mainwindow.h"

loginwindow* loginwindow::m_loginwindow = nullptr;

loginwindow::loginwindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::loginwindow)
{
    ui->setupUi(this);

    ui->edit_Password->setEchoMode(QLineEdit::Password);            //输入的时候就显示圆点
    setWindowFlags(Qt::FramelessWindowHint | windowFlags());        //去窗口边框
    setAttribute(Qt::WA_TranslucentBackground);                     //把窗口背景设置为透明;
    setWindowIcon(QIcon(":/icon/Resource/icon/favicon.ico"));

    read_json();
    LoginInit();

    connect(ui->comboBox,SIGNAL(currentTextChanged(QString)),ui->edit_Password,SLOT(clear()));//如果用户名变了，密码就清除掉
}

void loginwindow::ReadInit(const QString &key, QStringList &value)
{
    value.clear();
    QString path = "login.ini";
    QSettings config(path, QSettings::IniFormat);
    QVariant variant = config.value(QString("config/") + key);
    value = variant.toStringList();
    qDebug() << "Read" << key << ":" << value;  // 调试信息
}

void loginwindow::WriteInit(const QString &key, const QStringList &value)
{
    QString path = "login.ini";
    QSettings config(path, QSettings::IniFormat);
    config.beginGroup("config");
    config.setValue(key, value);
    config.endGroup();
    qDebug() << "Write" << key << ":" << value;  // 调试信息
}

loginwindow* loginwindow::getInstance()
{
    if (!m_loginwindow) {
        m_loginwindow = new loginwindow();
    }
    return m_loginwindow;
}

void loginwindow::LoginInit()
{
    read_json();
    ui->comboBox->setEditable(true);//不加这条指令会空指针异常
    ui->comboBox->lineEdit()->setPlaceholderText(QStringLiteral("Username"));//没有输入用户名时，隐式显示Username

    m_userNameList.clear();
    m_passwdList.clear();
    m_isremberList.clear();
    m_privilegeList.clear();

    ui->comboBox->blockSignals(true);//true屏蔽信号
    ui->comboBox->clear();
    ui->comboBox->blockSignals(false);

    ReadInit("username", m_userNameList);
    ReadInit("password", m_passwdList);
    ReadInit("privilege", m_privilegeList);  // 区分账号特权
    ReadInit("isrembered", m_isremberList);

    if (!m_userNameList.isEmpty())  username = m_userNameList.at(0);
    if (!m_passwdList.isEmpty())    password = m_passwdList.at(0);
    if (!m_privilegeList.isEmpty()) privilege = m_privilegeList.at(0);
    if (!m_isremberList.isEmpty())  isrembered = m_isremberList.at(0);

    ui->comboBox->addItems(m_userNameList);
    ui->comboBox->setCurrentText(username);  // 附上列表里的第一个用户

    if (isrembered == "true") {
        ui->check_RemPass->setChecked(true);  // check_pw框打钩
        ui->edit_Password->setText(password);  // 附上列表里第一个用户的密码
    } else {
        ui->check_RemPass->setChecked(false);
        ui->edit_Password->clear();
    }
    ui->edit_Username->setText(ui->comboBox->currentText());
}

void loginwindow::read_json()
{
    //打开文件
    QFile file(QApplication::applicationDirPath()+"/config.json");
    if(!file.open(QIODevice::ReadOnly)) {
        //qDebug() << "File open failed!";
    } else {
        //qDebug() <<"File open successfully!";
    }
    QJsonDocument jdc(QJsonDocument::fromJson(file.readAll()));
    QJsonObject obj = jdc.object();
    QString save_name_flag=obj.value("SAVE_NAME").toString();
    QString save_password_flag=obj.value("SAVE_PASSWORD").toString();
    message_init(save_name_flag,save_password_flag);
}

void loginwindow::message_init(QString flag1,QString flag2)
{
    //根据json内容决定是否填充输入框
    //qDebug() << flag1 << "^^^" << flag2 ;
    if (flag1 == "1")
    {
        ui->edit_Username->setText(m_password);
        ui->check_RemName->setChecked(true);
    }
    if(flag2 == "1")
    {
        ui->edit_Password->setText(m_username);
        ui->check_RemPass->setChecked(true);
    }
}

// 更新json文件
void loginwindow::write_json()
{
    QFile file(QApplication::applicationDirPath()+"/config.json");
    if(!file.open(QIODevice::WriteOnly)) {
        //qDebug() << "File open failed!";
    } else {
        //qDebug() <<"File open successfully!";
    }
    QJsonObject obj;
    bool flag = ui->check_RemName->isChecked();
    if(flag == true)
    {
        obj["SAVE_NAME"] = "1";
    }
    else
        obj["SAVE_NAME"] = "0";
    flag = ui->check_RemPass->isChecked();
    if(flag == true)
    {
        obj["SAVE_PASSWORD"] = "1";
    }
    else
        obj["SAVE_PASSWORD"] = "0";
    QJsonDocument jdoc(obj);
    file.write(jdoc.toJson());
    file.flush();
}

loginwindow::~loginwindow()
{
    delete ui;
}

void loginwindow::mousePressEvent(QMouseEvent *e)
{
    if(e->button() == Qt::LeftButton)
    {
        p = e->globalPos() - this->frameGeometry().topLeft();   //求坐标差值:当前点击坐标-窗口左上角坐标
    }
}

void loginwindow::mouseMoveEvent(QMouseEvent *e)
{
    if(e->buttons() & Qt::LeftButton)
    {
        move(e->globalPos() - p);       //移到左上角
    }

}

void loginwindow::on_btn_Min_clicked()
{
    showMinimized();
}

void loginwindow::on_btn_Close_clicked()
{
    close();
}

void loginwindow::on_btn_Login_clicked()
{
    username = ui->edit_Username->text(); // 从输入框获取账号
    password = ui->edit_Password->text();  // 从输入框获取密码
    bool isRember = ui->check_RemPass->checkState();

    if (m_userNameList.contains(username)) {
        for (int i = 0; i < m_userNameList.size(); ++i) {
            if (m_userNameList.at(i) == username) {
                if (m_passwdList.at(i) == password || m_passwdList.at(i) == "NOPASSWORD") {
                    // 用户名和密码匹配
                    privilege = m_privilegeList.at(i);
                    if(isRember)
                        m_isremberList.replace(i,"true");
                    else {
                        m_isremberList.replace(i,"false");
                    }
                    //m_isremberList.replace(i,isRember);
                    isrembered = m_isremberList.at(i);

                    // 移动到列表最前面
                    m_userNameList.removeAt(i);
                    m_passwdList.removeAt(i);
                    m_privilegeList.removeAt(i);
                    m_isremberList.removeAt(i);

                    m_userNameList.push_front(username);
                    m_passwdList.push_front(password);
                    m_privilegeList.push_front(privilege);
                    m_isremberList.push_front(isrembered);

                    WriteInit("username", m_userNameList);
                    WriteInit("password", m_passwdList);
                    WriteInit("privilege", m_privilegeList);
                    WriteInit("isrembered", m_isremberList);

                    MainWindow * m = new MainWindow(privilege);
                    m->show();
                    this->close();  //关闭当前主窗口
                    write_json();

                    return;
                } else {
                    // 密码错误
                    QMessageBox::critical(this, "错误", "密码错误", QMessageBox::Ok);
                    ui->edit_Password->clear();
                    return;
                }
            }
        }
    } else {
        // 用户名不存在
        qDebug() << "用户名不存在";
        QMessageBox::critical(this, "错误", "用户名不存在", QMessageBox::Ok);
        ui->comboBox->clearEditText();
        ui->edit_Password->clear();
    }
}

void loginwindow::on_btn_Clear_clicked()
{
    ui->edit_Password->clear(); //清空密码栏
    ui->edit_Username->clear(); //清空账号栏
}

void loginwindow::on_btn_Register_clicked()
{
    UserRegister * m_UserRegister = UserRegister::getInstance();
    m_UserRegister ->show();
    connect(m_UserRegister, &UserRegister::registerSuccess, this, &loginwindow::LoginInit);
}

void loginwindow::on_btn_Forget_clicked()
{
    UserForget * m_UserForget = UserForget::getInstance();
    m_UserForget ->show();
}

void loginwindow::on_comboBox_activated(const QString &arg1)
{
    ui->edit_Username->setText(arg1);
}

void loginwindow::on_check_RemPass_clicked()
{
    if(ui->check_RemPass->isChecked()){
        isrembered = "true";
    }else{
        isrembered = "false";
    }
}

void loginwindow::on_comboBox_currentIndexChanged(int index)
{
    if(m_isremberList.at(index) == "true"){
        ui->check_RemPass->setChecked(true);
        ui->edit_Password->setText(m_passwdList.at(index));
    }
    else {
        ui->check_RemPass->setChecked(false);
        ui->edit_Password->clear();
    }
}
