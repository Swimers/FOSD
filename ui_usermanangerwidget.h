/********************************************************************************
** Form generated from reading UI file 'usermanangerwidget.ui'
**
** Created by: Qt User Interface Compiler version 5.12.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_USERMANANGERWIDGET_H
#define UI_USERMANANGERWIDGET_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
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

class Ui_UserManangerWidget
{
public:
    QGridLayout *gridLayout_2;
    QFrame *frame;
    QGridLayout *gridLayout;
    QVBoxLayout *verticalLayout;
    QLabel *label;
    QComboBox *comboBox;
    QLabel *label_2;
    QLineEdit *edit_oldPW;
    QLabel *label_3;
    QLineEdit *edit_newPW;
    QSpacerItem *verticalSpacer;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer;
    QPushButton *btn_log;

    void setupUi(QWidget *UserManangerWidget)
    {
        if (UserManangerWidget->objectName().isEmpty())
            UserManangerWidget->setObjectName(QString::fromUtf8("UserManangerWidget"));
        UserManangerWidget->resize(357, 330);
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/resource/resource_main/user_100px.png"), QSize(), QIcon::Normal, QIcon::Off);
        UserManangerWidget->setWindowIcon(icon);
        gridLayout_2 = new QGridLayout(UserManangerWidget);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        frame = new QFrame(UserManangerWidget);
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

        comboBox = new QComboBox(frame);
        comboBox->setObjectName(QString::fromUtf8("comboBox"));
        sizePolicy.setHeightForWidth(comboBox->sizePolicy().hasHeightForWidth());
        comboBox->setSizePolicy(sizePolicy);

        verticalLayout->addWidget(comboBox);

        label_2 = new QLabel(frame);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        sizePolicy.setHeightForWidth(label_2->sizePolicy().hasHeightForWidth());
        label_2->setSizePolicy(sizePolicy);

        verticalLayout->addWidget(label_2);

        edit_oldPW = new QLineEdit(frame);
        edit_oldPW->setObjectName(QString::fromUtf8("edit_oldPW"));
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Preferred);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(edit_oldPW->sizePolicy().hasHeightForWidth());
        edit_oldPW->setSizePolicy(sizePolicy1);
        edit_oldPW->setEchoMode(QLineEdit::Password);

        verticalLayout->addWidget(edit_oldPW);

        label_3 = new QLabel(frame);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        sizePolicy.setHeightForWidth(label_3->sizePolicy().hasHeightForWidth());
        label_3->setSizePolicy(sizePolicy);

        verticalLayout->addWidget(label_3);

        edit_newPW = new QLineEdit(frame);
        edit_newPW->setObjectName(QString::fromUtf8("edit_newPW"));
        sizePolicy1.setHeightForWidth(edit_newPW->sizePolicy().hasHeightForWidth());
        edit_newPW->setSizePolicy(sizePolicy1);
        edit_newPW->setEchoMode(QLineEdit::Password);

        verticalLayout->addWidget(edit_newPW);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        btn_log = new QPushButton(frame);
        btn_log->setObjectName(QString::fromUtf8("btn_log"));
        QSizePolicy sizePolicy2(QSizePolicy::Minimum, QSizePolicy::Preferred);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(btn_log->sizePolicy().hasHeightForWidth());
        btn_log->setSizePolicy(sizePolicy2);

        horizontalLayout->addWidget(btn_log);

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


        retranslateUi(UserManangerWidget);

        QMetaObject::connectSlotsByName(UserManangerWidget);
    } // setupUi

    void retranslateUi(QWidget *UserManangerWidget)
    {
        UserManangerWidget->setWindowTitle(QApplication::translate("UserManangerWidget", "\344\277\256\346\224\271\345\257\206\347\240\201", nullptr));
        label->setText(QApplication::translate("UserManangerWidget", "\347\224\250\346\210\267\345\220\215", nullptr));
        label_2->setText(QApplication::translate("UserManangerWidget", "\346\227\247\345\257\206\347\240\201", nullptr));
        edit_oldPW->setText(QString());
        edit_oldPW->setPlaceholderText(QApplication::translate("UserManangerWidget", "\345\257\206\347\240\201", nullptr));
        label_3->setText(QApplication::translate("UserManangerWidget", "\346\226\260\345\257\206\347\240\201", nullptr));
        edit_newPW->setText(QString());
        edit_newPW->setPlaceholderText(QApplication::translate("UserManangerWidget", "\345\257\206\347\240\201", nullptr));
        btn_log->setText(QApplication::translate("UserManangerWidget", "\347\241\256\345\256\232", nullptr));
    } // retranslateUi

};

namespace Ui {
    class UserManangerWidget: public Ui_UserManangerWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_USERMANANGERWIDGET_H
