#ifndef LOGIN_H
#define LOGIN_H

#include <QMainWindow>

#include "userregister.h"
#include "userforget.h"
#include "mainwindow.h"

#include "QMouseEvent"
#include "QDebug"
#include "QMessageBox"
#include "QJsonObject"
#include "QJsonDocument"
#include "QFile"
#include "QByteArray"

namespace Ui {
class loginwindow;
}

class loginwindow : public QMainWindow
{
    Q_OBJECT

public:
    ~loginwindow();

    loginwindow(const loginwindow&) = delete;               //删除拷贝函数
    loginwindow& operator=(const loginwindow&) = delete;    //删除拷贝运算符
    static loginwindow* getInstance();                      //声明获取单例方法

public:
    void read_json();                                       //读json
    void write_json();                                      //写json
    void message_init(QString flag1,QString flag2);         //根据json内容决定是否填充输入框

signals:
    void login();
    void close_window();

protected:
    void mouseMoveEvent(QMouseEvent *e);
    void mousePressEvent(QMouseEvent *e);

private slots:
    void on_btn_Min_clicked();
    void on_btn_Close_clicked();
    void on_btn_Login_clicked();
    void on_btn_Clear_clicked();
    void on_btn_Register_clicked();
    void on_btn_Forget_clicked();
    void on_comboBox_activated(const QString &arg1);
    void on_check_RemPass_clicked();
    void on_comboBox_currentIndexChanged(int index);
    void LoginInit();

private:
    explicit loginwindow(QWidget *parent = nullptr);        //私有化构建函数
    Ui::loginwindow *ui;
    static loginwindow* m_loginwindow;                      //声明静态指针

    QPoint p;

    UserForget * m_UserForget;
    UserRegister * m_UserRegister;
    MainWindow * m_MainWindow;

private:
    void ReadInit(const QString &key, QStringList &value) ;
    void WriteInit(const QString &key, const QStringList &value);

    QString privilege;
    QString admin_username;
    QString admin_password;
    QString username;
    QString password;

    QStringList m_passwdList;
    QStringList m_isremberList;
    QStringList m_userNameList;
    QStringList m_privilegeList;
    QString isrembered;

    QString m_username;
    QString m_password;
};

#endif // LOGIN_H
