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
<<<<<<< Updated upstream
=======
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolButton>
>>>>>>> Stashed changes
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QGridLayout *gridLayout_2;
<<<<<<< Updated upstream
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
=======
    QWidget *sideBar;
    QVBoxLayout *verticalLayout;
    QToolButton *tbtn_Demo;
    QToolButton *tbtn_Disp2D;
    QToolButton *tbtn_Disp3D;
    QSpacerItem *verticalSpacer;
    QToolButton *tbtn_saveConfig;
    QToolButton *tbtn_userInfo;
    QToolButton *tbtn_Exit;
    QStackedWidget *stackedWidget;
    QMenuBar *menubar;
    QStatusBar *statusbar;
>>>>>>> Stashed changes

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
<<<<<<< Updated upstream
        MainWindow->resize(1240, 958);
=======
        MainWindow->resize(1213, 784);
>>>>>>> Stashed changes
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/resource/resource_main/airport_48px.png"), QSize(), QIcon::Normal, QIcon::Off);
        MainWindow->setWindowIcon(icon);
        MainWindow->setWindowOpacity(1.000000000000000);
        MainWindow->setStyleSheet(QString::fromUtf8(""));
        MainWindow->setIconSize(QSize(32, 32));
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        centralwidget->setStyleSheet(QString::fromUtf8(""));
        gridLayout_2 = new QGridLayout(centralwidget);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
<<<<<<< Updated upstream
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
=======
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
>>>>>>> Stashed changes

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

        tbtn_saveConfig = new QToolButton(sideBar);
        tbtn_saveConfig->setObjectName(QString::fromUtf8("tbtn_saveConfig"));
        sizePolicy.setHeightForWidth(tbtn_saveConfig->sizePolicy().hasHeightForWidth());
        tbtn_saveConfig->setSizePolicy(sizePolicy);
        tbtn_saveConfig->setMinimumSize(QSize(0, 50));
        tbtn_saveConfig->setFont(font1);
        QIcon icon4;
        icon4.addFile(QString::fromUtf8(":/resource/resource_main/user_100px.png"), QSize(), QIcon::Normal, QIcon::Off);
        tbtn_saveConfig->setIcon(icon4);
        tbtn_saveConfig->setIconSize(QSize(32, 32));
        tbtn_saveConfig->setCheckable(false);
        tbtn_saveConfig->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
        tbtn_saveConfig->setAutoRaise(true);

        verticalLayout->addWidget(tbtn_saveConfig);

        tbtn_userInfo = new QToolButton(sideBar);
        tbtn_userInfo->setObjectName(QString::fromUtf8("tbtn_userInfo"));
        sizePolicy.setHeightForWidth(tbtn_userInfo->sizePolicy().hasHeightForWidth());
        tbtn_userInfo->setSizePolicy(sizePolicy);
        tbtn_userInfo->setMinimumSize(QSize(0, 50));
        tbtn_userInfo->setFont(font1);
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


<<<<<<< Updated upstream
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
=======
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
>>>>>>> Stashed changes

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
<<<<<<< Updated upstream
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
=======
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "\345\256\236\346\227\266\347\233\221\346\216\247\347\263\273\347\273\237", nullptr));
        tbtn_Demo->setText(QApplication::translate("MainWindow", "\350\247\243\350\260\203\344\273\252", nullptr));
        tbtn_Disp2D->setText(QApplication::translate("MainWindow", "\344\272\214\347\273\264\351\252\214\350\257\201", nullptr));
        tbtn_Disp3D->setText(QApplication::translate("MainWindow", "\345\217\230\345\275\242\345\234\272", nullptr));
        tbtn_saveConfig->setText(QApplication::translate("MainWindow", "\344\277\235\345\255\230\351\205\215\347\275\256", nullptr));
        tbtn_userInfo->setText(QApplication::translate("MainWindow", "\347\224\250\346\210\267\347\256\241\347\220\206", nullptr));
        tbtn_Exit->setText(QApplication::translate("MainWindow", "\351\200\200\345\207\272\347\263\273\347\273\237", nullptr));
>>>>>>> Stashed changes
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
