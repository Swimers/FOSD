#include "login.h"
#include "ui_login.h"

#include <QApplication>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QRegularExpression>
#include <QSettings>
#include <QWidget>

loginwindow* loginwindow::m_loginwindow = nullptr;

loginwindow::loginwindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::loginwindow)
{
    ui->setupUi(this);

    setWindowFlags(Qt::FramelessWindowHint | windowFlags());
    setAttribute(Qt::WA_TranslucentBackground);
    setWindowIcon(QIcon(":/icon/icon/favicon.ico"));

    bindUiControls();
    LoginInit();
}

loginwindow::~loginwindow()
{
    delete ui;
}

loginwindow* loginwindow::getInstance()
{
    if (!m_loginwindow) {
        m_loginwindow = new loginwindow();
    }
    return m_loginwindow;
}

void loginwindow::bindUiControls()
{
    m_titleLabel = ui->label_Title;
    m_hintLabel = ui->label_Hint;
    m_userSelector = ui->comboBox;
    m_usernameEdit = ui->edit_Username;
    m_passwordEdit = ui->edit_Password;
    m_confirmPasswordEdit = ui->edit_ConfirmPassword;
    m_privilegeBox = ui->cbox_Privilege;
    m_rememberNameCheck = ui->check_RemName;
    m_rememberPasswordCheck = ui->check_RemPass;
    m_loginButton = ui->btn_Login;
    m_registerButton = ui->btn_Register;
    m_resetButton = ui->btn_Forget;
    m_clearButton = ui->btn_Clear;

    m_userSelector->setEditable(true);
    m_userSelector->lineEdit()->setPlaceholderText("Username");
    m_privilegeBox->setItemData(0, "1");
    m_privilegeBox->setItemData(1, "2");
    m_privilegeBox->setItemData(2, "3");

    // setupUi() uses Qt auto-connect for on_<objectName>_<signal>() slots.
    // Keep only connections that do not have an auto-connect slot name.
    connect(m_passwordEdit, &QLineEdit::returnPressed, this, &loginwindow::on_btn_Login_clicked);
    connect(m_confirmPasswordEdit, &QLineEdit::returnPressed, this, &loginwindow::on_btn_Login_clicked);
}

void loginwindow::ReadInit(const QString &key, QStringList &value)
{
    QSettings config("login.ini", QSettings::IniFormat);
    value = config.value(QString("config/") + key).toStringList();
}

void loginwindow::WriteInit(const QString &key, const QStringList &value)
{
    QSettings config("login.ini", QSettings::IniFormat);
    config.beginGroup("config");
    config.setValue(key, value);
    config.endGroup();
}

bool loginwindow::loadUsers()
{
    ReadInit("username", m_userNameList);
    ReadInit("password", m_passwdList);
    ReadInit("privilege", m_privilegeList);
    ReadInit("isrembered", m_isremberList);

    const int count = m_userNameList.size();
    while (m_passwdList.size() < count) m_passwdList.append("");
    while (m_privilegeList.size() < count) m_privilegeList.append("3");
    while (m_isremberList.size() < count) m_isremberList.append("false");

    if (m_passwdList.size() > count) m_passwdList = m_passwdList.mid(0, count);
    if (m_privilegeList.size() > count) m_privilegeList = m_privilegeList.mid(0, count);
    if (m_isremberList.size() > count) m_isremberList = m_isremberList.mid(0, count);

    if (count == 0) {
        m_userNameList << "admin";
        m_passwdList << "admin";
        m_privilegeList << "1";
        m_isremberList << "false";
        saveUsers();
    }

    return true;
}

void loginwindow::saveUsers()
{
    WriteInit("username", m_userNameList);
    WriteInit("password", m_passwdList);
    WriteInit("privilege", m_privilegeList);
    WriteInit("isrembered", m_isremberList);
}

void loginwindow::refreshUserSelector()
{
    m_userSelector->blockSignals(true);
    m_userSelector->clear();
    m_userSelector->addItems(m_userNameList);
    m_userSelector->blockSignals(false);
}

void loginwindow::LoginInit()
{
    loadUsers();
    refreshUserSelector();
    read_json();

    if (!m_userNameList.isEmpty()) {
        m_userSelector->setCurrentIndex(0);
        on_comboBox_currentIndexChanged(0);
    }

    switchMode(AuthMode::Login);
}

