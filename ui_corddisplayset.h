/********************************************************************************
** Form generated from reading UI file 'corddisplayset.ui'
**
** Created by: Qt User Interface Compiler version 5.12.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CORDDISPLAYSET_H
#define UI_CORDDISPLAYSET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_CordDisplaySet
{
public:
    QTableWidget *tableWidget;
    QWidget *layoutWidget;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer_3;
    QPushButton *AddCord;
    QSpacerItem *horizontalSpacer;
    QPushButton *DeleteCord;
    QSpacerItem *horizontalSpacer_2;
    QPushButton *EditCord;
    QSpacerItem *horizontalSpacer_4;

    void setupUi(QDialog *CordDisplaySet)
    {
        if (CordDisplaySet->objectName().isEmpty())
            CordDisplaySet->setObjectName(QString::fromUtf8("CordDisplaySet"));
        CordDisplaySet->resize(488, 284);
        tableWidget = new QTableWidget(CordDisplaySet);
        if (tableWidget->columnCount() < 4)
            tableWidget->setColumnCount(4);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(1, __qtablewidgetitem1);
        QTableWidgetItem *__qtablewidgetitem2 = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(2, __qtablewidgetitem2);
        QTableWidgetItem *__qtablewidgetitem3 = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(3, __qtablewidgetitem3);
        if (tableWidget->rowCount() < 2)
            tableWidget->setRowCount(2);
        QTableWidgetItem *__qtablewidgetitem4 = new QTableWidgetItem();
        tableWidget->setVerticalHeaderItem(0, __qtablewidgetitem4);
        QTableWidgetItem *__qtablewidgetitem5 = new QTableWidgetItem();
        tableWidget->setVerticalHeaderItem(1, __qtablewidgetitem5);
        tableWidget->setObjectName(QString::fromUtf8("tableWidget"));
        tableWidget->setGeometry(QRect(9, 9, 470, 179));
        layoutWidget = new QWidget(CordDisplaySet);
        layoutWidget->setObjectName(QString::fromUtf8("layoutWidget"));
        layoutWidget->setGeometry(QRect(9, 243, 423, 32));
        horizontalLayout = new QHBoxLayout(layoutWidget);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_3);

        AddCord = new QPushButton(layoutWidget);
        AddCord->setObjectName(QString::fromUtf8("AddCord"));
        AddCord->setMinimumSize(QSize(0, 30));

        horizontalLayout->addWidget(AddCord);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        DeleteCord = new QPushButton(layoutWidget);
        DeleteCord->setObjectName(QString::fromUtf8("DeleteCord"));
        DeleteCord->setMinimumSize(QSize(0, 30));

        horizontalLayout->addWidget(DeleteCord);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_2);

        EditCord = new QPushButton(layoutWidget);
        EditCord->setObjectName(QString::fromUtf8("EditCord"));
        EditCord->setMinimumSize(QSize(0, 30));

        horizontalLayout->addWidget(EditCord);

        horizontalSpacer_4 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_4);


        retranslateUi(CordDisplaySet);

        QMetaObject::connectSlotsByName(CordDisplaySet);
    } // setupUi

    void retranslateUi(QDialog *CordDisplaySet)
    {
        CordDisplaySet->setWindowTitle(QApplication::translate("CordDisplaySet", "Dialog", nullptr));
        QTableWidgetItem *___qtablewidgetitem = tableWidget->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QApplication::translate("CordDisplaySet", "\347\273\223\347\202\271\347\274\226\345\217\267", nullptr));
        QTableWidgetItem *___qtablewidgetitem1 = tableWidget->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QApplication::translate("CordDisplaySet", "X\345\235\220\346\240\207", nullptr));
        QTableWidgetItem *___qtablewidgetitem2 = tableWidget->horizontalHeaderItem(2);
        ___qtablewidgetitem2->setText(QApplication::translate("CordDisplaySet", "Y\345\235\220\346\240\207", nullptr));
        QTableWidgetItem *___qtablewidgetitem3 = tableWidget->horizontalHeaderItem(3);
        ___qtablewidgetitem3->setText(QApplication::translate("CordDisplaySet", "Z\345\235\220\346\240\207", nullptr));
        QTableWidgetItem *___qtablewidgetitem4 = tableWidget->verticalHeaderItem(0);
        ___qtablewidgetitem4->setText(QApplication::translate("CordDisplaySet", "1", nullptr));
        QTableWidgetItem *___qtablewidgetitem5 = tableWidget->verticalHeaderItem(1);
        ___qtablewidgetitem5->setText(QApplication::translate("CordDisplaySet", "2", nullptr));
        AddCord->setText(QApplication::translate("CordDisplaySet", "\346\267\273\345\212\240\347\273\223\347\202\271", nullptr));
        DeleteCord->setText(QApplication::translate("CordDisplaySet", "\345\210\240\351\231\244\347\273\223\347\202\271", nullptr));
        EditCord->setText(QApplication::translate("CordDisplaySet", "\347\274\226\350\276\221\347\273\223\347\202\271", nullptr));
    } // retranslateUi

};

namespace Ui {
    class CordDisplaySet: public Ui_CordDisplaySet {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CORDDISPLAYSET_H
