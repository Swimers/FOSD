/********************************************************************************
** Form generated from reading UI file 'disp3dkeypointselect.ui'
**
** Created by: Qt User Interface Compiler version 5.12.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DISP3DKEYPOINTSELECT_H
#define UI_DISP3DKEYPOINTSELECT_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDockWidget>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_disp3dkeypointselect
{
public:
    QHBoxLayout *horizontalLayout;
    QTableWidget *tablew_PointPara;
    QDockWidget *dockWidget;
    QWidget *dockWidgetContents;
    QGridLayout *gridLayout_4;
    QPushButton *pbtn_EditPoint;
    QLabel *label_14;
    QFrame *line;
    QRadioButton *radioButton;
    QPushButton *pbtn_Exit;
    QLabel *lab_XPos;
    QPushButton *pbtn_DeletePoint;
    QComboBox *cbox_KeyPointID;
    QLabel *label_13;
    QLabel *label_15;
    QLabel *lab_NumPoints;
    QLabel *lab_ZPos;
    QLabel *label_16;
    QFrame *line_2;
    QRadioButton *radioButton_2;
    QLabel *lab_NumKeyPoints;
    QLabel *label_11;
    QLabel *lab_YPos;
    QSpacerItem *verticalSpacer_2;
    QLabel *label_4;
    QPushButton *pbtn_AddPoint;
    QRadioButton *radioButton_3;

    void setupUi(QWidget *disp3dkeypointselect)
    {
        if (disp3dkeypointselect->objectName().isEmpty())
            disp3dkeypointselect->setObjectName(QString::fromUtf8("disp3dkeypointselect"));
        disp3dkeypointselect->resize(597, 458);
        horizontalLayout = new QHBoxLayout(disp3dkeypointselect);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        tablew_PointPara = new QTableWidget(disp3dkeypointselect);
        if (tablew_PointPara->columnCount() < 4)
            tablew_PointPara->setColumnCount(4);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        tablew_PointPara->setHorizontalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        tablew_PointPara->setHorizontalHeaderItem(1, __qtablewidgetitem1);
        QTableWidgetItem *__qtablewidgetitem2 = new QTableWidgetItem();
        tablew_PointPara->setHorizontalHeaderItem(2, __qtablewidgetitem2);
        QTableWidgetItem *__qtablewidgetitem3 = new QTableWidgetItem();
        tablew_PointPara->setHorizontalHeaderItem(3, __qtablewidgetitem3);
        if (tablew_PointPara->rowCount() < 1)
            tablew_PointPara->setRowCount(1);
        QTableWidgetItem *__qtablewidgetitem4 = new QTableWidgetItem();
        tablew_PointPara->setVerticalHeaderItem(0, __qtablewidgetitem4);
        tablew_PointPara->setObjectName(QString::fromUtf8("tablew_PointPara"));
        tablew_PointPara->setRowCount(1);

        horizontalLayout->addWidget(tablew_PointPara);

        dockWidget = new QDockWidget(disp3dkeypointselect);
        dockWidget->setObjectName(QString::fromUtf8("dockWidget"));
        dockWidget->setMaximumSize(QSize(200, 524287));
        dockWidget->setFeatures(QDockWidget::DockWidgetMovable);
        dockWidgetContents = new QWidget();
        dockWidgetContents->setObjectName(QString::fromUtf8("dockWidgetContents"));
        gridLayout_4 = new QGridLayout(dockWidgetContents);
        gridLayout_4->setObjectName(QString::fromUtf8("gridLayout_4"));
        pbtn_EditPoint = new QPushButton(dockWidgetContents);
        pbtn_EditPoint->setObjectName(QString::fromUtf8("pbtn_EditPoint"));

        gridLayout_4->addWidget(pbtn_EditPoint, 8, 0, 1, 2);

        label_14 = new QLabel(dockWidgetContents);
        label_14->setObjectName(QString::fromUtf8("label_14"));

        gridLayout_4->addWidget(label_14, 4, 0, 1, 1);

        line = new QFrame(dockWidgetContents);
        line->setObjectName(QString::fromUtf8("line"));
        line->setFrameShape(QFrame::HLine);
        line->setFrameShadow(QFrame::Sunken);

        gridLayout_4->addWidget(line, 9, 0, 1, 2);

        radioButton = new QRadioButton(dockWidgetContents);
        radioButton->setObjectName(QString::fromUtf8("radioButton"));

        gridLayout_4->addWidget(radioButton, 11, 0, 1, 2);

        pbtn_Exit = new QPushButton(dockWidgetContents);
        pbtn_Exit->setObjectName(QString::fromUtf8("pbtn_Exit"));

        gridLayout_4->addWidget(pbtn_Exit, 14, 1, 1, 1);

        lab_XPos = new QLabel(dockWidgetContents);
        lab_XPos->setObjectName(QString::fromUtf8("lab_XPos"));

        gridLayout_4->addWidget(lab_XPos, 4, 1, 1, 1);

        pbtn_DeletePoint = new QPushButton(dockWidgetContents);
        pbtn_DeletePoint->setObjectName(QString::fromUtf8("pbtn_DeletePoint"));

        gridLayout_4->addWidget(pbtn_DeletePoint, 7, 1, 1, 1);

        cbox_KeyPointID = new QComboBox(dockWidgetContents);
        cbox_KeyPointID->setObjectName(QString::fromUtf8("cbox_KeyPointID"));

        gridLayout_4->addWidget(cbox_KeyPointID, 3, 1, 1, 1);

        label_13 = new QLabel(dockWidgetContents);
        label_13->setObjectName(QString::fromUtf8("label_13"));

        gridLayout_4->addWidget(label_13, 3, 0, 1, 1);

        label_15 = new QLabel(dockWidgetContents);
        label_15->setObjectName(QString::fromUtf8("label_15"));

        gridLayout_4->addWidget(label_15, 5, 0, 1, 1);

        lab_NumPoints = new QLabel(dockWidgetContents);
        lab_NumPoints->setObjectName(QString::fromUtf8("lab_NumPoints"));

        gridLayout_4->addWidget(lab_NumPoints, 0, 1, 1, 1);

        lab_ZPos = new QLabel(dockWidgetContents);
        lab_ZPos->setObjectName(QString::fromUtf8("lab_ZPos"));

        gridLayout_4->addWidget(lab_ZPos, 6, 1, 1, 1);

        label_16 = new QLabel(dockWidgetContents);
        label_16->setObjectName(QString::fromUtf8("label_16"));

        gridLayout_4->addWidget(label_16, 6, 0, 1, 1);

        line_2 = new QFrame(dockWidgetContents);
        line_2->setObjectName(QString::fromUtf8("line_2"));
        line_2->setFrameShape(QFrame::HLine);
        line_2->setFrameShadow(QFrame::Sunken);

        gridLayout_4->addWidget(line_2, 2, 0, 1, 2);

        radioButton_2 = new QRadioButton(dockWidgetContents);
        radioButton_2->setObjectName(QString::fromUtf8("radioButton_2"));

        gridLayout_4->addWidget(radioButton_2, 12, 0, 1, 2);

        lab_NumKeyPoints = new QLabel(dockWidgetContents);
        lab_NumKeyPoints->setObjectName(QString::fromUtf8("lab_NumKeyPoints"));

        gridLayout_4->addWidget(lab_NumKeyPoints, 1, 1, 1, 1);

        label_11 = new QLabel(dockWidgetContents);
        label_11->setObjectName(QString::fromUtf8("label_11"));

        gridLayout_4->addWidget(label_11, 1, 0, 1, 1);

        lab_YPos = new QLabel(dockWidgetContents);
        lab_YPos->setObjectName(QString::fromUtf8("lab_YPos"));

        gridLayout_4->addWidget(lab_YPos, 5, 1, 1, 1);

        verticalSpacer_2 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_4->addItem(verticalSpacer_2, 13, 0, 1, 1);

        label_4 = new QLabel(dockWidgetContents);
        label_4->setObjectName(QString::fromUtf8("label_4"));

        gridLayout_4->addWidget(label_4, 0, 0, 1, 1);

        pbtn_AddPoint = new QPushButton(dockWidgetContents);
        pbtn_AddPoint->setObjectName(QString::fromUtf8("pbtn_AddPoint"));

        gridLayout_4->addWidget(pbtn_AddPoint, 7, 0, 1, 1);

        radioButton_3 = new QRadioButton(dockWidgetContents);
        radioButton_3->setObjectName(QString::fromUtf8("radioButton_3"));

        gridLayout_4->addWidget(radioButton_3, 10, 0, 1, 2);

        dockWidget->setWidget(dockWidgetContents);

        horizontalLayout->addWidget(dockWidget);


        retranslateUi(disp3dkeypointselect);

        QMetaObject::connectSlotsByName(disp3dkeypointselect);
    } // setupUi

    void retranslateUi(QWidget *disp3dkeypointselect)
    {
        disp3dkeypointselect->setWindowTitle(QApplication::translate("disp3dkeypointselect", "Form", nullptr));
        QTableWidgetItem *___qtablewidgetitem = tablew_PointPara->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QApplication::translate("disp3dkeypointselect", "\347\273\223\347\202\271\347\274\226\345\217\267", nullptr));
        QTableWidgetItem *___qtablewidgetitem1 = tablew_PointPara->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QApplication::translate("disp3dkeypointselect", "X\345\235\220\346\240\207", nullptr));
        QTableWidgetItem *___qtablewidgetitem2 = tablew_PointPara->horizontalHeaderItem(2);
        ___qtablewidgetitem2->setText(QApplication::translate("disp3dkeypointselect", "Y\345\235\220\346\240\207", nullptr));
        QTableWidgetItem *___qtablewidgetitem3 = tablew_PointPara->horizontalHeaderItem(3);
        ___qtablewidgetitem3->setText(QApplication::translate("disp3dkeypointselect", "Z\345\235\220\346\240\207", nullptr));
        QTableWidgetItem *___qtablewidgetitem4 = tablew_PointPara->verticalHeaderItem(0);
        ___qtablewidgetitem4->setText(QApplication::translate("disp3dkeypointselect", "1", nullptr));
        dockWidget->setWindowTitle(QApplication::translate("disp3dkeypointselect", "\345\205\263\351\224\256\347\202\271\351\200\211\346\213\251", nullptr));
        pbtn_EditPoint->setText(QApplication::translate("disp3dkeypointselect", "\347\274\226\350\276\221\350\212\202\347\202\271", nullptr));
        label_14->setText(QApplication::translate("disp3dkeypointselect", "\345\216\237X\345\235\220\346\240\207\357\274\232", nullptr));
        radioButton->setText(QApplication::translate("disp3dkeypointselect", "\346\230\276\347\244\272\345\275\242\345\217\230\351\207\217", nullptr));
        pbtn_Exit->setText(QApplication::translate("disp3dkeypointselect", "\351\200\200\345\207\272", nullptr));
        lab_XPos->setText(QString());
        pbtn_DeletePoint->setText(QApplication::translate("disp3dkeypointselect", "\345\210\240\351\231\244\350\212\202\347\202\271", nullptr));
        label_13->setText(QApplication::translate("disp3dkeypointselect", "\345\205\263\351\224\256\347\202\271\347\274\226\345\217\267\357\274\232", nullptr));
        label_15->setText(QApplication::translate("disp3dkeypointselect", "\345\216\237Y\345\235\220\346\240\207\357\274\232", nullptr));
        lab_NumPoints->setText(QString());
        lab_ZPos->setText(QString());
        label_16->setText(QApplication::translate("disp3dkeypointselect", "\345\216\237Z\345\235\220\346\240\207\357\274\232", nullptr));
        radioButton_2->setText(QApplication::translate("disp3dkeypointselect", "\346\230\276\347\244\272\345\216\237\345\247\213\344\275\215\347\275\256", nullptr));
        lab_NumKeyPoints->setText(QString());
        label_11->setText(QApplication::translate("disp3dkeypointselect", "\345\205\263\351\224\256\350\212\202\347\202\271\346\225\260\357\274\232", nullptr));
        lab_YPos->setText(QString());
        label_4->setText(QApplication::translate("disp3dkeypointselect", "\346\250\241\345\236\213\350\212\202\347\202\271\346\225\260\357\274\232", nullptr));
        pbtn_AddPoint->setText(QApplication::translate("disp3dkeypointselect", "\346\267\273\345\212\240\350\212\202\347\202\271", nullptr));
        radioButton_3->setText(QApplication::translate("disp3dkeypointselect", "\346\270\262\346\237\223\345\205\263\351\224\256\347\202\271", nullptr));
    } // retranslateUi

};

namespace Ui {
    class disp3dkeypointselect: public Ui_disp3dkeypointselect {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DISP3DKEYPOINTSELECT_H
