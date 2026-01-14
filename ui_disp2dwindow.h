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

class Ui_Disp2DWindow
{
public:
    QGridLayout *gridLayout_2;
    QGroupBox *TwoD_groupBox_Picture;
    QVBoxLayout *verticalLayout_2;
    QLabel *TwoD_Label_picture;
    QHBoxLayout *horizontalLayout;
    QTableView *TwoD_tableView;
    QVBoxLayout *verticalLayout;
    QPushButton *Button_ChangeOriginalCoordinates;
    QPushButton *Button_ParaRevise;
    QPushButton *Button_ChangeBackGround;
    QPushButton *Button_Observe;

    void setupUi(QWidget *Disp2DWindow)
    {
        if (Disp2DWindow->objectName().isEmpty())
            Disp2DWindow->setObjectName(QString::fromUtf8("Disp2DWindow"));
        Disp2DWindow->resize(1003, 744);
        gridLayout_2 = new QGridLayout(Disp2DWindow);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        TwoD_groupBox_Picture = new QGroupBox(Disp2DWindow);
        TwoD_groupBox_Picture->setObjectName(QString::fromUtf8("TwoD_groupBox_Picture"));
        verticalLayout_2 = new QVBoxLayout(TwoD_groupBox_Picture);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        TwoD_Label_picture = new QLabel(TwoD_groupBox_Picture);
        TwoD_Label_picture->setObjectName(QString::fromUtf8("TwoD_Label_picture"));
        QSizePolicy sizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(TwoD_Label_picture->sizePolicy().hasHeightForWidth());
        TwoD_Label_picture->setSizePolicy(sizePolicy);
        TwoD_Label_picture->setTextFormat(Qt::AutoText);
        TwoD_Label_picture->setPixmap(QPixmap(QString::fromUtf8(":/resource/resource_disp2d/image2.jpg")));
        TwoD_Label_picture->setScaledContents(true);
        TwoD_Label_picture->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);

        verticalLayout_2->addWidget(TwoD_Label_picture);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        TwoD_tableView = new QTableView(TwoD_groupBox_Picture);
        TwoD_tableView->setObjectName(QString::fromUtf8("TwoD_tableView"));

        horizontalLayout->addWidget(TwoD_tableView);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(15, -1, 15, -1);
        Button_ChangeOriginalCoordinates = new QPushButton(TwoD_groupBox_Picture);
        Button_ChangeOriginalCoordinates->setObjectName(QString::fromUtf8("Button_ChangeOriginalCoordinates"));
        Button_ChangeOriginalCoordinates->setMinimumSize(QSize(0, 30));

        verticalLayout->addWidget(Button_ChangeOriginalCoordinates);

        Button_ParaRevise = new QPushButton(TwoD_groupBox_Picture);
        Button_ParaRevise->setObjectName(QString::fromUtf8("Button_ParaRevise"));
        Button_ParaRevise->setMinimumSize(QSize(0, 30));

        verticalLayout->addWidget(Button_ParaRevise);

        Button_ChangeBackGround = new QPushButton(TwoD_groupBox_Picture);
        Button_ChangeBackGround->setObjectName(QString::fromUtf8("Button_ChangeBackGround"));
        Button_ChangeBackGround->setMinimumSize(QSize(0, 30));

        verticalLayout->addWidget(Button_ChangeBackGround);

        Button_Observe = new QPushButton(TwoD_groupBox_Picture);
        Button_Observe->setObjectName(QString::fromUtf8("Button_Observe"));
        Button_Observe->setMinimumSize(QSize(0, 30));

        verticalLayout->addWidget(Button_Observe);


        horizontalLayout->addLayout(verticalLayout);

        horizontalLayout->setStretch(0, 50);
        horizontalLayout->setStretch(1, 10);

        verticalLayout_2->addLayout(horizontalLayout);

        verticalLayout_2->setStretch(0, 10);
        verticalLayout_2->setStretch(1, 10);

        gridLayout_2->addWidget(TwoD_groupBox_Picture, 0, 0, 1, 1);


        retranslateUi(Disp2DWindow);

        QMetaObject::connectSlotsByName(Disp2DWindow);
    } // setupUi

    void retranslateUi(QWidget *Disp2DWindow)
    {
        Disp2DWindow->setWindowTitle(QApplication::translate("Disp2DWindow", "Form", nullptr));
        TwoD_groupBox_Picture->setTitle(QString());
        TwoD_Label_picture->setText(QString());
        Button_ChangeOriginalCoordinates->setText(QApplication::translate("Disp2DWindow", "\345\257\274\345\205\245\345\210\235\345\247\213\345\235\220\346\240\207", nullptr));
        Button_ParaRevise->setText(QApplication::translate("Disp2DWindow", "\347\263\273\346\225\260\344\277\256\346\255\243", nullptr));
        Button_ChangeBackGround->setText(QApplication::translate("Disp2DWindow", "\346\233\264\346\215\242\350\203\214\346\231\257", nullptr));
        Button_Observe->setText(QApplication::translate("Disp2DWindow", "\345\220\257\345\212\250\347\233\221\346\216\247", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Disp2DWindow: public Ui_Disp2DWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DISP2DWINDOW_H
