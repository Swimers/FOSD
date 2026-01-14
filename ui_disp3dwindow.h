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
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDockWidget>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>
#include "QVTKWidget.h"

QT_BEGIN_NAMESPACE

class Ui_disp3dwindow
{
public:
    QAction *actionModelDock;
    QAction *actionInforDock;
    QAction *actionViewDock;
    QAction *actionStart;
    QAction *actionStop;
    QAction *actionPause;
    QAction *actionReset;
    QWidget *centralwidget;
    QGridLayout *gridLayout_4;
    QVTKWidget *vtk_stage;
    QHBoxLayout *horizontalLayout_2;
    QSpacerItem *horizontalSpacer;
    QLabel *label;
    QLabel *lab_KeyPointID;
    QSpacerItem *horizontalSpacer_3;
    QLabel *label_6;
    QLabel *lab_KeyPointRawPos;
    QSpacerItem *horizontalSpacer_4;
    QLabel *label_10;
    QLabel *lab_KeyPointDisplace;
    QSpacerItem *horizontalSpacer_5;
    QLabel *label_8;
    QLabel *lab_KeyPointNewPos;
    QSpacerItem *horizontalSpacer_2;
    QToolBar *toolBar;
    QDockWidget *dock_Model;
    QWidget *dockWidgetContents;
    QGridLayout *gridLayout_3;
    QPushButton *pbtn_KeyPointSelect;
    QHBoxLayout *horizontalLayout_4;
    QPushButton *pbtn_ScalarSet;
    QPushButton *pbtn_ScalarClear;
    QPushButton *pbtn_ModelExport;
    QHBoxLayout *horizontalLayout;
    QPushButton *pbtn_ModelLoad;
    QPushButton *pbtn_ModelClear;
    QFrame *line;
    QPushButton *pbtn_ModelMerge;
    QLabel *label_3;
    QFrame *line_3;
    QPushButton *pbtn_KeyPointEdit;
    QLabel *label_5;
    QFrame *line_2;
    QComboBox *cbox_FPS;
    QComboBox *cbox_ScalarSelect;
    QSpacerItem *verticalSpacer;
    QDockWidget *dock_View;
    QWidget *dockWidgetContents_2;
    QGridLayout *gridLayout_2;
    QCheckBox *cbox_GridAble;
    QSlider *hsd_GridSize;
    QFrame *line_4;
    QCheckBox *cbox_AxisAble;
    QCheckBox *cbox_CareamLockAble;
    QCheckBox *cbox_FellowAble;
    QSlider *hsd_AxisSize;
    QHBoxLayout *horizontalLayout_3;
    QLabel *label_7;
    QComboBox *cbox_SelectView;
    QPushButton *pbtn_SwitchView;
    QSpacerItem *verticalSpacer_3;
    QHBoxLayout *horizontalLayout_5;
    QSpacerItem *horizontalSpacer_6;
    QCheckBox *cbox_XOZGrid;
    QCheckBox *cbox_XOYGrid;
    QCheckBox *cbox_ZOYGrid;
    QDockWidget *dock_Infor;
    QWidget *dockWidgetContents_3;
    QGridLayout *gridLayout;
    QRadioButton *radioButton_3;
    QRadioButton *radioButton_4;
    QRadioButton *radioButton;
    QLabel *lab_MaxDeformation;
    QPushButton *pushButton_6;
    QLabel *lab_NumPoints;
    QRadioButton *radioButton_2;
    QFrame *line_5;
    QLabel *lab_NumKeyPoints;
    QLabel *lab_NumPolys;
    QSpacerItem *verticalSpacer_2;

