#ifndef USERFORGET_H
#define USERFORGET_H

#include <QWidget>

namespace Ui {
class UserForget;
}

class UserForget : public QWidget
{
    Q_OBJECT

public:
    ~UserForget();

    UserForget(const UserForget&) = delete;               //删除拷贝函数
    UserForget& operator=(const UserForget&) = delete;    //删除拷贝运算符
    static UserForget* getInstance();                     //声明获取单例方法

private:
    explicit UserForget(QWidget *parent = nullptr);
    Ui::UserForget *ui;
    static UserForget * m_UserForget;                      //声明静态指针
};

#endif // USERFORGET_H
