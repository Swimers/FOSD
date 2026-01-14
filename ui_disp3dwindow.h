/********************************************************************************
** Form generated from reading UI file 'disp3dwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.12.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DISP3DWINDOW_H
#define UI_DISP3DWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDockWidget>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QWidget>
#include "QVTKWidget.h"

QT_BEGIN_NAMESPACE

class Ui_Disp3DWindow
{
public:
    QAction *actionloadmodel;
    QVTKWidget *qvtkWidget;
    QDockWidget *dockWidget;
    QWidget *dockWidgetContents;
    QPushButton *btnLoadModel;
    QDockWidget *dockWidget_2;
    QWidget *dockWidgetContents_2;
    QLabel *label;
    QWidget *widget;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label_2;
    QSpacerItem *horizontalSpacer_2;

    void setupUi(QWidget *Disp3DWindow)
    {
        if (Disp3DWindow->objectName().isEmpty())
            Disp3DWindow->setObjectName(QString::fromUtf8("Disp3DWindow"));
        Disp3DWindow->resize(1105, 788);
        actionloadmodel = new QAction(Disp3DWindow);
        actionloadmodel->setObjectName(QString::fromUtf8("actionloadmodel"));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/resource/resource_disp3d/icon101.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionloadmodel->setIcon(icon);
        qvtkWidget = new QVTKWidget(Disp3DWindow);
        qvtkWidget->setObjectName(QString::fromUtf8("qvtkWidget"));
        qvtkWidget->setGeometry(QRect(30, 130, 741, 401));
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
        dockWidget = new QDockWidget(Disp3DWindow);
        dockWidget->setObjectName(QString::fromUtf8("dockWidget"));
        dockWidget->setGeometry(QRect(470, 60, 351, 251));
        dockWidget->setMaximumSize(QSize(500, 524287));
        dockWidget->setFloating(false);
        dockWidget->setFeatures(QDockWidget::DockWidgetClosable|QDockWidget::DockWidgetMovable);
        dockWidgetContents = new QWidget();
        dockWidgetContents->setObjectName(QString::fromUtf8("dockWidgetContents"));
        btnLoadModel = new QPushButton(dockWidgetContents);
        btnLoadModel->setObjectName(QString::fromUtf8("btnLoadModel"));
        btnLoadModel->setGeometry(QRect(50, 30, 75, 23));
        dockWidget->setWidget(dockWidgetContents);
        dockWidget_2 = new QDockWidget(Disp3DWindow);
        dockWidget_2->setObjectName(QString::fromUtf8("dockWidget_2"));
        dockWidget_2->setGeometry(QRect(790, 10, 231, 171));
        dockWidget_2->setMaximumSize(QSize(500, 524287));
        dockWidget_2->setFloating(false);
        dockWidget_2->setFeatures(QDockWidget::DockWidgetClosable);
        dockWidgetContents_2 = new QWidget();
        dockWidgetContents_2->setObjectName(QString::fromUtf8("dockWidgetContents_2"));
        label = new QLabel(dockWidgetContents_2);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(50, 20, 54, 12));
        widget = new QWidget(dockWidgetContents_2);
        widget->setObjectName(QString::fromUtf8("widget"));
        widget->setGeometry(QRect(-470, 150, 661, 31));
        horizontalLayout_2 = new QHBoxLayout(widget);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        label_2 = new QLabel(widget);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        horizontalLayout_2->addWidget(label_2);

        horizontalSpacer_2 = new QSpacerItem(586, 10, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_2);

        dockWidget_2->setWidget(dockWidgetContents_2);

        retranslateUi(Disp3DWindow);

        QMetaObject::connectSlotsByName(Disp3DWindow);
    } // setupUi

    void retranslateUi(QWidget *Disp3DWindow)
    {
        Disp3DWindow->setWindowTitle(QApplication::translate("Disp3DWindow", "Form", nullptr));
        actionloadmodel->setText(QApplication::translate("Disp3DWindow", "loadmodel", nullptr));
        dockWidget->setWindowTitle(QApplication::translate("Disp3DWindow", "\346\250\241\345\236\213", nullptr));
        btnLoadModel->setText(QApplication::translate("Disp3DWindow", "\346\250\241\345\236\213\346\225\260\346\215\256", nullptr));
        dockWidget_2->setWindowTitle(QApplication::translate("Disp3DWindow", "\344\277\241\346\201\257", nullptr));
        label->setText(QApplication::translate("Disp3DWindow", "\344\277\241\346\201\257\346\240\217", nullptr));
        label_2->setText(QApplication::translate("Disp3DWindow", "\345\216\237\345\247\213\345\235\220\346\240\207", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Disp3DWindow: public Ui_Disp3DWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DISP3DWINDOW_H
