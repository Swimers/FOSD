#ifndef LOGWIDGET_H
#define LOGWIDGET_H
#include <QWidget>
#include <QMessageBox>
#include <QFile>
#include <QDebug>
#include <QByteArray>
#include <QSettings>
#include "mainwindow.h"


namespace Ui {
class LogWidget;
}

class LogWidget : public QWidget
{
    Q_OBJECT

public:
    ~LogWidget();
    
    void func_init(); //功能初始化
    void paintEvent(QPaintEvent *event);
    void loadcfg();
    void user_clear();

    static LogWidget* instance();  // 添加静态方法获取实例

protected:
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    bool eventFilter(QObject *obj, QEvent *event);


private slots:
    void on_btn_log_clicked();//登录按钮按下之后触发的事件
    void setlogin(int);
    void on_check_pw_clicked();//改变记住密码的标志位

    void on_comboBox_currentIndexChanged(int index);

public slots:
    void pw_clear();//清空密码框


private:
    explicit LogWidget(QWidget *parent = 0);
    Ui::LogWidget *ui;
    static LogWidget* logWidgetInstance;  // 静态单例实例


    QString privilege;
    QString admin_username;
    QString admin_password;

    QString username;
    QString password;
    QPoint p;

    void ReadInit(const QString &key, QStringList &value) ;
    void WriteInit(const QString &key, const QStringList &value);
    QStringList m_passwdList;
    QStringList m_isremberList;
    QStringList m_userNameList;
    QStringList m_privilegeList;
    QString isrembered;
};
#endif // LOGWIDGET_H
