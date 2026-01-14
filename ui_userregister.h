/********************************************************************************
** Form generated from reading UI file 'userregister.ui'
**
** Created by: Qt User Interface Compiler version 5.12.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_USERREGISTER_H
#define UI_USERREGISTER_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_UserRegister
{
public:
    QGridLayout *gridLayout;
    QVBoxLayout *verticalLayout;
    QLabel *label;
    QLineEdit *edit_NewUsername;
    QLabel *label_2;
    QLineEdit *edit_NewPassword;
    QLabel *label_3;
    QLineEdit *edit_NewPasswordS;
    QLabel *label_4;
    QComboBox *cbox_SelectNewPri;
    QSpacerItem *verticalSpacer;
    QHBoxLayout *horizontalLayout;
    QPushButton *pbtn_RegisterSet;
    QPushButton *pbtn_RegisterClear;

    void setupUi(QWidget *UserRegister)
    {
        if (UserRegister->objectName().isEmpty())
            UserRegister->setObjectName(QString::fromUtf8("UserRegister"));
        UserRegister->resize(268, 381);
        gridLayout = new QGridLayout(UserRegister);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(10);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(20, 20, 20, 20);
        label = new QLabel(UserRegister);
        label->setObjectName(QString::fromUtf8("label"));

        verticalLayout->addWidget(label);

        edit_NewUsername = new QLineEdit(UserRegister);
        edit_NewUsername->setObjectName(QString::fromUtf8("edit_NewUsername"));
        edit_NewUsername->setMinimumSize(QSize(0, 30));

        verticalLayout->addWidget(edit_NewUsername);

        label_2 = new QLabel(UserRegister);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        verticalLayout->addWidget(label_2);

        edit_NewPassword = new QLineEdit(UserRegister);
        edit_NewPassword->setObjectName(QString::fromUtf8("edit_NewPassword"));
        edit_NewPassword->setMinimumSize(QSize(0, 30));

        verticalLayout->addWidget(edit_NewPassword);

        label_3 = new QLabel(UserRegister);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        verticalLayout->addWidget(label_3);

        edit_NewPasswordS = new QLineEdit(UserRegister);
        edit_NewPasswordS->setObjectName(QString::fromUtf8("edit_NewPasswordS"));
        edit_NewPasswordS->setMinimumSize(QSize(0, 30));

        verticalLayout->addWidget(edit_NewPasswordS);

        label_4 = new QLabel(UserRegister);
        label_4->setObjectName(QString::fromUtf8("label_4"));

        verticalLayout->addWidget(label_4);

        cbox_SelectNewPri = new QComboBox(UserRegister);
        cbox_SelectNewPri->addItem(QString());
        cbox_SelectNewPri->addItem(QString());
        cbox_SelectNewPri->addItem(QString());
        cbox_SelectNewPri->setObjectName(QString::fromUtf8("cbox_SelectNewPri"));
        cbox_SelectNewPri->setMinimumSize(QSize(0, 30));

        verticalLayout->addWidget(cbox_SelectNewPri);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(20);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setContentsMargins(20, -1, 20, -1);
        pbtn_RegisterSet = new QPushButton(UserRegister);
        pbtn_RegisterSet->setObjectName(QString::fromUtf8("pbtn_RegisterSet"));
        pbtn_RegisterSet->setMinimumSize(QSize(0, 30));

        horizontalLayout->addWidget(pbtn_RegisterSet);

        pbtn_RegisterClear = new QPushButton(UserRegister);
        pbtn_RegisterClear->setObjectName(QString::fromUtf8("pbtn_RegisterClear"));
        pbtn_RegisterClear->setMinimumSize(QSize(0, 30));

        horizontalLayout->addWidget(pbtn_RegisterClear);


        verticalLayout->addLayout(horizontalLayout);


        gridLayout->addLayout(verticalLayout, 0, 0, 1, 1);


        retranslateUi(UserRegister);

        QMetaObject::connectSlotsByName(UserRegister);
    } // setupUi

    void retranslateUi(QWidget *UserRegister)
    {
        UserRegister->setWindowTitle(QApplication::translate("UserRegister", "Form", nullptr));
        label->setText(QApplication::translate("UserRegister", "\347\224\250\346\210\267\345\220\215", nullptr));
        edit_NewUsername->setText(QString());
        edit_NewUsername->setPlaceholderText(QApplication::translate("UserRegister", "Username", nullptr));
        label_2->setText(QApplication::translate("UserRegister", "\345\257\206\347\240\201", nullptr));
        edit_NewPassword->setText(QString());
        edit_NewPassword->setPlaceholderText(QApplication::translate("UserRegister", "Password", nullptr));
        label_3->setText(QApplication::translate("UserRegister", "\347\241\256\350\256\244\345\257\206\347\240\201", nullptr));
        edit_NewPasswordS->setText(QString());
        edit_NewPasswordS->setPlaceholderText(QApplication::translate("UserRegister", "Password", nullptr));
        label_4->setText(QApplication::translate("UserRegister", "\347\224\250\346\210\267\346\235\203\351\231\220", nullptr));
        cbox_SelectNewPri->setItemText(0, QApplication::translate("UserRegister", "\347\256\241\347\220\206\345\221\230", nullptr));
        cbox_SelectNewPri->setItemText(1, QApplication::translate("UserRegister", "\347\224\250\346\210\267", nullptr));
        cbox_SelectNewPri->setItemText(2, QApplication::translate("UserRegister", "\346\270\270\345\256\242", nullptr));

        pbtn_RegisterSet->setText(QApplication::translate("UserRegister", "\346\263\250\345\206\214", nullptr));
        pbtn_RegisterClear->setText(QApplication::translate("UserRegister", "\351\207\215\347\275\256", nullptr));
    } // retranslateUi

};

namespace Ui {
    class UserRegister: public Ui_UserRegister {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_USERREGISTER_H
