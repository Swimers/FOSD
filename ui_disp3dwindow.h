/********************************************************************************
** Form generated from reading UI file 'disp3dwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.12.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DISP3DWINDOW_H
#define UI_DISP3DWINDOW_H

#include <QVTKWidget.h>
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
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSplitter>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_disp3dwindow
{
public:
    QAction *actionModelDock;
    QAction *actionInforDock;
    QAction *actionStart;
    QAction *actionStop;
    QAction *actionReset;
    QWidget *centralwidget;
    QGridLayout *gridLayout;
    QVTKWidget *vtkw_Disp3D;
    QSplitter *splitter_2;
    QSplitter *splitter;
    QDockWidget *dock_Model;
    QWidget *dockWidgetContents;
<<<<<<< Updated upstream
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
=======
    QVBoxLayout *verticalLayout_3;
    QVBoxLayout *verticalLayout_2;
    QPushButton *pbtn_LoadModel;
    QHBoxLayout *horizontalLayout;
    QComboBox *cbox_SelectView;
    QPushButton *pbtn_ChangeView;
    QHBoxLayout *horizontalLayout_2;
    QCheckBox *cbox_DispEdge;
    QCheckBox *cbox_Fellow;
    QCheckBox *cbox_DispAxis;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout_3;
    QLabel *label_4;
    QLineEdit *edit_SphereSize;
    QPushButton *pbtn_SetSize;
    QHBoxLayout *horizontalLayout_8;
    QSpacerItem *horizontalSpacer_2;
    QPushButton *pbtn_AddPoint;
    QSpacerItem *horizontalSpacer;
    QHBoxLayout *horizontalLayout_4;
    QLabel *label_5;
    QComboBox *cbox_FreSeclect;
    QSpacerItem *verticalSpacer;
    QGroupBox *groupBox;
    QGridLayout *gridLayout_3;
    QLineEdit *edit_ColorMaxRange;
    QComboBox *cbox_AixalSelect;
    QPushButton *pbtn_DispAnalysis;
    QDockWidget *dock_Infor;
    QWidget *dockWidgetContents_2;
    QVBoxLayout *verticalLayout_4;
    QLabel *label_point;
    QLabel *label_element;
    QLabel *label_flagpoint;
    QLabel *label_dispy;
    QToolBar *toolBar;
    QStatusBar *statusBar;
>>>>>>> Stashed changes

    void setupUi(QMainWindow *disp3dwindow)
    {
        if (disp3dwindow->objectName().isEmpty())
            disp3dwindow->setObjectName(QString::fromUtf8("disp3dwindow"));
        disp3dwindow->resize(1002, 600);
        disp3dwindow->setStyleSheet(QString::fromUtf8("QToolBar{\n"
"background-color: rgb(232, 232, 232);\n"
"}"));
        disp3dwindow->setDockOptions(QMainWindow::AllowTabbedDocks|QMainWindow::AnimatedDocks);
        actionModelDock = new QAction(disp3dwindow);
        actionModelDock->setObjectName(QString::fromUtf8("actionModelDock"));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/icon/Resource/icon/Model_icon.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionModelDock->setIcon(icon);
        actionInforDock = new QAction(disp3dwindow);
        actionInforDock->setObjectName(QString::fromUtf8("actionInforDock"));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/icon/Resource/icon/Help_icon.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionInforDock->setIcon(icon1);
        actionStart = new QAction(disp3dwindow);
        actionStart->setObjectName(QString::fromUtf8("actionStart"));
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/icon/Resource/icon/Play_icon.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionStart->setIcon(icon2);
        actionStop = new QAction(disp3dwindow);
        actionStop->setObjectName(QString::fromUtf8("actionStop"));
        QIcon icon3;
        icon3.addFile(QString::fromUtf8(":/icon/Resource/icon/Stop_icon.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionStop->setIcon(icon3);
        actionReset = new QAction(disp3dwindow);
        actionReset->setObjectName(QString::fromUtf8("actionReset"));
        QIcon icon4;
        icon4.addFile(QString::fromUtf8(":/icon/Resource/icon/Reset_icon.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionReset->setIcon(icon4);
        centralwidget = new QWidget(disp3dwindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        gridLayout = new QGridLayout(centralwidget);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        vtkw_Disp3D = new QVTKWidget(centralwidget);
        vtkw_Disp3D->setObjectName(QString::fromUtf8("vtkw_Disp3D"));
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
        vtkw_Disp3D->setPalette(palette);

        gridLayout->addWidget(vtkw_Disp3D, 0, 0, 1, 1);

        splitter_2 = new QSplitter(centralwidget);
        splitter_2->setObjectName(QString::fromUtf8("splitter_2"));
        splitter_2->setOrientation(Qt::Horizontal);
        splitter = new QSplitter(splitter_2);
        splitter->setObjectName(QString::fromUtf8("splitter"));
        splitter->setOrientation(Qt::Vertical);
        dock_Model = new QDockWidget(splitter);
        dock_Model->setObjectName(QString::fromUtf8("dock_Model"));
        dock_Model->setMaximumSize(QSize(250, 524287));
        dock_Model->setFloating(false);
        dock_Model->setFeatures(QDockWidget::DockWidgetClosable|QDockWidget::DockWidgetMovable);
        dockWidgetContents = new QWidget();
        dockWidgetContents->setObjectName(QString::fromUtf8("dockWidgetContents"));
<<<<<<< Updated upstream
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
=======
        verticalLayout_3 = new QVBoxLayout(dockWidgetContents);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        pbtn_LoadModel = new QPushButton(dockWidgetContents);
        pbtn_LoadModel->setObjectName(QString::fromUtf8("pbtn_LoadModel"));
        pbtn_LoadModel->setMinimumSize(QSize(0, 25));

        verticalLayout_2->addWidget(pbtn_LoadModel);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        cbox_SelectView = new QComboBox(dockWidgetContents);
        cbox_SelectView->addItem(QString());
>>>>>>> Stashed changes
        cbox_SelectView->addItem(QString());
        cbox_SelectView->addItem(QString());
        cbox_SelectView->addItem(QString());
        cbox_SelectView->addItem(QString());
        cbox_SelectView->addItem(QString());
        cbox_SelectView->addItem(QString());
        cbox_SelectView->setObjectName(QString::fromUtf8("cbox_SelectView"));
        cbox_SelectView->setMinimumSize(QSize(0, 25));

        horizontalLayout->addWidget(cbox_SelectView);

        pbtn_ChangeView = new QPushButton(dockWidgetContents);
        pbtn_ChangeView->setObjectName(QString::fromUtf8("pbtn_ChangeView"));
        pbtn_ChangeView->setMinimumSize(QSize(0, 25));

        horizontalLayout->addWidget(pbtn_ChangeView);


<<<<<<< Updated upstream
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
=======
        verticalLayout_2->addLayout(horizontalLayout);
>>>>>>> Stashed changes

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        cbox_DispEdge = new QCheckBox(dockWidgetContents);
        cbox_DispEdge->setObjectName(QString::fromUtf8("cbox_DispEdge"));
        cbox_DispEdge->setMinimumSize(QSize(0, 25));

        horizontalLayout_2->addWidget(cbox_DispEdge);

        cbox_Fellow = new QCheckBox(dockWidgetContents);
        cbox_Fellow->setObjectName(QString::fromUtf8("cbox_Fellow"));

        horizontalLayout_2->addWidget(cbox_Fellow);

        cbox_DispAxis = new QCheckBox(dockWidgetContents);
        cbox_DispAxis->setObjectName(QString::fromUtf8("cbox_DispAxis"));

        horizontalLayout_2->addWidget(cbox_DispAxis);


        verticalLayout_2->addLayout(horizontalLayout_2);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        label_4 = new QLabel(dockWidgetContents);
        label_4->setObjectName(QString::fromUtf8("label_4"));

        horizontalLayout_3->addWidget(label_4);

        edit_SphereSize = new QLineEdit(dockWidgetContents);
        edit_SphereSize->setObjectName(QString::fromUtf8("edit_SphereSize"));
        edit_SphereSize->setMinimumSize(QSize(0, 30));

        horizontalLayout_3->addWidget(edit_SphereSize);

        pbtn_SetSize = new QPushButton(dockWidgetContents);
        pbtn_SetSize->setObjectName(QString::fromUtf8("pbtn_SetSize"));
        pbtn_SetSize->setMinimumSize(QSize(0, 30));

        horizontalLayout_3->addWidget(pbtn_SetSize);


        verticalLayout->addLayout(horizontalLayout_3);

        horizontalLayout_8 = new QHBoxLayout();
        horizontalLayout_8->setObjectName(QString::fromUtf8("horizontalLayout_8"));
        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_8->addItem(horizontalSpacer_2);

        pbtn_AddPoint = new QPushButton(dockWidgetContents);
        pbtn_AddPoint->setObjectName(QString::fromUtf8("pbtn_AddPoint"));
        pbtn_AddPoint->setMinimumSize(QSize(0, 30));

        horizontalLayout_8->addWidget(pbtn_AddPoint);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_8->addItem(horizontalSpacer);


        verticalLayout->addLayout(horizontalLayout_8);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        label_5 = new QLabel(dockWidgetContents);
        label_5->setObjectName(QString::fromUtf8("label_5"));

        horizontalLayout_4->addWidget(label_5);

        cbox_FreSeclect = new QComboBox(dockWidgetContents);
        cbox_FreSeclect->addItem(QString());
        cbox_FreSeclect->addItem(QString());
        cbox_FreSeclect->addItem(QString());
        cbox_FreSeclect->addItem(QString());
        cbox_FreSeclect->addItem(QString());
        cbox_FreSeclect->setObjectName(QString::fromUtf8("cbox_FreSeclect"));
        cbox_FreSeclect->setMinimumSize(QSize(0, 30));

        horizontalLayout_4->addWidget(cbox_FreSeclect);


        verticalLayout->addLayout(horizontalLayout_4);


        verticalLayout_2->addLayout(verticalLayout);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_2->addItem(verticalSpacer);

        groupBox = new QGroupBox(dockWidgetContents);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        gridLayout_3 = new QGridLayout(groupBox);
        gridLayout_3->setObjectName(QString::fromUtf8("gridLayout_3"));
        gridLayout_3->setContentsMargins(0, 15, 0, -1);
        edit_ColorMaxRange = new QLineEdit(groupBox);
        edit_ColorMaxRange->setObjectName(QString::fromUtf8("edit_ColorMaxRange"));
        edit_ColorMaxRange->setMinimumSize(QSize(0, 30));

        gridLayout_3->addWidget(edit_ColorMaxRange, 1, 1, 1, 1);

        cbox_AixalSelect = new QComboBox(groupBox);
        cbox_AixalSelect->addItem(QString());
        cbox_AixalSelect->addItem(QString());
        cbox_AixalSelect->addItem(QString());
        cbox_AixalSelect->setObjectName(QString::fromUtf8("cbox_AixalSelect"));
        cbox_AixalSelect->setMinimumSize(QSize(0, 30));

        gridLayout_3->addWidget(cbox_AixalSelect, 1, 0, 1, 1);

        pbtn_DispAnalysis = new QPushButton(groupBox);
        pbtn_DispAnalysis->setObjectName(QString::fromUtf8("pbtn_DispAnalysis"));
        pbtn_DispAnalysis->setMinimumSize(QSize(0, 30));

        gridLayout_3->addWidget(pbtn_DispAnalysis, 2, 1, 1, 1);

        gridLayout_3->setRowStretch(0, 5);
        gridLayout_3->setColumnStretch(0, 5);

        verticalLayout_2->addWidget(groupBox);


        verticalLayout_3->addLayout(verticalLayout_2);

        dock_Model->setWidget(dockWidgetContents);
        splitter->addWidget(dock_Model);
        dock_Infor = new QDockWidget(splitter);
        dock_Infor->setObjectName(QString::fromUtf8("dock_Infor"));
        dock_Infor->setMaximumSize(QSize(300, 524287));
        dock_Infor->setFloating(false);
        dock_Infor->setFeatures(QDockWidget::DockWidgetClosable|QDockWidget::DockWidgetMovable);
<<<<<<< Updated upstream
        dock_Infor->setAllowedAreas(Qt::RightDockWidgetArea);
        dockWidgetContents_3 = new QWidget();
        dockWidgetContents_3->setObjectName(QString::fromUtf8("dockWidgetContents_3"));
        gridLayout = new QGridLayout(dockWidgetContents_3);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        radioButton_3 = new QRadioButton(dockWidgetContents_3);
        radioButton_3->setObjectName(QString::fromUtf8("radioButton_3"));
=======
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
>>>>>>> Stashed changes

        verticalLayout_4->addWidget(label_flagpoint);

        label_dispy = new QLabel(dockWidgetContents_2);
        label_dispy->setObjectName(QString::fromUtf8("label_dispy"));

        verticalLayout_4->addWidget(label_dispy);

<<<<<<< Updated upstream
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
=======
        dock_Infor->setWidget(dockWidgetContents_2);
        splitter->addWidget(dock_Infor);
        splitter_2->addWidget(splitter);
>>>>>>> Stashed changes

        gridLayout->addWidget(splitter_2, 0, 1, 1, 1);

<<<<<<< Updated upstream
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
=======
        disp3dwindow->setCentralWidget(centralwidget);
        toolBar = new QToolBar(disp3dwindow);
        toolBar->setObjectName(QString::fromUtf8("toolBar"));
        toolBar->setMinimumSize(QSize(0, 30));
        disp3dwindow->addToolBar(Qt::TopToolBarArea, toolBar);
        statusBar = new QStatusBar(disp3dwindow);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        disp3dwindow->setStatusBar(statusBar);
>>>>>>> Stashed changes

        toolBar->addAction(actionModelDock);
        toolBar->addAction(actionInforDock);
        toolBar->addSeparator();
        toolBar->addAction(actionStart);
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
        actionStart->setText(QApplication::translate("disp3dwindow", "Start", nullptr));
        actionStop->setText(QApplication::translate("disp3dwindow", "Stop", nullptr));
        actionReset->setText(QApplication::translate("disp3dwindow", "Reset", nullptr));
<<<<<<< Updated upstream
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
=======
        dock_Model->setWindowTitle(QApplication::translate("disp3dwindow", "\346\250\241\345\236\213", nullptr));
        pbtn_LoadModel->setText(QApplication::translate("disp3dwindow", "\345\257\274\345\205\245\346\250\241\345\236\213\346\225\260\346\215\256", nullptr));
        cbox_SelectView->setItemText(0, QApplication::translate("disp3dwindow", "\346\255\243\346\226\271", nullptr));
        cbox_SelectView->setItemText(1, QApplication::translate("disp3dwindow", "\345\267\246\344\276\247", nullptr));
        cbox_SelectView->setItemText(2, QApplication::translate("disp3dwindow", "\345\220\216\346\226\271", nullptr));
        cbox_SelectView->setItemText(3, QApplication::translate("disp3dwindow", "\345\217\263\344\276\247", nullptr));
>>>>>>> Stashed changes
        cbox_SelectView->setItemText(4, QApplication::translate("disp3dwindow", "\344\270\213\346\226\271", nullptr));
        cbox_SelectView->setItemText(5, QApplication::translate("disp3dwindow", "\344\270\212\346\226\271", nullptr));
        cbox_SelectView->setItemText(6, QApplication::translate("disp3dwindow", "\346\255\243\344\270\212", nullptr));

<<<<<<< Updated upstream
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
=======
        pbtn_ChangeView->setText(QApplication::translate("disp3dwindow", "\345\210\207\346\215\242\350\247\206\350\247\222", nullptr));
        cbox_DispEdge->setText(QApplication::translate("disp3dwindow", "\350\276\271\347\274\230\346\230\276\347\244\272", nullptr));
        cbox_Fellow->setText(QApplication::translate("disp3dwindow", "\347\204\246\347\202\271\350\267\237\350\270\252", nullptr));
        cbox_DispAxis->setText(QApplication::translate("disp3dwindow", "\345\235\220\346\240\207\350\275\264", nullptr));
        label_4->setText(QApplication::translate("disp3dwindow", "\345\205\263\351\224\256\347\202\271\345\244\247\345\260\217", nullptr));
        pbtn_SetSize->setText(QApplication::translate("disp3dwindow", "\345\244\247\345\260\217\350\256\276\347\275\256", nullptr));
        pbtn_AddPoint->setText(QApplication::translate("disp3dwindow", "\346\267\273\345\212\240\345\205\263\351\224\256\347\202\271", nullptr));
        label_5->setText(QApplication::translate("disp3dwindow", "\345\210\267\346\226\260\351\242\221\347\216\207", nullptr));
        cbox_FreSeclect->setItemText(0, QApplication::translate("disp3dwindow", "1hz", nullptr));
        cbox_FreSeclect->setItemText(1, QApplication::translate("disp3dwindow", "10hz", nullptr));
        cbox_FreSeclect->setItemText(2, QApplication::translate("disp3dwindow", "20hz", nullptr));
        cbox_FreSeclect->setItemText(3, QApplication::translate("disp3dwindow", "50hz", nullptr));
        cbox_FreSeclect->setItemText(4, QApplication::translate("disp3dwindow", "100hz", nullptr));

        groupBox->setTitle(QString());
        edit_ColorMaxRange->setText(QApplication::translate("disp3dwindow", "30", nullptr));
        cbox_AixalSelect->setItemText(0, QApplication::translate("disp3dwindow", "x\346\226\271\345\220\221\345\217\230\345\275\242", nullptr));
        cbox_AixalSelect->setItemText(1, QApplication::translate("disp3dwindow", "y\346\226\271\345\220\221\345\217\230\345\275\242", nullptr));
        cbox_AixalSelect->setItemText(2, QApplication::translate("disp3dwindow", "z\346\226\271\345\220\221\345\217\230\345\275\242", nullptr));

        pbtn_DispAnalysis->setText(QApplication::translate("disp3dwindow", "\351\242\234\350\211\262\346\230\240\345\260\204", nullptr));
        dock_Infor->setWindowTitle(QApplication::translate("disp3dwindow", "\344\277\241\346\201\257", nullptr));
        label_point->setText(QApplication::translate("disp3dwindow", "\350\212\202\347\202\271\346\225\260:", nullptr));
        label_element->setText(QApplication::translate("disp3dwindow", "\345\215\225\345\205\203\346\225\260:", nullptr));
        label_flagpoint->setText(QApplication::translate("disp3dwindow", "\345\205\263\351\224\256\347\202\271\346\225\260:", nullptr));
        label_dispy->setText(QApplication::translate("disp3dwindow", "\344\270\273\346\226\271\345\220\221\346\234\200\345\244\247\345\217\230\345\275\242:", nullptr));
        toolBar->setWindowTitle(QApplication::translate("disp3dwindow", "toolBar", nullptr));
>>>>>>> Stashed changes
    } // retranslateUi

};

namespace Ui {
    class disp3dwindow: public Ui_disp3dwindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DISP3DWINDOW_H
