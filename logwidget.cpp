#include "logwidget.h"
#include "ui_logwidget.h"
#include<qpainter.h>
#include<QMouseEvent>
#include<QMessageBox>
#include<QWidget>
#include<QMainWindow>
#include "mainwindow.h"
#pragma execution_character_set("utf-8")

LogWidget* LogWidget::logWidgetInstance = nullptr;

LogWidget::LogWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LogWidget)
{
    ui->setupUi(this);

    func_init();
    loadcfg();


    ui->comboBox->installEventFilter(this);
    this->setWindowTitle("登录");
    this->setWindowFlags(this->windowFlags()&~Qt::WindowMaximizeButtonHint);
    this->setFixedSize(this->width(),this->height());
    //记住密码
    connect(ui->comboBox,SIGNAL(currentTextChanged(QString)),ui->edit_pw,SLOT(clear()));//如果用户名变了，密码就清除掉
}

LogWidget::~LogWidget()
{
    delete ui;
    qDebug()<<"登录界面销毁";
}

LogWidget* LogWidget::instance()
{
    if (!logWidgetInstance) {
        logWidgetInstance = new LogWidget();
    }
    return logWidgetInstance;
}

void LogWidget::func_init()
{
    ui->comboBox->setEditable(true);//不加这条指令会空指针异常
    ui->comboBox->lineEdit()->setPlaceholderText(QStringLiteral("Username"));//没有输入用户名时，隐式显示Username

    m_userNameList.clear();
    m_passwdList.clear();
    m_isremberList.clear();
    m_privilegeList.clear();
}



void LogWidget::loadcfg() {
    ui->comboBox->blockSignals(true);//true屏蔽信号
    ui->comboBox->clear();
    ui->comboBox->blockSignals(false);


    ReadInit("username", m_userNameList);
    ReadInit("password", m_passwdList);
    ReadInit("privilege", m_privilegeList);  // 区分账号特权
    ReadInit("isrembered", m_isremberList);

    if (!m_userNameList.isEmpty()) {
        username = m_userNameList.at(0);
    }
    if (!m_passwdList.isEmpty()) {
        password = m_passwdList.at(0);
    }
    if (!m_privilegeList.isEmpty()) {
        privilege = m_privilegeList.at(0);
    }
    if (!m_isremberList.isEmpty()) {
        isrembered = m_isremberList.at(0);
    }

    ui->comboBox->addItems(m_userNameList);
    ui->comboBox->setCurrentText(username);  // 附上列表里的第一个用户

    if (isrembered == "true") {
        ui->check_pw->setChecked(true);  // check_pw框打钩
        ui->edit_pw->setText(password);  // 附上列表里第一个用户的密码
    } else {
        ui->check_pw->setChecked(false);
        ui->edit_pw->clear();
    }
}

void LogWidget::setlogin(int index) {
    if (m_passwdList.size() > index) {
        password = m_passwdList.at(index);
        isrembered = m_isremberList.at(index);
        if (isrembered == "true") {
            ui->check_pw->setChecked(true);
            ui->edit_pw->setText(password);
        } else {
            ui->check_pw->setChecked(false);
            ui->edit_pw->clear();
        }
    }
}
void LogWidget::on_btn_log_clicked() {
    username = ui->comboBox->currentText(); // 从输入框获取账号
    password = ui->edit_pw->text();  // 从输入框获取密码
    bool isRember = ui->check_pw->checkState();

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

                    MainWindow* m = new MainWindow(privilege);
                    m->show();
                    //m->SetPrivilege(privilege);
                    this->hide();  //关闭当前主窗口

                    return;
                } else {
                    // 密码错误
                    QMessageBox::critical(this, "错误", "密码错误", QMessageBox::Ok);
                    ui->edit_pw->clear();
                    return;
                }
            }
        }
    } else {
        // 用户名不存在
        qDebug() << "用户名不存在";
        QMessageBox::critical(this, "错误", "用户名不存在", QMessageBox::Ok);
        ui->comboBox->clearEditText();
        ui->edit_pw->clear();
    }

}


//读账号密码
void LogWidget::ReadInit(const QString &key, QStringList &value) {
    value.clear();
    QString path = "login.ini";
    QSettings config(path, QSettings::IniFormat);
    QVariant variant = config.value(QString("config/") + key);
    value = variant.toStringList();
    qDebug() << "Read" << key << ":" << value;  // 调试信息
}

void LogWidget::WriteInit(const QString &key, const QStringList &value) {
    QString path = "login.ini";
    QSettings config(path, QSettings::IniFormat);
    config.beginGroup("config");
    config.setValue(key, value);
    config.endGroup();
    qDebug() << "Write" << key << ":" << value;  // 调试信息
}

void LogWidget::on_check_pw_clicked()//改变记住密码的标志位
{
    if(ui->check_pw->isChecked())
    {
        isrembered = "true";
    }else{
        isrembered = "false";
    }
}

void LogWidget::pw_clear()//清空密码框
{
    ui->edit_pw->clear();
    ui->check_pw->setChecked(false);
    isrembered="flase";
}
void LogWidget::user_clear()//清空用户框
{
    ui->comboBox->clear();
}

void LogWidget::paintEvent(QPaintEvent *event)
{
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget,&opt,&p,this);
}


void LogWidget::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
     {
         //求坐标差值
         //当前点击坐标-窗口左上角坐标
         p = event->globalPos() - this->frameGeometry().topLeft();
     }
}
void LogWidget::mouseMoveEvent(QMouseEvent *event)
{
    if(event->buttons() & Qt::LeftButton)
       {
           //移到左上角
           move(event->globalPos() - p);
       }
}

bool LogWidget::eventFilter(QObject *obj, QEvent *event)
{
    if(obj == ui->comboBox)
    {
        if(event->type() == QEvent::MouseMove)
    {
            return true;
        }
    }
    return QWidget::eventFilter(obj,event);
}



void LogWidget::on_comboBox_currentIndexChanged(int index)
{
    if(m_isremberList.at(index) == "true"){
        ui->check_pw->setChecked(true);
        ui->edit_pw->setText(m_passwdList.at(index));
    }
    else {
        ui->check_pw->setChecked(false);
        ui->edit_pw->clear();
    }
}
