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
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QGridLayout *gridLayout_2;
    QWidget *sideBar;
    QVBoxLayout *verticalLayout;
    QToolButton *tbtn_Demo;
    QToolButton *tbtn_Disp2D;
    QToolButton *tbtn_Disp3D;
    QSpacerItem *verticalSpacer;
    QToolButton *tbtn_userInfo;
    QToolButton *tbtn_Exit;
    QStackedWidget *stackedWidget;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(1213, 784);
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/resource/resource_main/airport_48px.png"), QSize(), QIcon::Normal, QIcon::Off);
        MainWindow->setWindowIcon(icon);
        MainWindow->setWindowOpacity(0.980000000000000);
        MainWindow->setStyleSheet(QString::fromUtf8(""));
        MainWindow->setIconSize(QSize(32, 32));
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        centralwidget->setStyleSheet(QString::fromUtf8(""));
        gridLayout_2 = new QGridLayout(centralwidget);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        gridLayout_2->setHorizontalSpacing(0);
        gridLayout_2->setVerticalSpacing(6);
        gridLayout_2->setContentsMargins(0, 0, 0, 0);
        sideBar = new QWidget(centralwidget);
        sideBar->setObjectName(QString::fromUtf8("sideBar"));
        QPalette palette;
        QBrush brush(QColor(204, 204, 204, 255));
        brush.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::Button, brush);
        palette.setBrush(QPalette::Active, QPalette::Base, brush);
        palette.setBrush(QPalette::Active, QPalette::Window, brush);
        palette.setBrush(QPalette::Inactive, QPalette::Button, brush);
        palette.setBrush(QPalette::Inactive, QPalette::Base, brush);
        palette.setBrush(QPalette::Inactive, QPalette::Window, brush);
        palette.setBrush(QPalette::Disabled, QPalette::Button, brush);
        palette.setBrush(QPalette::Disabled, QPalette::Base, brush);
        palette.setBrush(QPalette::Disabled, QPalette::Window, brush);
        sideBar->setPalette(palette);
        sideBar->setStyleSheet(QString::fromUtf8(""));
        verticalLayout = new QVBoxLayout(sideBar);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        tbtn_Demo = new QToolButton(sideBar);
        tbtn_Demo->setObjectName(QString::fromUtf8("tbtn_Demo"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(tbtn_Demo->sizePolicy().hasHeightForWidth());
        tbtn_Demo->setSizePolicy(sizePolicy);
        tbtn_Demo->setMinimumSize(QSize(0, 50));
        QFont font;
        font.setFamily(QString::fromUtf8("\351\232\266\344\271\246"));
        font.setPointSize(12);
        font.setBold(false);
        font.setItalic(false);
        font.setUnderline(false);
        font.setWeight(50);
        font.setStrikeOut(false);
        tbtn_Demo->setFont(font);
        tbtn_Demo->setContextMenuPolicy(Qt::DefaultContextMenu);
        tbtn_Demo->setLayoutDirection(Qt::LeftToRight);
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/resource/resource_main/icon17.png"), QSize(), QIcon::Normal, QIcon::Off);
        tbtn_Demo->setIcon(icon1);
        tbtn_Demo->setIconSize(QSize(32, 32));
        tbtn_Demo->setCheckable(true);
        tbtn_Demo->setChecked(false);
        tbtn_Demo->setAutoRepeat(false);
        tbtn_Demo->setAutoExclusive(false);
        tbtn_Demo->setPopupMode(QToolButton::DelayedPopup);
        tbtn_Demo->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
        tbtn_Demo->setAutoRaise(true);
        tbtn_Demo->setArrowType(Qt::NoArrow);

        verticalLayout->addWidget(tbtn_Demo);

        tbtn_Disp2D = new QToolButton(sideBar);
        tbtn_Disp2D->setObjectName(QString::fromUtf8("tbtn_Disp2D"));
        tbtn_Disp2D->setMinimumSize(QSize(0, 50));
        QFont font1;
        font1.setFamily(QString::fromUtf8("\351\232\266\344\271\246"));
        font1.setPointSize(12);
        tbtn_Disp2D->setFont(font1);
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/resource/resource_main/icon25.png"), QSize(), QIcon::Normal, QIcon::Off);
        tbtn_Disp2D->setIcon(icon2);
        tbtn_Disp2D->setIconSize(QSize(32, 32));
        tbtn_Disp2D->setCheckable(true);
        tbtn_Disp2D->setChecked(false);
        tbtn_Disp2D->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
        tbtn_Disp2D->setAutoRaise(true);

        verticalLayout->addWidget(tbtn_Disp2D);

        tbtn_Disp3D = new QToolButton(sideBar);
        tbtn_Disp3D->setObjectName(QString::fromUtf8("tbtn_Disp3D"));
        sizePolicy.setHeightForWidth(tbtn_Disp3D->sizePolicy().hasHeightForWidth());
        tbtn_Disp3D->setSizePolicy(sizePolicy);
        tbtn_Disp3D->setMinimumSize(QSize(0, 50));
        tbtn_Disp3D->setFont(font1);
        QIcon icon3;
        icon3.addFile(QString::fromUtf8(":/resource/resource_main/icon18.png"), QSize(), QIcon::Normal, QIcon::Off);
        tbtn_Disp3D->setIcon(icon3);
        tbtn_Disp3D->setIconSize(QSize(32, 32));
        tbtn_Disp3D->setCheckable(true);
        tbtn_Disp3D->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
        tbtn_Disp3D->setAutoRaise(true);

        verticalLayout->addWidget(tbtn_Disp3D);

        verticalSpacer = new QSpacerItem(17, 480, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer);

        tbtn_userInfo = new QToolButton(sideBar);
        tbtn_userInfo->setObjectName(QString::fromUtf8("tbtn_userInfo"));
        sizePolicy.setHeightForWidth(tbtn_userInfo->sizePolicy().hasHeightForWidth());
        tbtn_userInfo->setSizePolicy(sizePolicy);
        tbtn_userInfo->setMinimumSize(QSize(0, 50));
        tbtn_userInfo->setFont(font1);
        QIcon icon4;
        icon4.addFile(QString::fromUtf8(":/resource/resource_main/user_100px.png"), QSize(), QIcon::Normal, QIcon::Off);
        tbtn_userInfo->setIcon(icon4);
        tbtn_userInfo->setIconSize(QSize(32, 32));
        tbtn_userInfo->setCheckable(true);
        tbtn_userInfo->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
        tbtn_userInfo->setAutoRaise(true);

        verticalLayout->addWidget(tbtn_userInfo);

        tbtn_Exit = new QToolButton(sideBar);
        tbtn_Exit->setObjectName(QString::fromUtf8("tbtn_Exit"));
        sizePolicy.setHeightForWidth(tbtn_Exit->sizePolicy().hasHeightForWidth());
        tbtn_Exit->setSizePolicy(sizePolicy);
        tbtn_Exit->setMinimumSize(QSize(0, 50));
        tbtn_Exit->setFont(font1);
        QIcon icon5;
        icon5.addFile(QString::fromUtf8(":/resource/resource_main/icon30.png"), QSize(), QIcon::Normal, QIcon::Off);
        tbtn_Exit->setIcon(icon5);
        tbtn_Exit->setIconSize(QSize(32, 32));
        tbtn_Exit->setCheckable(true);
        tbtn_Exit->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
        tbtn_Exit->setAutoRaise(true);

        verticalLayout->addWidget(tbtn_Exit);


        gridLayout_2->addWidget(sideBar, 0, 0, 1, 1);

        stackedWidget = new QStackedWidget(centralwidget);
        stackedWidget->setObjectName(QString::fromUtf8("stackedWidget"));
        QFont font2;
        font2.setFamily(QString::fromUtf8("Agency FB"));
        font2.setBold(false);
        font2.setWeight(50);
        stackedWidget->setFont(font2);
        stackedWidget->setAutoFillBackground(false);
        stackedWidget->setStyleSheet(QString::fromUtf8(""));
        stackedWidget->setFrameShape(QFrame::NoFrame);
        stackedWidget->setFrameShadow(QFrame::Raised);

        gridLayout_2->addWidget(stackedWidget, 0, 1, 1, 1);

        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 1213, 23));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        stackedWidget->setCurrentIndex(-1);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "\345\256\236\346\227\266\347\233\221\346\216\247\347\263\273\347\273\237", nullptr));
        tbtn_Demo->setText(QApplication::translate("MainWindow", "\350\247\243\350\260\203\344\273\252", nullptr));
        tbtn_Disp2D->setText(QApplication::translate("MainWindow", "\344\272\214\347\273\264\351\252\214\350\257\201", nullptr));
        tbtn_Disp3D->setText(QApplication::translate("MainWindow", "\345\217\230\345\275\242\345\234\272", nullptr));
        tbtn_userInfo->setText(QApplication::translate("MainWindow", "\347\224\250\346\210\267\347\256\241\347\220\206", nullptr));
        tbtn_Exit->setText(QApplication::translate("MainWindow", "\351\200\200\345\207\272\347\263\273\347\273\237", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
