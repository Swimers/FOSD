/********************************************************************************
** Form generated from reading UI file 'connectdebug.ui'
**
** Created by: Qt User Interface Compiler version 5.12.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CONNECTDEBUG_H
#define UI_CONNECTDEBUG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_connectdebug
{
public:
    QVBoxLayout *verticalLayout;
    QSpacerItem *verticalSpacer_2;
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QLineEdit *edit_ConnectState;
    QSpacerItem *verticalSpacer;
    QHBoxLayout *horizontalLayout_2;
    QPushButton *pbtn_ConnectDebugStart;
    QPushButton *pbtn_ConnectDebugClose;
    QSpacerItem *verticalSpacer_3;

    void setupUi(QWidget *connectdebug)
    {
        if (connectdebug->objectName().isEmpty())
            connectdebug->setObjectName(QString::fromUtf8("connectdebug"));
        connectdebug->resize(369, 262);
        verticalLayout = new QVBoxLayout(connectdebug);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalSpacer_2 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer_2);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setContentsMargins(20, -1, -1, -1);
        label = new QLabel(connectdebug);
        label->setObjectName(QString::fromUtf8("label"));

        horizontalLayout->addWidget(label);

        edit_ConnectState = new QLineEdit(connectdebug);
        edit_ConnectState->setObjectName(QString::fromUtf8("edit_ConnectState"));
        edit_ConnectState->setMinimumSize(QSize(0, 30));

        horizontalLayout->addWidget(edit_ConnectState);


        verticalLayout->addLayout(horizontalLayout);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(20);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        horizontalLayout_2->setContentsMargins(20, -1, 20, -1);
        pbtn_ConnectDebugStart = new QPushButton(connectdebug);
        pbtn_ConnectDebugStart->setObjectName(QString::fromUtf8("pbtn_ConnectDebugStart"));
        pbtn_ConnectDebugStart->setMinimumSize(QSize(0, 30));

        horizontalLayout_2->addWidget(pbtn_ConnectDebugStart);

        pbtn_ConnectDebugClose = new QPushButton(connectdebug);
        pbtn_ConnectDebugClose->setObjectName(QString::fromUtf8("pbtn_ConnectDebugClose"));
        pbtn_ConnectDebugClose->setMinimumSize(QSize(0, 30));

        horizontalLayout_2->addWidget(pbtn_ConnectDebugClose);


        verticalLayout->addLayout(horizontalLayout_2);

        verticalSpacer_3 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer_3);


        retranslateUi(connectdebug);

        QMetaObject::connectSlotsByName(connectdebug);
    } // setupUi

    void retranslateUi(QWidget *connectdebug)
    {
        connectdebug->setWindowTitle(QApplication::translate("connectdebug", "Form", nullptr));
        label->setText(QApplication::translate("connectdebug", "\345\275\223\345\211\215\347\212\266\346\200\201\357\274\232", nullptr));
        pbtn_ConnectDebugStart->setText(QApplication::translate("connectdebug", "\345\220\257\345\212\250", nullptr));
        pbtn_ConnectDebugClose->setText(QApplication::translate("connectdebug", "\345\205\263\351\227\255", nullptr));
    } // retranslateUi

};

namespace Ui {
    class connectdebug: public Ui_connectdebug {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CONNECTDEBUG_H
