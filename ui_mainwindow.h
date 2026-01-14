/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.12.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "switchcontrol.h"

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QGridLayout *gridLayout_2;
    QStackedWidget *stackedWidget;
    QHBoxLayout *horizontalLayout_3;
    QSpacerItem *horizontalSpacer;
    QPushButton *pbtn_Minimize;
    QPushButton *pbtn_Full;
    QPushButton *pbtn_Maximize;
    QVBoxLayout *vboxLayout;
    QPushButton *pbtn_JieTiaoYi;
    QPushButton *pbtn_2DPlot;
    QPushButton *pbtn_3DPlot;
    QSpacerItem *verticalSpacer;
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    SwitchControl *switch_Style;
    QPushButton *pbtn_UserManage;
    QPushButton *pbtn_TurnOff;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(1240, 958);
        QIcon icon;
        QString iconThemeName = QString::fromUtf8("D:\\CommonFiles\\Projects_And_Works\\SummerProject\\Qt_Software\\TestVersion\\Test3\\Resource\\icon\\UserIcon2.png");
        if (QIcon::hasThemeIcon(iconThemeName)) {
            icon = QIcon::fromTheme(iconThemeName);
        } else {
            icon.addFile(QString::fromUtf8("."), QSize(), QIcon::Normal, QIcon::Off);
        }
        MainWindow->setWindowIcon(icon);
        MainWindow->setStyleSheet(QString::fromUtf8(""));
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        centralwidget->setStyleSheet(QString::fromUtf8(""));
        gridLayout_2 = new QGridLayout(centralwidget);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        stackedWidget = new QStackedWidget(centralwidget);
        stackedWidget->setObjectName(QString::fromUtf8("stackedWidget"));
        stackedWidget->setStyleSheet(QString::fromUtf8(""));

        gridLayout_2->addWidget(stackedWidget, 1, 1, 1, 1);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer);

        pbtn_Minimize = new QPushButton(centralwidget);
        pbtn_Minimize->setObjectName(QString::fromUtf8("pbtn_Minimize"));
        pbtn_Minimize->setMinimumSize(QSize(25, 25));
        pbtn_Minimize->setMaximumSize(QSize(25, 25));

        horizontalLayout_3->addWidget(pbtn_Minimize);

        pbtn_Full = new QPushButton(centralwidget);
        pbtn_Full->setObjectName(QString::fromUtf8("pbtn_Full"));
        pbtn_Full->setMinimumSize(QSize(25, 25));
        pbtn_Full->setMaximumSize(QSize(25, 25));

        horizontalLayout_3->addWidget(pbtn_Full);

        pbtn_Maximize = new QPushButton(centralwidget);
        pbtn_Maximize->setObjectName(QString::fromUtf8("pbtn_Maximize"));
        pbtn_Maximize->setMinimumSize(QSize(25, 25));
        pbtn_Maximize->setMaximumSize(QSize(25, 25));

        horizontalLayout_3->addWidget(pbtn_Maximize);


        gridLayout_2->addLayout(horizontalLayout_3, 0, 1, 1, 1);

        vboxLayout = new QVBoxLayout();
        vboxLayout->setSpacing(10);
        vboxLayout->setObjectName(QString::fromUtf8("vboxLayout"));
        vboxLayout->setContentsMargins(10, 20, 10, 20);
        pbtn_JieTiaoYi = new QPushButton(centralwidget);
        pbtn_JieTiaoYi->setObjectName(QString::fromUtf8("pbtn_JieTiaoYi"));
        pbtn_JieTiaoYi->setMinimumSize(QSize(0, 50));
        pbtn_JieTiaoYi->setStyleSheet(QString::fromUtf8(""));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/icon/Resource/icon/Menu.png"), QSize(), QIcon::Normal, QIcon::Off);
        pbtn_JieTiaoYi->setIcon(icon1);
        pbtn_JieTiaoYi->setIconSize(QSize(24, 24));

        vboxLayout->addWidget(pbtn_JieTiaoYi);

        pbtn_2DPlot = new QPushButton(centralwidget);
        pbtn_2DPlot->setObjectName(QString::fromUtf8("pbtn_2DPlot"));
        QSizePolicy sizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(pbtn_2DPlot->sizePolicy().hasHeightForWidth());
        pbtn_2DPlot->setSizePolicy(sizePolicy);
        pbtn_2DPlot->setMinimumSize(QSize(120, 50));
        pbtn_2DPlot->setBaseSize(QSize(0, 0));
        pbtn_2DPlot->setStyleSheet(QString::fromUtf8(""));
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/icon/Resource/icon/2D_Plot.png"), QSize(), QIcon::Normal, QIcon::Off);
        pbtn_2DPlot->setIcon(icon2);
        pbtn_2DPlot->setIconSize(QSize(24, 24));

        vboxLayout->addWidget(pbtn_2DPlot);

        pbtn_3DPlot = new QPushButton(centralwidget);
        pbtn_3DPlot->setObjectName(QString::fromUtf8("pbtn_3DPlot"));
        pbtn_3DPlot->setMinimumSize(QSize(0, 50));
        pbtn_3DPlot->setStyleSheet(QString::fromUtf8(""));
        QIcon icon3;
        icon3.addFile(QString::fromUtf8(":/icon/Resource/icon/3D_Plot.png"), QSize(), QIcon::Normal, QIcon::Off);
        pbtn_3DPlot->setIcon(icon3);
        pbtn_3DPlot->setIconSize(QSize(24, 24));

        vboxLayout->addWidget(pbtn_3DPlot);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        vboxLayout->addItem(verticalSpacer);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        label = new QLabel(centralwidget);
        label->setObjectName(QString::fromUtf8("label"));

        horizontalLayout->addWidget(label);

        switch_Style = new SwitchControl(centralwidget);
        switch_Style->setObjectName(QString::fromUtf8("switch_Style"));
        switch_Style->setMinimumSize(QSize(0, 20));
        switch_Style->setMaximumSize(QSize(40, 16777215));

        horizontalLayout->addWidget(switch_Style);


        vboxLayout->addLayout(horizontalLayout);

        pbtn_UserManage = new QPushButton(centralwidget);
        pbtn_UserManage->setObjectName(QString::fromUtf8("pbtn_UserManage"));
        pbtn_UserManage->setMinimumSize(QSize(0, 50));
        QFont font;
        font.setFamily(QString::fromUtf8("\351\273\221\344\275\223"));
        font.setPointSize(13);
        font.setBold(true);
        font.setItalic(false);
        font.setWeight(75);
        pbtn_UserManage->setFont(font);
        QIcon icon4;
        icon4.addFile(QString::fromUtf8(":/icon/Resource/icon/UserManage.png"), QSize(), QIcon::Normal, QIcon::Off);
        pbtn_UserManage->setIcon(icon4);
        pbtn_UserManage->setIconSize(QSize(24, 24));

        vboxLayout->addWidget(pbtn_UserManage);

        pbtn_TurnOff = new QPushButton(centralwidget);
        pbtn_TurnOff->setObjectName(QString::fromUtf8("pbtn_TurnOff"));
        pbtn_TurnOff->setMinimumSize(QSize(0, 50));
        pbtn_TurnOff->setFont(font);
        QIcon icon5;
        icon5.addFile(QString::fromUtf8(":/icon/Resource/icon/TurnOff.png"), QSize(), QIcon::Normal, QIcon::Off);
        pbtn_TurnOff->setIcon(icon5);
        pbtn_TurnOff->setIconSize(QSize(24, 24));

        vboxLayout->addWidget(pbtn_TurnOff);


        gridLayout_2->addLayout(vboxLayout, 0, 0, 2, 1);

        MainWindow->setCentralWidget(centralwidget);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", nullptr));
        pbtn_Minimize->setText(QApplication::translate("MainWindow", "-", nullptr));
        pbtn_Full->setText(QApplication::translate("MainWindow", "\345\217\243", nullptr));
        pbtn_Maximize->setText(QApplication::translate("MainWindow", "X", nullptr));
        pbtn_JieTiaoYi->setText(QApplication::translate("MainWindow", "\350\260\203 \350\247\243 \344\273\252", nullptr));
        pbtn_2DPlot->setText(QApplication::translate("MainWindow", "\344\272\214\347\273\264\351\252\214\350\257\201", nullptr));
        pbtn_3DPlot->setText(QApplication::translate("MainWindow", "\345\217\230 \345\275\242 \345\234\272", nullptr));
        label->setText(QApplication::translate("MainWindow", "\346\267\261\346\265\205\345\210\207\346\215\242", nullptr));
        pbtn_UserManage->setText(QApplication::translate("MainWindow", "\347\224\250\346\210\267\347\256\241\347\220\206", nullptr));
        pbtn_TurnOff->setText(QApplication::translate("MainWindow", "\351\200\200\345\207\272\347\263\273\347\273\237", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
