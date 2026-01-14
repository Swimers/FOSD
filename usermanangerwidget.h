#ifndef USERMANANGERWIDGET_H
#define USERMANANGERWIDGET_H

#include <QWidget>

#include <QSettings>

namespace Ui {
class UserManangerWidget;
}

class UserManangerWidget : public QWidget
{
    Q_OBJECT

public:
    explicit UserManangerWidget(QWidget *parent = nullptr);
    ~UserManangerWidget();

    void ReadInit(const QString &key, QStringList &value);
    void WriteInit(const QString &key, const QStringList &value);
private slots:
    void on_btn_log_clicked();

private:
    Ui::UserManangerWidget *ui;

    QSettings *settings = nullptr;

    QStringList m_passwdList;
    QStringList m_userNameList;
};

#endif // USERMANANGERWIDGET_H
