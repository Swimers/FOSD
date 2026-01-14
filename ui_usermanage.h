/********************************************************************************
** Form generated from reading UI file 'usermanage.ui'
**
** Created by: Qt User Interface Compiler version 5.12.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_USERMANAGE_H
#define UI_USERMANAGE_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_UserManage
{
public:
    QGridLayout *gridLayout_2;
    QFrame *frame;
    QGridLayout *gridLayout;
    QVBoxLayout *verticalLayout;
    QLabel *label;
    QComboBox *cbox_MUsername;
    QLabel *label_2;
    QLineEdit *edit_OldPassword;
    QLabel *label_3;
    QLineEdit *edit_NewPassword;
    QSpacerItem *verticalSpacer;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer;
    QPushButton *pbtn_UserSet;

    void setupUi(QWidget *UserManage)
    {
        if (UserManage->objectName().isEmpty())
            UserManage->setObjectName(QString::fromUtf8("UserManage"));
        UserManage->resize(289, 354);
        gridLayout_2 = new QGridLayout(UserManage);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        frame = new QFrame(UserManage);
        frame->setObjectName(QString::fromUtf8("frame"));
        frame->setFrameShape(QFrame::StyledPanel);
        frame->setFrameShadow(QFrame::Raised);
        gridLayout = new QGridLayout(frame);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        label = new QLabel(frame);
        label->setObjectName(QString::fromUtf8("label"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(label->sizePolicy().hasHeightForWidth());
        label->setSizePolicy(sizePolicy);

        verticalLayout->addWidget(label);

        cbox_MUsername = new QComboBox(frame);
        cbox_MUsername->setObjectName(QString::fromUtf8("cbox_MUsername"));
        sizePolicy.setHeightForWidth(cbox_MUsername->sizePolicy().hasHeightForWidth());
        cbox_MUsername->setSizePolicy(sizePolicy);
        cbox_MUsername->setMinimumSize(QSize(0, 30));

        verticalLayout->addWidget(cbox_MUsername);

        label_2 = new QLabel(frame);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        sizePolicy.setHeightForWidth(label_2->sizePolicy().hasHeightForWidth());
        label_2->setSizePolicy(sizePolicy);

        verticalLayout->addWidget(label_2);

        edit_OldPassword = new QLineEdit(frame);
        edit_OldPassword->setObjectName(QString::fromUtf8("edit_OldPassword"));
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Preferred);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(edit_OldPassword->sizePolicy().hasHeightForWidth());
        edit_OldPassword->setSizePolicy(sizePolicy1);
        edit_OldPassword->setMinimumSize(QSize(0, 30));
        edit_OldPassword->setEchoMode(QLineEdit::Password);

        verticalLayout->addWidget(edit_OldPassword);

        label_3 = new QLabel(frame);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        sizePolicy.setHeightForWidth(label_3->sizePolicy().hasHeightForWidth());
        label_3->setSizePolicy(sizePolicy);

        verticalLayout->addWidget(label_3);

        edit_NewPassword = new QLineEdit(frame);
        edit_NewPassword->setObjectName(QString::fromUtf8("edit_NewPassword"));
        sizePolicy1.setHeightForWidth(edit_NewPassword->sizePolicy().hasHeightForWidth());
        edit_NewPassword->setSizePolicy(sizePolicy1);
        edit_NewPassword->setMinimumSize(QSize(0, 30));
        edit_NewPassword->setEchoMode(QLineEdit::Password);

        verticalLayout->addWidget(edit_NewPassword);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        pbtn_UserSet = new QPushButton(frame);
        pbtn_UserSet->setObjectName(QString::fromUtf8("pbtn_UserSet"));
        QSizePolicy sizePolicy2(QSizePolicy::Minimum, QSizePolicy::Preferred);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(pbtn_UserSet->sizePolicy().hasHeightForWidth());
        pbtn_UserSet->setSizePolicy(sizePolicy2);
        pbtn_UserSet->setMinimumSize(QSize(0, 30));

        horizontalLayout->addWidget(pbtn_UserSet);

        horizontalLayout->setStretch(0, 10);
        horizontalLayout->setStretch(1, 8);

        verticalLayout->addLayout(horizontalLayout);

        verticalLayout->setStretch(0, 1);
        verticalLayout->setStretch(1, 1);
        verticalLayout->setStretch(2, 1);
        verticalLayout->setStretch(3, 1);
        verticalLayout->setStretch(4, 1);
        verticalLayout->setStretch(5, 1);
        verticalLayout->setStretch(6, 1);
        verticalLayout->setStretch(7, 1);

        gridLayout->addLayout(verticalLayout, 0, 0, 1, 1);


        gridLayout_2->addWidget(frame, 0, 0, 1, 1);


        retranslateUi(UserManage);

        QMetaObject::connectSlotsByName(UserManage);
    } // setupUi

    void retranslateUi(QWidget *UserManage)
    {
        UserManage->setWindowTitle(QApplication::translate("UserManage", "Form", nullptr));
        label->setText(QApplication::translate("UserManage", "\347\224\250\346\210\267\345\220\215", nullptr));
        label_2->setText(QApplication::translate("UserManage", "\346\227\247\345\257\206\347\240\201", nullptr));
        edit_OldPassword->setText(QString());
        edit_OldPassword->setPlaceholderText(QApplication::translate("UserManage", "Password", nullptr));
        label_3->setText(QApplication::translate("UserManage", "\346\226\260\345\257\206\347\240\201", nullptr));
        edit_NewPassword->setText(QString());
        edit_NewPassword->setPlaceholderText(QApplication::translate("UserManage", "Password", nullptr));
        pbtn_UserSet->setText(QApplication::translate("UserManage", "\347\241\256\345\256\232", nullptr));
    } // retranslateUi

};

namespace Ui {
    class UserManage: public Ui_UserManage {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_USERMANAGE_H
