/********************************************************************************
** Form generated from reading UI file 'disp3dkeypointedit.ui'
**
** Created by: Qt User Interface Compiler version 5.12.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DISP3DKEYPOINTEDIT_H
#define UI_DISP3DKEYPOINTEDIT_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDockWidget>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_disp3dkeypointedit
{
public:
    QHBoxLayout *horizontalLayout;
    QTabWidget *tabWidget;
    QWidget *tabAll;
    QGridLayout *gridLayout_3;
    QPushButton *pbtn_SelectAll;
    QPushButton *pbtn_ClearAll;
    QWidget *tab;
    QVBoxLayout *verticalLayout;
    QSpacerItem *verticalSpacer_3;
    QLabel *label_12;
    QHBoxLayout *horizontalLayout_4;
    QLabel *label_4;
    QLineEdit *edit_Xmin;
    QLineEdit *edit_Ymin;
    QLineEdit *edit_Zmin;
    QHBoxLayout *horizontalLayout_6;
    QLabel *label_11;
    QLineEdit *edit_Xmax;
    QLineEdit *edit_Ymax;
    QLineEdit *edit_Zmax;
    QFrame *line_2;
    QHBoxLayout *horizontalLayout_7;
    QPushButton *pbtn_DispCube;
    QPushButton *pushButton;
    QPushButton *pbtn_SelectCube;
    QPushButton *pbtn_ClearCube;
    QSpacerItem *verticalSpacer_2;
    QWidget *tab_2;
    QDockWidget *InforDock;
    QWidget *dockWidgetContents;
    QGridLayout *gridLayout;
    QLabel *lab_NumKeyPoint;
    QLabel *lab_NumPoint;
    QLabel *label;
    QSpacerItem *verticalSpacer;
    QFrame *line;
    QLabel *label_3;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label_5;
    QLineEdit *edit_IDKeyPoint;
    QPushButton *pbtn_FindKeyPoint;
    QHBoxLayout *horizontalLayout_3;
    QLabel *label_2;
    QComboBox *cbox_IDKeyPoint;
    QGridLayout *gridLayout_2;
    QLabel *label_6;
    QLabel *lab_ZPosition;
    QLabel *label_7;
    QLabel *lab_YPosition;
    QLabel *lab_XPosition;
    QLabel *label_8;
    QLabel *label_9;

    void setupUi(QWidget *disp3dkeypointedit)
    {
        if (disp3dkeypointedit->objectName().isEmpty())
            disp3dkeypointedit->setObjectName(QString::fromUtf8("disp3dkeypointedit"));
        disp3dkeypointedit->resize(548, 373);
        horizontalLayout = new QHBoxLayout(disp3dkeypointedit);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        tabWidget = new QTabWidget(disp3dkeypointedit);
        tabWidget->setObjectName(QString::fromUtf8("tabWidget"));
        tabAll = new QWidget();
        tabAll->setObjectName(QString::fromUtf8("tabAll"));
        gridLayout_3 = new QGridLayout(tabAll);
        gridLayout_3->setObjectName(QString::fromUtf8("gridLayout_3"));
        pbtn_SelectAll = new QPushButton(tabAll);
        pbtn_SelectAll->setObjectName(QString::fromUtf8("pbtn_SelectAll"));

        gridLayout_3->addWidget(pbtn_SelectAll, 0, 0, 1, 1);

        pbtn_ClearAll = new QPushButton(tabAll);
        pbtn_ClearAll->setObjectName(QString::fromUtf8("pbtn_ClearAll"));

        gridLayout_3->addWidget(pbtn_ClearAll, 1, 0, 1, 1);

        tabWidget->addTab(tabAll, QString());
        tab = new QWidget();
        tab->setObjectName(QString::fromUtf8("tab"));
        verticalLayout = new QVBoxLayout(tab);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalSpacer_3 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer_3);

        label_12 = new QLabel(tab);
        label_12->setObjectName(QString::fromUtf8("label_12"));
        QFont font;
        font.setFamily(QString::fromUtf8("Agency FB"));
        font.setPointSize(14);
        label_12->setFont(font);
        label_12->setAlignment(Qt::AlignCenter);

        verticalLayout->addWidget(label_12);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        label_4 = new QLabel(tab);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        QFont font1;
        font1.setFamily(QString::fromUtf8("Agency FB"));
        font1.setPointSize(12);
        label_4->setFont(font1);

        horizontalLayout_4->addWidget(label_4);

        edit_Xmin = new QLineEdit(tab);
        edit_Xmin->setObjectName(QString::fromUtf8("edit_Xmin"));
        edit_Xmin->setMinimumSize(QSize(0, 30));
        edit_Xmin->setFont(font1);

        horizontalLayout_4->addWidget(edit_Xmin);

        edit_Ymin = new QLineEdit(tab);
        edit_Ymin->setObjectName(QString::fromUtf8("edit_Ymin"));
        edit_Ymin->setMinimumSize(QSize(0, 30));
        edit_Ymin->setFont(font1);

        horizontalLayout_4->addWidget(edit_Ymin);

        edit_Zmin = new QLineEdit(tab);
        edit_Zmin->setObjectName(QString::fromUtf8("edit_Zmin"));
        edit_Zmin->setMinimumSize(QSize(0, 30));
        edit_Zmin->setFont(font1);

        horizontalLayout_4->addWidget(edit_Zmin);


        verticalLayout->addLayout(horizontalLayout_4);

        horizontalLayout_6 = new QHBoxLayout();
        horizontalLayout_6->setObjectName(QString::fromUtf8("horizontalLayout_6"));
        label_11 = new QLabel(tab);
        label_11->setObjectName(QString::fromUtf8("label_11"));
        label_11->setFont(font1);

        horizontalLayout_6->addWidget(label_11);

        edit_Xmax = new QLineEdit(tab);
        edit_Xmax->setObjectName(QString::fromUtf8("edit_Xmax"));
        edit_Xmax->setMinimumSize(QSize(0, 30));
        edit_Xmax->setFont(font1);

        horizontalLayout_6->addWidget(edit_Xmax);

        edit_Ymax = new QLineEdit(tab);
        edit_Ymax->setObjectName(QString::fromUtf8("edit_Ymax"));
        edit_Ymax->setMinimumSize(QSize(0, 30));
        edit_Ymax->setFont(font1);

        horizontalLayout_6->addWidget(edit_Ymax);

        edit_Zmax = new QLineEdit(tab);
        edit_Zmax->setObjectName(QString::fromUtf8("edit_Zmax"));
        edit_Zmax->setMinimumSize(QSize(0, 30));
        edit_Zmax->setFont(font1);

        horizontalLayout_6->addWidget(edit_Zmax);


        verticalLayout->addLayout(horizontalLayout_6);

        line_2 = new QFrame(tab);
        line_2->setObjectName(QString::fromUtf8("line_2"));
        line_2->setFrameShape(QFrame::HLine);
        line_2->setFrameShadow(QFrame::Sunken);

        verticalLayout->addWidget(line_2);

        horizontalLayout_7 = new QHBoxLayout();
        horizontalLayout_7->setObjectName(QString::fromUtf8("horizontalLayout_7"));
        pbtn_DispCube = new QPushButton(tab);
        pbtn_DispCube->setObjectName(QString::fromUtf8("pbtn_DispCube"));

        horizontalLayout_7->addWidget(pbtn_DispCube);

        pushButton = new QPushButton(tab);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));

        horizontalLayout_7->addWidget(pushButton);

        pbtn_SelectCube = new QPushButton(tab);
        pbtn_SelectCube->setObjectName(QString::fromUtf8("pbtn_SelectCube"));

        horizontalLayout_7->addWidget(pbtn_SelectCube);

        pbtn_ClearCube = new QPushButton(tab);
        pbtn_ClearCube->setObjectName(QString::fromUtf8("pbtn_ClearCube"));

        horizontalLayout_7->addWidget(pbtn_ClearCube);


        verticalLayout->addLayout(horizontalLayout_7);

        verticalSpacer_2 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer_2);

        tabWidget->addTab(tab, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName(QString::fromUtf8("tab_2"));
        tabWidget->addTab(tab_2, QString());

        horizontalLayout->addWidget(tabWidget);

        InforDock = new QDockWidget(disp3dkeypointedit);
        InforDock->setObjectName(QString::fromUtf8("InforDock"));
        InforDock->setMaximumSize(QSize(180, 524287));
        InforDock->setFeatures(QDockWidget::DockWidgetMovable);
        dockWidgetContents = new QWidget();
        dockWidgetContents->setObjectName(QString::fromUtf8("dockWidgetContents"));
        gridLayout = new QGridLayout(dockWidgetContents);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        lab_NumKeyPoint = new QLabel(dockWidgetContents);
        lab_NumKeyPoint->setObjectName(QString::fromUtf8("lab_NumKeyPoint"));

        gridLayout->addWidget(lab_NumKeyPoint, 1, 1, 1, 1);

        lab_NumPoint = new QLabel(dockWidgetContents);
        lab_NumPoint->setObjectName(QString::fromUtf8("lab_NumPoint"));

        gridLayout->addWidget(lab_NumPoint, 0, 1, 1, 1);

        label = new QLabel(dockWidgetContents);
        label->setObjectName(QString::fromUtf8("label"));

        gridLayout->addWidget(label, 0, 0, 1, 1);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout->addItem(verticalSpacer, 6, 0, 1, 1);

        line = new QFrame(dockWidgetContents);
        line->setObjectName(QString::fromUtf8("line"));
        line->setFrameShape(QFrame::HLine);
        line->setFrameShadow(QFrame::Sunken);

        gridLayout->addWidget(line, 2, 0, 1, 2);

        label_3 = new QLabel(dockWidgetContents);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        gridLayout->addWidget(label_3, 1, 0, 1, 1);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        label_5 = new QLabel(dockWidgetContents);
        label_5->setObjectName(QString::fromUtf8("label_5"));

        horizontalLayout_2->addWidget(label_5);

        edit_IDKeyPoint = new QLineEdit(dockWidgetContents);
        edit_IDKeyPoint->setObjectName(QString::fromUtf8("edit_IDKeyPoint"));

        horizontalLayout_2->addWidget(edit_IDKeyPoint);

        pbtn_FindKeyPoint = new QPushButton(dockWidgetContents);
        pbtn_FindKeyPoint->setObjectName(QString::fromUtf8("pbtn_FindKeyPoint"));
        pbtn_FindKeyPoint->setMaximumSize(QSize(40, 16777215));

        horizontalLayout_2->addWidget(pbtn_FindKeyPoint);


        gridLayout->addLayout(horizontalLayout_2, 4, 0, 1, 2);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        label_2 = new QLabel(dockWidgetContents);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        horizontalLayout_3->addWidget(label_2);

        cbox_IDKeyPoint = new QComboBox(dockWidgetContents);
        cbox_IDKeyPoint->setObjectName(QString::fromUtf8("cbox_IDKeyPoint"));

        horizontalLayout_3->addWidget(cbox_IDKeyPoint);


        gridLayout->addLayout(horizontalLayout_3, 3, 0, 1, 2);

        gridLayout_2 = new QGridLayout();
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        label_6 = new QLabel(dockWidgetContents);
        label_6->setObjectName(QString::fromUtf8("label_6"));
        label_6->setMinimumSize(QSize(0, 20));

        gridLayout_2->addWidget(label_6, 0, 0, 1, 2);

        lab_ZPosition = new QLabel(dockWidgetContents);
        lab_ZPosition->setObjectName(QString::fromUtf8("lab_ZPosition"));

        gridLayout_2->addWidget(lab_ZPosition, 3, 1, 1, 1);

        label_7 = new QLabel(dockWidgetContents);
        label_7->setObjectName(QString::fromUtf8("label_7"));
        label_7->setMinimumSize(QSize(0, 18));

        gridLayout_2->addWidget(label_7, 1, 0, 1, 1);

        lab_YPosition = new QLabel(dockWidgetContents);
        lab_YPosition->setObjectName(QString::fromUtf8("lab_YPosition"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(lab_YPosition->sizePolicy().hasHeightForWidth());
        lab_YPosition->setSizePolicy(sizePolicy);

        gridLayout_2->addWidget(lab_YPosition, 2, 1, 1, 1);

        lab_XPosition = new QLabel(dockWidgetContents);
        lab_XPosition->setObjectName(QString::fromUtf8("lab_XPosition"));

        gridLayout_2->addWidget(lab_XPosition, 1, 1, 1, 1);

        label_8 = new QLabel(dockWidgetContents);
        label_8->setObjectName(QString::fromUtf8("label_8"));
        label_8->setMinimumSize(QSize(0, 18));

        gridLayout_2->addWidget(label_8, 2, 0, 1, 1);

        label_9 = new QLabel(dockWidgetContents);
        label_9->setObjectName(QString::fromUtf8("label_9"));
        label_9->setMinimumSize(QSize(0, 18));

        gridLayout_2->addWidget(label_9, 3, 0, 1, 1);


        gridLayout->addLayout(gridLayout_2, 5, 0, 1, 2);

        InforDock->setWidget(dockWidgetContents);

        horizontalLayout->addWidget(InforDock);


        retranslateUi(disp3dkeypointedit);

        tabWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(disp3dkeypointedit);
    } // setupUi

    void retranslateUi(QWidget *disp3dkeypointedit)
    {
        disp3dkeypointedit->setWindowTitle(QApplication::translate("disp3dkeypointedit", "Form", nullptr));
        pbtn_SelectAll->setText(QApplication::translate("disp3dkeypointedit", "\351\200\211\346\213\251\345\205\250\351\203\250\347\202\271", nullptr));
        pbtn_ClearAll->setText(QApplication::translate("disp3dkeypointedit", "\345\210\240\351\231\244\345\205\250\351\203\250\347\202\271", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tabAll), QApplication::translate("disp3dkeypointedit", "\345\205\250\351\203\250\351\200\211\346\213\251", nullptr));
        label_12->setText(QApplication::translate("disp3dkeypointedit", "\350\256\276\345\256\232\347\253\213\346\226\271\344\275\223", nullptr));
        label_4->setText(QApplication::translate("disp3dkeypointedit", "\345\257\271\350\247\222\347\202\271-1\357\274\232", nullptr));
        edit_Xmin->setText(QApplication::translate("disp3dkeypointedit", "-100", nullptr));
        edit_Ymin->setText(QApplication::translate("disp3dkeypointedit", "-200", nullptr));
        edit_Zmin->setText(QApplication::translate("disp3dkeypointedit", "-100", nullptr));
        label_11->setText(QApplication::translate("disp3dkeypointedit", "\345\257\271\350\247\222\347\202\271-2\357\274\232", nullptr));
        edit_Xmax->setText(QApplication::translate("disp3dkeypointedit", "1000", nullptr));
        edit_Ymax->setText(QApplication::translate("disp3dkeypointedit", "500", nullptr));
        edit_Zmax->setText(QApplication::translate("disp3dkeypointedit", "200", nullptr));
        pbtn_DispCube->setText(QApplication::translate("disp3dkeypointedit", "\346\270\262\346\237\223\350\214\203\345\233\264", nullptr));
        pushButton->setText(QApplication::translate("disp3dkeypointedit", "\345\210\240\351\231\244\346\270\262\346\237\223", nullptr));
        pbtn_SelectCube->setText(QApplication::translate("disp3dkeypointedit", "\351\200\211\346\213\251\345\205\263\351\224\256\347\202\271", nullptr));
        pbtn_ClearCube->setText(QApplication::translate("disp3dkeypointedit", "\345\210\240\351\231\244\345\205\263\351\224\256\347\202\271", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab), QApplication::translate("disp3dkeypointedit", "\347\253\213\346\226\271\344\275\223\351\200\211\346\213\251", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab_2), QApplication::translate("disp3dkeypointedit", "\346\235\241\344\273\266\351\200\211\346\213\251", nullptr));
        InforDock->setWindowTitle(QApplication::translate("disp3dkeypointedit", "\345\205\263\351\224\256\347\202\271\344\277\241\346\201\257", nullptr));
        lab_NumKeyPoint->setText(QString());
        lab_NumPoint->setText(QString());
        label->setText(QApplication::translate("disp3dkeypointedit", "\346\250\241\345\236\213\350\212\202\347\202\271\346\225\260\357\274\232", nullptr));
        label_3->setText(QApplication::translate("disp3dkeypointedit", "\345\205\263\351\224\256\350\212\202\347\202\271\346\225\260\357\274\233", nullptr));
        label_5->setText(QApplication::translate("disp3dkeypointedit", "\345\205\263\351\224\256\347\202\271\346\237\245\350\257\242\357\274\232", nullptr));
        pbtn_FindKeyPoint->setText(QApplication::translate("disp3dkeypointedit", "\346\237\245\350\257\242", nullptr));
        label_2->setText(QApplication::translate("disp3dkeypointedit", "\345\205\263\351\224\256\347\202\271\345\210\227\350\241\250\357\274\232", nullptr));
        label_6->setText(QApplication::translate("disp3dkeypointedit", "\345\205\263\351\224\256\347\202\271\345\235\220\346\240\207\357\274\232", nullptr));
        lab_ZPosition->setText(QString());
        label_7->setText(QApplication::translate("disp3dkeypointedit", "X\350\275\264\345\235\220\346\240\207\357\274\210mm\357\274\211\357\274\232", nullptr));
        lab_YPosition->setText(QString());
        lab_XPosition->setText(QString());
        label_8->setText(QApplication::translate("disp3dkeypointedit", "Y\350\275\264\345\235\220\346\240\207\357\274\210mm\357\274\211\357\274\232", nullptr));
        label_9->setText(QApplication::translate("disp3dkeypointedit", "Z\350\275\264\345\235\220\346\240\207\357\274\210mm\357\274\211\357\274\232", nullptr));
    } // retranslateUi

};

namespace Ui {
    class disp3dkeypointedit: public Ui_disp3dkeypointedit {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DISP3DKEYPOINTEDIT_H
