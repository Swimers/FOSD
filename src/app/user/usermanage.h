#ifndef USERMANAGE_H
#define USERMANAGE_H

#include <QWidget>

#include <QSettings>

namespace Ui {
class UserManage;
}

class UserManage : public QWidget
{
    Q_OBJECT

public:
    ~UserManage();

    UserManage(const UserManage&) = delete;               //删除拷贝函数
    UserManage& operator=(const UserManage&) = delete;    //删除拷贝运算符
    static UserManage* getInstance();                     //声明获取单例方法

    void ReadInit(const QString &key, QStringList &value);
    void WriteInit(const QString &key, const QStringList &value);


private slots:
    void on_pbtn_UserSet_clicked();

private:
    explicit UserManage(QWidget *parent = nullptr);        //私有化构建函数
    Ui::UserManage *ui;
    static UserManage* m_UserManage;                      //声明静态指针
    QSettings *settings = nullptr;

    QStringList m_passwdList;
    QStringList m_userNameList;
};

#endif // USERMANAGE_H
