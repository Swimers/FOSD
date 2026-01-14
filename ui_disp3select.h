/********************************************************************************
** Form generated from reading UI file 'disp3select.ui'
**
** Created by: Qt User Interface Compiler version 5.12.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DISP3SELECT_H
#define UI_DISP3SELECT_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_disp3select
{
public:
    QGridLayout *gridLayout;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label;
    QComboBox *cbox_SelectPoint;
    QSpacerItem *horizontalSpacer_4;
    QLineEdit *edit_PoontNum;
    QPushButton *btn_FindPoint;
    QHBoxLayout *horizontalLayout;
    QLabel *label_2;
    QLineEdit *edit_NodeX;
    QLineEdit *edit_NodeY;
    QLineEdit *edit_NodeZ;
    QHBoxLayout *horizontalLayout_3;
    QSpacerItem *horizontalSpacer_2;
    QPushButton *pbtn_SelectSet;
    QSpacerItem *horizontalSpacer;
    QPushButton *pbtn_SelectCancel;
    QSpacerItem *horizontalSpacer_3;

    void setupUi(QDialog *disp3select)
    {
        if (disp3select->objectName().isEmpty())
            disp3select->setObjectName(QString::fromUtf8("disp3select"));
        disp3select->resize(400, 300);
        gridLayout = new QGridLayout(disp3select);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        label = new QLabel(disp3select);
        label->setObjectName(QString::fromUtf8("label"));
        QFont font;
        font.setBold(true);
        font.setWeight(75);
        label->setFont(font);

        horizontalLayout_2->addWidget(label);

        cbox_SelectPoint = new QComboBox(disp3select);
        cbox_SelectPoint->setObjectName(QString::fromUtf8("cbox_SelectPoint"));
        cbox_SelectPoint->setMinimumSize(QSize(0, 30));

        horizontalLayout_2->addWidget(cbox_SelectPoint);

        horizontalSpacer_4 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_4);

        edit_PoontNum = new QLineEdit(disp3select);
        edit_PoontNum->setObjectName(QString::fromUtf8("edit_PoontNum"));
        edit_PoontNum->setMinimumSize(QSize(0, 30));

        horizontalLayout_2->addWidget(edit_PoontNum);

        btn_FindPoint = new QPushButton(disp3select);
        btn_FindPoint->setObjectName(QString::fromUtf8("btn_FindPoint"));
        btn_FindPoint->setMinimumSize(QSize(0, 30));

        horizontalLayout_2->addWidget(btn_FindPoint);

        horizontalLayout_2->setStretch(0, 10);
        horizontalLayout_2->setStretch(1, 10);
        horizontalLayout_2->setStretch(3, 10);
        horizontalLayout_2->setStretch(4, 10);

        verticalLayout->addLayout(horizontalLayout_2);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        label_2 = new QLabel(disp3select);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setFont(font);

        horizontalLayout->addWidget(label_2);

        edit_NodeX = new QLineEdit(disp3select);
        edit_NodeX->setObjectName(QString::fromUtf8("edit_NodeX"));
        edit_NodeX->setMinimumSize(QSize(0, 30));

        horizontalLayout->addWidget(edit_NodeX);

        edit_NodeY = new QLineEdit(disp3select);
        edit_NodeY->setObjectName(QString::fromUtf8("edit_NodeY"));
        edit_NodeY->setMinimumSize(QSize(0, 30));

        horizontalLayout->addWidget(edit_NodeY);

        edit_NodeZ = new QLineEdit(disp3select);
        edit_NodeZ->setObjectName(QString::fromUtf8("edit_NodeZ"));
        edit_NodeZ->setMinimumSize(QSize(0, 30));

        horizontalLayout->addWidget(edit_NodeZ);


        verticalLayout->addLayout(horizontalLayout);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_2);

        pbtn_SelectSet = new QPushButton(disp3select);
        pbtn_SelectSet->setObjectName(QString::fromUtf8("pbtn_SelectSet"));
        pbtn_SelectSet->setMinimumSize(QSize(0, 30));

        horizontalLayout_3->addWidget(pbtn_SelectSet);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer);

        pbtn_SelectCancel = new QPushButton(disp3select);
        pbtn_SelectCancel->setObjectName(QString::fromUtf8("pbtn_SelectCancel"));
        pbtn_SelectCancel->setMinimumSize(QSize(0, 30));

        horizontalLayout_3->addWidget(pbtn_SelectCancel);

        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_3);

        horizontalLayout_3->setStretch(0, 20);
        horizontalLayout_3->setStretch(1, 8);
        horizontalLayout_3->setStretch(2, 8);
        horizontalLayout_3->setStretch(3, 8);
        horizontalLayout_3->setStretch(4, 8);

        verticalLayout->addLayout(horizontalLayout_3);


        gridLayout->addLayout(verticalLayout, 0, 0, 1, 1);


        retranslateUi(disp3select);

        QMetaObject::connectSlotsByName(disp3select);
    } // setupUi

    void retranslateUi(QDialog *disp3select)
    {
        disp3select->setWindowTitle(QApplication::translate("disp3select", "Dialog", nullptr));
        label->setText(QApplication::translate("disp3select", "\347\273\223\347\202\271\347\274\226\345\217\267:", nullptr));
        btn_FindPoint->setText(QApplication::translate("disp3select", "\346\237\245\350\257\242", nullptr));
        label_2->setText(QApplication::translate("disp3select", "\347\273\223\347\202\271\345\235\220\346\240\207:", nullptr));
        pbtn_SelectSet->setText(QApplication::translate("disp3select", "\347\241\256\345\256\232", nullptr));
        pbtn_SelectCancel->setText(QApplication::translate("disp3select", "\345\217\226\346\266\210", nullptr));
    } // retranslateUi

};

namespace Ui {
    class disp3select: public Ui_disp3select {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DISP3SELECT_H
