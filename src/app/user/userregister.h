#ifndef USERREGISTER_H
#define USERREGISTER_H

#include <QWidget>

namespace Ui {
class UserRegister;
}

class UserRegister : public QWidget
{
    Q_OBJECT

public:
    ~UserRegister();

    UserRegister(const UserRegister&) = delete;               //删除拷贝函数
    UserRegister& operator=(const UserRegister&) = delete;    //删除拷贝运算符
    static UserRegister* getInstance();                       //声明获取单例方法

signals:
    void registerSuccess(); // 新增信号声明

private slots:
    void on_pbtn_RegisterSet_clicked();

private:
    explicit UserRegister(QWidget *parent = nullptr);
    Ui::UserRegister *ui;
    static UserRegister * m_UserRegister;                      //声明静态指针

private:
    void ReadInit(const QString &key, QStringList &value) ;
    void WriteInit(const QString &key, const QStringList &value);
};

#endif // USERREGISTER_H
