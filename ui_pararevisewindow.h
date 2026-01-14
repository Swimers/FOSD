/********************************************************************************
** Form generated from reading UI file 'pararevisewindow.ui'
**
** Created by: Qt User Interface Compiler version 5.12.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PARAREVISEWINDOW_H
#define UI_PARAREVISEWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ParaReviseWindow
{
public:
    QWidget *centralwidget;
    QVBoxLayout *verticalLayout;
    QGroupBox *groupBox;
    QGridLayout *gridLayout;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label;
    QLineEdit *lineEditOrder1;
    QHBoxLayout *horizontalLayout_3;
    QLabel *label_2;
    QLineEdit *lineEditOrder2;
    QHBoxLayout *horizontalLayout_4;
    QLabel *label_4;
    QLineEdit *lineEditOrder3;
    QHBoxLayout *horizontalLayout_5;
    QLabel *label_3;
    QLineEdit *lineEditOrder4;
    QHBoxLayout *horizontalLayout_6;
    QLabel *label_5;
    QLineEdit *lineEditOrder5;
    QHBoxLayout *horizontalLayout_7;
    QLabel *label_6;
    QLineEdit *lineEditOrder6;
    QGroupBox *groupBox_2;
    QGridLayout *gridLayout_2;
    QHBoxLayout *horizontalLayout_8;
    QLabel *label_7;
    QLineEdit *lineEditPara1;
    QHBoxLayout *horizontalLayout_9;
    QLabel *label_8;
    QLineEdit *lineEditPara2;
    QHBoxLayout *horizontalLayout_10;
    QLabel *label_9;
    QLineEdit *lineEditPara3;
    QHBoxLayout *horizontalLayout_11;
    QLabel *label_10;
    QLineEdit *lineEditPara4;
    QHBoxLayout *horizontalLayout_12;
    QLabel *label_11;
    QLineEdit *lineEditPara5;
    QHBoxLayout *horizontalLayout_13;
    QLabel *label_12;
    QLineEdit *lineEditPara6;
    QGroupBox *groupBox_3;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer;
    QPushButton *btnOk;
    QSpacerItem *horizontalSpacer_2;
    QPushButton *btnCancel;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *ParaReviseWindow)
    {
        if (ParaReviseWindow->objectName().isEmpty())
            ParaReviseWindow->setObjectName(QString::fromUtf8("ParaReviseWindow"));
        ParaReviseWindow->resize(599, 397);
        centralwidget = new QWidget(ParaReviseWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        verticalLayout = new QVBoxLayout(centralwidget);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        groupBox = new QGroupBox(centralwidget);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        gridLayout = new QGridLayout(groupBox);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        label = new QLabel(groupBox);
        label->setObjectName(QString::fromUtf8("label"));

        horizontalLayout_2->addWidget(label);

        lineEditOrder1 = new QLineEdit(groupBox);
        lineEditOrder1->setObjectName(QString::fromUtf8("lineEditOrder1"));

        horizontalLayout_2->addWidget(lineEditOrder1);

        horizontalLayout_2->setStretch(0, 4);
        horizontalLayout_2->setStretch(1, 6);

        gridLayout->addLayout(horizontalLayout_2, 0, 0, 1, 1);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        label_2 = new QLabel(groupBox);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        horizontalLayout_3->addWidget(label_2);

        lineEditOrder2 = new QLineEdit(groupBox);
        lineEditOrder2->setObjectName(QString::fromUtf8("lineEditOrder2"));

        horizontalLayout_3->addWidget(lineEditOrder2);


        gridLayout->addLayout(horizontalLayout_3, 0, 1, 1, 1);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        label_4 = new QLabel(groupBox);
        label_4->setObjectName(QString::fromUtf8("label_4"));

        horizontalLayout_4->addWidget(label_4);

        lineEditOrder3 = new QLineEdit(groupBox);
        lineEditOrder3->setObjectName(QString::fromUtf8("lineEditOrder3"));

        horizontalLayout_4->addWidget(lineEditOrder3);


        gridLayout->addLayout(horizontalLayout_4, 0, 2, 1, 1);

        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setObjectName(QString::fromUtf8("horizontalLayout_5"));
        label_3 = new QLabel(groupBox);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        horizontalLayout_5->addWidget(label_3);

        lineEditOrder4 = new QLineEdit(groupBox);
        lineEditOrder4->setObjectName(QString::fromUtf8("lineEditOrder4"));

        horizontalLayout_5->addWidget(lineEditOrder4);

        horizontalLayout_5->setStretch(0, 4);
        horizontalLayout_5->setStretch(1, 6);

        gridLayout->addLayout(horizontalLayout_5, 1, 0, 1, 1);

        horizontalLayout_6 = new QHBoxLayout();
        horizontalLayout_6->setSpacing(6);
        horizontalLayout_6->setObjectName(QString::fromUtf8("horizontalLayout_6"));
        label_5 = new QLabel(groupBox);
        label_5->setObjectName(QString::fromUtf8("label_5"));

        horizontalLayout_6->addWidget(label_5);

        lineEditOrder5 = new QLineEdit(groupBox);
        lineEditOrder5->setObjectName(QString::fromUtf8("lineEditOrder5"));

        horizontalLayout_6->addWidget(lineEditOrder5);

        horizontalLayout_6->setStretch(1, 6);

        gridLayout->addLayout(horizontalLayout_6, 1, 1, 1, 1);

        horizontalLayout_7 = new QHBoxLayout();
        horizontalLayout_7->setObjectName(QString::fromUtf8("horizontalLayout_7"));
        label_6 = new QLabel(groupBox);
        label_6->setObjectName(QString::fromUtf8("label_6"));

        horizontalLayout_7->addWidget(label_6);

        lineEditOrder6 = new QLineEdit(groupBox);
        lineEditOrder6->setObjectName(QString::fromUtf8("lineEditOrder6"));

        horizontalLayout_7->addWidget(lineEditOrder6);

        horizontalLayout_7->setStretch(1, 6);

        gridLayout->addLayout(horizontalLayout_7, 1, 2, 1, 1);


        verticalLayout->addWidget(groupBox);

        groupBox_2 = new QGroupBox(centralwidget);
        groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
        gridLayout_2 = new QGridLayout(groupBox_2);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        horizontalLayout_8 = new QHBoxLayout();
        horizontalLayout_8->setObjectName(QString::fromUtf8("horizontalLayout_8"));
        label_7 = new QLabel(groupBox_2);
        label_7->setObjectName(QString::fromUtf8("label_7"));

        horizontalLayout_8->addWidget(label_7);

        lineEditPara1 = new QLineEdit(groupBox_2);
        lineEditPara1->setObjectName(QString::fromUtf8("lineEditPara1"));

        horizontalLayout_8->addWidget(lineEditPara1);

        horizontalLayout_8->setStretch(0, 4);
        horizontalLayout_8->setStretch(1, 6);

        gridLayout_2->addLayout(horizontalLayout_8, 0, 0, 1, 1);

        horizontalLayout_9 = new QHBoxLayout();
        horizontalLayout_9->setObjectName(QString::fromUtf8("horizontalLayout_9"));
        label_8 = new QLabel(groupBox_2);
        label_8->setObjectName(QString::fromUtf8("label_8"));

        horizontalLayout_9->addWidget(label_8);

        lineEditPara2 = new QLineEdit(groupBox_2);
        lineEditPara2->setObjectName(QString::fromUtf8("lineEditPara2"));

        horizontalLayout_9->addWidget(lineEditPara2);


        gridLayout_2->addLayout(horizontalLayout_9, 0, 1, 1, 1);

        horizontalLayout_10 = new QHBoxLayout();
        horizontalLayout_10->setObjectName(QString::fromUtf8("horizontalLayout_10"));
        label_9 = new QLabel(groupBox_2);
        label_9->setObjectName(QString::fromUtf8("label_9"));

        horizontalLayout_10->addWidget(label_9);

        lineEditPara3 = new QLineEdit(groupBox_2);
        lineEditPara3->setObjectName(QString::fromUtf8("lineEditPara3"));

        horizontalLayout_10->addWidget(lineEditPara3);


        gridLayout_2->addLayout(horizontalLayout_10, 0, 2, 1, 1);

        horizontalLayout_11 = new QHBoxLayout();
        horizontalLayout_11->setObjectName(QString::fromUtf8("horizontalLayout_11"));
        label_10 = new QLabel(groupBox_2);
        label_10->setObjectName(QString::fromUtf8("label_10"));

        horizontalLayout_11->addWidget(label_10);

        lineEditPara4 = new QLineEdit(groupBox_2);
        lineEditPara4->setObjectName(QString::fromUtf8("lineEditPara4"));

        horizontalLayout_11->addWidget(lineEditPara4);

        horizontalLayout_11->setStretch(0, 4);
        horizontalLayout_11->setStretch(1, 6);

        gridLayout_2->addLayout(horizontalLayout_11, 1, 0, 1, 1);

        horizontalLayout_12 = new QHBoxLayout();
        horizontalLayout_12->setSpacing(6);
        horizontalLayout_12->setObjectName(QString::fromUtf8("horizontalLayout_12"));
        label_11 = new QLabel(groupBox_2);
        label_11->setObjectName(QString::fromUtf8("label_11"));

        horizontalLayout_12->addWidget(label_11);

        lineEditPara5 = new QLineEdit(groupBox_2);
        lineEditPara5->setObjectName(QString::fromUtf8("lineEditPara5"));

        horizontalLayout_12->addWidget(lineEditPara5);

        horizontalLayout_12->setStretch(1, 6);

        gridLayout_2->addLayout(horizontalLayout_12, 1, 1, 1, 1);

        horizontalLayout_13 = new QHBoxLayout();
        horizontalLayout_13->setObjectName(QString::fromUtf8("horizontalLayout_13"));
        label_12 = new QLabel(groupBox_2);
        label_12->setObjectName(QString::fromUtf8("label_12"));

        horizontalLayout_13->addWidget(label_12);

        lineEditPara6 = new QLineEdit(groupBox_2);
        lineEditPara6->setObjectName(QString::fromUtf8("lineEditPara6"));

        horizontalLayout_13->addWidget(lineEditPara6);

        horizontalLayout_13->setStretch(1, 6);

        gridLayout_2->addLayout(horizontalLayout_13, 1, 2, 1, 1);


        verticalLayout->addWidget(groupBox_2);

        groupBox_3 = new QGroupBox(centralwidget);
        groupBox_3->setObjectName(QString::fromUtf8("groupBox_3"));
        horizontalLayout = new QHBoxLayout(groupBox_3);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalSpacer = new QSpacerItem(325, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        btnOk = new QPushButton(groupBox_3);
        btnOk->setObjectName(QString::fromUtf8("btnOk"));
        btnOk->setMinimumSize(QSize(0, 30));

        horizontalLayout->addWidget(btnOk);

        horizontalSpacer_2 = new QSpacerItem(62, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_2);

        btnCancel = new QPushButton(groupBox_3);
        btnCancel->setObjectName(QString::fromUtf8("btnCancel"));
        btnCancel->setMinimumSize(QSize(0, 30));

        horizontalLayout->addWidget(btnCancel);

        horizontalLayout->setStretch(0, 20);
        horizontalLayout->setStretch(1, 4);
        horizontalLayout->setStretch(2, 4);
        horizontalLayout->setStretch(3, 4);

        verticalLayout->addWidget(groupBox_3);

        verticalLayout->setStretch(0, 10);
        verticalLayout->setStretch(1, 10);
        verticalLayout->setStretch(2, 4);
        ParaReviseWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(ParaReviseWindow);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 599, 23));
        ParaReviseWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(ParaReviseWindow);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        ParaReviseWindow->setStatusBar(statusbar);

        retranslateUi(ParaReviseWindow);

        QMetaObject::connectSlotsByName(ParaReviseWindow);
    } // setupUi

    void retranslateUi(QMainWindow *ParaReviseWindow)
    {
        ParaReviseWindow->setWindowTitle(QApplication::translate("ParaReviseWindow", "\347\263\273\346\225\260\344\277\256\346\255\243", nullptr));
        groupBox->setTitle(QApplication::translate("ParaReviseWindow", "\344\274\240\346\204\237\345\231\250\351\241\272\345\272\217", nullptr));
        label->setText(QApplication::translate("ParaReviseWindow", "\344\274\240\346\204\237\345\231\2501\357\274\232", nullptr));
        label_2->setText(QApplication::translate("ParaReviseWindow", "\344\274\240\346\204\237\345\231\2502\357\274\232", nullptr));
        label_4->setText(QApplication::translate("ParaReviseWindow", "\344\274\240\346\204\237\345\231\2503\357\274\232", nullptr));
        label_3->setText(QApplication::translate("ParaReviseWindow", "\344\274\240\346\204\237\345\231\2504\357\274\232", nullptr));
        label_5->setText(QApplication::translate("ParaReviseWindow", "\344\274\240\346\204\237\345\231\2505\357\274\232", nullptr));
        label_6->setText(QApplication::translate("ParaReviseWindow", "\344\274\240\346\204\237\345\231\2506\357\274\232", nullptr));
        groupBox_2->setTitle(QApplication::translate("ParaReviseWindow", "\344\274\240\346\204\237\345\231\250\347\263\273\346\225\260", nullptr));
        label_7->setText(QApplication::translate("ParaReviseWindow", "\344\274\240\346\204\237\345\231\2501\357\274\232", nullptr));
        label_8->setText(QApplication::translate("ParaReviseWindow", "\344\274\240\346\204\237\345\231\2502\357\274\232", nullptr));
        label_9->setText(QApplication::translate("ParaReviseWindow", "\344\274\240\346\204\237\345\231\2503\357\274\232", nullptr));
        label_10->setText(QApplication::translate("ParaReviseWindow", "\344\274\240\346\204\237\345\231\2504\357\274\232", nullptr));
        label_11->setText(QApplication::translate("ParaReviseWindow", "\344\274\240\346\204\237\345\231\2505\357\274\232", nullptr));
        label_12->setText(QApplication::translate("ParaReviseWindow", "\344\274\240\346\204\237\345\231\2506\357\274\232", nullptr));
        groupBox_3->setTitle(QString());
        btnOk->setText(QApplication::translate("ParaReviseWindow", "\347\241\256\345\256\232", nullptr));
        btnCancel->setText(QApplication::translate("ParaReviseWindow", "\345\217\226\346\266\210", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ParaReviseWindow: public Ui_ParaReviseWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PARAREVISEWINDOW_H
