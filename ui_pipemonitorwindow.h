/********************************************************************************
** Form generated from reading UI file 'pipemonitorwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.12.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PIPEMONITORWINDOW_H
#define UI_PIPEMONITORWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_PipeMonitorWindow
{
public:
    QGridLayout *gridLayout;
    QSpacerItem *verticalSpacer_2;
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QLineEdit *lineEditState;
    QSpacerItem *verticalSpacer;
    QHBoxLayout *horizontalLayout_2;
    QPushButton *btnState;
    QPushButton *btnClose;
    QSpacerItem *verticalSpacer_3;

    void setupUi(QWidget *PipeMonitorWindow)
    {
        if (PipeMonitorWindow->objectName().isEmpty())
            PipeMonitorWindow->setObjectName(QString::fromUtf8("PipeMonitorWindow"));
        PipeMonitorWindow->resize(248, 206);
        gridLayout = new QGridLayout(PipeMonitorWindow);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        verticalSpacer_2 = new QSpacerItem(20, 35, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout->addItem(verticalSpacer_2, 0, 0, 1, 1);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        label = new QLabel(PipeMonitorWindow);
        label->setObjectName(QString::fromUtf8("label"));

        horizontalLayout->addWidget(label);

        lineEditState = new QLineEdit(PipeMonitorWindow);
        lineEditState->setObjectName(QString::fromUtf8("lineEditState"));

        horizontalLayout->addWidget(lineEditState);


        gridLayout->addLayout(horizontalLayout, 1, 0, 1, 1);

        verticalSpacer = new QSpacerItem(20, 36, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout->addItem(verticalSpacer, 2, 0, 1, 1);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        btnState = new QPushButton(PipeMonitorWindow);
        btnState->setObjectName(QString::fromUtf8("btnState"));
        btnState->setMinimumSize(QSize(0, 25));

        horizontalLayout_2->addWidget(btnState);

        btnClose = new QPushButton(PipeMonitorWindow);
        btnClose->setObjectName(QString::fromUtf8("btnClose"));
        btnClose->setMinimumSize(QSize(0, 25));

        horizontalLayout_2->addWidget(btnClose);


        gridLayout->addLayout(horizontalLayout_2, 3, 0, 1, 1);

        verticalSpacer_3 = new QSpacerItem(20, 35, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout->addItem(verticalSpacer_3, 4, 0, 1, 1);


        retranslateUi(PipeMonitorWindow);

        QMetaObject::connectSlotsByName(PipeMonitorWindow);
    } // setupUi

    void retranslateUi(QWidget *PipeMonitorWindow)
    {
        PipeMonitorWindow->setWindowTitle(QApplication::translate("PipeMonitorWindow", "\346\266\262\345\206\267\347\256\241\351\201\223\347\233\221\346\265\213", nullptr));
        label->setText(QApplication::translate("PipeMonitorWindow", "\345\275\223\345\211\215\347\212\266\346\200\201\357\274\232", nullptr));
        btnState->setText(QApplication::translate("PipeMonitorWindow", "\345\274\200\345\247\213", nullptr));
        btnClose->setText(QApplication::translate("PipeMonitorWindow", "\345\205\263\351\227\255", nullptr));
    } // retranslateUi

};

namespace Ui {
    class PipeMonitorWindow: public Ui_PipeMonitorWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PIPEMONITORWINDOW_H
