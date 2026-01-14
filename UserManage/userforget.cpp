#include "userforget.h"
#include "ui_userforget.h"

UserForget* UserForget::m_UserForget = nullptr;

UserForget::UserForget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::UserForget)
{
    ui->setupUi(this);

    this->setWindowTitle("用户修改");
}

UserForget::~UserForget()
{
    delete ui;
}

UserForget *UserForget::getInstance()
{
    if (!m_UserForget) {
        m_UserForget = new UserForget();
    }
    return m_UserForget;
}
