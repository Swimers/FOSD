/********************************************************************************
** Form generated from reading UI file 'login.ui'
**
** Created by: Qt User Interface Compiler version 5.12.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_LOGIN_H
#define UI_LOGIN_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_loginwindow
{
public:
    QWidget *centralWidget;
    QGridLayout *gridLayout;
    QFrame *frame;
    QVBoxLayout *verticalLayout_2;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer;
    QToolButton *btn_Min;
    QToolButton *btn_Close;
    QLabel *label;
    QHBoxLayout *horizontalLayout_2;
    QSpacerItem *horizontalSpacer_2;
    QToolButton *btn_Icon;
    QSpacerItem *horizontalSpacer_3;
    QSpacerItem *verticalSpacer_3;
    QComboBox *comboBox;
    QLineEdit *edit_Username;
    QLineEdit *edit_Password;
    QSpacerItem *verticalSpacer_2;
    QHBoxLayout *horizontalLayout_3;
    QCheckBox *check_RemName;
    QCheckBox *check_RemPass;
    QSpacerItem *horizontalSpacer_4;
    QPushButton *btn_Clear;
    QLabel *label_3;
    QPushButton *btn_Register;
    QSpacerItem *verticalSpacer;
    QHBoxLayout *horizontalLayout_4;
    QSpacerItem *horizontalSpacer_5;
    QPushButton *btn_Login;
    QSpacerItem *horizontalSpacer_6;
    QSpacerItem *verticalSpacer_4;

    void setupUi(QMainWindow *loginwindow)
    {
        if (loginwindow->objectName().isEmpty())
            loginwindow->setObjectName(QString::fromUtf8("loginwindow"));
        loginwindow->resize(392, 500);
        loginwindow->setStyleSheet(QString::fromUtf8("\n"
"QCheckBox{\n"
"font-size:17px;\n"
"background:rgba(85,170,255,0);\n"
"color:grey;\n"
"font-style:MingLiU-ExtB;\n"
"}\n"
"QComboBox{\n"
"background:transparent;\n"
"border:none;\n"
"color:#717072;\n"
"border-bottom:1px solid #717072;\n"
"}\n"
"QComboBox QAbstractItemView\n"
"{\n"
"color:#717072;\n"
"border: 1px solid white;\n"
"}\n"
"QMessageBox\n"
"{\n"
"background-color:white;\n"
"}\n"
"QMessageBox QPushButton\n"
"{\n"
"background-color:rgb(197, 199, 200);\n"
"}\n"
"QMessageBox QLabel\n"
"{\n"
"color:black;\n"
"}\n"
"\n"
"QPushButton, QToolButton, QCommandLinkButton{\n"
"    padding: 0 5px 0 5px;\n"
"    border-style: solid;\n"
"    border-top-color: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 #c1c9cf, stop:1 #d2d8dd);\n"
"    border-right-color: qlineargradient(spread:pad, x1:1, y1:0, x2:0, y2:0, stop:0 #c1c9cf, stop:1 #d2d8dd);\n"
"    border-bottom-color: qlineargradient(spread:pad, x1:0, y1:1, x2:0, y2:0, stop:0 #c1c9cf, stop:1 #d2d8dd);\n"
"    border-left-color: qlineargradient(sprea"
                        "d:pad, x1:0, y1:0, x2:1, y2:0, stop:0 #c1c9cf, stop:1 #d2d8dd);\n"
"    border-width: 2px;\n"
"    border-radius: 8px;\n"
"    color: #616161;\n"
"    font-weight: bold;\n"
"    background-color: qlineargradient(spread:pad, x1:0.5, y1:0, x2:0.5, y2:1, stop:0 #fbfdfd, stop:0.5 #ffffff, stop:1 #fbfdfd);\n"
"}\n"
"QPushButton::default, QToolButton::default, QCommandLinkButton::default{\n"
"    border: 2px solid transparent;\n"
"    color: #FFFFFF;\n"
"    background-color: qlineargradient(spread:pad, x1:0.5, y1:0, x2:0.5, y2:1, stop:0 #84afe5, stop:1 #1168e4);\n"
"}\n"
"QPushButton:hover, QToolButton:hover, QCommandLinkButton:hover{\n"
"    color: #3d3d3d;\n"
"}\n"
"QPushButton:pressed, QToolButton:pressed, QCommandLinkButton:pressed{\n"
"    color: #aeaeae;\n"
"    background-color: qlineargradient(spread:pad, x1:0.5, y1:0, x2:0.5, y2:1, stop:0 #ffffff, stop:0.5 #fbfdfd, stop:1 #ffffff);\n"
"}\n"
"QPushButton:disabled, QToolButton:disabled, QCommandLinkButton:disabled{\n"
"    color: #616161;\n"
"    background-"
                        "color: qlineargradient(spread:pad, x1:0.5, y1:0, x2:0.5, y2:1, stop:0 #dce7eb, stop:0.5 #e0e8eb, stop:1 #dee7ec);\n"
"}\n"
""));
        centralWidget = new QWidget(loginwindow);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        gridLayout = new QGridLayout(centralWidget);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        frame = new QFrame(centralWidget);
        frame->setObjectName(QString::fromUtf8("frame"));
        frame->setStyleSheet(QString::fromUtf8("background-image: url(:/backgrounds/Resource/backgrounds/jianbian_background.png);"));
        frame->setFrameShape(QFrame::StyledPanel);
        frame->setFrameShadow(QFrame::Raised);
        verticalLayout_2 = new QVBoxLayout(frame);
        verticalLayout_2->setSpacing(10);
        verticalLayout_2->setContentsMargins(11, 11, 11, 11);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        verticalLayout_2->setContentsMargins(20, -1, 20, -1);
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        btn_Min = new QToolButton(frame);
        btn_Min->setObjectName(QString::fromUtf8("btn_Min"));

        horizontalLayout->addWidget(btn_Min);

        btn_Close = new QToolButton(frame);
        btn_Close->setObjectName(QString::fromUtf8("btn_Close"));

        horizontalLayout->addWidget(btn_Close);


        verticalLayout_2->addLayout(horizontalLayout);

        label = new QLabel(frame);
        label->setObjectName(QString::fromUtf8("label"));

        verticalLayout_2->addWidget(label);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_2);

        btn_Icon = new QToolButton(frame);
        btn_Icon->setObjectName(QString::fromUtf8("btn_Icon"));
        btn_Icon->setMinimumSize(QSize(120, 120));
        btn_Icon->setStyleSheet(QString::fromUtf8("QToolButton\n"
"{\n"
"border-radius:60px;\n"
"}"));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/icon/Resource/icon/UserIcon.png"), QSize(), QIcon::Normal, QIcon::Off);
        btn_Icon->setIcon(icon);
        btn_Icon->setIconSize(QSize(60, 60));

        horizontalLayout_2->addWidget(btn_Icon);

        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_3);


        verticalLayout_2->addLayout(horizontalLayout_2);

        verticalSpacer_3 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_2->addItem(verticalSpacer_3);

        comboBox = new QComboBox(frame);
        comboBox->setObjectName(QString::fromUtf8("comboBox"));
        comboBox->setMinimumSize(QSize(0, 40));
        QFont font;
        font.setPointSize(11);
        font.setBold(false);
        font.setItalic(false);
        font.setWeight(50);
        comboBox->setFont(font);
        comboBox->setStyleSheet(QString::fromUtf8("font: 11pt ;"));

        verticalLayout_2->addWidget(comboBox);

        edit_Username = new QLineEdit(frame);
        edit_Username->setObjectName(QString::fromUtf8("edit_Username"));
        edit_Username->setMinimumSize(QSize(0, 40));
        edit_Username->setBaseSize(QSize(4, 0));
        QFont font1;
        font1.setPointSize(11);
        edit_Username->setFont(font1);

        verticalLayout_2->addWidget(edit_Username);

        edit_Password = new QLineEdit(frame);
        edit_Password->setObjectName(QString::fromUtf8("edit_Password"));
        edit_Password->setMinimumSize(QSize(0, 40));
        edit_Password->setBaseSize(QSize(4, 0));
        edit_Password->setFont(font1);

        verticalLayout_2->addWidget(edit_Password);

        verticalSpacer_2 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_2->addItem(verticalSpacer_2);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setSpacing(6);
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        check_RemName = new QCheckBox(frame);
        check_RemName->setObjectName(QString::fromUtf8("check_RemName"));
        check_RemName->setStyleSheet(QString::fromUtf8("background: transparent;\n"
"font: 10pt \"\351\273\221\344\275\223\";"));

        horizontalLayout_3->addWidget(check_RemName);

        check_RemPass = new QCheckBox(frame);
        check_RemPass->setObjectName(QString::fromUtf8("check_RemPass"));
        QFont font2;
        font2.setFamily(QString::fromUtf8("\351\273\221\344\275\223"));
        font2.setPointSize(10);
        font2.setBold(false);
        font2.setItalic(false);
        font2.setWeight(50);
        check_RemPass->setFont(font2);
        check_RemPass->setStyleSheet(QString::fromUtf8("background: transparent;\n"
"font: 10pt \"\351\273\221\344\275\223\";"));

        horizontalLayout_3->addWidget(check_RemPass);

        horizontalSpacer_4 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_4);

        btn_Clear = new QPushButton(frame);
        btn_Clear->setObjectName(QString::fromUtf8("btn_Clear"));
        btn_Clear->setMinimumSize(QSize(0, 30));
        QFont font3;
        font3.setFamily(QString::fromUtf8("Algerian"));
        font3.setBold(true);
        font3.setWeight(75);
        btn_Clear->setFont(font3);
        btn_Clear->setStyleSheet(QString::fromUtf8("background: transparent;\n"
""));
        btn_Clear->setFlat(false);

        horizontalLayout_3->addWidget(btn_Clear);

        label_3 = new QLabel(frame);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setMaximumSize(QSize(10, 16777215));

        horizontalLayout_3->addWidget(label_3);

        btn_Register = new QPushButton(frame);
        btn_Register->setObjectName(QString::fromUtf8("btn_Register"));
        btn_Register->setMinimumSize(QSize(0, 30));
        btn_Register->setStyleSheet(QString::fromUtf8("background: transparent;"));
        btn_Register->setFlat(false);

        horizontalLayout_3->addWidget(btn_Register);


        verticalLayout_2->addLayout(horizontalLayout_3);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_2->addItem(verticalSpacer);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setSpacing(6);
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        horizontalSpacer_5 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer_5);

        btn_Login = new QPushButton(frame);
        btn_Login->setObjectName(QString::fromUtf8("btn_Login"));
        btn_Login->setMinimumSize(QSize(140, 30));
        btn_Login->setFont(font3);
        btn_Login->setStyleSheet(QString::fromUtf8(""));

        horizontalLayout_4->addWidget(btn_Login);

        horizontalSpacer_6 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer_6);


        verticalLayout_2->addLayout(horizontalLayout_4);

        verticalSpacer_4 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_2->addItem(verticalSpacer_4);


        gridLayout->addWidget(frame, 0, 0, 1, 1);

        loginwindow->setCentralWidget(centralWidget);

        retranslateUi(loginwindow);

        QMetaObject::connectSlotsByName(loginwindow);
    } // setupUi

    void retranslateUi(QMainWindow *loginwindow)
    {
        loginwindow->setWindowTitle(QApplication::translate("loginwindow", "MainWindow", nullptr));
        btn_Min->setText(QApplication::translate("loginwindow", "-", nullptr));
        btn_Close->setText(QApplication::translate("loginwindow", "X", nullptr));
        label->setText(QApplication::translate("loginwindow", "Logo", nullptr));
        btn_Icon->setText(QApplication::translate("loginwindow", "...", nullptr));
        edit_Username->setPlaceholderText(QApplication::translate("loginwindow", "Username", nullptr));
        edit_Password->setPlaceholderText(QApplication::translate("loginwindow", "Password", nullptr));
        check_RemName->setText(QApplication::translate("loginwindow", "\350\256\260\344\275\217\350\264\246\345\217\267", nullptr));
        check_RemPass->setText(QApplication::translate("loginwindow", "\350\256\260\344\275\217\345\257\206\347\240\201", nullptr));
        btn_Clear->setText(QApplication::translate("loginwindow", "\351\207\215\347\275\256", nullptr));
        label_3->setText(QApplication::translate("loginwindow", " |", nullptr));
        btn_Register->setText(QApplication::translate("loginwindow", "\347\224\250\346\210\267\346\263\250\345\206\214", nullptr));
        btn_Login->setText(QApplication::translate("loginwindow", "\347\231\273\345\275\225", nullptr));
    } // retranslateUi

};

namespace Ui {
    class loginwindow: public Ui_loginwindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LOGIN_H
