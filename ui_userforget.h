/********************************************************************************
** Form generated from reading UI file 'userforget.ui'
**
** Created by: Qt User Interface Compiler version 5.12.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_USERFORGET_H
#define UI_USERFORGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_UserForget
{
public:
    QVBoxLayout *verticalLayout;
    QVBoxLayout *verticalLayout_2;
    QLabel *label;
    QLineEdit *edit_NewPassword1_2;
    QLabel *label_2;
    QComboBox *cbox_SelectPri;
    QLabel *label_3;
    QLineEdit *edit_NewPassword1;
    QLabel *label_4;
    QLineEdit *edit_NewPassword2;
    QSpacerItem *verticalSpacer;
    QHBoxLayout *horizontalLayout;
    QPushButton *pbtn_ForgetSet;
    QPushButton *pbtn_ForgetClear;

    void setupUi(QWidget *UserForget)
    {
        if (UserForget->objectName().isEmpty())
            UserForget->setObjectName(QString::fromUtf8("UserForget"));
        UserForget->resize(279, 378);
        verticalLayout = new QVBoxLayout(UserForget);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setSpacing(10);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        verticalLayout_2->setContentsMargins(20, 10, 20, 1);
        label = new QLabel(UserForget);
        label->setObjectName(QString::fromUtf8("label"));
        label->setMinimumSize(QSize(0, 20));

        verticalLayout_2->addWidget(label);

        edit_NewPassword1_2 = new QLineEdit(UserForget);
        edit_NewPassword1_2->setObjectName(QString::fromUtf8("edit_NewPassword1_2"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(edit_NewPassword1_2->sizePolicy().hasHeightForWidth());
        edit_NewPassword1_2->setSizePolicy(sizePolicy);
        edit_NewPassword1_2->setMinimumSize(QSize(0, 30));

        verticalLayout_2->addWidget(edit_NewPassword1_2);

        label_2 = new QLabel(UserForget);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        verticalLayout_2->addWidget(label_2);

        cbox_SelectPri = new QComboBox(UserForget);
        cbox_SelectPri->setObjectName(QString::fromUtf8("cbox_SelectPri"));
        QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(cbox_SelectPri->sizePolicy().hasHeightForWidth());
        cbox_SelectPri->setSizePolicy(sizePolicy1);
        cbox_SelectPri->setMinimumSize(QSize(0, 30));

        verticalLayout_2->addWidget(cbox_SelectPri);

        label_3 = new QLabel(UserForget);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        verticalLayout_2->addWidget(label_3);

        edit_NewPassword1 = new QLineEdit(UserForget);
        edit_NewPassword1->setObjectName(QString::fromUtf8("edit_NewPassword1"));
        sizePolicy.setHeightForWidth(edit_NewPassword1->sizePolicy().hasHeightForWidth());
        edit_NewPassword1->setSizePolicy(sizePolicy);
        edit_NewPassword1->setMinimumSize(QSize(0, 30));

        verticalLayout_2->addWidget(edit_NewPassword1);

        label_4 = new QLabel(UserForget);
        label_4->setObjectName(QString::fromUtf8("label_4"));

        verticalLayout_2->addWidget(label_4);

        edit_NewPassword2 = new QLineEdit(UserForget);
        edit_NewPassword2->setObjectName(QString::fromUtf8("edit_NewPassword2"));
        sizePolicy.setHeightForWidth(edit_NewPassword2->sizePolicy().hasHeightForWidth());
        edit_NewPassword2->setSizePolicy(sizePolicy);
        edit_NewPassword2->setMinimumSize(QSize(0, 30));

        verticalLayout_2->addWidget(edit_NewPassword2);

        verticalSpacer = new QSpacerItem(20, 87, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_2->addItem(verticalSpacer);


        verticalLayout->addLayout(verticalLayout_2);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(10);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setContentsMargins(20, 0, 20, -1);
        pbtn_ForgetSet = new QPushButton(UserForget);
        pbtn_ForgetSet->setObjectName(QString::fromUtf8("pbtn_ForgetSet"));
        QSizePolicy sizePolicy2(QSizePolicy::Minimum, QSizePolicy::Preferred);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(pbtn_ForgetSet->sizePolicy().hasHeightForWidth());
        pbtn_ForgetSet->setSizePolicy(sizePolicy2);
        pbtn_ForgetSet->setMinimumSize(QSize(0, 30));

        horizontalLayout->addWidget(pbtn_ForgetSet);

        pbtn_ForgetClear = new QPushButton(UserForget);
        pbtn_ForgetClear->setObjectName(QString::fromUtf8("pbtn_ForgetClear"));
        sizePolicy2.setHeightForWidth(pbtn_ForgetClear->sizePolicy().hasHeightForWidth());
        pbtn_ForgetClear->setSizePolicy(sizePolicy2);
        pbtn_ForgetClear->setMinimumSize(QSize(0, 30));

        horizontalLayout->addWidget(pbtn_ForgetClear);


        verticalLayout->addLayout(horizontalLayout);


        retranslateUi(UserForget);

        QMetaObject::connectSlotsByName(UserForget);
    } // setupUi

    void retranslateUi(QWidget *UserForget)
    {
        UserForget->setWindowTitle(QApplication::translate("UserForget", "Form", nullptr));
        label->setText(QApplication::translate("UserForget", "\347\224\250\346\210\267\345\220\215", nullptr));
        edit_NewPassword1_2->setText(QString());
        edit_NewPassword1_2->setPlaceholderText(QApplication::translate("UserForget", "Password", nullptr));
        label_2->setText(QApplication::translate("UserForget", "\347\224\250\346\210\267\346\235\203\351\231\220", nullptr));
        label_3->setText(QApplication::translate("UserForget", "\346\226\260\345\257\206\347\240\201", nullptr));
        edit_NewPassword1->setText(QString());
        edit_NewPassword1->setPlaceholderText(QApplication::translate("UserForget", "Password", nullptr));
        label_4->setText(QApplication::translate("UserForget", "\347\241\256\350\256\244\346\226\260\345\257\206\347\240\201", nullptr));
        edit_NewPassword2->setText(QString());
        edit_NewPassword2->setPlaceholderText(QApplication::translate("UserForget", "Password", nullptr));
        pbtn_ForgetSet->setText(QApplication::translate("UserForget", "\347\241\256\345\256\232", nullptr));
        pbtn_ForgetClear->setText(QApplication::translate("UserForget", "\346\270\205\351\231\244", nullptr));
    } // retranslateUi

};

namespace Ui {
    class UserForget: public Ui_UserForget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_USERFORGET_H
