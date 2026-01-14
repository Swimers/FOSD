/********************************************************************************
** Form generated from reading UI file 'pointtable.ui'
**
** Created by: Qt User Interface Compiler version 5.12.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_POINTTABLE_H
#define UI_POINTTABLE_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTableView>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_PointTable
{
public:
    QVBoxLayout *verticalLayout;
    QGridLayout *gridLayout;
    QComboBox *comboBox;
    QPushButton *btnok;
    QPushButton *btnstop;
    QTableView *tableView;

    void setupUi(QWidget *PointTable)
    {
        if (PointTable->objectName().isEmpty())
            PointTable->setObjectName(QString::fromUtf8("PointTable"));
        PointTable->resize(510, 344);
        verticalLayout = new QVBoxLayout(PointTable);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        gridLayout = new QGridLayout();
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        comboBox = new QComboBox(PointTable);
        comboBox->addItem(QString());
        comboBox->addItem(QString());
        comboBox->addItem(QString());
        comboBox->setObjectName(QString::fromUtf8("comboBox"));
        comboBox->setMinimumSize(QSize(0, 30));

        gridLayout->addWidget(comboBox, 0, 0, 1, 1);

        btnok = new QPushButton(PointTable);
        btnok->setObjectName(QString::fromUtf8("btnok"));
        btnok->setMinimumSize(QSize(0, 30));

        gridLayout->addWidget(btnok, 0, 1, 1, 1);

        btnstop = new QPushButton(PointTable);
        btnstop->setObjectName(QString::fromUtf8("btnstop"));
        btnstop->setMinimumSize(QSize(0, 30));

        gridLayout->addWidget(btnstop, 0, 2, 1, 1);


        verticalLayout->addLayout(gridLayout);

        tableView = new QTableView(PointTable);
        tableView->setObjectName(QString::fromUtf8("tableView"));

        verticalLayout->addWidget(tableView);


        retranslateUi(PointTable);

        QMetaObject::connectSlotsByName(PointTable);
    } // setupUi

    void retranslateUi(QWidget *PointTable)
    {
        PointTable->setWindowTitle(QApplication::translate("PointTable", "Form", nullptr));
        comboBox->setItemText(0, QApplication::translate("PointTable", "mode1\357\274\232\346\214\211\347\274\226\345\217\267", nullptr));
        comboBox->setItemText(1, QApplication::translate("PointTable", "mode2\357\274\232\346\214\211\345\235\220\346\240\207", nullptr));
        comboBox->setItemText(2, QApplication::translate("PointTable", "mode3\357\274\232\346\214\211\345\217\230\345\275\242", nullptr));

        btnok->setText(QApplication::translate("PointTable", "\345\274\200\345\247\213", nullptr));
        btnstop->setText(QApplication::translate("PointTable", "\346\232\202\345\201\234", nullptr));
    } // retranslateUi

};

namespace Ui {
    class PointTable: public Ui_PointTable {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_POINTTABLE_H