void loginwindow::switchMode(AuthMode mode)
{
    m_mode = mode;
    const bool loginMode = mode == AuthMode::Login;
    const bool registerMode = mode == AuthMode::Register;
    const bool resetMode = mode == AuthMode::ResetPassword;

    m_userSelector->setVisible(loginMode);
    m_usernameEdit->setVisible(!loginMode);
    m_confirmPasswordEdit->setVisible(registerMode || resetMode);
    m_privilegeBox->setVisible(registerMode);
    m_rememberNameCheck->setVisible(loginMode);
    m_rememberPasswordCheck->setVisible(loginMode);

    if (loginMode) {
        m_titleLabel->setText("FOSD User Login");
        m_hintLabel->setText("Enter your account credentials to open the monitoring system.");
        m_loginButton->setText("Login");
        m_registerButton->setText("Create account");
        m_resetButton->setText("Reset password");
    } else if (registerMode) {
        m_titleLabel->setText("Create Account");
        m_hintLabel->setText("Create a local user. The account is saved to the existing login.ini file.");
        m_loginButton->setText("Register");
        m_registerButton->setText("Back to login");
        m_resetButton->setText("Reset password");
        m_usernameEdit->setText(m_userSelector->currentText().trimmed());
        m_passwordEdit->clear();
        m_confirmPasswordEdit->clear();
        m_usernameEdit->setFocus();
    } else {
        m_titleLabel->setText("Reset Password");
        m_hintLabel->setText("Enter an existing username and confirm the new password.");
        m_loginButton->setText("Save password");
        m_registerButton->setText("Create account");
        m_resetButton->setText("Back to login");
        m_usernameEdit->setText(m_userSelector->currentText().trimmed());
        m_passwordEdit->clear();
        m_confirmPasswordEdit->clear();
        m_usernameEdit->setFocus();
    }
}

void loginwindow::read_json()
{
    QFile file(QApplication::applicationDirPath() + "/config.json");
    if (!file.open(QIODevice::ReadOnly)) {
        return;
    }

    const QJsonObject obj = QJsonDocument::fromJson(file.readAll()).object();
    message_init(obj.value("SAVE_NAME").toString(), obj.value("SAVE_PASSWORD").toString());
}

void loginwindow::message_init(QString flag1, QString flag2)
{
    if (m_rememberNameCheck) {
        m_rememberNameCheck->setChecked(flag1 == "1");
    }
    if (m_rememberPasswordCheck) {
        m_rememberPasswordCheck->setChecked(flag2 == "1");
    }
}

void loginwindow::write_json()
{
    QFile file(QApplication::applicationDirPath() + "/config.json");
    if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
        return;
    }

    QJsonObject obj;
    obj["SAVE_NAME"] = m_rememberNameCheck && m_rememberNameCheck->isChecked() ? "1" : "0";
    obj["SAVE_PASSWORD"] = m_rememberPasswordCheck && m_rememberPasswordCheck->isChecked() ? "1" : "0";

    file.write(QJsonDocument(obj).toJson());
    file.flush();
}

int loginwindow::userIndex(const QString &name) const
{
    return m_userNameList.indexOf(name.trimmed());
}

bool loginwindow::isValidUsername(const QString &name) const
{
    static const QRegularExpression re("^[A-Za-z0-9_\\-.]{3,32}$");
    return re.match(name).hasMatch();
}

bool loginwindow::isValidPassword(const QString &pass) const
{
    return pass.size() >= 4 && pass.size() <= 64;
}

QString loginwindow::selectedPrivilege() const
{
    return m_privilegeBox->currentData().toString();
}

void loginwindow::showMessage(const QString &text, bool error)
{
    Q_UNUSED(error);
    m_hintLabel->setText(text);
}

void loginwindow::mousePressEvent(QMouseEvent *e)
{
    if (e->button() == Qt::LeftButton && isDragArea(e->pos())) {
        m_dragging = true;
        p = e->globalPos() - frameGeometry().topLeft();
        e->accept();
        return;
    }
    QMainWindow::mousePressEvent(e);
}

void loginwindow::mouseMoveEvent(QMouseEvent *e)
{
    if (m_dragging && (e->buttons() & Qt::LeftButton)) {
        move(e->globalPos() - p);
        e->accept();
        return;
    }
    QMainWindow::mouseMoveEvent(e);
}

