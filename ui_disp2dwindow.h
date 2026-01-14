/********************************************************************************
** Form generated from reading UI file 'disp2dwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.12.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DISP2DWINDOW_H
#define UI_DISP2DWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTableView>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_disp2dwindow
{
public:
    QGridLayout *gridLayout;
    QGroupBox *TwoD_groupBox_Picture;
    QVBoxLayout *verticalLayout_3;
    QLabel *lable_Picture;
    QHBoxLayout *horizontalLayout_2;
    QTableView *table_Disp2DPara;
    QVBoxLayout *verticalLayout_4;
    QPushButton *pbtn_ChangeOriginalCoordinates;
    QPushButton *pbtn_ParaRevise;
    QPushButton *pbtn_ChangeBackGround;
    QPushButton *pbtn_Observe;

    void setupUi(QWidget *disp2dwindow)
    {
        if (disp2dwindow->objectName().isEmpty())
            disp2dwindow->setObjectName(QString::fromUtf8("disp2dwindow"));
        disp2dwindow->resize(1052, 689);
        gridLayout = new QGridLayout(disp2dwindow);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        TwoD_groupBox_Picture = new QGroupBox(disp2dwindow);
        TwoD_groupBox_Picture->setObjectName(QString::fromUtf8("TwoD_groupBox_Picture"));
        verticalLayout_3 = new QVBoxLayout(TwoD_groupBox_Picture);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        lable_Picture = new QLabel(TwoD_groupBox_Picture);
        lable_Picture->setObjectName(QString::fromUtf8("lable_Picture"));
        QSizePolicy sizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(lable_Picture->sizePolicy().hasHeightForWidth());
        lable_Picture->setSizePolicy(sizePolicy);
        lable_Picture->setTextFormat(Qt::AutoText);
        lable_Picture->setPixmap(QPixmap(QString::fromUtf8(":/resource_disp2d/Resource/resource_disp2d/image6.jpg")));
        lable_Picture->setScaledContents(true);
        lable_Picture->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);

        verticalLayout_3->addWidget(lable_Picture);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        table_Disp2DPara = new QTableView(TwoD_groupBox_Picture);
        table_Disp2DPara->setObjectName(QString::fromUtf8("table_Disp2DPara"));
        table_Disp2DPara->setStyleSheet(QString::fromUtf8(""));

        horizontalLayout_2->addWidget(table_Disp2DPara);

        verticalLayout_4 = new QVBoxLayout();
        verticalLayout_4->setObjectName(QString::fromUtf8("verticalLayout_4"));
        verticalLayout_4->setContentsMargins(15, -1, 15, -1);
        pbtn_ChangeOriginalCoordinates = new QPushButton(TwoD_groupBox_Picture);
        pbtn_ChangeOriginalCoordinates->setObjectName(QString::fromUtf8("pbtn_ChangeOriginalCoordinates"));
        QSizePolicy sizePolicy1(QSizePolicy::Minimum, QSizePolicy::Preferred);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(pbtn_ChangeOriginalCoordinates->sizePolicy().hasHeightForWidth());
        pbtn_ChangeOriginalCoordinates->setSizePolicy(sizePolicy1);
        pbtn_ChangeOriginalCoordinates->setMinimumSize(QSize(0, 30));
        pbtn_ChangeOriginalCoordinates->setMaximumSize(QSize(16777215, 50));

        verticalLayout_4->addWidget(pbtn_ChangeOriginalCoordinates);

        pbtn_ParaRevise = new QPushButton(TwoD_groupBox_Picture);
        pbtn_ParaRevise->setObjectName(QString::fromUtf8("pbtn_ParaRevise"));
        sizePolicy1.setHeightForWidth(pbtn_ParaRevise->sizePolicy().hasHeightForWidth());
        pbtn_ParaRevise->setSizePolicy(sizePolicy1);
        pbtn_ParaRevise->setMinimumSize(QSize(0, 30));
        pbtn_ParaRevise->setMaximumSize(QSize(16777215, 50));

        verticalLayout_4->addWidget(pbtn_ParaRevise);

        pbtn_ChangeBackGround = new QPushButton(TwoD_groupBox_Picture);
        pbtn_ChangeBackGround->setObjectName(QString::fromUtf8("pbtn_ChangeBackGround"));
        sizePolicy1.setHeightForWidth(pbtn_ChangeBackGround->sizePolicy().hasHeightForWidth());
        pbtn_ChangeBackGround->setSizePolicy(sizePolicy1);
        pbtn_ChangeBackGround->setMinimumSize(QSize(0, 30));
        pbtn_ChangeBackGround->setMaximumSize(QSize(16777215, 50));

        verticalLayout_4->addWidget(pbtn_ChangeBackGround);

        pbtn_Observe = new QPushButton(TwoD_groupBox_Picture);
        pbtn_Observe->setObjectName(QString::fromUtf8("pbtn_Observe"));
        sizePolicy1.setHeightForWidth(pbtn_Observe->sizePolicy().hasHeightForWidth());
        pbtn_Observe->setSizePolicy(sizePolicy1);
        pbtn_Observe->setMinimumSize(QSize(0, 30));
        pbtn_Observe->setMaximumSize(QSize(16777215, 50));

        verticalLayout_4->addWidget(pbtn_Observe);


        horizontalLayout_2->addLayout(verticalLayout_4);

        horizontalLayout_2->setStretch(0, 50);
        horizontalLayout_2->setStretch(1, 10);

        verticalLayout_3->addLayout(horizontalLayout_2);

        verticalLayout_3->setStretch(0, 10);
        verticalLayout_3->setStretch(1, 10);

        gridLayout->addWidget(TwoD_groupBox_Picture, 0, 0, 1, 1);


        retranslateUi(disp2dwindow);

        QMetaObject::connectSlotsByName(disp2dwindow);
    } // setupUi

    void retranslateUi(QWidget *disp2dwindow)
    {
        disp2dwindow->setWindowTitle(QApplication::translate("disp2dwindow", "Form", nullptr));
        TwoD_groupBox_Picture->setTitle(QString());
        lable_Picture->setText(QString());
        pbtn_ChangeOriginalCoordinates->setText(QApplication::translate("disp2dwindow", "\345\257\274\345\205\245\345\210\235\345\247\213\345\235\220\346\240\207", nullptr));
        pbtn_ParaRevise->setText(QApplication::translate("disp2dwindow", "\347\263\273\346\225\260\344\277\256\346\255\243", nullptr));
        pbtn_ChangeBackGround->setText(QApplication::translate("disp2dwindow", "\346\233\264\346\215\242\350\203\214\346\231\257", nullptr));
        pbtn_Observe->setText(QApplication::translate("disp2dwindow", "\345\220\257\345\212\250\347\233\221\346\216\247", nullptr));
    } // retranslateUi

};

namespace Ui {
    class disp2dwindow: public Ui_disp2dwindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DISP2DWINDOW_H
