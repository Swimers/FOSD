/********************************************************************************
** Form generated from reading UI file 'disp3dmainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.12.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DISP3DMAINWINDOW_H
#define UI_DISP3DMAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDockWidget>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSplitter>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "QVTKWidget.h"

QT_BEGIN_NAMESPACE

class Ui_disp3dMainWindow
{
public:
    QAction *actionModelDock;
    QAction *actionInforDock;
    QAction *actionStart;
    QAction *actionStop;
    QAction *actionReset;
    QWidget *centralwidget;
    QGridLayout *gridLayout;
    QSplitter *splitter_2;
    QVTKWidget *qvtkWidget;
    QSplitter *splitter;
    QDockWidget *dockWidgetModel;
    QWidget *dockWidgetContents;
    QVBoxLayout *verticalLayout_3;
    QVBoxLayout *verticalLayout_2;
    QPushButton *btnLoadModel;
    QHBoxLayout *horizontalLayout;
    QComboBox *comboBoxView;
    QPushButton *btnChangeView;
    QHBoxLayout *horizontalLayout_2;
    QCheckBox *checkBoxEdge;
    QCheckBox *checkBoxAxis;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout_3;
    QLabel *label_4;
    QLineEdit *editSphereSize;
    QPushButton *btkSetSize;
    QPushButton *btnAddPoint;
    QHBoxLayout *horizontalLayout_4;
    QLabel *label_5;
    QComboBox *comboBoxHz;
    QSpacerItem *verticalSpacer_2;
    QGroupBox *groupBox;
    QGridLayout *gridLayout_3;
    QLineEdit *editMaxRange;
    QComboBox *comboBoxDisp;
    QPushButton *btkDispAnalysis;
    QSpacerItem *verticalSpacer;
    QDockWidget *dockWidgetInfor;
    QWidget *dockWidgetContents_2;
    QVBoxLayout *verticalLayout_4;
    QLabel *label_point;
    QLabel *label_element;
    QLabel *label_flagpoint;
    QLabel *label_dispy;
    QMenuBar *menubar;
    QToolBar *toolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *disp3dMainWindow)
    {
        if (disp3dMainWindow->objectName().isEmpty())
            disp3dMainWindow->setObjectName(QString::fromUtf8("disp3dMainWindow"));
        disp3dMainWindow->resize(1117, 762);
        actionModelDock = new QAction(disp3dMainWindow);
        actionModelDock->setObjectName(QString::fromUtf8("actionModelDock"));
        actionModelDock->setCheckable(false);
        actionModelDock->setChecked(false);
        actionModelDock->setEnabled(true);
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/resource/resource_disp3d/icon101.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionModelDock->setIcon(icon);
        actionInforDock = new QAction(disp3dMainWindow);
        actionInforDock->setObjectName(QString::fromUtf8("actionInforDock"));
        actionInforDock->setCheckable(false);
        actionInforDock->setChecked(false);
        actionInforDock->setEnabled(true);
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/resource/resource_disp3d/information_48px.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionInforDock->setIcon(icon1);
        actionInforDock->setAutoRepeat(false);
        actionStart = new QAction(disp3dMainWindow);
        actionStart->setObjectName(QString::fromUtf8("actionStart"));
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/resource/resource_disp3d/icon15.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionStart->setIcon(icon2);
        actionStop = new QAction(disp3dMainWindow);
        actionStop->setObjectName(QString::fromUtf8("actionStop"));
        QIcon icon3;
        icon3.addFile(QString::fromUtf8(":/resource/resource_disp3d/icon13.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionStop->setIcon(icon3);
        actionReset = new QAction(disp3dMainWindow);
        actionReset->setObjectName(QString::fromUtf8("actionReset"));
        QIcon icon4;
        icon4.addFile(QString::fromUtf8(":/resource/resource_disp3d/icon103.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionReset->setIcon(icon4);
        centralwidget = new QWidget(disp3dMainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        gridLayout = new QGridLayout(centralwidget);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        splitter_2 = new QSplitter(centralwidget);
        splitter_2->setObjectName(QString::fromUtf8("splitter_2"));
        splitter_2->setOrientation(Qt::Horizontal);
        qvtkWidget = new QVTKWidget(splitter_2);
        qvtkWidget->setObjectName(QString::fromUtf8("qvtkWidget"));
        QPalette palette;
        QBrush brush(QColor(255, 255, 255, 255));
        brush.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::Base, brush);
        QBrush brush1(QColor(62, 68, 83, 255));
        brush1.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::Window, brush1);
        palette.setBrush(QPalette::Inactive, QPalette::Base, brush);
        palette.setBrush(QPalette::Inactive, QPalette::Window, brush1);
        palette.setBrush(QPalette::Disabled, QPalette::Base, brush1);
        palette.setBrush(QPalette::Disabled, QPalette::Window, brush1);
        qvtkWidget->setPalette(palette);
        splitter_2->addWidget(qvtkWidget);
        splitter = new QSplitter(splitter_2);
        splitter->setObjectName(QString::fromUtf8("splitter"));
        splitter->setOrientation(Qt::Vertical);
        dockWidgetModel = new QDockWidget(splitter);
        dockWidgetModel->setObjectName(QString::fromUtf8("dockWidgetModel"));
        dockWidgetModel->setMaximumSize(QSize(500, 524287));
        dockWidgetModel->setFloating(false);
        dockWidgetModel->setFeatures(QDockWidget::DockWidgetClosable|QDockWidget::DockWidgetMovable);
        dockWidgetContents = new QWidget();
        dockWidgetContents->setObjectName(QString::fromUtf8("dockWidgetContents"));
        verticalLayout_3 = new QVBoxLayout(dockWidgetContents);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        btnLoadModel = new QPushButton(dockWidgetContents);
        btnLoadModel->setObjectName(QString::fromUtf8("btnLoadModel"));
        btnLoadModel->setMinimumSize(QSize(0, 25));

        verticalLayout_2->addWidget(btnLoadModel);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        comboBoxView = new QComboBox(dockWidgetContents);
        comboBoxView->addItem(QString());
        comboBoxView->addItem(QString());
        comboBoxView->addItem(QString());
        comboBoxView->addItem(QString());
        comboBoxView->addItem(QString());
        comboBoxView->addItem(QString());
        comboBoxView->addItem(QString());
        comboBoxView->setObjectName(QString::fromUtf8("comboBoxView"));
        comboBoxView->setMinimumSize(QSize(0, 25));

        horizontalLayout->addWidget(comboBoxView);

        btnChangeView = new QPushButton(dockWidgetContents);
        btnChangeView->setObjectName(QString::fromUtf8("btnChangeView"));
        btnChangeView->setMinimumSize(QSize(0, 25));

        horizontalLayout->addWidget(btnChangeView);


        verticalLayout_2->addLayout(horizontalLayout);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        checkBoxEdge = new QCheckBox(dockWidgetContents);
        checkBoxEdge->setObjectName(QString::fromUtf8("checkBoxEdge"));
        checkBoxEdge->setMinimumSize(QSize(0, 25));

        horizontalLayout_2->addWidget(checkBoxEdge);

        checkBoxAxis = new QCheckBox(dockWidgetContents);
        checkBoxAxis->setObjectName(QString::fromUtf8("checkBoxAxis"));

        horizontalLayout_2->addWidget(checkBoxAxis);


        verticalLayout_2->addLayout(horizontalLayout_2);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        label_4 = new QLabel(dockWidgetContents);
        label_4->setObjectName(QString::fromUtf8("label_4"));

        horizontalLayout_3->addWidget(label_4);

        editSphereSize = new QLineEdit(dockWidgetContents);
        editSphereSize->setObjectName(QString::fromUtf8("editSphereSize"));
        editSphereSize->setMinimumSize(QSize(0, 30));

        horizontalLayout_3->addWidget(editSphereSize);

        btkSetSize = new QPushButton(dockWidgetContents);
        btkSetSize->setObjectName(QString::fromUtf8("btkSetSize"));
        btkSetSize->setMinimumSize(QSize(0, 30));

        horizontalLayout_3->addWidget(btkSetSize);


        verticalLayout->addLayout(horizontalLayout_3);

        btnAddPoint = new QPushButton(dockWidgetContents);
        btnAddPoint->setObjectName(QString::fromUtf8("btnAddPoint"));
        btnAddPoint->setMinimumSize(QSize(0, 30));

        verticalLayout->addWidget(btnAddPoint);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        label_5 = new QLabel(dockWidgetContents);
        label_5->setObjectName(QString::fromUtf8("label_5"));

        horizontalLayout_4->addWidget(label_5);

        comboBoxHz = new QComboBox(dockWidgetContents);
        comboBoxHz->addItem(QString());
        comboBoxHz->addItem(QString());
        comboBoxHz->addItem(QString());
        comboBoxHz->addItem(QString());
        comboBoxHz->addItem(QString());
        comboBoxHz->setObjectName(QString::fromUtf8("comboBoxHz"));
        comboBoxHz->setMinimumSize(QSize(0, 30));

        horizontalLayout_4->addWidget(comboBoxHz);


        verticalLayout->addLayout(horizontalLayout_4);


        verticalLayout_2->addLayout(verticalLayout);

        verticalSpacer_2 = new QSpacerItem(20, 20, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_2->addItem(verticalSpacer_2);

        groupBox = new QGroupBox(dockWidgetContents);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        gridLayout_3 = new QGridLayout(groupBox);
        gridLayout_3->setObjectName(QString::fromUtf8("gridLayout_3"));
        gridLayout_3->setContentsMargins(0, 15, 0, -1);
        editMaxRange = new QLineEdit(groupBox);
        editMaxRange->setObjectName(QString::fromUtf8("editMaxRange"));
        editMaxRange->setMinimumSize(QSize(0, 30));

        gridLayout_3->addWidget(editMaxRange, 0, 1, 1, 1);

        comboBoxDisp = new QComboBox(groupBox);
        comboBoxDisp->addItem(QString());
        comboBoxDisp->addItem(QString());
        comboBoxDisp->addItem(QString());
        comboBoxDisp->setObjectName(QString::fromUtf8("comboBoxDisp"));
        comboBoxDisp->setMinimumSize(QSize(0, 30));

        gridLayout_3->addWidget(comboBoxDisp, 0, 0, 1, 1);

        btkDispAnalysis = new QPushButton(groupBox);
        btkDispAnalysis->setObjectName(QString::fromUtf8("btkDispAnalysis"));
        btkDispAnalysis->setMinimumSize(QSize(0, 30));

        gridLayout_3->addWidget(btkDispAnalysis, 1, 1, 1, 1);

        gridLayout_3->setRowStretch(0, 5);
        gridLayout_3->setRowStretch(1, 5);
        gridLayout_3->setColumnStretch(0, 5);
        gridLayout_3->setColumnStretch(1, 5);

        verticalLayout_2->addWidget(groupBox);


        verticalLayout_3->addLayout(verticalLayout_2);

        verticalSpacer = new QSpacerItem(20, 82, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_3->addItem(verticalSpacer);

        dockWidgetModel->setWidget(dockWidgetContents);
        splitter->addWidget(dockWidgetModel);
        dockWidgetInfor = new QDockWidget(splitter);
        dockWidgetInfor->setObjectName(QString::fromUtf8("dockWidgetInfor"));
        dockWidgetInfor->setMaximumSize(QSize(500, 524287));
        dockWidgetInfor->setFloating(false);
        dockWidgetInfor->setFeatures(QDockWidget::DockWidgetClosable|QDockWidget::DockWidgetMovable);
        dockWidgetContents_2 = new QWidget();
        dockWidgetContents_2->setObjectName(QString::fromUtf8("dockWidgetContents_2"));
        verticalLayout_4 = new QVBoxLayout(dockWidgetContents_2);
        verticalLayout_4->setObjectName(QString::fromUtf8("verticalLayout_4"));
        label_point = new QLabel(dockWidgetContents_2);
        label_point->setObjectName(QString::fromUtf8("label_point"));

        verticalLayout_4->addWidget(label_point);

        label_element = new QLabel(dockWidgetContents_2);
        label_element->setObjectName(QString::fromUtf8("label_element"));

        verticalLayout_4->addWidget(label_element);

        label_flagpoint = new QLabel(dockWidgetContents_2);
        label_flagpoint->setObjectName(QString::fromUtf8("label_flagpoint"));

        verticalLayout_4->addWidget(label_flagpoint);

        label_dispy = new QLabel(dockWidgetContents_2);
        label_dispy->setObjectName(QString::fromUtf8("label_dispy"));

        verticalLayout_4->addWidget(label_dispy);

        dockWidgetInfor->setWidget(dockWidgetContents_2);
        splitter->addWidget(dockWidgetInfor);
        splitter_2->addWidget(splitter);

        gridLayout->addWidget(splitter_2, 0, 0, 1, 1);

        disp3dMainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(disp3dMainWindow);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 1117, 23));
        disp3dMainWindow->setMenuBar(menubar);
        toolBar = new QToolBar(disp3dMainWindow);
        toolBar->setObjectName(QString::fromUtf8("toolBar"));
        toolBar->setStyleSheet(QString::fromUtf8(""));
        disp3dMainWindow->addToolBar(Qt::RightToolBarArea, toolBar);
        statusBar = new QStatusBar(disp3dMainWindow);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        disp3dMainWindow->setStatusBar(statusBar);

        toolBar->addAction(actionModelDock);
        toolBar->addAction(actionInforDock);
        toolBar->addSeparator();
        toolBar->addAction(actionStart);
        toolBar->addAction(actionStop);
        toolBar->addAction(actionReset);

        retranslateUi(disp3dMainWindow);

        QMetaObject::connectSlotsByName(disp3dMainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *disp3dMainWindow)
    {
        disp3dMainWindow->setWindowTitle(QApplication::translate("disp3dMainWindow", "MainWindow", nullptr));
        actionModelDock->setText(QApplication::translate("disp3dMainWindow", "ModelDock", nullptr));
#ifndef QT_NO_TOOLTIP
        actionModelDock->setToolTip(QApplication::translate("disp3dMainWindow", "ModelDock", nullptr));
#endif // QT_NO_TOOLTIP
        actionInforDock->setText(QApplication::translate("disp3dMainWindow", "InforDock", nullptr));
        actionStart->setText(QApplication::translate("disp3dMainWindow", "Start", nullptr));
#ifndef QT_NO_TOOLTIP
        actionStart->setToolTip(QApplication::translate("disp3dMainWindow", "Start", nullptr));
#endif // QT_NO_TOOLTIP
        actionStop->setText(QApplication::translate("disp3dMainWindow", "Stop", nullptr));
#ifndef QT_NO_TOOLTIP
        actionStop->setToolTip(QApplication::translate("disp3dMainWindow", "Stop", nullptr));
#endif // QT_NO_TOOLTIP
        actionReset->setText(QApplication::translate("disp3dMainWindow", "Reset", nullptr));
        dockWidgetModel->setWindowTitle(QApplication::translate("disp3dMainWindow", "\346\250\241\345\236\213", nullptr));
        btnLoadModel->setText(QApplication::translate("disp3dMainWindow", "\345\257\274\345\205\245\346\250\241\345\236\213\346\225\260\346\215\256", nullptr));
        comboBoxView->setItemText(0, QApplication::translate("disp3dMainWindow", "\346\255\243\346\226\271", nullptr));
        comboBoxView->setItemText(1, QApplication::translate("disp3dMainWindow", "\345\267\246\344\276\247", nullptr));
        comboBoxView->setItemText(2, QApplication::translate("disp3dMainWindow", "\345\220\216\346\226\271", nullptr));
        comboBoxView->setItemText(3, QApplication::translate("disp3dMainWindow", "\345\217\263\344\276\247", nullptr));
        comboBoxView->setItemText(4, QApplication::translate("disp3dMainWindow", "\344\270\213\346\226\271", nullptr));
        comboBoxView->setItemText(5, QApplication::translate("disp3dMainWindow", "\344\270\212\346\226\271", nullptr));
        comboBoxView->setItemText(6, QApplication::translate("disp3dMainWindow", "\346\255\243\344\270\212", nullptr));

        btnChangeView->setText(QApplication::translate("disp3dMainWindow", "\345\210\207\346\215\242\350\247\206\350\247\222", nullptr));
        checkBoxEdge->setText(QApplication::translate("disp3dMainWindow", "\350\276\271\347\274\230\346\230\276\347\244\272", nullptr));
        checkBoxAxis->setText(QApplication::translate("disp3dMainWindow", "\345\235\220\346\240\207\350\275\264", nullptr));
        label_4->setText(QApplication::translate("disp3dMainWindow", "\345\205\263\351\224\256\347\202\271\345\244\247\345\260\217", nullptr));
        btkSetSize->setText(QApplication::translate("disp3dMainWindow", "\345\244\247\345\260\217\350\256\276\347\275\256", nullptr));
        btnAddPoint->setText(QApplication::translate("disp3dMainWindow", "\346\267\273\345\212\240\345\205\263\351\224\256\347\202\271", nullptr));
        label_5->setText(QApplication::translate("disp3dMainWindow", "\345\210\267\346\226\260\351\242\221\347\216\207", nullptr));
        comboBoxHz->setItemText(0, QApplication::translate("disp3dMainWindow", "1hz", nullptr));
        comboBoxHz->setItemText(1, QApplication::translate("disp3dMainWindow", "10hz", nullptr));
        comboBoxHz->setItemText(2, QApplication::translate("disp3dMainWindow", "20hz", nullptr));
        comboBoxHz->setItemText(3, QApplication::translate("disp3dMainWindow", "50hz", nullptr));
        comboBoxHz->setItemText(4, QApplication::translate("disp3dMainWindow", "100hz", nullptr));

        groupBox->setTitle(QString());
        editMaxRange->setText(QApplication::translate("disp3dMainWindow", "30", nullptr));
        comboBoxDisp->setItemText(0, QApplication::translate("disp3dMainWindow", "x\346\226\271\345\220\221\345\217\230\345\275\242", nullptr));
        comboBoxDisp->setItemText(1, QApplication::translate("disp3dMainWindow", "y\346\226\271\345\220\221\345\217\230\345\275\242", nullptr));
        comboBoxDisp->setItemText(2, QApplication::translate("disp3dMainWindow", "z\346\226\271\345\220\221\345\217\230\345\275\242", nullptr));

        btkDispAnalysis->setText(QApplication::translate("disp3dMainWindow", "\351\242\234\350\211\262\346\230\240\345\260\204", nullptr));
        dockWidgetInfor->setWindowTitle(QApplication::translate("disp3dMainWindow", "\344\277\241\346\201\257", nullptr));
        label_point->setText(QApplication::translate("disp3dMainWindow", "\350\212\202\347\202\271\346\225\260:", nullptr));
        label_element->setText(QApplication::translate("disp3dMainWindow", "\345\215\225\345\205\203\346\225\260:", nullptr));
        label_flagpoint->setText(QApplication::translate("disp3dMainWindow", "\345\205\263\351\224\256\347\202\271\346\225\260:", nullptr));
        label_dispy->setText(QApplication::translate("disp3dMainWindow", "\344\270\273\346\226\271\345\220\221\346\234\200\345\244\247\345\217\230\345\275\242:", nullptr));
        toolBar->setWindowTitle(QApplication::translate("disp3dMainWindow", "toolBar", nullptr));
    } // retranslateUi

};

namespace Ui {
    class disp3dMainWindow: public Ui_disp3dMainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DISP3DMAINWINDOW_H