void loginwindow::mouseReleaseEvent(QMouseEvent *e)
{
    if (e->button() == Qt::LeftButton) {
        m_dragging = false;
    }
    QMainWindow::mouseReleaseEvent(e);
}

bool loginwindow::isDragArea(const QPoint &pos) const
{
    QWidget *child = childAt(pos);
    if (child && child != centralWidget() && child != ui->centralWidget) {
        return false;
    }

    return pos.y() <= 56;
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
    loadUsers();

    if (m_mode == AuthMode::Login) {
        username = m_userSelector->currentText().trimmed();
        password = m_passwordEdit->text();
        const int index = userIndex(username);

        if (index < 0) {
            showMessage("Username does not exist.", true);
            m_passwordEdit->clear();
            return;
        }
        if (m_passwdList.value(index) != password && m_passwdList.value(index) != "NOPASSWORD") {
            showMessage("Password is incorrect.", true);
            m_passwordEdit->clear();
            return;
        }

        privilege = m_privilegeList.value(index, "3");
        isrembered = m_rememberPasswordCheck->isChecked() ? "true" : "false";

        m_userNameList.move(index, 0);
        m_passwdList.move(index, 0);
        m_privilegeList.move(index, 0);
        m_isremberList.move(index, 0);
        m_isremberList[0] = isrembered;
        saveUsers();
        write_json();

        m_MainWindow = new OldMainWindow(privilege);
        m_MainWindow->show();
        close();
        return;
    }

    username = m_usernameEdit->text().trimmed();
    password = m_passwordEdit->text();
    const QString confirmPassword = m_confirmPasswordEdit->text();

    if (!isValidUsername(username)) {
        showMessage("Username must be 3-32 characters: letters, numbers, dot, dash or underscore.", true);
        return;
    }
    if (!isValidPassword(password)) {
        showMessage("Password must be 4-64 characters.", true);
        return;
    }
    if (password != confirmPassword) {
        showMessage("The two passwords do not match.", true);
        return;
    }

    const int index = userIndex(username);
    if (m_mode == AuthMode::Register) {
        if (index >= 0) {
            showMessage("Username already exists.", true);
            return;
        }
        m_userNameList.append(username);
        m_passwdList.append(password);
        m_privilegeList.append(selectedPrivilege());
        m_isremberList.append("false");
        saveUsers();
        refreshUserSelector();
        m_userSelector->setCurrentText(username);
        switchMode(AuthMode::Login);
        showMessage("Account created. You can login now.");
        return;
    }

    if (index < 0) {
        showMessage("Username does not exist.", true);
        return;
    }

    m_passwdList[index] = password;
    m_isremberList[index] = "false";
    saveUsers();
    refreshUserSelector();
    m_userSelector->setCurrentText(username);
    switchMode(AuthMode::Login);
    showMessage("Password updated. Please login with the new password.");
}

void loginwindow::on_btn_Clear_clicked()
{
    if (m_mode == AuthMode::Login) {
        m_userSelector->setCurrentText("");
    } else {
        m_usernameEdit->clear();
        m_confirmPasswordEdit->clear();
    }
    m_passwordEdit->clear();
    showMessage("Input cleared.");
}

void loginwindow::on_btn_Register_clicked()
{
    switchMode(m_mode == AuthMode::Register ? AuthMode::Login : AuthMode::Register);
}

void loginwindow::on_btn_Forget_clicked()
{
    switchMode(m_mode == AuthMode::ResetPassword ? AuthMode::Login : AuthMode::ResetPassword);
}

void loginwindow::on_comboBox_activated(const QString &arg1)
{
    if (m_mode != AuthMode::Login) {
        return;
    }
    m_usernameEdit->setText(arg1);
}

void loginwindow::on_check_RemPass_clicked()
{
    isrembered = m_rememberPasswordCheck->isChecked() ? "true" : "false";
}

void loginwindow::on_comboBox_currentIndexChanged(int index)
{
    if (index < 0 || index >= m_userNameList.size()) {
        m_passwordEdit->clear();
        return;
    }

    m_usernameEdit->setText(m_userNameList.at(index));
    const bool remembered = m_isremberList.value(index) == "true";
    m_rememberPasswordCheck->setChecked(remembered);
    if (remembered) {
        m_passwordEdit->setText(m_passwdList.value(index));
    } else {
        m_passwordEdit->clear();
    }
}
