/********************************************************************************
** Form generated from reading UI file 'disp2dpara.ui'
**
** Created by: Qt User Interface Compiler version 5.12.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DISP2DPARA_H
#define UI_DISP2DPARA_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_disp2dpara
{
public:
    QGridLayout *gridLayout_3;
    QHBoxLayout *horizontalLayout_18;
    QGroupBox *groupBox;
    QGridLayout *gridLayout;
    QHBoxLayout *horizontalLayout_6;
    QLabel *label_5;
    QLineEdit *edit_Sensor5Order;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label;
    QLineEdit *edit_Sensor1Order;
    QHBoxLayout *horizontalLayout_5;
    QLabel *label_3;
    QLineEdit *edit_Sensor4Order;
    QHBoxLayout *horizontalLayout_3;
    QLabel *label_2;
    QLineEdit *edit_Sensor2Order;
    QHBoxLayout *horizontalLayout_4;
    QLabel *label_4;
    QLineEdit *edit_Sensor3Order;
    QHBoxLayout *horizontalLayout_7;
    QLabel *label_6;
    QLineEdit *edit_Sensor6Order;
    QHBoxLayout *horizontalLayout_19;
    QLabel *label_17;
    QLineEdit *edit_Sensor7Order;
    QHBoxLayout *horizontalLayout_20;
    QLabel *label_18;
    QLineEdit *edit_Sensor8Order;
    QHBoxLayout *horizontalLayout_21;
    QLabel *label_19;
    QLineEdit *edit_Sensor9Order;
    QHBoxLayout *horizontalLayout_22;
    QLabel *label_20;
    QLineEdit *edit_Sensor10Order;
    QHBoxLayout *horizontalLayout_23;
    QLabel *label_21;
    QLineEdit *edit_Sensor11Order;
    QHBoxLayout *horizontalLayout_24;
    QLabel *label_22;
    QLineEdit *edit_Sensor12Order;
    QGroupBox *groupBox_2;
    QGridLayout *gridLayout_2;
    QHBoxLayout *horizontalLayout_25;
    QLabel *label_23;
    QLineEdit *edit_Sensor7Para;
    QHBoxLayout *horizontalLayout_28;
    QLabel *label_26;
    QLineEdit *edit_Sensor10Para;
    QHBoxLayout *horizontalLayout_30;
    QLabel *label_28;
    QLineEdit *edit_Sensor12Para;
    QHBoxLayout *horizontalLayout_26;
    QLabel *label_24;
    QLineEdit *edit_Sensor8Para;
    QHBoxLayout *horizontalLayout_29;
    QLabel *label_27;
    QLineEdit *edit_Sensor11Para;
    QHBoxLayout *horizontalLayout_11;
    QLabel *label_10;
    QLineEdit *edit_Sensor4Para;
    QHBoxLayout *horizontalLayout_10;
    QLabel *label_9;
    QLineEdit *edit_Sensor3Para;
    QHBoxLayout *horizontalLayout_8;
    QLabel *label_7;
    QLineEdit *edit_Sensor1Para;
    QHBoxLayout *horizontalLayout_12;
    QLabel *label_11;
    QLineEdit *edit_Sensor5Para;
    QHBoxLayout *horizontalLayout_13;
    QLabel *label_12;
    QLineEdit *edit_Sensor6Para;
    QHBoxLayout *horizontalLayout_27;
    QLabel *label_25;
    QLineEdit *edit_Sensor9Para;
    QHBoxLayout *horizontalLayout_9;
    QLabel *label_8;
    QLineEdit *edit_Sensor2Para;
    QGroupBox *groupBox_4;
    QVBoxLayout *verticalLayout_3;
    QVBoxLayout *verticalLayout;
    QLabel *label_13;
    QLineEdit *edit_Xaxis;
    QVBoxLayout *verticalLayout_4;
    QLabel *label_14;
    QLineEdit *edit_Yaxis;
    QVBoxLayout *verticalLayout_2;
    QLabel *label_15;
    QLineEdit *edit_Zaxis;
    QSpacerItem *verticalSpacer;
    QGroupBox *groupBox_3;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer;
    QPushButton *pbtn_SenerSet;
    QSpacerItem *horizontalSpacer_2;
    QPushButton *pbtn_SenerCancel;

    void setupUi(QWidget *disp2dpara)
    {
        if (disp2dpara->objectName().isEmpty())
            disp2dpara->setObjectName(QString::fromUtf8("disp2dpara"));
        disp2dpara->resize(1029, 351);
        gridLayout_3 = new QGridLayout(disp2dpara);
        gridLayout_3->setObjectName(QString::fromUtf8("gridLayout_3"));
        horizontalLayout_18 = new QHBoxLayout();
        horizontalLayout_18->setObjectName(QString::fromUtf8("horizontalLayout_18"));
        groupBox = new QGroupBox(disp2dpara);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        gridLayout = new QGridLayout(groupBox);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        horizontalLayout_6 = new QHBoxLayout();
        horizontalLayout_6->setSpacing(6);
        horizontalLayout_6->setObjectName(QString::fromUtf8("horizontalLayout_6"));
        label_5 = new QLabel(groupBox);
        label_5->setObjectName(QString::fromUtf8("label_5"));

        horizontalLayout_6->addWidget(label_5);

        edit_Sensor5Order = new QLineEdit(groupBox);
        edit_Sensor5Order->setObjectName(QString::fromUtf8("edit_Sensor5Order"));
        edit_Sensor5Order->setMinimumSize(QSize(0, 30));

        horizontalLayout_6->addWidget(edit_Sensor5Order);

        horizontalLayout_6->setStretch(1, 6);

        gridLayout->addLayout(horizontalLayout_6, 3, 0, 1, 1);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        label = new QLabel(groupBox);
        label->setObjectName(QString::fromUtf8("label"));

        horizontalLayout_2->addWidget(label);

        edit_Sensor1Order = new QLineEdit(groupBox);
        edit_Sensor1Order->setObjectName(QString::fromUtf8("edit_Sensor1Order"));
        edit_Sensor1Order->setMinimumSize(QSize(0, 30));

        horizontalLayout_2->addWidget(edit_Sensor1Order);

        horizontalLayout_2->setStretch(0, 4);
        horizontalLayout_2->setStretch(1, 6);

        gridLayout->addLayout(horizontalLayout_2, 0, 0, 1, 1);

        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setObjectName(QString::fromUtf8("horizontalLayout_5"));
        label_3 = new QLabel(groupBox);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        horizontalLayout_5->addWidget(label_3);

        edit_Sensor4Order = new QLineEdit(groupBox);
        edit_Sensor4Order->setObjectName(QString::fromUtf8("edit_Sensor4Order"));
        edit_Sensor4Order->setMinimumSize(QSize(0, 30));

        horizontalLayout_5->addWidget(edit_Sensor4Order);

        horizontalLayout_5->setStretch(0, 4);
        horizontalLayout_5->setStretch(1, 6);

        gridLayout->addLayout(horizontalLayout_5, 4, 0, 1, 1);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        label_2 = new QLabel(groupBox);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        horizontalLayout_3->addWidget(label_2);

        edit_Sensor2Order = new QLineEdit(groupBox);
        edit_Sensor2Order->setObjectName(QString::fromUtf8("edit_Sensor2Order"));
        edit_Sensor2Order->setMinimumSize(QSize(0, 30));

        horizontalLayout_3->addWidget(edit_Sensor2Order);


        gridLayout->addLayout(horizontalLayout_3, 1, 0, 1, 1);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        label_4 = new QLabel(groupBox);
        label_4->setObjectName(QString::fromUtf8("label_4"));

        horizontalLayout_4->addWidget(label_4);

        edit_Sensor3Order = new QLineEdit(groupBox);
        edit_Sensor3Order->setObjectName(QString::fromUtf8("edit_Sensor3Order"));
        edit_Sensor3Order->setMinimumSize(QSize(0, 30));

        horizontalLayout_4->addWidget(edit_Sensor3Order);


        gridLayout->addLayout(horizontalLayout_4, 2, 0, 1, 1);

        horizontalLayout_7 = new QHBoxLayout();
        horizontalLayout_7->setObjectName(QString::fromUtf8("horizontalLayout_7"));
        label_6 = new QLabel(groupBox);
        label_6->setObjectName(QString::fromUtf8("label_6"));

        horizontalLayout_7->addWidget(label_6);

        edit_Sensor6Order = new QLineEdit(groupBox);
        edit_Sensor6Order->setObjectName(QString::fromUtf8("edit_Sensor6Order"));
        edit_Sensor6Order->setMinimumSize(QSize(0, 30));

        horizontalLayout_7->addWidget(edit_Sensor6Order);

        horizontalLayout_7->setStretch(1, 6);

        gridLayout->addLayout(horizontalLayout_7, 5, 0, 1, 1);

        horizontalLayout_19 = new QHBoxLayout();
        horizontalLayout_19->setObjectName(QString::fromUtf8("horizontalLayout_19"));
        label_17 = new QLabel(groupBox);
        label_17->setObjectName(QString::fromUtf8("label_17"));

        horizontalLayout_19->addWidget(label_17);

        edit_Sensor7Order = new QLineEdit(groupBox);
        edit_Sensor7Order->setObjectName(QString::fromUtf8("edit_Sensor7Order"));
        edit_Sensor7Order->setMinimumSize(QSize(0, 30));

        horizontalLayout_19->addWidget(edit_Sensor7Order);

        horizontalLayout_19->setStretch(0, 4);
        horizontalLayout_19->setStretch(1, 6);

        gridLayout->addLayout(horizontalLayout_19, 0, 1, 1, 1);

        horizontalLayout_20 = new QHBoxLayout();
        horizontalLayout_20->setObjectName(QString::fromUtf8("horizontalLayout_20"));
        label_18 = new QLabel(groupBox);
        label_18->setObjectName(QString::fromUtf8("label_18"));

        horizontalLayout_20->addWidget(label_18);

        edit_Sensor8Order = new QLineEdit(groupBox);
        edit_Sensor8Order->setObjectName(QString::fromUtf8("edit_Sensor8Order"));
        edit_Sensor8Order->setMinimumSize(QSize(0, 30));

        horizontalLayout_20->addWidget(edit_Sensor8Order);

        horizontalLayout_20->setStretch(0, 4);
        horizontalLayout_20->setStretch(1, 6);

        gridLayout->addLayout(horizontalLayout_20, 1, 1, 1, 1);

        horizontalLayout_21 = new QHBoxLayout();
        horizontalLayout_21->setObjectName(QString::fromUtf8("horizontalLayout_21"));
        label_19 = new QLabel(groupBox);
        label_19->setObjectName(QString::fromUtf8("label_19"));

        horizontalLayout_21->addWidget(label_19);

        edit_Sensor9Order = new QLineEdit(groupBox);
        edit_Sensor9Order->setObjectName(QString::fromUtf8("edit_Sensor9Order"));
        edit_Sensor9Order->setMinimumSize(QSize(0, 30));

        horizontalLayout_21->addWidget(edit_Sensor9Order);

        horizontalLayout_21->setStretch(0, 4);
        horizontalLayout_21->setStretch(1, 6);

        gridLayout->addLayout(horizontalLayout_21, 2, 1, 1, 1);

        horizontalLayout_22 = new QHBoxLayout();
        horizontalLayout_22->setObjectName(QString::fromUtf8("horizontalLayout_22"));
        label_20 = new QLabel(groupBox);
        label_20->setObjectName(QString::fromUtf8("label_20"));

        horizontalLayout_22->addWidget(label_20);

        edit_Sensor10Order = new QLineEdit(groupBox);
        edit_Sensor10Order->setObjectName(QString::fromUtf8("edit_Sensor10Order"));
        edit_Sensor10Order->setMinimumSize(QSize(0, 30));

        horizontalLayout_22->addWidget(edit_Sensor10Order);

        horizontalLayout_22->setStretch(0, 4);
        horizontalLayout_22->setStretch(1, 6);

        gridLayout->addLayout(horizontalLayout_22, 3, 1, 1, 1);

        horizontalLayout_23 = new QHBoxLayout();
        horizontalLayout_23->setObjectName(QString::fromUtf8("horizontalLayout_23"));
        label_21 = new QLabel(groupBox);
        label_21->setObjectName(QString::fromUtf8("label_21"));

        horizontalLayout_23->addWidget(label_21);

        edit_Sensor11Order = new QLineEdit(groupBox);
        edit_Sensor11Order->setObjectName(QString::fromUtf8("edit_Sensor11Order"));
        edit_Sensor11Order->setMinimumSize(QSize(0, 30));

        horizontalLayout_23->addWidget(edit_Sensor11Order);

        horizontalLayout_23->setStretch(0, 4);
        horizontalLayout_23->setStretch(1, 6);

        gridLayout->addLayout(horizontalLayout_23, 4, 1, 1, 1);

        horizontalLayout_24 = new QHBoxLayout();
        horizontalLayout_24->setObjectName(QString::fromUtf8("horizontalLayout_24"));
        label_22 = new QLabel(groupBox);
        label_22->setObjectName(QString::fromUtf8("label_22"));

        horizontalLayout_24->addWidget(label_22);

        edit_Sensor12Order = new QLineEdit(groupBox);
        edit_Sensor12Order->setObjectName(QString::fromUtf8("edit_Sensor12Order"));
        edit_Sensor12Order->setMinimumSize(QSize(0, 30));

        horizontalLayout_24->addWidget(edit_Sensor12Order);

        horizontalLayout_24->setStretch(0, 4);
        horizontalLayout_24->setStretch(1, 6);

        gridLayout->addLayout(horizontalLayout_24, 5, 1, 1, 1);


        horizontalLayout_18->addWidget(groupBox);

        groupBox_2 = new QGroupBox(disp2dpara);
        groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
        gridLayout_2 = new QGridLayout(groupBox_2);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        horizontalLayout_25 = new QHBoxLayout();
        horizontalLayout_25->setObjectName(QString::fromUtf8("horizontalLayout_25"));
        label_23 = new QLabel(groupBox_2);
        label_23->setObjectName(QString::fromUtf8("label_23"));

        horizontalLayout_25->addWidget(label_23);

        edit_Sensor7Para = new QLineEdit(groupBox_2);
        edit_Sensor7Para->setObjectName(QString::fromUtf8("edit_Sensor7Para"));
        edit_Sensor7Para->setMinimumSize(QSize(0, 30));

        horizontalLayout_25->addWidget(edit_Sensor7Para);

        horizontalLayout_25->setStretch(0, 4);
        horizontalLayout_25->setStretch(1, 6);

        gridLayout_2->addLayout(horizontalLayout_25, 0, 2, 1, 1);

        horizontalLayout_28 = new QHBoxLayout();
        horizontalLayout_28->setObjectName(QString::fromUtf8("horizontalLayout_28"));
        label_26 = new QLabel(groupBox_2);
        label_26->setObjectName(QString::fromUtf8("label_26"));

        horizontalLayout_28->addWidget(label_26);

        edit_Sensor10Para = new QLineEdit(groupBox_2);
        edit_Sensor10Para->setObjectName(QString::fromUtf8("edit_Sensor10Para"));
        edit_Sensor10Para->setMinimumSize(QSize(0, 30));

        horizontalLayout_28->addWidget(edit_Sensor10Para);

        horizontalLayout_28->setStretch(0, 4);
        horizontalLayout_28->setStretch(1, 6);

        gridLayout_2->addLayout(horizontalLayout_28, 3, 2, 1, 1);

        horizontalLayout_30 = new QHBoxLayout();
        horizontalLayout_30->setObjectName(QString::fromUtf8("horizontalLayout_30"));
        label_28 = new QLabel(groupBox_2);
        label_28->setObjectName(QString::fromUtf8("label_28"));

        horizontalLayout_30->addWidget(label_28);

        edit_Sensor12Para = new QLineEdit(groupBox_2);
        edit_Sensor12Para->setObjectName(QString::fromUtf8("edit_Sensor12Para"));
        edit_Sensor12Para->setMinimumSize(QSize(0, 30));

        horizontalLayout_30->addWidget(edit_Sensor12Para);

        horizontalLayout_30->setStretch(0, 4);
        horizontalLayout_30->setStretch(1, 6);

        gridLayout_2->addLayout(horizontalLayout_30, 5, 2, 1, 1);

        horizontalLayout_26 = new QHBoxLayout();
        horizontalLayout_26->setObjectName(QString::fromUtf8("horizontalLayout_26"));
        label_24 = new QLabel(groupBox_2);
        label_24->setObjectName(QString::fromUtf8("label_24"));

        horizontalLayout_26->addWidget(label_24);

        edit_Sensor8Para = new QLineEdit(groupBox_2);
        edit_Sensor8Para->setObjectName(QString::fromUtf8("edit_Sensor8Para"));
        edit_Sensor8Para->setMinimumSize(QSize(0, 30));

        horizontalLayout_26->addWidget(edit_Sensor8Para);

        horizontalLayout_26->setStretch(0, 4);
        horizontalLayout_26->setStretch(1, 6);

        gridLayout_2->addLayout(horizontalLayout_26, 1, 2, 1, 1);

        horizontalLayout_29 = new QHBoxLayout();
        horizontalLayout_29->setObjectName(QString::fromUtf8("horizontalLayout_29"));
        label_27 = new QLabel(groupBox_2);
        label_27->setObjectName(QString::fromUtf8("label_27"));

        horizontalLayout_29->addWidget(label_27);

        edit_Sensor11Para = new QLineEdit(groupBox_2);
        edit_Sensor11Para->setObjectName(QString::fromUtf8("edit_Sensor11Para"));
        edit_Sensor11Para->setMinimumSize(QSize(0, 30));

        horizontalLayout_29->addWidget(edit_Sensor11Para);

        horizontalLayout_29->setStretch(0, 4);
        horizontalLayout_29->setStretch(1, 6);

        gridLayout_2->addLayout(horizontalLayout_29, 4, 2, 1, 1);

        horizontalLayout_11 = new QHBoxLayout();
        horizontalLayout_11->setObjectName(QString::fromUtf8("horizontalLayout_11"));
        label_10 = new QLabel(groupBox_2);
        label_10->setObjectName(QString::fromUtf8("label_10"));

        horizontalLayout_11->addWidget(label_10);

        edit_Sensor4Para = new QLineEdit(groupBox_2);
        edit_Sensor4Para->setObjectName(QString::fromUtf8("edit_Sensor4Para"));
        edit_Sensor4Para->setMinimumSize(QSize(0, 30));

        horizontalLayout_11->addWidget(edit_Sensor4Para);

        horizontalLayout_11->setStretch(0, 4);
        horizontalLayout_11->setStretch(1, 6);

        gridLayout_2->addLayout(horizontalLayout_11, 4, 1, 1, 1);

        horizontalLayout_10 = new QHBoxLayout();
        horizontalLayout_10->setObjectName(QString::fromUtf8("horizontalLayout_10"));
        label_9 = new QLabel(groupBox_2);
        label_9->setObjectName(QString::fromUtf8("label_9"));

        horizontalLayout_10->addWidget(label_9);

        edit_Sensor3Para = new QLineEdit(groupBox_2);
        edit_Sensor3Para->setObjectName(QString::fromUtf8("edit_Sensor3Para"));
        edit_Sensor3Para->setMinimumSize(QSize(0, 30));

        horizontalLayout_10->addWidget(edit_Sensor3Para);


        gridLayout_2->addLayout(horizontalLayout_10, 2, 1, 1, 1);

        horizontalLayout_8 = new QHBoxLayout();
        horizontalLayout_8->setObjectName(QString::fromUtf8("horizontalLayout_8"));
        label_7 = new QLabel(groupBox_2);
        label_7->setObjectName(QString::fromUtf8("label_7"));

        horizontalLayout_8->addWidget(label_7);

        edit_Sensor1Para = new QLineEdit(groupBox_2);
        edit_Sensor1Para->setObjectName(QString::fromUtf8("edit_Sensor1Para"));
        edit_Sensor1Para->setMinimumSize(QSize(0, 30));

        horizontalLayout_8->addWidget(edit_Sensor1Para);

        horizontalLayout_8->setStretch(0, 4);
        horizontalLayout_8->setStretch(1, 6);

        gridLayout_2->addLayout(horizontalLayout_8, 0, 1, 1, 1);

        horizontalLayout_12 = new QHBoxLayout();
        horizontalLayout_12->setSpacing(6);
        horizontalLayout_12->setObjectName(QString::fromUtf8("horizontalLayout_12"));
        label_11 = new QLabel(groupBox_2);
        label_11->setObjectName(QString::fromUtf8("label_11"));

        horizontalLayout_12->addWidget(label_11);

        edit_Sensor5Para = new QLineEdit(groupBox_2);
        edit_Sensor5Para->setObjectName(QString::fromUtf8("edit_Sensor5Para"));
        edit_Sensor5Para->setMinimumSize(QSize(0, 30));

        horizontalLayout_12->addWidget(edit_Sensor5Para);

        horizontalLayout_12->setStretch(1, 6);

        gridLayout_2->addLayout(horizontalLayout_12, 3, 1, 1, 1);

        horizontalLayout_13 = new QHBoxLayout();
        horizontalLayout_13->setObjectName(QString::fromUtf8("horizontalLayout_13"));
        label_12 = new QLabel(groupBox_2);
        label_12->setObjectName(QString::fromUtf8("label_12"));

        horizontalLayout_13->addWidget(label_12);

        edit_Sensor6Para = new QLineEdit(groupBox_2);
        edit_Sensor6Para->setObjectName(QString::fromUtf8("edit_Sensor6Para"));
        edit_Sensor6Para->setMinimumSize(QSize(0, 30));

        horizontalLayout_13->addWidget(edit_Sensor6Para);

        horizontalLayout_13->setStretch(1, 6);

        gridLayout_2->addLayout(horizontalLayout_13, 5, 1, 1, 1);

        horizontalLayout_27 = new QHBoxLayout();
        horizontalLayout_27->setObjectName(QString::fromUtf8("horizontalLayout_27"));
        label_25 = new QLabel(groupBox_2);
        label_25->setObjectName(QString::fromUtf8("label_25"));

        horizontalLayout_27->addWidget(label_25);

        edit_Sensor9Para = new QLineEdit(groupBox_2);
        edit_Sensor9Para->setObjectName(QString::fromUtf8("edit_Sensor9Para"));
        edit_Sensor9Para->setMinimumSize(QSize(0, 30));

        horizontalLayout_27->addWidget(edit_Sensor9Para);

        horizontalLayout_27->setStretch(0, 4);
        horizontalLayout_27->setStretch(1, 6);

        gridLayout_2->addLayout(horizontalLayout_27, 2, 2, 1, 1);

        horizontalLayout_9 = new QHBoxLayout();
        horizontalLayout_9->setObjectName(QString::fromUtf8("horizontalLayout_9"));
        label_8 = new QLabel(groupBox_2);
        label_8->setObjectName(QString::fromUtf8("label_8"));

        horizontalLayout_9->addWidget(label_8);

        edit_Sensor2Para = new QLineEdit(groupBox_2);
        edit_Sensor2Para->setObjectName(QString::fromUtf8("edit_Sensor2Para"));
        edit_Sensor2Para->setMinimumSize(QSize(0, 30));

        horizontalLayout_9->addWidget(edit_Sensor2Para);


        gridLayout_2->addLayout(horizontalLayout_9, 1, 1, 1, 1);


        horizontalLayout_18->addWidget(groupBox_2);

        groupBox_4 = new QGroupBox(disp2dpara);
        groupBox_4->setObjectName(QString::fromUtf8("groupBox_4"));
        verticalLayout_3 = new QVBoxLayout(groupBox_4);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        label_13 = new QLabel(groupBox_4);
        label_13->setObjectName(QString::fromUtf8("label_13"));

        verticalLayout->addWidget(label_13);

        edit_Xaxis = new QLineEdit(groupBox_4);
        edit_Xaxis->setObjectName(QString::fromUtf8("edit_Xaxis"));

        verticalLayout->addWidget(edit_Xaxis);


        verticalLayout_3->addLayout(verticalLayout);

        verticalLayout_4 = new QVBoxLayout();
        verticalLayout_4->setObjectName(QString::fromUtf8("verticalLayout_4"));
        label_14 = new QLabel(groupBox_4);
        label_14->setObjectName(QString::fromUtf8("label_14"));

        verticalLayout_4->addWidget(label_14);

        edit_Yaxis = new QLineEdit(groupBox_4);
        edit_Yaxis->setObjectName(QString::fromUtf8("edit_Yaxis"));

        verticalLayout_4->addWidget(edit_Yaxis);


        verticalLayout_3->addLayout(verticalLayout_4);

        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        label_15 = new QLabel(groupBox_4);
        label_15->setObjectName(QString::fromUtf8("label_15"));

        verticalLayout_2->addWidget(label_15);

        edit_Zaxis = new QLineEdit(groupBox_4);
        edit_Zaxis->setObjectName(QString::fromUtf8("edit_Zaxis"));

        verticalLayout_2->addWidget(edit_Zaxis);


        verticalLayout_3->addLayout(verticalLayout_2);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_3->addItem(verticalSpacer);


        horizontalLayout_18->addWidget(groupBox_4);


        gridLayout_3->addLayout(horizontalLayout_18, 0, 0, 1, 1);

        groupBox_3 = new QGroupBox(disp2dpara);
        groupBox_3->setObjectName(QString::fromUtf8("groupBox_3"));
        horizontalLayout = new QHBoxLayout(groupBox_3);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalSpacer = new QSpacerItem(325, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        pbtn_SenerSet = new QPushButton(groupBox_3);
        pbtn_SenerSet->setObjectName(QString::fromUtf8("pbtn_SenerSet"));
        pbtn_SenerSet->setMinimumSize(QSize(0, 30));

        horizontalLayout->addWidget(pbtn_SenerSet);

        horizontalSpacer_2 = new QSpacerItem(62, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_2);

        pbtn_SenerCancel = new QPushButton(groupBox_3);
        pbtn_SenerCancel->setObjectName(QString::fromUtf8("pbtn_SenerCancel"));
        pbtn_SenerCancel->setMinimumSize(QSize(0, 30));

        horizontalLayout->addWidget(pbtn_SenerCancel);

        horizontalLayout->setStretch(0, 20);
        horizontalLayout->setStretch(1, 4);
        horizontalLayout->setStretch(2, 4);
        horizontalLayout->setStretch(3, 4);

        gridLayout_3->addWidget(groupBox_3, 1, 0, 1, 1);


        retranslateUi(disp2dpara);

        QMetaObject::connectSlotsByName(disp2dpara);
    } // setupUi

    void retranslateUi(QWidget *disp2dpara)
    {
        disp2dpara->setWindowTitle(QApplication::translate("disp2dpara", "Form", nullptr));
        groupBox->setTitle(QApplication::translate("disp2dpara", "\344\274\240\346\204\237\345\231\250\351\241\272\345\272\217", nullptr));
        label_5->setText(QApplication::translate("disp2dpara", "\344\274\240\346\204\237\345\231\2505\357\274\232", nullptr));
        label->setText(QApplication::translate("disp2dpara", "\344\274\240\346\204\237\345\231\2501\357\274\232", nullptr));
        label_3->setText(QApplication::translate("disp2dpara", "\344\274\240\346\204\237\345\231\2504\357\274\232", nullptr));
        label_2->setText(QApplication::translate("disp2dpara", "\344\274\240\346\204\237\345\231\2502\357\274\232", nullptr));
        label_4->setText(QApplication::translate("disp2dpara", "\344\274\240\346\204\237\345\231\2503\357\274\232", nullptr));
        label_6->setText(QApplication::translate("disp2dpara", "\344\274\240\346\204\237\345\231\2506\357\274\232", nullptr));
        label_17->setText(QApplication::translate("disp2dpara", "\344\274\240\346\204\237\345\231\2507\357\274\232", nullptr));
        label_18->setText(QApplication::translate("disp2dpara", "\344\274\240\346\204\237\345\231\2508\357\274\232", nullptr));
        label_19->setText(QApplication::translate("disp2dpara", "\344\274\240\346\204\237\345\231\2509\357\274\232", nullptr));
        label_20->setText(QApplication::translate("disp2dpara", "\344\274\240\346\204\237\345\231\25010\357\274\232", nullptr));
        label_21->setText(QApplication::translate("disp2dpara", "\344\274\240\346\204\237\345\231\25011\357\274\232", nullptr));
        label_22->setText(QApplication::translate("disp2dpara", "\344\274\240\346\204\237\345\231\25012\357\274\232", nullptr));
        groupBox_2->setTitle(QApplication::translate("disp2dpara", "\344\274\240\346\204\237\345\231\250\347\263\273\346\225\260", nullptr));
        label_23->setText(QApplication::translate("disp2dpara", "\344\274\240\346\204\237\345\231\2507\357\274\232", nullptr));
        label_26->setText(QApplication::translate("disp2dpara", "\344\274\240\346\204\237\345\231\25010\357\274\232", nullptr));
        label_28->setText(QApplication::translate("disp2dpara", "\344\274\240\346\204\237\345\231\25012\357\274\232", nullptr));
        label_24->setText(QApplication::translate("disp2dpara", "\344\274\240\346\204\237\345\231\2508\357\274\232", nullptr));
        label_27->setText(QApplication::translate("disp2dpara", "\344\274\240\346\204\237\345\231\25011\357\274\232", nullptr));
        label_10->setText(QApplication::translate("disp2dpara", "\344\274\240\346\204\237\345\231\2504\357\274\232", nullptr));
        label_9->setText(QApplication::translate("disp2dpara", "\344\274\240\346\204\237\345\231\2503\357\274\232", nullptr));
        label_7->setText(QApplication::translate("disp2dpara", "\344\274\240\346\204\237\345\231\2501\357\274\232", nullptr));
        label_11->setText(QApplication::translate("disp2dpara", "\344\274\240\346\204\237\345\231\2505\357\274\232", nullptr));
        label_12->setText(QApplication::translate("disp2dpara", "\344\274\240\346\204\237\345\231\2506\357\274\232", nullptr));
        label_25->setText(QApplication::translate("disp2dpara", "\344\274\240\346\204\237\345\231\2509\357\274\232", nullptr));
        label_8->setText(QApplication::translate("disp2dpara", "\344\274\240\346\204\237\345\231\2502\357\274\232", nullptr));
        groupBox_4->setTitle(QApplication::translate("disp2dpara", "\345\216\237\345\247\213\345\235\220\346\240\207\346\240\241\345\207\206\345\200\274", nullptr));
        label_13->setText(QApplication::translate("disp2dpara", "X\345\235\220\346\240\207\346\240\241\345\207\206\345\200\274", nullptr));
        label_14->setText(QApplication::translate("disp2dpara", "Y\345\235\220\346\240\207\346\240\241\345\207\206\345\200\274", nullptr));
        label_15->setText(QApplication::translate("disp2dpara", "Z\345\235\220\346\240\207\346\240\241\345\207\206\345\200\274", nullptr));
        groupBox_3->setTitle(QString());
        pbtn_SenerSet->setText(QApplication::translate("disp2dpara", "\347\241\256\345\256\232", nullptr));
        pbtn_SenerCancel->setText(QApplication::translate("disp2dpara", "\345\217\226\346\266\210", nullptr));
    } // retranslateUi

};

namespace Ui {
    class disp2dpara: public Ui_disp2dpara {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DISP2DPARA_H
