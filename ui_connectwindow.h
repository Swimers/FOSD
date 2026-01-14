/********************************************************************************
** Form generated from reading UI file 'connectwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.12.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CONNECTWINDOW_H
#define UI_CONNECTWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QTableView>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "qcustomplot.h"

QT_BEGIN_NAMESPACE

class Ui_connectwindow
{
public:
    QGridLayout *gridLayout_3;
    QFrame *frame;
    QHBoxLayout *horizontalLayout_4;
    QVBoxLayout *verticalLayout;
    QGroupBox *groupBox_2;
    QGridLayout *gridLayout;
    QPushButton *pbtn_OpenSerial;
    QComboBox *cbox_CommuteChoose;
    QFrame *frame_4;
    QGridLayout *gridLayout_10;
    QComboBox *cbox_SerialNameChoose;
    QLabel *label_11;
    QHBoxLayout *horizontalLayout_8;
    QLabel *label_12;
    QComboBox *cbox_SerialBaudChoose;
    QPushButton *pbtn_OpenNet;
    QFrame *frame_2;
    QGridLayout *gridLayout_6;
    QComboBox *cbox_LocakIP;
    QLabel *label;
    QHBoxLayout *horizontalLayout_3;
    QLabel *label_3;
    QLineEdit *edit_LocalPort;
    QLabel *label_18;
    QPushButton *pbtn_RefreshSerial;
    QFrame *frame_3;
    QGridLayout *gridLayout_7;
    QLabel *label_15;
    QLineEdit *edit_TargetIP;
    QHBoxLayout *horizontalLayout_5;
    QLabel *label_16;
    QLineEdit *edit_TargetPort;
    QSpacerItem *verticalSpacer;
    QSpacerItem *verticalSpacer_2;
    QGroupBox *groupBox_3;
    QGridLayout *gridLayout_2;
    QLabel *label_led;
    QLabel *label_4;
    QPushButton *pbtn_Diagnosis;
    QVBoxLayout *verticalLayout_5;
    QTabWidget *tabWidget;
    QWidget *tab;
    QGridLayout *gridLayout_9;
    QCustomPlot *cplot_Spectrum;
    QWidget *tab_2;
    QGridLayout *gridLayout_8;
    QTableView *tablev_Wavelength;
    QWidget *tab_3;
    QVBoxLayout *verticalLayout_4;
    QLabel *label_13;
    QPlainTextEdit *ptedit_SerialRec;
    QPlainTextEdit *ptedit_SerialSend;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer;
    QPushButton *pbtn_SerialClear;
    QSpacerItem *horizontalSpacer_2;
    QPushButton *pbtn_SerialSend;
    QSpacerItem *horizontalSpacer_3;
    QWidget *tab_4;
    QVBoxLayout *verticalLayout_6;
    QLabel *label_14;
    QPlainTextEdit *ptedit_UdpRec;
    QPlainTextEdit *ptedit_UdpSend;
    QHBoxLayout *horizontalLayout_2;
    QSpacerItem *horizontalSpacer_4;
    QPushButton *pbtn_UdpClear;
    QSpacerItem *horizontalSpacer_5;
    QPushButton *pbtn_UdpSend;
    QSpacerItem *horizontalSpacer_6;
    QWidget *tab_5;
    QVBoxLayout *verticalLayout_9;
    QLabel *label_17;
    QPlainTextEdit *ptedit_TcpRec;
    QPlainTextEdit *ptedit_TcpSen;
    QHBoxLayout *horizontalLayout_7;
    QSpacerItem *horizontalSpacer_13;
    QPushButton *pbtn_TcpClear;
    QSpacerItem *horizontalSpacer_14;
    QPushButton *pbtn_TcpSend;
    QSpacerItem *horizontalSpacer_15;
    QGroupBox *groupBox_select;
    QGridLayout *gridLayout_5;
    QPushButton *pbtn_SelectCH;
    QLabel *label_2;
    QCheckBox *ckbox_CH13;
    QCheckBox *ckbox_CH4;
    QCheckBox *ckbox_CH15;
    QCheckBox *ckbox_CH11;
    QCheckBox *ckbox_CH12;
    QCheckBox *ckbox_CH10;
    QCheckBox *ckbox_CH6;
    QCheckBox *ckbox_CH7;
    QPushButton *pbtn_SelectAllCH;
    QCheckBox *ckbox_CH8;
    QCheckBox *ckbox_CH9;
    QCheckBox *ckbox_CH14;
    QCheckBox *ckbox_CH1;
    QCheckBox *ckbox_CH2;
    QCheckBox *ckbox_CH3;
    QCheckBox *ckbox_CH5;
    QCheckBox *ckbox_CH16;
    QVBoxLayout *verticalLayout_3;
    QGroupBox *groupBox;
    QGridLayout *gridLayout_4;
    QTextEdit *textEdit_APara;
    QLabel *label_9;
    QTextEdit *textEdit_StrainTopPara;
    QLabel *label_7;
    QTextEdit *textEdit_StrainLowPara;
    QLabel *label_8;
    QComboBox *cbox_SaveCycle;
    QLabel *label_10;
    QLabel *label_5;
    QTextEdit *textEdit_BPara;
    QComboBox *cbox_ModleSelect;
    QComboBox *cbox_CampingFre;
    QLabel *label_6;
    QPushButton *pbtn_WaveLengthReset;
    QPushButton *pbtn_ParaSet;
    QPushButton *pbtn_ParaRest;
    QGroupBox *groupBox_save;
    QVBoxLayout *verticalLayout_2;
    QPushButton *pbtn_SaveDynamic;
    QPushButton *pbtn_SaveStatic;
    QPushButton *pbtn_SaveSpectrum;
    QHBoxLayout *horizontalLayout_6;
    QLabel *label_20;
    QLabel *lab_State;

    void setupUi(QWidget *connectwindow)
    {
        if (connectwindow->objectName().isEmpty())
            connectwindow->setObjectName(QString::fromUtf8("connectwindow"));
        connectwindow->resize(1331, 809);
        gridLayout_3 = new QGridLayout(connectwindow);
        gridLayout_3->setObjectName(QString::fromUtf8("gridLayout_3"));
        frame = new QFrame(connectwindow);
        frame->setObjectName(QString::fromUtf8("frame"));
        frame->setFrameShape(QFrame::StyledPanel);
        frame->setFrameShadow(QFrame::Raised);
        horizontalLayout_4 = new QHBoxLayout(frame);
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        groupBox_2 = new QGroupBox(frame);
        groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
        groupBox_2->setMaximumSize(QSize(250, 16777215));
        QFont font;
        font.setPointSize(14);
        font.setBold(true);
        font.setWeight(75);
        groupBox_2->setFont(font);
        gridLayout = new QGridLayout(groupBox_2);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        pbtn_OpenSerial = new QPushButton(groupBox_2);
        pbtn_OpenSerial->setObjectName(QString::fromUtf8("pbtn_OpenSerial"));
        QSizePolicy sizePolicy(QSizePolicy::Minimum, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(pbtn_OpenSerial->sizePolicy().hasHeightForWidth());
        pbtn_OpenSerial->setSizePolicy(sizePolicy);
        pbtn_OpenSerial->setMinimumSize(QSize(0, 50));
        pbtn_OpenSerial->setMaximumSize(QSize(16777215, 50));
        QFont font1;
        font1.setPointSize(13);
        font1.setBold(false);
        font1.setWeight(50);
        pbtn_OpenSerial->setFont(font1);

        gridLayout->addWidget(pbtn_OpenSerial, 20, 0, 1, 2);

        cbox_CommuteChoose = new QComboBox(groupBox_2);
        cbox_CommuteChoose->addItem(QString());
        cbox_CommuteChoose->addItem(QString());
        cbox_CommuteChoose->setObjectName(QString::fromUtf8("cbox_CommuteChoose"));
        QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(cbox_CommuteChoose->sizePolicy().hasHeightForWidth());
        cbox_CommuteChoose->setSizePolicy(sizePolicy1);
        cbox_CommuteChoose->setMaximumSize(QSize(16777215, 30));
        QFont font2;
        font2.setPointSize(10);
        font2.setBold(false);
        font2.setWeight(50);
        cbox_CommuteChoose->setFont(font2);

        gridLayout->addWidget(cbox_CommuteChoose, 0, 1, 1, 1);

        frame_4 = new QFrame(groupBox_2);
        frame_4->setObjectName(QString::fromUtf8("frame_4"));
        frame_4->setStyleSheet(QString::fromUtf8(""));
        frame_4->setFrameShape(QFrame::StyledPanel);
        frame_4->setFrameShadow(QFrame::Raised);
        gridLayout_10 = new QGridLayout(frame_4);
        gridLayout_10->setObjectName(QString::fromUtf8("gridLayout_10"));
        cbox_SerialNameChoose = new QComboBox(frame_4);
        cbox_SerialNameChoose->setObjectName(QString::fromUtf8("cbox_SerialNameChoose"));
        QSizePolicy sizePolicy2(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(cbox_SerialNameChoose->sizePolicy().hasHeightForWidth());
        cbox_SerialNameChoose->setSizePolicy(sizePolicy2);
        cbox_SerialNameChoose->setFont(font1);

        gridLayout_10->addWidget(cbox_SerialNameChoose, 1, 0, 1, 1);

        label_11 = new QLabel(frame_4);
        label_11->setObjectName(QString::fromUtf8("label_11"));
        label_11->setFont(font1);

        gridLayout_10->addWidget(label_11, 0, 0, 1, 1);

        horizontalLayout_8 = new QHBoxLayout();
        horizontalLayout_8->setObjectName(QString::fromUtf8("horizontalLayout_8"));
        label_12 = new QLabel(frame_4);
        label_12->setObjectName(QString::fromUtf8("label_12"));
        label_12->setFont(font1);

        horizontalLayout_8->addWidget(label_12);

        cbox_SerialBaudChoose = new QComboBox(frame_4);
        cbox_SerialBaudChoose->addItem(QString());
        cbox_SerialBaudChoose->addItem(QString());
        cbox_SerialBaudChoose->addItem(QString());
        cbox_SerialBaudChoose->addItem(QString());
        cbox_SerialBaudChoose->addItem(QString());
        cbox_SerialBaudChoose->addItem(QString());
        cbox_SerialBaudChoose->setObjectName(QString::fromUtf8("cbox_SerialBaudChoose"));
        sizePolicy1.setHeightForWidth(cbox_SerialBaudChoose->sizePolicy().hasHeightForWidth());
        cbox_SerialBaudChoose->setSizePolicy(sizePolicy1);
        cbox_SerialBaudChoose->setFont(font1);

        horizontalLayout_8->addWidget(cbox_SerialBaudChoose);


        gridLayout_10->addLayout(horizontalLayout_8, 2, 0, 1, 1);


        gridLayout->addWidget(frame_4, 14, 0, 1, 2);

        pbtn_OpenNet = new QPushButton(groupBox_2);
        pbtn_OpenNet->setObjectName(QString::fromUtf8("pbtn_OpenNet"));
        sizePolicy.setHeightForWidth(pbtn_OpenNet->sizePolicy().hasHeightForWidth());
        pbtn_OpenNet->setSizePolicy(sizePolicy);
        pbtn_OpenNet->setMinimumSize(QSize(0, 50));
        pbtn_OpenNet->setMaximumSize(QSize(16777215, 50));
        pbtn_OpenNet->setFont(font1);

        gridLayout->addWidget(pbtn_OpenNet, 19, 0, 1, 2);

        frame_2 = new QFrame(groupBox_2);
        frame_2->setObjectName(QString::fromUtf8("frame_2"));
        frame_2->setStyleSheet(QString::fromUtf8(""));
        frame_2->setFrameShape(QFrame::StyledPanel);
        frame_2->setFrameShadow(QFrame::Raised);
        gridLayout_6 = new QGridLayout(frame_2);
        gridLayout_6->setObjectName(QString::fromUtf8("gridLayout_6"));
        cbox_LocakIP = new QComboBox(frame_2);
        cbox_LocakIP->setObjectName(QString::fromUtf8("cbox_LocakIP"));
        cbox_LocakIP->setMinimumSize(QSize(0, 28));
        QFont font3;
        font3.setPointSize(13);
        font3.setBold(true);
        font3.setWeight(75);
        cbox_LocakIP->setFont(font3);

        gridLayout_6->addWidget(cbox_LocakIP, 1, 0, 1, 1);

        label = new QLabel(frame_2);
        label->setObjectName(QString::fromUtf8("label"));
        label->setMaximumSize(QSize(16777215, 20));
        label->setFont(font1);

        gridLayout_6->addWidget(label, 0, 0, 1, 1);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        label_3 = new QLabel(frame_2);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setMinimumSize(QSize(0, 0));
        label_3->setMaximumSize(QSize(16777215, 20));
        label_3->setFont(font1);

        horizontalLayout_3->addWidget(label_3);

        edit_LocalPort = new QLineEdit(frame_2);
        edit_LocalPort->setObjectName(QString::fromUtf8("edit_LocalPort"));
        QSizePolicy sizePolicy3(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(edit_LocalPort->sizePolicy().hasHeightForWidth());
        edit_LocalPort->setSizePolicy(sizePolicy3);
        edit_LocalPort->setMaximumSize(QSize(80, 16777215));
        edit_LocalPort->setFont(font1);

        horizontalLayout_3->addWidget(edit_LocalPort);


        gridLayout_6->addLayout(horizontalLayout_3, 4, 0, 1, 1);


        gridLayout->addWidget(frame_2, 2, 0, 1, 2);

        label_18 = new QLabel(groupBox_2);
        label_18->setObjectName(QString::fromUtf8("label_18"));
        label_18->setFont(font1);

        gridLayout->addWidget(label_18, 0, 0, 1, 1);

        pbtn_RefreshSerial = new QPushButton(groupBox_2);
        pbtn_RefreshSerial->setObjectName(QString::fromUtf8("pbtn_RefreshSerial"));
        sizePolicy.setHeightForWidth(pbtn_RefreshSerial->sizePolicy().hasHeightForWidth());
        pbtn_RefreshSerial->setSizePolicy(sizePolicy);
        pbtn_RefreshSerial->setMinimumSize(QSize(0, 50));
        pbtn_RefreshSerial->setMaximumSize(QSize(16777215, 50));
        pbtn_RefreshSerial->setFont(font1);

        gridLayout->addWidget(pbtn_RefreshSerial, 21, 0, 1, 2);

        frame_3 = new QFrame(groupBox_2);
        frame_3->setObjectName(QString::fromUtf8("frame_3"));
        frame_3->setStyleSheet(QString::fromUtf8(""));
        frame_3->setFrameShape(QFrame::StyledPanel);
        frame_3->setFrameShadow(QFrame::Raised);
        gridLayout_7 = new QGridLayout(frame_3);
        gridLayout_7->setObjectName(QString::fromUtf8("gridLayout_7"));
        label_15 = new QLabel(frame_3);
        label_15->setObjectName(QString::fromUtf8("label_15"));
        label_15->setFont(font1);

        gridLayout_7->addWidget(label_15, 0, 0, 1, 1);

        edit_TargetIP = new QLineEdit(frame_3);
        edit_TargetIP->setObjectName(QString::fromUtf8("edit_TargetIP"));
        edit_TargetIP->setFont(font1);

        gridLayout_7->addWidget(edit_TargetIP, 1, 0, 1, 1);

        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setObjectName(QString::fromUtf8("horizontalLayout_5"));
        label_16 = new QLabel(frame_3);
        label_16->setObjectName(QString::fromUtf8("label_16"));
        label_16->setFont(font1);

        horizontalLayout_5->addWidget(label_16);

        edit_TargetPort = new QLineEdit(frame_3);
        edit_TargetPort->setObjectName(QString::fromUtf8("edit_TargetPort"));
        edit_TargetPort->setMaximumSize(QSize(80, 16777215));
        edit_TargetPort->setFont(font1);

        horizontalLayout_5->addWidget(edit_TargetPort);


        gridLayout_7->addLayout(horizontalLayout_5, 2, 0, 1, 1);


        gridLayout->addWidget(frame_3, 12, 0, 1, 2);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout->addItem(verticalSpacer, 18, 0, 1, 1);

        verticalSpacer_2 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout->addItem(verticalSpacer_2, 18, 1, 1, 1);


        verticalLayout->addWidget(groupBox_2);

        groupBox_3 = new QGroupBox(frame);
        groupBox_3->setObjectName(QString::fromUtf8("groupBox_3"));
        groupBox_3->setMaximumSize(QSize(16777215, 100));
        groupBox_3->setFont(font3);
        gridLayout_2 = new QGridLayout(groupBox_3);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        label_led = new QLabel(groupBox_3);
        label_led->setObjectName(QString::fromUtf8("label_led"));

        gridLayout_2->addWidget(label_led, 1, 1, 1, 1);

        label_4 = new QLabel(groupBox_3);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        QFont font4;
        font4.setPointSize(12);
        font4.setBold(true);
        font4.setWeight(75);
        label_4->setFont(font4);

        gridLayout_2->addWidget(label_4, 1, 0, 1, 1);

        pbtn_Diagnosis = new QPushButton(groupBox_3);
        pbtn_Diagnosis->setObjectName(QString::fromUtf8("pbtn_Diagnosis"));
        pbtn_Diagnosis->setMinimumSize(QSize(0, 30));
        pbtn_Diagnosis->setFont(font3);

        gridLayout_2->addWidget(pbtn_Diagnosis, 2, 0, 1, 2);


        verticalLayout->addWidget(groupBox_3);


        horizontalLayout_4->addLayout(verticalLayout);

        verticalLayout_5 = new QVBoxLayout();
        verticalLayout_5->setObjectName(QString::fromUtf8("verticalLayout_5"));
        tabWidget = new QTabWidget(frame);
        tabWidget->setObjectName(QString::fromUtf8("tabWidget"));
        QFont font5;
        font5.setPointSize(13);
        tabWidget->setFont(font5);
        tabWidget->setStyleSheet(QString::fromUtf8("QTabWidget::pane {\n"
"    border: 1px solid #CCCCCC;\n"
"    border-radius: 4px;\n"
"    padding: 5px;\n"
"}\n"
"\n"
"QTabBar::tab {\n"
"    border: 1px solid #CCCCCC;\n"
"    border-radius: 6px;\n"
"    min-width: 80px;\n"
"    padding: 6px 12px;\n"
"    margin-right: 4px;\n"
"}\n"
"\n"
"QTabBar::tab:selected {\n"
"    background-color: #0078D7;\n"
"    color: white;\n"
"    border-color: #005A9E;\n"
"}\n"
""));
        tabWidget->setTabShape(QTabWidget::Triangular);
        tab = new QWidget();
        tab->setObjectName(QString::fromUtf8("tab"));
        gridLayout_9 = new QGridLayout(tab);
        gridLayout_9->setObjectName(QString::fromUtf8("gridLayout_9"));
        cplot_Spectrum = new QCustomPlot(tab);
        cplot_Spectrum->setObjectName(QString::fromUtf8("cplot_Spectrum"));

        gridLayout_9->addWidget(cplot_Spectrum, 0, 0, 1, 1);

        tabWidget->addTab(tab, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName(QString::fromUtf8("tab_2"));
        gridLayout_8 = new QGridLayout(tab_2);
        gridLayout_8->setObjectName(QString::fromUtf8("gridLayout_8"));
        tablev_Wavelength = new QTableView(tab_2);
        tablev_Wavelength->setObjectName(QString::fromUtf8("tablev_Wavelength"));

        gridLayout_8->addWidget(tablev_Wavelength, 0, 0, 1, 1);

        tabWidget->addTab(tab_2, QString());
        tab_3 = new QWidget();
        tab_3->setObjectName(QString::fromUtf8("tab_3"));
        verticalLayout_4 = new QVBoxLayout(tab_3);
        verticalLayout_4->setObjectName(QString::fromUtf8("verticalLayout_4"));
        label_13 = new QLabel(tab_3);
        label_13->setObjectName(QString::fromUtf8("label_13"));
        label_13->setAlignment(Qt::AlignCenter);

        verticalLayout_4->addWidget(label_13);

        ptedit_SerialRec = new QPlainTextEdit(tab_3);
        ptedit_SerialRec->setObjectName(QString::fromUtf8("ptedit_SerialRec"));

        verticalLayout_4->addWidget(ptedit_SerialRec);

        ptedit_SerialSend = new QPlainTextEdit(tab_3);
        ptedit_SerialSend->setObjectName(QString::fromUtf8("ptedit_SerialSend"));
        ptedit_SerialSend->setMaximumSize(QSize(16777215, 120));

        verticalLayout_4->addWidget(ptedit_SerialSend);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalSpacer = new QSpacerItem(79, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        pbtn_SerialClear = new QPushButton(tab_3);
        pbtn_SerialClear->setObjectName(QString::fromUtf8("pbtn_SerialClear"));
        pbtn_SerialClear->setMinimumSize(QSize(0, 35));
        pbtn_SerialClear->setBaseSize(QSize(0, 0));

        horizontalLayout->addWidget(pbtn_SerialClear);

        horizontalSpacer_2 = new QSpacerItem(118, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_2);

        pbtn_SerialSend = new QPushButton(tab_3);
        pbtn_SerialSend->setObjectName(QString::fromUtf8("pbtn_SerialSend"));
        pbtn_SerialSend->setMinimumSize(QSize(0, 35));

        horizontalLayout->addWidget(pbtn_SerialSend);

        horizontalSpacer_3 = new QSpacerItem(210, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_3);

        horizontalLayout->setStretch(0, 10);
        horizontalLayout->setStretch(1, 20);
        horizontalLayout->setStretch(2, 10);
        horizontalLayout->setStretch(3, 20);
        horizontalLayout->setStretch(4, 10);

        verticalLayout_4->addLayout(horizontalLayout);

        verticalLayout_4->setStretch(1, 50);
        verticalLayout_4->setStretch(2, 10);
        verticalLayout_4->setStretch(3, 5);
        tabWidget->addTab(tab_3, QString());
        tab_4 = new QWidget();
        tab_4->setObjectName(QString::fromUtf8("tab_4"));
        verticalLayout_6 = new QVBoxLayout(tab_4);
        verticalLayout_6->setObjectName(QString::fromUtf8("verticalLayout_6"));
        label_14 = new QLabel(tab_4);
        label_14->setObjectName(QString::fromUtf8("label_14"));
        label_14->setAlignment(Qt::AlignCenter);

        verticalLayout_6->addWidget(label_14);

        ptedit_UdpRec = new QPlainTextEdit(tab_4);
        ptedit_UdpRec->setObjectName(QString::fromUtf8("ptedit_UdpRec"));

        verticalLayout_6->addWidget(ptedit_UdpRec);

        ptedit_UdpSend = new QPlainTextEdit(tab_4);
        ptedit_UdpSend->setObjectName(QString::fromUtf8("ptedit_UdpSend"));
        ptedit_UdpSend->setMaximumSize(QSize(16777215, 120));

        verticalLayout_6->addWidget(ptedit_UdpSend);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        horizontalSpacer_4 = new QSpacerItem(79, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_4);

        pbtn_UdpClear = new QPushButton(tab_4);
        pbtn_UdpClear->setObjectName(QString::fromUtf8("pbtn_UdpClear"));
        pbtn_UdpClear->setMinimumSize(QSize(0, 35));
        pbtn_UdpClear->setBaseSize(QSize(0, 0));

        horizontalLayout_2->addWidget(pbtn_UdpClear);

        horizontalSpacer_5 = new QSpacerItem(118, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_5);

        pbtn_UdpSend = new QPushButton(tab_4);
        pbtn_UdpSend->setObjectName(QString::fromUtf8("pbtn_UdpSend"));
        pbtn_UdpSend->setMinimumSize(QSize(0, 35));

        horizontalLayout_2->addWidget(pbtn_UdpSend);

        horizontalSpacer_6 = new QSpacerItem(210, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_6);

        horizontalLayout_2->setStretch(1, 20);
        horizontalLayout_2->setStretch(2, 10);
        horizontalLayout_2->setStretch(3, 20);
        horizontalLayout_2->setStretch(4, 10);

        verticalLayout_6->addLayout(horizontalLayout_2);

        tabWidget->addTab(tab_4, QString());
        tab_5 = new QWidget();
        tab_5->setObjectName(QString::fromUtf8("tab_5"));
        verticalLayout_9 = new QVBoxLayout(tab_5);
        verticalLayout_9->setObjectName(QString::fromUtf8("verticalLayout_9"));
        label_17 = new QLabel(tab_5);
        label_17->setObjectName(QString::fromUtf8("label_17"));
        label_17->setAlignment(Qt::AlignCenter);

        verticalLayout_9->addWidget(label_17);

        ptedit_TcpRec = new QPlainTextEdit(tab_5);
        ptedit_TcpRec->setObjectName(QString::fromUtf8("ptedit_TcpRec"));

        verticalLayout_9->addWidget(ptedit_TcpRec);

        ptedit_TcpSen = new QPlainTextEdit(tab_5);
        ptedit_TcpSen->setObjectName(QString::fromUtf8("ptedit_TcpSen"));
        ptedit_TcpSen->setMaximumSize(QSize(16777215, 120));

        verticalLayout_9->addWidget(ptedit_TcpSen);

        horizontalLayout_7 = new QHBoxLayout();
        horizontalLayout_7->setObjectName(QString::fromUtf8("horizontalLayout_7"));
        horizontalSpacer_13 = new QSpacerItem(79, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_7->addItem(horizontalSpacer_13);

        pbtn_TcpClear = new QPushButton(tab_5);
        pbtn_TcpClear->setObjectName(QString::fromUtf8("pbtn_TcpClear"));
        pbtn_TcpClear->setMinimumSize(QSize(0, 35));
        pbtn_TcpClear->setBaseSize(QSize(0, 0));

        horizontalLayout_7->addWidget(pbtn_TcpClear);

        horizontalSpacer_14 = new QSpacerItem(118, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_7->addItem(horizontalSpacer_14);

        pbtn_TcpSend = new QPushButton(tab_5);
        pbtn_TcpSend->setObjectName(QString::fromUtf8("pbtn_TcpSend"));
        pbtn_TcpSend->setMinimumSize(QSize(0, 35));

        horizontalLayout_7->addWidget(pbtn_TcpSend);

        horizontalSpacer_15 = new QSpacerItem(210, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_7->addItem(horizontalSpacer_15);

        horizontalLayout_7->setStretch(1, 20);
        horizontalLayout_7->setStretch(2, 10);
        horizontalLayout_7->setStretch(3, 20);
        horizontalLayout_7->setStretch(4, 10);

        verticalLayout_9->addLayout(horizontalLayout_7);

        tabWidget->addTab(tab_5, QString());

        verticalLayout_5->addWidget(tabWidget);

        groupBox_select = new QGroupBox(frame);
        groupBox_select->setObjectName(QString::fromUtf8("groupBox_select"));
        QFont font6;
        font6.setPointSize(15);
        font6.setBold(true);
        font6.setWeight(75);
        groupBox_select->setFont(font6);
        groupBox_select->setAutoFillBackground(false);
        groupBox_select->setAlignment(Qt::AlignBottom|Qt::AlignLeading|Qt::AlignLeft);
        groupBox_select->setFlat(false);
        groupBox_select->setCheckable(false);
        gridLayout_5 = new QGridLayout(groupBox_select);
        gridLayout_5->setObjectName(QString::fromUtf8("gridLayout_5"));
        pbtn_SelectCH = new QPushButton(groupBox_select);
        pbtn_SelectCH->setObjectName(QString::fromUtf8("pbtn_SelectCH"));
        pbtn_SelectCH->setMinimumSize(QSize(0, 35));
        pbtn_SelectCH->setFont(font3);

        gridLayout_5->addWidget(pbtn_SelectCH, 1, 10, 1, 1);

        label_2 = new QLabel(groupBox_select);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setFont(font1);

        gridLayout_5->addWidget(label_2, 0, 0, 1, 1);

        ckbox_CH13 = new QCheckBox(groupBox_select);
        ckbox_CH13->setObjectName(QString::fromUtf8("ckbox_CH13"));
        ckbox_CH13->setEnabled(true);
        QFont font7;
        font7.setPointSize(12);
        font7.setBold(false);
        font7.setWeight(50);
        ckbox_CH13->setFont(font7);
        ckbox_CH13->setChecked(false);
        ckbox_CH13->setTristate(false);

        gridLayout_5->addWidget(ckbox_CH13, 1, 6, 1, 1);

        ckbox_CH4 = new QCheckBox(groupBox_select);
        ckbox_CH4->setObjectName(QString::fromUtf8("ckbox_CH4"));
        ckbox_CH4->setFont(font7);
        ckbox_CH4->setChecked(false);

        gridLayout_5->addWidget(ckbox_CH4, 0, 5, 1, 1);

        ckbox_CH15 = new QCheckBox(groupBox_select);
        ckbox_CH15->setObjectName(QString::fromUtf8("ckbox_CH15"));
        ckbox_CH15->setEnabled(true);
        ckbox_CH15->setFont(font7);
        ckbox_CH15->setChecked(false);
        ckbox_CH15->setTristate(false);

        gridLayout_5->addWidget(ckbox_CH15, 1, 8, 1, 1);

        ckbox_CH11 = new QCheckBox(groupBox_select);
        ckbox_CH11->setObjectName(QString::fromUtf8("ckbox_CH11"));
        ckbox_CH11->setEnabled(true);
        ckbox_CH11->setFont(font7);
        ckbox_CH11->setChecked(false);
        ckbox_CH11->setTristate(false);

        gridLayout_5->addWidget(ckbox_CH11, 1, 4, 1, 1);

        ckbox_CH12 = new QCheckBox(groupBox_select);
        ckbox_CH12->setObjectName(QString::fromUtf8("ckbox_CH12"));
        ckbox_CH12->setEnabled(true);
        ckbox_CH12->setFont(font7);
        ckbox_CH12->setChecked(false);
        ckbox_CH12->setTristate(false);

        gridLayout_5->addWidget(ckbox_CH12, 1, 5, 1, 1);

        ckbox_CH10 = new QCheckBox(groupBox_select);
        ckbox_CH10->setObjectName(QString::fromUtf8("ckbox_CH10"));
        ckbox_CH10->setEnabled(true);
        ckbox_CH10->setFont(font7);
        ckbox_CH10->setChecked(false);
        ckbox_CH10->setTristate(false);

        gridLayout_5->addWidget(ckbox_CH10, 1, 3, 1, 1);

        ckbox_CH6 = new QCheckBox(groupBox_select);
        ckbox_CH6->setObjectName(QString::fromUtf8("ckbox_CH6"));
        ckbox_CH6->setFont(font7);
        ckbox_CH6->setChecked(false);

        gridLayout_5->addWidget(ckbox_CH6, 0, 7, 1, 1);

        ckbox_CH7 = new QCheckBox(groupBox_select);
        ckbox_CH7->setObjectName(QString::fromUtf8("ckbox_CH7"));
        ckbox_CH7->setFont(font7);
        ckbox_CH7->setChecked(false);

        gridLayout_5->addWidget(ckbox_CH7, 0, 8, 1, 1);

        pbtn_SelectAllCH = new QPushButton(groupBox_select);
        pbtn_SelectAllCH->setObjectName(QString::fromUtf8("pbtn_SelectAllCH"));
        pbtn_SelectAllCH->setMinimumSize(QSize(0, 35));
        pbtn_SelectAllCH->setFont(font3);

        gridLayout_5->addWidget(pbtn_SelectAllCH, 0, 10, 1, 1);

        ckbox_CH8 = new QCheckBox(groupBox_select);
        ckbox_CH8->setObjectName(QString::fromUtf8("ckbox_CH8"));
        ckbox_CH8->setEnabled(true);
        ckbox_CH8->setFont(font7);
        ckbox_CH8->setChecked(false);
        ckbox_CH8->setTristate(false);

        gridLayout_5->addWidget(ckbox_CH8, 0, 9, 1, 1);

        ckbox_CH9 = new QCheckBox(groupBox_select);
        ckbox_CH9->setObjectName(QString::fromUtf8("ckbox_CH9"));
        ckbox_CH9->setEnabled(true);
        ckbox_CH9->setFont(font7);
        ckbox_CH9->setChecked(false);
        ckbox_CH9->setTristate(false);

        gridLayout_5->addWidget(ckbox_CH9, 1, 2, 1, 1);

        ckbox_CH14 = new QCheckBox(groupBox_select);
        ckbox_CH14->setObjectName(QString::fromUtf8("ckbox_CH14"));
        ckbox_CH14->setEnabled(true);
        ckbox_CH14->setFont(font7);
        ckbox_CH14->setChecked(false);
        ckbox_CH14->setTristate(false);

        gridLayout_5->addWidget(ckbox_CH14, 1, 7, 1, 1);

        ckbox_CH1 = new QCheckBox(groupBox_select);
        ckbox_CH1->setObjectName(QString::fromUtf8("ckbox_CH1"));
        ckbox_CH1->setEnabled(true);
        ckbox_CH1->setFont(font7);
        ckbox_CH1->setChecked(false);
        ckbox_CH1->setTristate(false);

        gridLayout_5->addWidget(ckbox_CH1, 0, 2, 1, 1);

        ckbox_CH2 = new QCheckBox(groupBox_select);
        ckbox_CH2->setObjectName(QString::fromUtf8("ckbox_CH2"));
        ckbox_CH2->setFont(font7);
        ckbox_CH2->setChecked(false);

        gridLayout_5->addWidget(ckbox_CH2, 0, 3, 1, 1);

        ckbox_CH3 = new QCheckBox(groupBox_select);
        ckbox_CH3->setObjectName(QString::fromUtf8("ckbox_CH3"));
        ckbox_CH3->setFont(font7);
        ckbox_CH3->setCheckable(true);
        ckbox_CH3->setChecked(false);

        gridLayout_5->addWidget(ckbox_CH3, 0, 4, 1, 1);

        ckbox_CH5 = new QCheckBox(groupBox_select);
        ckbox_CH5->setObjectName(QString::fromUtf8("ckbox_CH5"));
        ckbox_CH5->setFont(font7);
        ckbox_CH5->setChecked(false);

        gridLayout_5->addWidget(ckbox_CH5, 0, 6, 1, 1);

        ckbox_CH16 = new QCheckBox(groupBox_select);
        ckbox_CH16->setObjectName(QString::fromUtf8("ckbox_CH16"));
        ckbox_CH16->setEnabled(true);
        ckbox_CH16->setFont(font7);
        ckbox_CH16->setChecked(false);
        ckbox_CH16->setTristate(false);

        gridLayout_5->addWidget(ckbox_CH16, 1, 9, 1, 1);


        verticalLayout_5->addWidget(groupBox_select);


        horizontalLayout_4->addLayout(verticalLayout_5);

        verticalLayout_3 = new QVBoxLayout();
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        groupBox = new QGroupBox(frame);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        groupBox->setMaximumSize(QSize(200, 16777215));
        groupBox->setFont(font3);
        gridLayout_4 = new QGridLayout(groupBox);
        gridLayout_4->setObjectName(QString::fromUtf8("gridLayout_4"));
        textEdit_APara = new QTextEdit(groupBox);
        textEdit_APara->setObjectName(QString::fromUtf8("textEdit_APara"));
        QSizePolicy sizePolicy4(QSizePolicy::Expanding, QSizePolicy::Preferred);
        sizePolicy4.setHorizontalStretch(0);
        sizePolicy4.setVerticalStretch(0);
        sizePolicy4.setHeightForWidth(textEdit_APara->sizePolicy().hasHeightForWidth());
        textEdit_APara->setSizePolicy(sizePolicy4);
        textEdit_APara->setMaximumSize(QSize(80, 30));
        textEdit_APara->setSizeIncrement(QSize(0, 0));

        gridLayout_4->addWidget(textEdit_APara, 2, 1, 1, 2);

        label_9 = new QLabel(groupBox);
        label_9->setObjectName(QString::fromUtf8("label_9"));
        label_9->setMaximumSize(QSize(16777215, 20));
        label_9->setFont(font1);

        gridLayout_4->addWidget(label_9, 8, 0, 1, 3);

        textEdit_StrainTopPara = new QTextEdit(groupBox);
        textEdit_StrainTopPara->setObjectName(QString::fromUtf8("textEdit_StrainTopPara"));
        textEdit_StrainTopPara->setMaximumSize(QSize(16777215, 30));

        gridLayout_4->addWidget(textEdit_StrainTopPara, 9, 2, 1, 1);

        label_7 = new QLabel(groupBox);
        label_7->setObjectName(QString::fromUtf8("label_7"));
        label_7->setMaximumSize(QSize(16777215, 20));
        label_7->setFont(font1);

        gridLayout_4->addWidget(label_7, 4, 0, 1, 3);

        textEdit_StrainLowPara = new QTextEdit(groupBox);
        textEdit_StrainLowPara->setObjectName(QString::fromUtf8("textEdit_StrainLowPara"));
        textEdit_StrainLowPara->setMaximumSize(QSize(16777215, 30));

        gridLayout_4->addWidget(textEdit_StrainLowPara, 9, 0, 1, 1);

        label_8 = new QLabel(groupBox);
        label_8->setObjectName(QString::fromUtf8("label_8"));
        label_8->setMaximumSize(QSize(16777215, 20));
        label_8->setFont(font1);

        gridLayout_4->addWidget(label_8, 6, 0, 1, 3);

        cbox_SaveCycle = new QComboBox(groupBox);
        cbox_SaveCycle->addItem(QString());
        cbox_SaveCycle->addItem(QString());
        cbox_SaveCycle->addItem(QString());
        cbox_SaveCycle->addItem(QString());
        cbox_SaveCycle->setObjectName(QString::fromUtf8("cbox_SaveCycle"));
        sizePolicy1.setHeightForWidth(cbox_SaveCycle->sizePolicy().hasHeightForWidth());
        cbox_SaveCycle->setSizePolicy(sizePolicy1);
        cbox_SaveCycle->setMaximumSize(QSize(16777215, 30));

        gridLayout_4->addWidget(cbox_SaveCycle, 7, 0, 1, 3);

        label_10 = new QLabel(groupBox);
        label_10->setObjectName(QString::fromUtf8("label_10"));
        QFont font8;
        font8.setPointSize(11);
        font8.setBold(false);
        font8.setWeight(50);
        label_10->setFont(font8);

        gridLayout_4->addWidget(label_10, 9, 1, 1, 1);

        label_5 = new QLabel(groupBox);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        label_5->setFont(font1);

        gridLayout_4->addWidget(label_5, 2, 0, 1, 1);

        textEdit_BPara = new QTextEdit(groupBox);
        textEdit_BPara->setObjectName(QString::fromUtf8("textEdit_BPara"));
        sizePolicy4.setHeightForWidth(textEdit_BPara->sizePolicy().hasHeightForWidth());
        textEdit_BPara->setSizePolicy(sizePolicy4);
        textEdit_BPara->setMaximumSize(QSize(80, 30));

        gridLayout_4->addWidget(textEdit_BPara, 3, 1, 1, 2);

        cbox_ModleSelect = new QComboBox(groupBox);
        cbox_ModleSelect->addItem(QString());
        cbox_ModleSelect->setObjectName(QString::fromUtf8("cbox_ModleSelect"));
        sizePolicy1.setHeightForWidth(cbox_ModleSelect->sizePolicy().hasHeightForWidth());
        cbox_ModleSelect->setSizePolicy(sizePolicy1);
        cbox_ModleSelect->setMaximumSize(QSize(16777215, 30));
        cbox_ModleSelect->setFont(font1);

        gridLayout_4->addWidget(cbox_ModleSelect, 1, 0, 1, 3);

        cbox_CampingFre = new QComboBox(groupBox);
        cbox_CampingFre->addItem(QString());
        cbox_CampingFre->addItem(QString());
        cbox_CampingFre->addItem(QString());
        cbox_CampingFre->setObjectName(QString::fromUtf8("cbox_CampingFre"));
        sizePolicy1.setHeightForWidth(cbox_CampingFre->sizePolicy().hasHeightForWidth());
        cbox_CampingFre->setSizePolicy(sizePolicy1);
        cbox_CampingFre->setMaximumSize(QSize(16777215, 30));

        gridLayout_4->addWidget(cbox_CampingFre, 5, 0, 1, 3);

        label_6 = new QLabel(groupBox);
        label_6->setObjectName(QString::fromUtf8("label_6"));
        label_6->setFont(font1);

        gridLayout_4->addWidget(label_6, 3, 0, 1, 1);

        pbtn_WaveLengthReset = new QPushButton(groupBox);
        pbtn_WaveLengthReset->setObjectName(QString::fromUtf8("pbtn_WaveLengthReset"));
        sizePolicy.setHeightForWidth(pbtn_WaveLengthReset->sizePolicy().hasHeightForWidth());
        pbtn_WaveLengthReset->setSizePolicy(sizePolicy);
        pbtn_WaveLengthReset->setMaximumSize(QSize(16777215, 30));
        pbtn_WaveLengthReset->setFont(font1);

        gridLayout_4->addWidget(pbtn_WaveLengthReset, 0, 0, 1, 3);

        pbtn_ParaSet = new QPushButton(groupBox);
        pbtn_ParaSet->setObjectName(QString::fromUtf8("pbtn_ParaSet"));
        sizePolicy.setHeightForWidth(pbtn_ParaSet->sizePolicy().hasHeightForWidth());
        pbtn_ParaSet->setSizePolicy(sizePolicy);
        pbtn_ParaSet->setMinimumSize(QSize(0, 50));
        pbtn_ParaSet->setMaximumSize(QSize(16777215, 50));
        pbtn_ParaSet->setFont(font1);

        gridLayout_4->addWidget(pbtn_ParaSet, 10, 0, 1, 1);

        pbtn_ParaRest = new QPushButton(groupBox);
        pbtn_ParaRest->setObjectName(QString::fromUtf8("pbtn_ParaRest"));
        pbtn_ParaRest->setMinimumSize(QSize(0, 50));
        pbtn_ParaRest->setMaximumSize(QSize(16777215, 50));
        pbtn_ParaRest->setFont(font1);

        gridLayout_4->addWidget(pbtn_ParaRest, 10, 2, 1, 1);

        gridLayout_4->setRowStretch(0, 10);
        gridLayout_4->setRowStretch(1, 10);
        gridLayout_4->setRowStretch(2, 10);
        gridLayout_4->setRowStretch(3, 10);
        gridLayout_4->setRowStretch(4, 10);
        gridLayout_4->setRowStretch(5, 10);
        gridLayout_4->setRowStretch(6, 10);
        gridLayout_4->setRowStretch(7, 10);
        gridLayout_4->setRowStretch(8, 10);
        gridLayout_4->setRowStretch(9, 10);
        gridLayout_4->setRowStretch(10, 10);

        verticalLayout_3->addWidget(groupBox);

        groupBox_save = new QGroupBox(frame);
        groupBox_save->setObjectName(QString::fromUtf8("groupBox_save"));
        groupBox_save->setFont(font3);
        groupBox_save->setAutoFillBackground(false);
        verticalLayout_2 = new QVBoxLayout(groupBox_save);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        pbtn_SaveDynamic = new QPushButton(groupBox_save);
        pbtn_SaveDynamic->setObjectName(QString::fromUtf8("pbtn_SaveDynamic"));
        sizePolicy.setHeightForWidth(pbtn_SaveDynamic->sizePolicy().hasHeightForWidth());
        pbtn_SaveDynamic->setSizePolicy(sizePolicy);
        pbtn_SaveDynamic->setFont(font1);

        verticalLayout_2->addWidget(pbtn_SaveDynamic);

        pbtn_SaveStatic = new QPushButton(groupBox_save);
        pbtn_SaveStatic->setObjectName(QString::fromUtf8("pbtn_SaveStatic"));
        sizePolicy.setHeightForWidth(pbtn_SaveStatic->sizePolicy().hasHeightForWidth());
        pbtn_SaveStatic->setSizePolicy(sizePolicy);
        pbtn_SaveStatic->setFont(font1);

        verticalLayout_2->addWidget(pbtn_SaveStatic);

        pbtn_SaveSpectrum = new QPushButton(groupBox_save);
        pbtn_SaveSpectrum->setObjectName(QString::fromUtf8("pbtn_SaveSpectrum"));
        sizePolicy.setHeightForWidth(pbtn_SaveSpectrum->sizePolicy().hasHeightForWidth());
        pbtn_SaveSpectrum->setSizePolicy(sizePolicy);
        pbtn_SaveSpectrum->setFont(font1);

        verticalLayout_2->addWidget(pbtn_SaveSpectrum);


        verticalLayout_3->addWidget(groupBox_save);

        verticalLayout_3->setStretch(0, 30);
        verticalLayout_3->setStretch(1, 10);

        horizontalLayout_4->addLayout(verticalLayout_3);


        gridLayout_3->addWidget(frame, 0, 0, 1, 1);

        horizontalLayout_6 = new QHBoxLayout();
        horizontalLayout_6->setObjectName(QString::fromUtf8("horizontalLayout_6"));
        label_20 = new QLabel(connectwindow);
        label_20->setObjectName(QString::fromUtf8("label_20"));
        label_20->setMaximumSize(QSize(60, 30));
        QFont font9;
        font9.setPointSize(11);
        label_20->setFont(font9);

        horizontalLayout_6->addWidget(label_20);

        lab_State = new QLabel(connectwindow);
        lab_State->setObjectName(QString::fromUtf8("lab_State"));
        lab_State->setMaximumSize(QSize(16777215, 30));
        lab_State->setFont(font9);
        lab_State->setStyleSheet(QString::fromUtf8("color: rgb(90, 150, 89);"));

        horizontalLayout_6->addWidget(lab_State);


        gridLayout_3->addLayout(horizontalLayout_6, 1, 0, 1, 1);


        retranslateUi(connectwindow);

        tabWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(connectwindow);
    } // setupUi

    void retranslateUi(QWidget *connectwindow)
    {
        connectwindow->setWindowTitle(QApplication::translate("connectwindow", "Form", nullptr));
        groupBox_2->setTitle(QApplication::translate("connectwindow", "\351\200\232\344\277\241\350\256\276\347\275\256", nullptr));
        pbtn_OpenSerial->setText(QApplication::translate("connectwindow", "\346\211\223\345\274\200\344\270\262\345\217\243", nullptr));
        cbox_CommuteChoose->setItemText(0, QApplication::translate("connectwindow", "UDP\351\200\232\344\277\241", nullptr));
        cbox_CommuteChoose->setItemText(1, QApplication::translate("connectwindow", "TCP\351\200\232\344\277\241", nullptr));

        label_11->setText(QApplication::translate("connectwindow", "\344\270\262\345\217\243\345\220\215\357\274\232", nullptr));
        label_12->setText(QApplication::translate("connectwindow", "\346\263\242\347\211\271\347\216\207\357\274\232", nullptr));
        cbox_SerialBaudChoose->setItemText(0, QApplication::translate("connectwindow", "921600", nullptr));
        cbox_SerialBaudChoose->setItemText(1, QApplication::translate("connectwindow", "115200", nullptr));
        cbox_SerialBaudChoose->setItemText(2, QApplication::translate("connectwindow", "9600", nullptr));
        cbox_SerialBaudChoose->setItemText(3, QApplication::translate("connectwindow", "2400", nullptr));
        cbox_SerialBaudChoose->setItemText(4, QApplication::translate("connectwindow", "4800", nullptr));
        cbox_SerialBaudChoose->setItemText(5, QApplication::translate("connectwindow", "1200", nullptr));

        cbox_SerialBaudChoose->setCurrentText(QApplication::translate("connectwindow", "921600", nullptr));
        pbtn_OpenNet->setText(QApplication::translate("connectwindow", "\350\277\236\346\216\245\347\275\221\347\273\234", nullptr));
        label->setText(QApplication::translate("connectwindow", "\346\234\254\345\234\260\345\234\260\345\235\200\357\274\232", nullptr));
        label_3->setText(QApplication::translate("connectwindow", "\346\234\254\345\234\260\347\253\257\345\217\243\357\274\232", nullptr));
        edit_LocalPort->setText(QApplication::translate("connectwindow", "5060", nullptr));
        label_18->setText(QApplication::translate("connectwindow", "\351\200\232\344\277\241\346\226\271\345\274\217\357\274\232", nullptr));
        pbtn_RefreshSerial->setText(QApplication::translate("connectwindow", "\345\210\267\346\226\260\344\270\262\345\217\243", nullptr));
        label_15->setText(QApplication::translate("connectwindow", "\347\233\256\346\240\207\345\234\260\345\235\200\357\274\232", nullptr));
        edit_TargetIP->setText(QApplication::translate("connectwindow", "10.12.124.158", nullptr));
        label_16->setText(QApplication::translate("connectwindow", "\347\233\256\346\240\207\347\253\257\345\217\243\357\274\232", nullptr));
        edit_TargetPort->setText(QApplication::translate("connectwindow", "5073", nullptr));
        groupBox_3->setTitle(QApplication::translate("connectwindow", "\347\212\266\346\200\201\346\214\207\347\244\272", nullptr));
        label_led->setText(QApplication::translate("connectwindow", "LED", nullptr));
        label_4->setText(QApplication::translate("connectwindow", "\350\277\220\350\241\214\347\212\266\346\200\201", nullptr));
        pbtn_Diagnosis->setText(QApplication::translate("connectwindow", "\346\225\205\351\232\234\350\257\212\346\226\255", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab), QApplication::translate("connectwindow", "\345\205\211\350\260\261", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab_2), QApplication::translate("connectwindow", "\346\263\242\351\225\277", nullptr));
        label_13->setText(QApplication::translate("connectwindow", "\344\270\262\345\217\243\346\224\266\345\217\221", nullptr));
        pbtn_SerialClear->setText(QApplication::translate("connectwindow", "\346\270\205\351\231\244\346\216\245\346\224\266", nullptr));
        pbtn_SerialSend->setText(QApplication::translate("connectwindow", "\347\202\271\345\207\273\345\217\221\351\200\201", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab_3), QApplication::translate("connectwindow", "\344\270\262\345\217\243", nullptr));
        label_14->setText(QApplication::translate("connectwindow", "UDP\345\215\217\350\256\256\346\224\266\345\217\221", nullptr));
        pbtn_UdpClear->setText(QApplication::translate("connectwindow", "\346\270\205\351\231\244\346\216\245\346\224\266", nullptr));
        pbtn_UdpSend->setText(QApplication::translate("connectwindow", "\347\202\271\345\207\273\345\217\221\351\200\201", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab_4), QApplication::translate("connectwindow", "UDP", nullptr));
        label_17->setText(QApplication::translate("connectwindow", "TCP\345\215\217\350\256\256\346\224\266\345\217\221", nullptr));
        pbtn_TcpClear->setText(QApplication::translate("connectwindow", "\346\270\205\351\231\244\346\216\245\346\224\266", nullptr));
        pbtn_TcpSend->setText(QApplication::translate("connectwindow", "\347\202\271\345\207\273\345\217\221\351\200\201", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab_5), QApplication::translate("connectwindow", "TCP", nullptr));
        groupBox_select->setTitle(QApplication::translate("connectwindow", "\351\200\232\351\201\223\347\255\233\351\200\211\357\274\232", nullptr));
        pbtn_SelectCH->setText(QApplication::translate("connectwindow", "\351\200\232\351\201\223\347\255\233\351\200\211", nullptr));
        label_2->setText(QApplication::translate("connectwindow", "\346\211\200\345\261\236\351\200\232\351\201\223\357\274\232", nullptr));
        ckbox_CH13->setText(QApplication::translate("connectwindow", "CH13", nullptr));
        ckbox_CH4->setText(QApplication::translate("connectwindow", "CH4", nullptr));
        ckbox_CH15->setText(QApplication::translate("connectwindow", "CH15", nullptr));
        ckbox_CH11->setText(QApplication::translate("connectwindow", "CH11", nullptr));
        ckbox_CH12->setText(QApplication::translate("connectwindow", "CH12", nullptr));
        ckbox_CH10->setText(QApplication::translate("connectwindow", "CH10", nullptr));
        ckbox_CH6->setText(QApplication::translate("connectwindow", "CH6", nullptr));
        ckbox_CH7->setText(QApplication::translate("connectwindow", "CH7", nullptr));
        pbtn_SelectAllCH->setText(QApplication::translate("connectwindow", "\345\205\250\351\200\211", nullptr));
        ckbox_CH8->setText(QApplication::translate("connectwindow", "CH8", nullptr));
        ckbox_CH9->setText(QApplication::translate("connectwindow", "CH9", nullptr));
        ckbox_CH14->setText(QApplication::translate("connectwindow", "CH14", nullptr));
        ckbox_CH1->setText(QApplication::translate("connectwindow", "CH1", nullptr));
        ckbox_CH2->setText(QApplication::translate("connectwindow", "CH2", nullptr));
        ckbox_CH3->setText(QApplication::translate("connectwindow", "CH3", nullptr));
        ckbox_CH5->setText(QApplication::translate("connectwindow", "CH5", nullptr));
        ckbox_CH16->setText(QApplication::translate("connectwindow", "CH16", nullptr));
        groupBox->setTitle(QApplication::translate("connectwindow", "\345\217\202\346\225\260\350\256\276\347\275\256", nullptr));
        label_9->setText(QApplication::translate("connectwindow", "\345\272\224\345\212\233\351\230\210\345\200\274\357\274\232", nullptr));
        textEdit_StrainTopPara->setHtml(QApplication::translate("connectwindow", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'SimSun'; font-size:13pt; font-weight:600; font-style:normal;\">\n"
"<p align=\"center\" style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><br /></p></body></html>", nullptr));
        label_7->setText(QApplication::translate("connectwindow", "\346\225\260\346\215\256\351\207\207\351\233\206\351\242\221\347\216\207(Hz)", nullptr));
        textEdit_StrainLowPara->setHtml(QApplication::translate("connectwindow", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'SimSun'; font-size:13pt; font-weight:600; font-style:normal;\">\n"
"<p align=\"center\" style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><br /></p></body></html>", nullptr));
        label_8->setText(QApplication::translate("connectwindow", "\344\277\235\345\255\230\346\227\266\351\227\264\351\227\264\351\232\224\357\274\210ms\357\274\211", nullptr));
        cbox_SaveCycle->setItemText(0, QApplication::translate("connectwindow", "1000", nullptr));
        cbox_SaveCycle->setItemText(1, QApplication::translate("connectwindow", "10", nullptr));
        cbox_SaveCycle->setItemText(2, QApplication::translate("connectwindow", "100", nullptr));
        cbox_SaveCycle->setItemText(3, QApplication::translate("connectwindow", "10000", nullptr));

        label_10->setText(QApplication::translate("connectwindow", "\350\207\263", nullptr));
        label_5->setText(QApplication::translate("connectwindow", "\345\217\202\346\225\2601:", nullptr));
        cbox_ModleSelect->setItemText(0, QApplication::translate("connectwindow", "\345\272\224\345\217\230", nullptr));

        cbox_CampingFre->setItemText(0, QApplication::translate("connectwindow", "1", nullptr));
        cbox_CampingFre->setItemText(1, QApplication::translate("connectwindow", "10", nullptr));
        cbox_CampingFre->setItemText(2, QApplication::translate("connectwindow", "100", nullptr));

        label_6->setText(QApplication::translate("connectwindow", "\345\217\202\346\225\2602:", nullptr));
        pbtn_WaveLengthReset->setText(QApplication::translate("connectwindow", "\346\263\242\351\225\277\345\244\215\344\275\215", nullptr));
        pbtn_ParaSet->setText(QApplication::translate("connectwindow", "\347\241\256\350\256\244", nullptr));
        pbtn_ParaRest->setText(QApplication::translate("connectwindow", "\351\207\215\347\275\256", nullptr));
        groupBox_save->setTitle(QApplication::translate("connectwindow", "\344\277\235\345\255\230\344\270\216\350\276\223\345\207\272", nullptr));
        pbtn_SaveDynamic->setText(QApplication::translate("connectwindow", "\350\207\252\345\212\250\344\277\235\345\255\230", nullptr));
        pbtn_SaveStatic->setText(QApplication::translate("connectwindow", "\346\211\213\345\212\250\344\277\235\345\255\230", nullptr));
        pbtn_SaveSpectrum->setText(QApplication::translate("connectwindow", "\345\205\211\350\260\261\344\277\235\345\255\230", nullptr));
        label_20->setText(QApplication::translate("connectwindow", "\345\275\223\345\211\215\347\212\266\346\200\201\357\274\232", nullptr));
        lab_State->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class connectwindow: public Ui_connectwindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CONNECTWINDOW_H