    void setupUi(QMainWindow *disp3dwindow)
    {
        if (disp3dwindow->objectName().isEmpty())
            disp3dwindow->setObjectName(QString::fromUtf8("disp3dwindow"));
        disp3dwindow->resize(1024, 766);
        actionModelDock = new QAction(disp3dwindow);
        actionModelDock->setObjectName(QString::fromUtf8("actionModelDock"));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/icon/Resource/icon/3D_Plot.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionModelDock->setIcon(icon);
        actionInforDock = new QAction(disp3dwindow);
        actionInforDock->setObjectName(QString::fromUtf8("actionInforDock"));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/icon/Resource/icon/Help_icon.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionInforDock->setIcon(icon1);
        actionViewDock = new QAction(disp3dwindow);
        actionViewDock->setObjectName(QString::fromUtf8("actionViewDock"));
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/icon/Resource/icon/Model_icon.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionViewDock->setIcon(icon2);
        actionStart = new QAction(disp3dwindow);
        actionStart->setObjectName(QString::fromUtf8("actionStart"));
        QIcon icon3;
        icon3.addFile(QString::fromUtf8(":/icon/Resource/icon/Play_icon.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionStart->setIcon(icon3);
        actionStop = new QAction(disp3dwindow);
        actionStop->setObjectName(QString::fromUtf8("actionStop"));
        QIcon icon4;
        icon4.addFile(QString::fromUtf8(":/icon/Resource/icon/Stop_icon.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionStop->setIcon(icon4);
        actionPause = new QAction(disp3dwindow);
        actionPause->setObjectName(QString::fromUtf8("actionPause"));
        QIcon icon5;
        icon5.addFile(QString::fromUtf8(":/icon/Resource/icon/Pasue_icon.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionPause->setIcon(icon5);
        actionReset = new QAction(disp3dwindow);
        actionReset->setObjectName(QString::fromUtf8("actionReset"));
        QIcon icon6;
        icon6.addFile(QString::fromUtf8(":/icon/Resource/icon/Reset_icon.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionReset->setIcon(icon6);
        centralwidget = new QWidget(disp3dwindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        gridLayout_4 = new QGridLayout(centralwidget);
        gridLayout_4->setObjectName(QString::fromUtf8("gridLayout_4"));
        vtk_stage = new QVTKWidget(centralwidget);
        vtk_stage->setObjectName(QString::fromUtf8("vtk_stage"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(vtk_stage->sizePolicy().hasHeightForWidth());
        vtk_stage->setSizePolicy(sizePolicy);

        gridLayout_4->addWidget(vtk_stage, 1, 0, 1, 1);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        horizontalLayout_2->setSizeConstraint(QLayout::SetNoConstraint);
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer);

        label = new QLabel(centralwidget);
        label->setObjectName(QString::fromUtf8("label"));
        QSizePolicy sizePolicy1(QSizePolicy::Maximum, QSizePolicy::Minimum);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(label->sizePolicy().hasHeightForWidth());
        label->setSizePolicy(sizePolicy1);
        QFont font;
        font.setBold(true);
        font.setWeight(75);
        label->setFont(font);

        horizontalLayout_2->addWidget(label);

        lab_KeyPointID = new QLabel(centralwidget);
        lab_KeyPointID->setObjectName(QString::fromUtf8("lab_KeyPointID"));
        sizePolicy1.setHeightForWidth(lab_KeyPointID->sizePolicy().hasHeightForWidth());
        lab_KeyPointID->setSizePolicy(sizePolicy1);

        horizontalLayout_2->addWidget(lab_KeyPointID);

        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_3);

        label_6 = new QLabel(centralwidget);
        label_6->setObjectName(QString::fromUtf8("label_6"));
        QSizePolicy sizePolicy2(QSizePolicy::Maximum, QSizePolicy::Preferred);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(label_6->sizePolicy().hasHeightForWidth());
        label_6->setSizePolicy(sizePolicy2);
        label_6->setFont(font);

        horizontalLayout_2->addWidget(label_6);

        lab_KeyPointRawPos = new QLabel(centralwidget);
        lab_KeyPointRawPos->setObjectName(QString::fromUtf8("lab_KeyPointRawPos"));
        sizePolicy2.setHeightForWidth(lab_KeyPointRawPos->sizePolicy().hasHeightForWidth());
        lab_KeyPointRawPos->setSizePolicy(sizePolicy2);

        horizontalLayout_2->addWidget(lab_KeyPointRawPos);

        horizontalSpacer_4 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_4);

        label_10 = new QLabel(centralwidget);
        label_10->setObjectName(QString::fromUtf8("label_10"));
        sizePolicy2.setHeightForWidth(label_10->sizePolicy().hasHeightForWidth());
        label_10->setSizePolicy(sizePolicy2);
        label_10->setFont(font);

        horizontalLayout_2->addWidget(label_10);

        lab_KeyPointDisplace = new QLabel(centralwidget);
        lab_KeyPointDisplace->setObjectName(QString::fromUtf8("lab_KeyPointDisplace"));
        sizePolicy2.setHeightForWidth(lab_KeyPointDisplace->sizePolicy().hasHeightForWidth());
        lab_KeyPointDisplace->setSizePolicy(sizePolicy2);

        horizontalLayout_2->addWidget(lab_KeyPointDisplace);

        horizontalSpacer_5 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_5);

        label_8 = new QLabel(centralwidget);
        label_8->setObjectName(QString::fromUtf8("label_8"));
        sizePolicy2.setHeightForWidth(label_8->sizePolicy().hasHeightForWidth());
        label_8->setSizePolicy(sizePolicy2);
        label_8->setFont(font);

        horizontalLayout_2->addWidget(label_8);

        lab_KeyPointNewPos = new QLabel(centralwidget);
        lab_KeyPointNewPos->setObjectName(QString::fromUtf8("lab_KeyPointNewPos"));
        sizePolicy2.setHeightForWidth(lab_KeyPointNewPos->sizePolicy().hasHeightForWidth());
        lab_KeyPointNewPos->setSizePolicy(sizePolicy2);

        horizontalLayout_2->addWidget(lab_KeyPointNewPos);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_2);


        gridLayout_4->addLayout(horizontalLayout_2, 2, 0, 1, 1);

        disp3dwindow->setCentralWidget(centralwidget);
        toolBar = new QToolBar(disp3dwindow);
        toolBar->setObjectName(QString::fromUtf8("toolBar"));
        disp3dwindow->addToolBar(Qt::TopToolBarArea, toolBar);
        dock_Model = new QDockWidget(disp3dwindow);
        dock_Model->setObjectName(QString::fromUtf8("dock_Model"));
        dock_Model->setFeatures(QDockWidget::DockWidgetClosable|QDockWidget::DockWidgetMovable);
        dock_Model->setAllowedAreas(Qt::RightDockWidgetArea);
        dockWidgetContents = new QWidget();
        dockWidgetContents->setObjectName(QString::fromUtf8("dockWidgetContents"));
        gridLayout_3 = new QGridLayout(dockWidgetContents);
        gridLayout_3->setObjectName(QString::fromUtf8("gridLayout_3"));
        pbtn_KeyPointSelect = new QPushButton(dockWidgetContents);
        pbtn_KeyPointSelect->setObjectName(QString::fromUtf8("pbtn_KeyPointSelect"));

        gridLayout_3->addWidget(pbtn_KeyPointSelect, 3, 1, 1, 1);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        pbtn_ScalarSet = new QPushButton(dockWidgetContents);
        pbtn_ScalarSet->setObjectName(QString::fromUtf8("pbtn_ScalarSet"));

        horizontalLayout_4->addWidget(pbtn_ScalarSet);

        pbtn_ScalarClear = new QPushButton(dockWidgetContents);
        pbtn_ScalarClear->setObjectName(QString::fromUtf8("pbtn_ScalarClear"));
        pbtn_ScalarClear->setMaximumSize(QSize(40, 16777215));

        horizontalLayout_4->addWidget(pbtn_ScalarClear);


        gridLayout_3->addLayout(horizontalLayout_4, 8, 0, 1, 2);

        pbtn_ModelExport = new QPushButton(dockWidgetContents);
        pbtn_ModelExport->setObjectName(QString::fromUtf8("pbtn_ModelExport"));

        gridLayout_3->addWidget(pbtn_ModelExport, 1, 1, 1, 1);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        pbtn_ModelLoad = new QPushButton(dockWidgetContents);
        pbtn_ModelLoad->setObjectName(QString::fromUtf8("pbtn_ModelLoad"));
        pbtn_ModelLoad->setMaximumSize(QSize(240, 23));

        horizontalLayout->addWidget(pbtn_ModelLoad);

        pbtn_ModelClear = new QPushButton(dockWidgetContents);
        pbtn_ModelClear->setObjectName(QString::fromUtf8("pbtn_ModelClear"));
        pbtn_ModelClear->setMaximumSize(QSize(40, 16777215));

        horizontalLayout->addWidget(pbtn_ModelClear);


        gridLayout_3->addLayout(horizontalLayout, 0, 0, 1, 2);

        line = new QFrame(dockWidgetContents);
        line->setObjectName(QString::fromUtf8("line"));
        line->setFrameShape(QFrame::HLine);
        line->setFrameShadow(QFrame::Sunken);

        gridLayout_3->addWidget(line, 2, 0, 1, 2);

        pbtn_ModelMerge = new QPushButton(dockWidgetContents);
        pbtn_ModelMerge->setObjectName(QString::fromUtf8("pbtn_ModelMerge"));

        gridLayout_3->addWidget(pbtn_ModelMerge, 1, 0, 1, 1);

        label_3 = new QLabel(dockWidgetContents);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setAlignment(Qt::AlignCenter);

        gridLayout_3->addWidget(label_3, 5, 0, 1, 1);

        line_3 = new QFrame(dockWidgetContents);
        line_3->setObjectName(QString::fromUtf8("line_3"));
        line_3->setFrameShape(QFrame::HLine);
        line_3->setFrameShadow(QFrame::Sunken);

        gridLayout_3->addWidget(line_3, 6, 0, 1, 2);

        pbtn_KeyPointEdit = new QPushButton(dockWidgetContents);
        pbtn_KeyPointEdit->setObjectName(QString::fromUtf8("pbtn_KeyPointEdit"));

        gridLayout_3->addWidget(pbtn_KeyPointEdit, 3, 0, 1, 1);

        label_5 = new QLabel(dockWidgetContents);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        label_5->setAlignment(Qt::AlignCenter);

        gridLayout_3->addWidget(label_5, 7, 0, 1, 1);

        line_2 = new QFrame(dockWidgetContents);
        line_2->setObjectName(QString::fromUtf8("line_2"));
        line_2->setFrameShape(QFrame::HLine);
        line_2->setFrameShadow(QFrame::Sunken);

        gridLayout_3->addWidget(line_2, 4, 0, 1, 2);

        cbox_FPS = new QComboBox(dockWidgetContents);
        cbox_FPS->addItem(QString());
        cbox_FPS->addItem(QString());
        cbox_FPS->addItem(QString());
        cbox_FPS->addItem(QString());
        cbox_FPS->addItem(QString());
        cbox_FPS->setObjectName(QString::fromUtf8("cbox_FPS"));

        gridLayout_3->addWidget(cbox_FPS, 5, 1, 1, 1);

        cbox_ScalarSelect = new QComboBox(dockWidgetContents);
        cbox_ScalarSelect->addItem(QString());
        cbox_ScalarSelect->addItem(QString());
        cbox_ScalarSelect->addItem(QString());
        cbox_ScalarSelect->setObjectName(QString::fromUtf8("cbox_ScalarSelect"));

        gridLayout_3->addWidget(cbox_ScalarSelect, 7, 1, 1, 1);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_3->addItem(verticalSpacer, 9, 0, 1, 1);

        dock_Model->setWidget(dockWidgetContents);
        disp3dwindow->addDockWidget(static_cast<Qt::DockWidgetArea>(2), dock_Model);
        dock_View = new QDockWidget(disp3dwindow);
        dock_View->setObjectName(QString::fromUtf8("dock_View"));
        dock_View->setFeatures(QDockWidget::DockWidgetClosable|QDockWidget::DockWidgetMovable);
        dock_View->setAllowedAreas(Qt::RightDockWidgetArea);
        dockWidgetContents_2 = new QWidget();
        dockWidgetContents_2->setObjectName(QString::fromUtf8("dockWidgetContents_2"));
        gridLayout_2 = new QGridLayout(dockWidgetContents_2);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        cbox_GridAble = new QCheckBox(dockWidgetContents_2);
        cbox_GridAble->setObjectName(QString::fromUtf8("cbox_GridAble"));

        gridLayout_2->addWidget(cbox_GridAble, 1, 0, 1, 1);

        hsd_GridSize = new QSlider(dockWidgetContents_2);
        hsd_GridSize->setObjectName(QString::fromUtf8("hsd_GridSize"));
        hsd_GridSize->setOrientation(Qt::Horizontal);

        gridLayout_2->addWidget(hsd_GridSize, 1, 1, 1, 1);

        line_4 = new QFrame(dockWidgetContents_2);
        line_4->setObjectName(QString::fromUtf8("line_4"));
        line_4->setFrameShape(QFrame::HLine);
        line_4->setFrameShadow(QFrame::Sunken);

        gridLayout_2->addWidget(line_4, 6, 0, 1, 2);

        cbox_AxisAble = new QCheckBox(dockWidgetContents_2);
        cbox_AxisAble->setObjectName(QString::fromUtf8("cbox_AxisAble"));

        gridLayout_2->addWidget(cbox_AxisAble, 0, 0, 1, 1);

        cbox_CareamLockAble = new QCheckBox(dockWidgetContents_2);
        cbox_CareamLockAble->setObjectName(QString::fromUtf8("cbox_CareamLockAble"));

        gridLayout_2->addWidget(cbox_CareamLockAble, 5, 0, 1, 1);

        cbox_FellowAble = new QCheckBox(dockWidgetContents_2);
        cbox_FellowAble->setObjectName(QString::fromUtf8("cbox_FellowAble"));

        gridLayout_2->addWidget(cbox_FellowAble, 5, 1, 1, 1);

        hsd_AxisSize = new QSlider(dockWidgetContents_2);
        hsd_AxisSize->setObjectName(QString::fromUtf8("hsd_AxisSize"));
        hsd_AxisSize->setOrientation(Qt::Horizontal);

        gridLayout_2->addWidget(hsd_AxisSize, 0, 1, 1, 1);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        label_7 = new QLabel(dockWidgetContents_2);
        label_7->setObjectName(QString::fromUtf8("label_7"));
        label_7->setAlignment(Qt::AlignCenter);

        horizontalLayout_3->addWidget(label_7);

        cbox_SelectView = new QComboBox(dockWidgetContents_2);
        cbox_SelectView->addItem(QString());
        cbox_SelectView->addItem(QString());
        cbox_SelectView->addItem(QString());
        cbox_SelectView->addItem(QString());
        cbox_SelectView->addItem(QString());
        cbox_SelectView->addItem(QString());
        cbox_SelectView->setObjectName(QString::fromUtf8("cbox_SelectView"));

        horizontalLayout_3->addWidget(cbox_SelectView);

        pbtn_SwitchView = new QPushButton(dockWidgetContents_2);
        pbtn_SwitchView->setObjectName(QString::fromUtf8("pbtn_SwitchView"));
        pbtn_SwitchView->setMaximumSize(QSize(40, 16777215));

        horizontalLayout_3->addWidget(pbtn_SwitchView);


        gridLayout_2->addLayout(horizontalLayout_3, 7, 0, 1, 2);

        verticalSpacer_3 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_2->addItem(verticalSpacer_3, 8, 0, 1, 1);

        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setObjectName(QString::fromUtf8("horizontalLayout_5"));
        horizontalSpacer_6 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_5->addItem(horizontalSpacer_6);

        cbox_XOZGrid = new QCheckBox(dockWidgetContents_2);
        cbox_XOZGrid->setObjectName(QString::fromUtf8("cbox_XOZGrid"));
        cbox_XOZGrid->setMinimumSize(QSize(37, 0));
        cbox_XOZGrid->setMaximumSize(QSize(37, 16777215));

        horizontalLayout_5->addWidget(cbox_XOZGrid);

        cbox_XOYGrid = new QCheckBox(dockWidgetContents_2);
        cbox_XOYGrid->setObjectName(QString::fromUtf8("cbox_XOYGrid"));
        cbox_XOYGrid->setMinimumSize(QSize(37, 0));
        cbox_XOYGrid->setMaximumSize(QSize(37, 16777215));

        horizontalLayout_5->addWidget(cbox_XOYGrid);

        cbox_ZOYGrid = new QCheckBox(dockWidgetContents_2);
        cbox_ZOYGrid->setObjectName(QString::fromUtf8("cbox_ZOYGrid"));
        cbox_ZOYGrid->setMinimumSize(QSize(37, 0));
        cbox_ZOYGrid->setMaximumSize(QSize(37, 16777215));

        horizontalLayout_5->addWidget(cbox_ZOYGrid);


        gridLayout_2->addLayout(horizontalLayout_5, 3, 0, 1, 2);

        dock_View->setWidget(dockWidgetContents_2);
        disp3dwindow->addDockWidget(static_cast<Qt::DockWidgetArea>(2), dock_View);
        dock_Infor = new QDockWidget(disp3dwindow);
        dock_Infor->setObjectName(QString::fromUtf8("dock_Infor"));
        dock_Infor->setFeatures(QDockWidget::DockWidgetClosable|QDockWidget::DockWidgetMovable);
        dock_Infor->setAllowedAreas(Qt::RightDockWidgetArea);
        dockWidgetContents_3 = new QWidget();
        dockWidgetContents_3->setObjectName(QString::fromUtf8("dockWidgetContents_3"));
        gridLayout = new QGridLayout(dockWidgetContents_3);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        radioButton_3 = new QRadioButton(dockWidgetContents_3);
        radioButton_3->setObjectName(QString::fromUtf8("radioButton_3"));

        gridLayout->addWidget(radioButton_3, 2, 0, 1, 1);

        radioButton_4 = new QRadioButton(dockWidgetContents_3);
        radioButton_4->setObjectName(QString::fromUtf8("radioButton_4"));

        gridLayout->addWidget(radioButton_4, 3, 0, 1, 1);

        radioButton = new QRadioButton(dockWidgetContents_3);
        radioButton->setObjectName(QString::fromUtf8("radioButton"));

        gridLayout->addWidget(radioButton, 0, 0, 1, 1);

        lab_MaxDeformation = new QLabel(dockWidgetContents_3);
        lab_MaxDeformation->setObjectName(QString::fromUtf8("lab_MaxDeformation"));

        gridLayout->addWidget(lab_MaxDeformation, 3, 1, 1, 1);

        pushButton_6 = new QPushButton(dockWidgetContents_3);
        pushButton_6->setObjectName(QString::fromUtf8("pushButton_6"));

        gridLayout->addWidget(pushButton_6, 5, 0, 1, 2);

        lab_NumPoints = new QLabel(dockWidgetContents_3);
        lab_NumPoints->setObjectName(QString::fromUtf8("lab_NumPoints"));

        gridLayout->addWidget(lab_NumPoints, 0, 1, 1, 1);

        radioButton_2 = new QRadioButton(dockWidgetContents_3);
        radioButton_2->setObjectName(QString::fromUtf8("radioButton_2"));

        gridLayout->addWidget(radioButton_2, 1, 0, 1, 1);

        line_5 = new QFrame(dockWidgetContents_3);
        line_5->setObjectName(QString::fromUtf8("line_5"));
        line_5->setFrameShape(QFrame::HLine);
        line_5->setFrameShadow(QFrame::Sunken);

        gridLayout->addWidget(line_5, 4, 0, 1, 2);

        lab_NumKeyPoints = new QLabel(dockWidgetContents_3);
        lab_NumKeyPoints->setObjectName(QString::fromUtf8("lab_NumKeyPoints"));

        gridLayout->addWidget(lab_NumKeyPoints, 2, 1, 1, 1);

        lab_NumPolys = new QLabel(dockWidgetContents_3);
        lab_NumPolys->setObjectName(QString::fromUtf8("lab_NumPolys"));

        gridLayout->addWidget(lab_NumPolys, 1, 1, 1, 1);

        verticalSpacer_2 = new QSpacerItem(20, 3, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout->addItem(verticalSpacer_2, 6, 0, 1, 1);

        dock_Infor->setWidget(dockWidgetContents_3);
        disp3dwindow->addDockWidget(static_cast<Qt::DockWidgetArea>(2), dock_Infor);

        toolBar->addAction(actionModelDock);
        toolBar->addAction(actionInforDock);
        toolBar->addAction(actionViewDock);
        toolBar->addSeparator();
        toolBar->addAction(actionStart);
        toolBar->addAction(actionPause);
        toolBar->addAction(actionStop);
        toolBar->addAction(actionReset);

        retranslateUi(disp3dwindow);

        QMetaObject::connectSlotsByName(disp3dwindow);
    } // setupUi

    void retranslateUi(QMainWindow *disp3dwindow)
    {
        disp3dwindow->setWindowTitle(QApplication::translate("disp3dwindow", "MainWindow", nullptr));
        actionModelDock->setText(QApplication::translate("disp3dwindow", "ModelDock", nullptr));
        actionInforDock->setText(QApplication::translate("disp3dwindow", "InforDock", nullptr));
        actionViewDock->setText(QApplication::translate("disp3dwindow", "ViewDock", nullptr));
        actionStart->setText(QApplication::translate("disp3dwindow", "Start", nullptr));
        actionStop->setText(QApplication::translate("disp3dwindow", "Stop", nullptr));
        actionPause->setText(QApplication::translate("disp3dwindow", "Pause", nullptr));
        actionReset->setText(QApplication::translate("disp3dwindow", "Reset", nullptr));
        label->setText(QApplication::translate("disp3dwindow", "\345\275\223\345\211\215\346\213\276\345\217\226\347\202\271\357\274\232", nullptr));
        lab_KeyPointID->setText(QApplication::translate("disp3dwindow", "###", nullptr));
        label_6->setText(QApplication::translate("disp3dwindow", "\345\210\235\345\247\213\345\235\220\346\240\207\357\274\232", nullptr));
        lab_KeyPointRawPos->setText(QApplication::translate("disp3dwindow", "(#,#,#)", nullptr));
        label_10->setText(QApplication::translate("disp3dwindow", "\345\217\230\345\275\242\351\207\217\357\274\232", nullptr));
        lab_KeyPointDisplace->setText(QApplication::translate("disp3dwindow", "(#,#,#)", nullptr));
        label_8->setText(QApplication::translate("disp3dwindow", "\345\275\223\345\211\215\345\235\220\346\240\207\357\274\232", nullptr));
        lab_KeyPointNewPos->setText(QApplication::translate("disp3dwindow", "(#,#,#)", nullptr));
        toolBar->setWindowTitle(QApplication::translate("disp3dwindow", "toolBar", nullptr));
        dock_Model->setWindowTitle(QApplication::translate("disp3dwindow", "\346\250\241\345\236\213\351\205\215\347\275\256", nullptr));
        pbtn_KeyPointSelect->setText(QApplication::translate("disp3dwindow", "\346\213\276\345\217\226\345\205\263\351\224\256\347\202\271", nullptr));
        pbtn_ScalarSet->setText(QApplication::translate("disp3dwindow", "\351\242\234\350\211\262\346\230\240\345\260\204", nullptr));
        pbtn_ScalarClear->setText(QApplication::translate("disp3dwindow", "\346\270\205\351\231\244", nullptr));
        pbtn_ModelExport->setText(QApplication::translate("disp3dwindow", "\346\250\241\345\236\213\345\257\274\345\207\272", nullptr));
        pbtn_ModelLoad->setText(QApplication::translate("disp3dwindow", "\346\250\241\345\236\213\345\257\274\345\205\245", nullptr));
        pbtn_ModelClear->setText(QApplication::translate("disp3dwindow", "\346\270\205\351\231\244", nullptr));
        pbtn_ModelMerge->setText(QApplication::translate("disp3dwindow", "\346\250\241\345\236\213\345\220\210\345\271\266", nullptr));
        label_3->setText(QApplication::translate("disp3dwindow", "\345\210\267\346\226\260\347\216\207\357\274\232", nullptr));
        pbtn_KeyPointEdit->setText(QApplication::translate("disp3dwindow", "\347\274\226\350\276\221\345\205\263\351\224\256\347\202\271", nullptr));
        label_5->setText(QApplication::translate("disp3dwindow", "\345\217\257\350\247\206\345\214\226\346\226\271\346\263\225\357\274\232", nullptr));
        cbox_FPS->setItemText(0, QApplication::translate("disp3dwindow", "1Hz", nullptr));
        cbox_FPS->setItemText(1, QApplication::translate("disp3dwindow", "10Hz", nullptr));
        cbox_FPS->setItemText(2, QApplication::translate("disp3dwindow", "32Hz", nullptr));
        cbox_FPS->setItemText(3, QApplication::translate("disp3dwindow", "60Hz", nullptr));
        cbox_FPS->setItemText(4, QApplication::translate("disp3dwindow", "144Hz", nullptr));

        cbox_ScalarSelect->setItemText(0, QApplication::translate("disp3dwindow", "X\350\275\264\345\220\221\345\217\230\345\275\242", nullptr));
        cbox_ScalarSelect->setItemText(1, QApplication::translate("disp3dwindow", "Y\350\275\264\345\220\221\345\217\230\345\275\242", nullptr));
        cbox_ScalarSelect->setItemText(2, QApplication::translate("disp3dwindow", "Z\350\275\264\345\220\221\345\217\230\345\275\242", nullptr));

        dock_View->setWindowTitle(QApplication::translate("disp3dwindow", "\347\275\221\346\240\274\350\247\206\345\233\276", nullptr));
        cbox_GridAble->setText(QApplication::translate("disp3dwindow", "\347\275\221\346\240\274\346\230\276\347\244\272", nullptr));
        cbox_AxisAble->setText(QApplication::translate("disp3dwindow", "\345\235\220\346\240\207\350\275\264\346\230\276\347\244\272", nullptr));
        cbox_CareamLockAble->setText(QApplication::translate("disp3dwindow", "\350\247\206\350\247\222\351\224\201\345\256\232", nullptr));
        cbox_FellowAble->setText(QApplication::translate("disp3dwindow", "\347\204\246\347\202\271\350\267\237\350\270\252", nullptr));
        label_7->setText(QApplication::translate("disp3dwindow", "\347\233\270\346\234\272\350\247\206\350\247\222\357\274\232", nullptr));
        cbox_SelectView->setItemText(0, QApplication::translate("disp3dwindow", "\346\255\243\345\211\215", nullptr));
        cbox_SelectView->setItemText(1, QApplication::translate("disp3dwindow", "\345\217\263\344\276\247", nullptr));
        cbox_SelectView->setItemText(2, QApplication::translate("disp3dwindow", "\345\267\246\344\276\247", nullptr));
        cbox_SelectView->setItemText(3, QApplication::translate("disp3dwindow", "\344\270\212\346\226\271", nullptr));
        cbox_SelectView->setItemText(4, QApplication::translate("disp3dwindow", "\344\270\213\346\226\271", nullptr));
        cbox_SelectView->setItemText(5, QApplication::translate("disp3dwindow", "\346\255\243\345\220\216", nullptr));

        pbtn_SwitchView->setText(QApplication::translate("disp3dwindow", "\345\210\207\346\215\242", nullptr));
        cbox_XOZGrid->setText(QApplication::translate("disp3dwindow", "XOZ", nullptr));
        cbox_XOYGrid->setText(QApplication::translate("disp3dwindow", "XOY", nullptr));
        cbox_ZOYGrid->setText(QApplication::translate("disp3dwindow", "ZOY", nullptr));
        dock_Infor->setWindowTitle(QApplication::translate("disp3dwindow", "\345\217\257\350\247\206\345\214\226\345\217\202\346\225\260", nullptr));
        radioButton_3->setText(QApplication::translate("disp3dwindow", "\345\205\263\351\224\256\347\202\271\346\225\260\357\274\232", nullptr));
        radioButton_4->setText(QApplication::translate("disp3dwindow", "\344\270\273\346\226\271\345\220\221\346\234\200\345\244\247\345\217\230\345\275\242\357\274\232", nullptr));
        radioButton->setText(QApplication::translate("disp3dwindow", "\350\212\202\347\202\271\346\225\260\357\274\232", nullptr));
        lab_MaxDeformation->setText(QString());
        pushButton_6->setText(QApplication::translate("disp3dwindow", "\345\205\263\351\224\256\347\202\271\345\217\230\345\275\242\346\230\276\347\244\272", nullptr));
        lab_NumPoints->setText(QString());
        radioButton_2->setText(QApplication::translate("disp3dwindow", "\344\270\211\350\247\222\345\275\242\346\225\260\357\274\232", nullptr));
        lab_NumKeyPoints->setText(QString());
        lab_NumPolys->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class disp3dwindow: public Ui_disp3dwindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DISP3DWINDOW_H
