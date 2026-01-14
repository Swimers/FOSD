/********************************************************************************
** Form generated from reading UI file 'disp3dpoint.ui'
**
** Created by: Qt User Interface Compiler version 5.12.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DISP3DPOINT_H
#define UI_DISP3DPOINT_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTableWidget>

QT_BEGIN_NAMESPACE

class Ui_disp3dpoint
{
public:
    QGridLayout *gridLayout;
    QTableWidget *tablew_PointPara;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer_3;
    QPushButton *pbtn_AddPoint;
    QSpacerItem *horizontalSpacer;
    QPushButton *pbtn_DelectPoint;
    QSpacerItem *horizontalSpacer_2;
    QPushButton *pbtn_EditPoint;
    QSpacerItem *horizontalSpacer_4;
    QPushButton *pbtn_EditPoint_2;
    QSpacerItem *horizontalSpacer_5;

    void setupUi(QDialog *disp3dpoint)
    {
        if (disp3dpoint->objectName().isEmpty())
            disp3dpoint->setObjectName(QString::fromUtf8("disp3dpoint"));
        disp3dpoint->resize(454, 341);
        gridLayout = new QGridLayout(disp3dpoint);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        tablew_PointPara = new QTableWidget(disp3dpoint);
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
        if (tablew_PointPara->rowCount() < 2)
            tablew_PointPara->setRowCount(2);
        QTableWidgetItem *__qtablewidgetitem4 = new QTableWidgetItem();
        tablew_PointPara->setVerticalHeaderItem(0, __qtablewidgetitem4);
        QTableWidgetItem *__qtablewidgetitem5 = new QTableWidgetItem();
        tablew_PointPara->setVerticalHeaderItem(1, __qtablewidgetitem5);
        tablew_PointPara->setObjectName(QString::fromUtf8("tablew_PointPara"));

        gridLayout->addWidget(tablew_PointPara, 0, 0, 1, 1);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_3);

        pbtn_AddPoint = new QPushButton(disp3dpoint);
        pbtn_AddPoint->setObjectName(QString::fromUtf8("pbtn_AddPoint"));
        pbtn_AddPoint->setMinimumSize(QSize(0, 30));

        horizontalLayout->addWidget(pbtn_AddPoint);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        pbtn_DelectPoint = new QPushButton(disp3dpoint);
        pbtn_DelectPoint->setObjectName(QString::fromUtf8("pbtn_DelectPoint"));
        pbtn_DelectPoint->setMinimumSize(QSize(0, 30));

        horizontalLayout->addWidget(pbtn_DelectPoint);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_2);

        pbtn_EditPoint = new QPushButton(disp3dpoint);
        pbtn_EditPoint->setObjectName(QString::fromUtf8("pbtn_EditPoint"));
        pbtn_EditPoint->setMinimumSize(QSize(0, 30));

        horizontalLayout->addWidget(pbtn_EditPoint);

        horizontalSpacer_4 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_4);

        pbtn_EditPoint_2 = new QPushButton(disp3dpoint);
        pbtn_EditPoint_2->setObjectName(QString::fromUtf8("pbtn_EditPoint_2"));
        pbtn_EditPoint_2->setMinimumSize(QSize(0, 30));

        horizontalLayout->addWidget(pbtn_EditPoint_2);

        horizontalSpacer_5 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_5);


        gridLayout->addLayout(horizontalLayout, 1, 0, 1, 1);


        retranslateUi(disp3dpoint);

        QMetaObject::connectSlotsByName(disp3dpoint);
    } // setupUi

    void retranslateUi(QDialog *disp3dpoint)
    {
        disp3dpoint->setWindowTitle(QApplication::translate("disp3dpoint", "Dialog", nullptr));
        QTableWidgetItem *___qtablewidgetitem = tablew_PointPara->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QApplication::translate("disp3dpoint", "\347\273\223\347\202\271\347\274\226\345\217\267", nullptr));
        QTableWidgetItem *___qtablewidgetitem1 = tablew_PointPara->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QApplication::translate("disp3dpoint", "X\345\235\220\346\240\207", nullptr));
        QTableWidgetItem *___qtablewidgetitem2 = tablew_PointPara->horizontalHeaderItem(2);
        ___qtablewidgetitem2->setText(QApplication::translate("disp3dpoint", "Y\345\235\220\346\240\207", nullptr));
        QTableWidgetItem *___qtablewidgetitem3 = tablew_PointPara->horizontalHeaderItem(3);
        ___qtablewidgetitem3->setText(QApplication::translate("disp3dpoint", "Z\345\235\220\346\240\207", nullptr));
        QTableWidgetItem *___qtablewidgetitem4 = tablew_PointPara->verticalHeaderItem(0);
        ___qtablewidgetitem4->setText(QApplication::translate("disp3dpoint", "1", nullptr));
        QTableWidgetItem *___qtablewidgetitem5 = tablew_PointPara->verticalHeaderItem(1);
        ___qtablewidgetitem5->setText(QApplication::translate("disp3dpoint", "2", nullptr));
        pbtn_AddPoint->setText(QApplication::translate("disp3dpoint", "\346\267\273\345\212\240\347\273\223\347\202\271", nullptr));
        pbtn_DelectPoint->setText(QApplication::translate("disp3dpoint", "\345\210\240\351\231\244\347\273\223\347\202\271", nullptr));
        pbtn_EditPoint->setText(QApplication::translate("disp3dpoint", "\347\274\226\350\276\221\347\273\223\347\202\271", nullptr));
        pbtn_EditPoint_2->setText(QApplication::translate("disp3dpoint", "\351\200\200\345\207\272", nullptr));
    } // retranslateUi

};

namespace Ui {
    class disp3dpoint: public Ui_disp3dpoint {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DISP3DPOINT_H
