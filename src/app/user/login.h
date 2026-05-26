#ifndef LOGIN_H
#define LOGIN_H

#include <QCheckBox>
#include <QComboBox>
#include <QLabel>
#include <QLineEdit>
#include <QMainWindow>
#include <QMouseEvent>
#include <QPushButton>
#include <QStringList>

#include "oldmainwindow.h"

namespace Ui {
class loginwindow;
}

class loginwindow : public QMainWindow
{
    Q_OBJECT

public:
    ~loginwindow();

    loginwindow(const loginwindow&) = delete;
    loginwindow& operator=(const loginwindow&) = delete;
    static loginwindow* getInstance();

public:
    void read_json();
    void write_json();
    void message_init(QString flag1, QString flag2);

signals:
    void login();
    void close_window();

protected:
    void mouseMoveEvent(QMouseEvent *e) override;
    void mousePressEvent(QMouseEvent *e) override;
    void mouseReleaseEvent(QMouseEvent *e) override;

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
    enum class AuthMode {
        Login,
        Register,
        ResetPassword
    };

    explicit loginwindow(QWidget *parent = nullptr);

    void bindUiControls();
    void switchMode(AuthMode mode);
    void refreshUserSelector();
    bool loadUsers();
    void saveUsers();
    int userIndex(const QString &name) const;
    bool isValidUsername(const QString &name) const;
    bool isValidPassword(const QString &pass) const;
    QString selectedPrivilege() const;
    void showMessage(const QString &text, bool error = false);
    bool isDragArea(const QPoint &pos) const;
    void ReadInit(const QString &key, QStringList &value);
    void WriteInit(const QString &key, const QStringList &value);

private:
    Ui::loginwindow *ui = nullptr;
    static loginwindow* m_loginwindow;

    QPoint p;
    bool m_dragging = false;
    OldMainWindow *m_MainWindow = nullptr;

    QString privilege;
    QString username;
    QString password;

    QStringList m_passwdList;
    QStringList m_isremberList;
    QStringList m_userNameList;
    QStringList m_privilegeList;
    QString isrembered;

    QString m_username;
    QString m_password;

    AuthMode m_mode = AuthMode::Login;
    QComboBox *m_userSelector = nullptr;
    QLineEdit *m_usernameEdit = nullptr;
    QLineEdit *m_passwordEdit = nullptr;
    QLineEdit *m_confirmPasswordEdit = nullptr;
    QComboBox *m_privilegeBox = nullptr;
    QCheckBox *m_rememberNameCheck = nullptr;
    QCheckBox *m_rememberPasswordCheck = nullptr;
    QPushButton *m_loginButton = nullptr;
    QPushButton *m_registerButton = nullptr;
    QPushButton *m_resetButton = nullptr;
    QPushButton *m_clearButton = nullptr;
    QLabel *m_titleLabel = nullptr;
    QLabel *m_hintLabel = nullptr;
};

#endif // LOGIN_H
