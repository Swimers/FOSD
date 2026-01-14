/********************************************************************************
** Form generated from reading UI file 'demowindow.ui'
**
** Created by: Qt User Interface Compiler version 5.12.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DEMOWINDOW_H
#define UI_DEMOWINDOW_H

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
#include <qcustomplot.h>

QT_BEGIN_NAMESPACE

class Ui_DemoWindow
{
public:
    QHBoxLayout *horizontalLayout_2;
    QFrame *frame;
    QGridLayout *gridLayout_3;
    QVBoxLayout *verticalLayout;
    QGroupBox *groupBox_2;
    QGridLayout *gridLayout;
    QLineEdit *lineEdit_2;
    QLabel *label_3;
    QComboBox *cmbportbaud;
    QLineEdit *lineEdit_3;
    QLabel *label_12;
    QPushButton *btnrefresh;
    QLabel *label;
    QComboBox *cBox_commutechoose;
    QLabel *label_11;
    QPushButton *Button_connect;
    QComboBox *cmbportname;
    QPushButton *btnclose;
    QGroupBox *groupBox_3;
    QGridLayout *gridLayout_2;
    QPlainTextEdit *plainTextEdit;
    QLabel *label_4;
    QLabel *label_led;
    QPushButton *btn_diagnosis;
    QVBoxLayout *verticalLayout_5;
    QTabWidget *tabWidget;
    QWidget *tab;
    QGridLayout *gridLayout_9;
    QCustomPlot *widget;
    QWidget *tab_2;
    QGridLayout *gridLayout_8;
    QTableView *tableView;
    QWidget *tab_3;
    QVBoxLayout *verticalLayout_4;
    QPlainTextEdit *ptereceive;
    QPlainTextEdit *ptesend;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer;
    QPushButton *btnclear;
    QSpacerItem *horizontalSpacer_2;
    QPushButton *btnsend;
    QSpacerItem *horizontalSpacer_3;
    QGroupBox *groupBox_select;
    QHBoxLayout *horizontalLayout_3;
    QLabel *label_2;
    QCheckBox *checkBox_1;
    QCheckBox *checkBox_2;
    QCheckBox *checkBox_3;
    QCheckBox *checkBox_4;
    QCheckBox *checkBox_5;
    QCheckBox *checkBox_6;
    QCheckBox *checkBox_7;
    QPushButton *Button_selectshow;
    QVBoxLayout *verticalLayout_3;
    QGroupBox *groupBox;
    QGridLayout *gridLayout_4;
    QPushButton *Button_reset;
    QComboBox *comboBox_parameter_set;
    QLabel *label_5;
    QTextEdit *textEdit_parameter_set;
    QLabel *label_6;
    QTextEdit *textEdit_parameter_set2;
    QLabel *label_7;
    QLabel *label_8;
    QComboBox *comboBox_3;
    QLabel *label_9;
    QTextEdit *textEdit_parameter_set3;
    QLabel *label_10;
    QTextEdit *textEdit_parameter_set4;
    QPushButton *Button_parameter_set;
    QComboBox *comboBox;
    QGroupBox *groupBox_save;
    QVBoxLayout *verticalLayout_2;
    QPushButton *Button_Save_dynamic;
    QPushButton *Button_Save_Static;
    QPushButton *Button_save_spectrum;

    void setupUi(QWidget *DemoWindow)
    {
        if (DemoWindow->objectName().isEmpty())
            DemoWindow->setObjectName(QString::fromUtf8("DemoWindow"));
        DemoWindow->resize(1213, 766);
        horizontalLayout_2 = new QHBoxLayout(DemoWindow);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        frame = new QFrame(DemoWindow);
        frame->setObjectName(QString::fromUtf8("frame"));
        frame->setFrameShape(QFrame::StyledPanel);
        frame->setFrameShadow(QFrame::Raised);
        gridLayout_3 = new QGridLayout(frame);
        gridLayout_3->setObjectName(QString::fromUtf8("gridLayout_3"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        groupBox_2 = new QGroupBox(frame);
        groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
        QFont font;
        font.setPointSize(14);
        font.setBold(true);
        font.setWeight(75);
        groupBox_2->setFont(font);
        gridLayout = new QGridLayout(groupBox_2);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        lineEdit_2 = new QLineEdit(groupBox_2);
        lineEdit_2->setObjectName(QString::fromUtf8("lineEdit_2"));
        QFont font1;
        font1.setPointSize(13);
        font1.setBold(false);
        font1.setWeight(50);
        lineEdit_2->setFont(font1);

        gridLayout->addWidget(lineEdit_2, 2, 0, 1, 2);

        label_3 = new QLabel(groupBox_2);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setMinimumSize(QSize(0, 0));
        label_3->setMaximumSize(QSize(16777215, 20));
        label_3->setFont(font1);

        gridLayout->addWidget(label_3, 3, 0, 1, 2);

        cmbportbaud = new QComboBox(groupBox_2);
        cmbportbaud->addItem(QString());
        cmbportbaud->addItem(QString());
        cmbportbaud->addItem(QString());
        cmbportbaud->addItem(QString());
        cmbportbaud->addItem(QString());
        cmbportbaud->addItem(QString());
        cmbportbaud->setObjectName(QString::fromUtf8("cmbportbaud"));
        cmbportbaud->setFont(font1);

        gridLayout->addWidget(cmbportbaud, 6, 1, 1, 1);

        lineEdit_3 = new QLineEdit(groupBox_2);
        lineEdit_3->setObjectName(QString::fromUtf8("lineEdit_3"));
        lineEdit_3->setFont(font1);

        gridLayout->addWidget(lineEdit_3, 4, 0, 1, 2);

        label_12 = new QLabel(groupBox_2);
        label_12->setObjectName(QString::fromUtf8("label_12"));
        label_12->setFont(font1);

        gridLayout->addWidget(label_12, 6, 0, 1, 1);

        btnrefresh = new QPushButton(groupBox_2);
        btnrefresh->setObjectName(QString::fromUtf8("btnrefresh"));
        btnrefresh->setFont(font1);

        gridLayout->addWidget(btnrefresh, 9, 0, 1, 2);

        label = new QLabel(groupBox_2);
        label->setObjectName(QString::fromUtf8("label"));
        label->setMaximumSize(QSize(16777215, 20));
        label->setFont(font1);

        gridLayout->addWidget(label, 1, 0, 1, 2);

        cBox_commutechoose = new QComboBox(groupBox_2);
        cBox_commutechoose->addItem(QString());
        cBox_commutechoose->addItem(QString());
        cBox_commutechoose->setObjectName(QString::fromUtf8("cBox_commutechoose"));
        cBox_commutechoose->setFont(font1);

        gridLayout->addWidget(cBox_commutechoose, 0, 0, 1, 2);

        label_11 = new QLabel(groupBox_2);
        label_11->setObjectName(QString::fromUtf8("label_11"));
        label_11->setFont(font1);

        gridLayout->addWidget(label_11, 5, 0, 1, 1);

        Button_connect = new QPushButton(groupBox_2);
        Button_connect->setObjectName(QString::fromUtf8("Button_connect"));
        Button_connect->setFont(font1);

        gridLayout->addWidget(Button_connect, 7, 0, 1, 2);

        cmbportname = new QComboBox(groupBox_2);
        cmbportname->setObjectName(QString::fromUtf8("cmbportname"));
        cmbportname->setFont(font1);

        gridLayout->addWidget(cmbportname, 5, 1, 1, 1);

        btnclose = new QPushButton(groupBox_2);
        btnclose->setObjectName(QString::fromUtf8("btnclose"));
        btnclose->setFont(font1);

        gridLayout->addWidget(btnclose, 8, 0, 1, 2);


        verticalLayout->addWidget(groupBox_2);

        groupBox_3 = new QGroupBox(frame);
        groupBox_3->setObjectName(QString::fromUtf8("groupBox_3"));
        QFont font2;
        font2.setPointSize(13);
        font2.setBold(true);
        font2.setWeight(75);
        groupBox_3->setFont(font2);
        gridLayout_2 = new QGridLayout(groupBox_3);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        plainTextEdit = new QPlainTextEdit(groupBox_3);
        plainTextEdit->setObjectName(QString::fromUtf8("plainTextEdit"));

        gridLayout_2->addWidget(plainTextEdit, 0, 0, 1, 2);

        label_4 = new QLabel(groupBox_3);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        QFont font3;
        font3.setPointSize(12);
        font3.setBold(true);
        font3.setWeight(75);
        label_4->setFont(font3);

        gridLayout_2->addWidget(label_4, 1, 0, 1, 1);

        label_led = new QLabel(groupBox_3);
        label_led->setObjectName(QString::fromUtf8("label_led"));

        gridLayout_2->addWidget(label_led, 1, 1, 1, 1);

        btn_diagnosis = new QPushButton(groupBox_3);
        btn_diagnosis->setObjectName(QString::fromUtf8("btn_diagnosis"));
        btn_diagnosis->setMinimumSize(QSize(0, 30));
        btn_diagnosis->setFont(font2);

        gridLayout_2->addWidget(btn_diagnosis, 2, 0, 1, 2);


        verticalLayout->addWidget(groupBox_3);

        verticalLayout->setStretch(0, 50);
        verticalLayout->setStretch(1, 25);

        gridLayout_3->addLayout(verticalLayout, 0, 0, 1, 1);

        verticalLayout_5 = new QVBoxLayout();
        verticalLayout_5->setObjectName(QString::fromUtf8("verticalLayout_5"));
        tabWidget = new QTabWidget(frame);
        tabWidget->setObjectName(QString::fromUtf8("tabWidget"));
        QFont font4;
        font4.setPointSize(13);
        tabWidget->setFont(font4);
        tabWidget->setTabShape(QTabWidget::Triangular);
        tab = new QWidget();
        tab->setObjectName(QString::fromUtf8("tab"));
        gridLayout_9 = new QGridLayout(tab);
        gridLayout_9->setObjectName(QString::fromUtf8("gridLayout_9"));
        widget = new QCustomPlot(tab);
        widget->setObjectName(QString::fromUtf8("widget"));

        gridLayout_9->addWidget(widget, 0, 0, 1, 1);

        tabWidget->addTab(tab, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName(QString::fromUtf8("tab_2"));
        gridLayout_8 = new QGridLayout(tab_2);
        gridLayout_8->setObjectName(QString::fromUtf8("gridLayout_8"));
        tableView = new QTableView(tab_2);
        tableView->setObjectName(QString::fromUtf8("tableView"));

        gridLayout_8->addWidget(tableView, 0, 0, 1, 1);

        tabWidget->addTab(tab_2, QString());
        tab_3 = new QWidget();
        tab_3->setObjectName(QString::fromUtf8("tab_3"));
        verticalLayout_4 = new QVBoxLayout(tab_3);
        verticalLayout_4->setObjectName(QString::fromUtf8("verticalLayout_4"));
        ptereceive = new QPlainTextEdit(tab_3);
        ptereceive->setObjectName(QString::fromUtf8("ptereceive"));

        verticalLayout_4->addWidget(ptereceive);

        ptesend = new QPlainTextEdit(tab_3);
        ptesend->setObjectName(QString::fromUtf8("ptesend"));

        verticalLayout_4->addWidget(ptesend);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalSpacer = new QSpacerItem(79, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        btnclear = new QPushButton(tab_3);
        btnclear->setObjectName(QString::fromUtf8("btnclear"));
        btnclear->setMinimumSize(QSize(0, 35));
        btnclear->setBaseSize(QSize(0, 0));

        horizontalLayout->addWidget(btnclear);

        horizontalSpacer_2 = new QSpacerItem(118, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_2);

        btnsend = new QPushButton(tab_3);
        btnsend->setObjectName(QString::fromUtf8("btnsend"));
        btnsend->setMinimumSize(QSize(0, 35));

        horizontalLayout->addWidget(btnsend);

        horizontalSpacer_3 = new QSpacerItem(210, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_3);

        horizontalLayout->setStretch(0, 10);
        horizontalLayout->setStretch(1, 20);
        horizontalLayout->setStretch(2, 10);
        horizontalLayout->setStretch(3, 20);
        horizontalLayout->setStretch(4, 10);

        verticalLayout_4->addLayout(horizontalLayout);

        verticalLayout_4->setStretch(0, 50);
        verticalLayout_4->setStretch(1, 10);
        verticalLayout_4->setStretch(2, 5);
        tabWidget->addTab(tab_3, QString());

        verticalLayout_5->addWidget(tabWidget);

        groupBox_select = new QGroupBox(frame);
        groupBox_select->setObjectName(QString::fromUtf8("groupBox_select"));
        QFont font5;
        font5.setPointSize(15);
        font5.setBold(true);
        font5.setWeight(75);
        groupBox_select->setFont(font5);
        groupBox_select->setAutoFillBackground(false);
        groupBox_select->setAlignment(Qt::AlignBottom|Qt::AlignLeading|Qt::AlignLeft);
        groupBox_select->setFlat(false);
        groupBox_select->setCheckable(false);
        horizontalLayout_3 = new QHBoxLayout(groupBox_select);
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        label_2 = new QLabel(groupBox_select);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setFont(font1);

        horizontalLayout_3->addWidget(label_2);

        checkBox_1 = new QCheckBox(groupBox_select);
        checkBox_1->setObjectName(QString::fromUtf8("checkBox_1"));
        checkBox_1->setEnabled(true);
        QFont font6;
        font6.setPointSize(12);
        font6.setBold(false);
        font6.setWeight(50);
        checkBox_1->setFont(font6);
        checkBox_1->setChecked(false);
        checkBox_1->setTristate(false);

        horizontalLayout_3->addWidget(checkBox_1);

        checkBox_2 = new QCheckBox(groupBox_select);
        checkBox_2->setObjectName(QString::fromUtf8("checkBox_2"));
        checkBox_2->setFont(font6);
        checkBox_2->setChecked(false);

        horizontalLayout_3->addWidget(checkBox_2);

        checkBox_3 = new QCheckBox(groupBox_select);
        checkBox_3->setObjectName(QString::fromUtf8("checkBox_3"));
        checkBox_3->setFont(font6);
        checkBox_3->setCheckable(true);
        checkBox_3->setChecked(false);

        horizontalLayout_3->addWidget(checkBox_3);

        checkBox_4 = new QCheckBox(groupBox_select);
        checkBox_4->setObjectName(QString::fromUtf8("checkBox_4"));
        checkBox_4->setFont(font6);
        checkBox_4->setChecked(false);

        horizontalLayout_3->addWidget(checkBox_4);

        checkBox_5 = new QCheckBox(groupBox_select);
        checkBox_5->setObjectName(QString::fromUtf8("checkBox_5"));
        checkBox_5->setFont(font6);
        checkBox_5->setChecked(false);

        horizontalLayout_3->addWidget(checkBox_5);

        checkBox_6 = new QCheckBox(groupBox_select);
        checkBox_6->setObjectName(QString::fromUtf8("checkBox_6"));
        checkBox_6->setFont(font6);
        checkBox_6->setChecked(false);

        horizontalLayout_3->addWidget(checkBox_6);

        checkBox_7 = new QCheckBox(groupBox_select);
        checkBox_7->setObjectName(QString::fromUtf8("checkBox_7"));
        checkBox_7->setFont(font6);
        checkBox_7->setChecked(false);

        horizontalLayout_3->addWidget(checkBox_7);

        Button_selectshow = new QPushButton(groupBox_select);
        Button_selectshow->setObjectName(QString::fromUtf8("Button_selectshow"));
        Button_selectshow->setMinimumSize(QSize(0, 35));
        Button_selectshow->setFont(font2);

        horizontalLayout_3->addWidget(Button_selectshow);


        verticalLayout_5->addWidget(groupBox_select);


        gridLayout_3->addLayout(verticalLayout_5, 0, 1, 1, 1);

        verticalLayout_3 = new QVBoxLayout();
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        groupBox = new QGroupBox(frame);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        groupBox->setFont(font2);
        gridLayout_4 = new QGridLayout(groupBox);
        gridLayout_4->setObjectName(QString::fromUtf8("gridLayout_4"));
        Button_reset = new QPushButton(groupBox);
        Button_reset->setObjectName(QString::fromUtf8("Button_reset"));
        Button_reset->setFont(font1);

        gridLayout_4->addWidget(Button_reset, 0, 0, 1, 3);

        comboBox_parameter_set = new QComboBox(groupBox);
        comboBox_parameter_set->addItem(QString());
        comboBox_parameter_set->setObjectName(QString::fromUtf8("comboBox_parameter_set"));
        comboBox_parameter_set->setFont(font1);

        gridLayout_4->addWidget(comboBox_parameter_set, 1, 0, 1, 3);

        label_5 = new QLabel(groupBox);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        label_5->setFont(font1);

        gridLayout_4->addWidget(label_5, 2, 0, 1, 1);

        textEdit_parameter_set = new QTextEdit(groupBox);
        textEdit_parameter_set->setObjectName(QString::fromUtf8("textEdit_parameter_set"));
        textEdit_parameter_set->setMaximumSize(QSize(80, 30));
        textEdit_parameter_set->setSizeIncrement(QSize(0, 0));

        gridLayout_4->addWidget(textEdit_parameter_set, 2, 1, 1, 2);

        label_6 = new QLabel(groupBox);
        label_6->setObjectName(QString::fromUtf8("label_6"));
        label_6->setFont(font1);

        gridLayout_4->addWidget(label_6, 3, 0, 1, 1);

        textEdit_parameter_set2 = new QTextEdit(groupBox);
        textEdit_parameter_set2->setObjectName(QString::fromUtf8("textEdit_parameter_set2"));
        textEdit_parameter_set2->setMaximumSize(QSize(80, 30));

        gridLayout_4->addWidget(textEdit_parameter_set2, 3, 1, 1, 2);

        label_7 = new QLabel(groupBox);
        label_7->setObjectName(QString::fromUtf8("label_7"));
        label_7->setMaximumSize(QSize(16777215, 20));
        label_7->setFont(font1);

        gridLayout_4->addWidget(label_7, 4, 0, 1, 3);

        label_8 = new QLabel(groupBox);
        label_8->setObjectName(QString::fromUtf8("label_8"));
        label_8->setMaximumSize(QSize(16777215, 20));
        label_8->setFont(font1);

        gridLayout_4->addWidget(label_8, 6, 0, 1, 3);

        comboBox_3 = new QComboBox(groupBox);
        comboBox_3->addItem(QString());
        comboBox_3->addItem(QString());
        comboBox_3->addItem(QString());
        comboBox_3->addItem(QString());
        comboBox_3->setObjectName(QString::fromUtf8("comboBox_3"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(comboBox_3->sizePolicy().hasHeightForWidth());
        comboBox_3->setSizePolicy(sizePolicy);

        gridLayout_4->addWidget(comboBox_3, 7, 0, 1, 3);

        label_9 = new QLabel(groupBox);
        label_9->setObjectName(QString::fromUtf8("label_9"));
        label_9->setMaximumSize(QSize(16777215, 20));
        label_9->setFont(font1);

        gridLayout_4->addWidget(label_9, 8, 0, 1, 3);

        textEdit_parameter_set3 = new QTextEdit(groupBox);
        textEdit_parameter_set3->setObjectName(QString::fromUtf8("textEdit_parameter_set3"));
        textEdit_parameter_set3->setMaximumSize(QSize(16777215, 40));

        gridLayout_4->addWidget(textEdit_parameter_set3, 9, 0, 1, 1);

        label_10 = new QLabel(groupBox);
        label_10->setObjectName(QString::fromUtf8("label_10"));
        QFont font7;
        font7.setPointSize(11);
        font7.setBold(false);
        font7.setWeight(50);
        label_10->setFont(font7);

        gridLayout_4->addWidget(label_10, 9, 1, 1, 1);

        textEdit_parameter_set4 = new QTextEdit(groupBox);
        textEdit_parameter_set4->setObjectName(QString::fromUtf8("textEdit_parameter_set4"));
        textEdit_parameter_set4->setMaximumSize(QSize(16777215, 40));

        gridLayout_4->addWidget(textEdit_parameter_set4, 9, 2, 1, 1);

        Button_parameter_set = new QPushButton(groupBox);
        Button_parameter_set->setObjectName(QString::fromUtf8("Button_parameter_set"));
        Button_parameter_set->setFont(font1);

        gridLayout_4->addWidget(Button_parameter_set, 10, 0, 1, 3);

        comboBox = new QComboBox(groupBox);
        comboBox->addItem(QString());
        comboBox->addItem(QString());
        comboBox->addItem(QString());
        comboBox->setObjectName(QString::fromUtf8("comboBox"));
        sizePolicy.setHeightForWidth(comboBox->sizePolicy().hasHeightForWidth());
        comboBox->setSizePolicy(sizePolicy);

        gridLayout_4->addWidget(comboBox, 5, 0, 1, 3);

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
        groupBox_save->setFont(font2);
        groupBox_save->setAutoFillBackground(false);
        verticalLayout_2 = new QVBoxLayout(groupBox_save);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        Button_Save_dynamic = new QPushButton(groupBox_save);
        Button_Save_dynamic->setObjectName(QString::fromUtf8("Button_Save_dynamic"));
        Button_Save_dynamic->setFont(font1);

        verticalLayout_2->addWidget(Button_Save_dynamic);

        Button_Save_Static = new QPushButton(groupBox_save);
        Button_Save_Static->setObjectName(QString::fromUtf8("Button_Save_Static"));
        Button_Save_Static->setFont(font1);

        verticalLayout_2->addWidget(Button_Save_Static);

        Button_save_spectrum = new QPushButton(groupBox_save);
        Button_save_spectrum->setObjectName(QString::fromUtf8("Button_save_spectrum"));
        Button_save_spectrum->setFont(font1);

        verticalLayout_2->addWidget(Button_save_spectrum);


        verticalLayout_3->addWidget(groupBox_save);

        verticalLayout_3->setStretch(0, 30);
        verticalLayout_3->setStretch(1, 10);

        gridLayout_3->addLayout(verticalLayout_3, 0, 2, 1, 1);

        gridLayout_3->setColumnStretch(0, 10);
        gridLayout_3->setColumnStretch(1, 50);
        gridLayout_3->setColumnStretch(2, 15);

        horizontalLayout_2->addWidget(frame);


        retranslateUi(DemoWindow);

        tabWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(DemoWindow);
    } // setupUi

    void retranslateUi(QWidget *DemoWindow)
    {
        DemoWindow->setWindowTitle(QApplication::translate("DemoWindow", "Form", nullptr));
        groupBox_2->setTitle(QApplication::translate("DemoWindow", "\351\200\232\344\277\241\350\256\276\347\275\256", nullptr));
        lineEdit_2->setText(QApplication::translate("DemoWindow", "192.9.100.41", nullptr));
        label_3->setText(QApplication::translate("DemoWindow", "\347\253\257\345\217\243\345\217\267\357\274\232", nullptr));
        cmbportbaud->setItemText(0, QApplication::translate("DemoWindow", "921600", nullptr));
        cmbportbaud->setItemText(1, QApplication::translate("DemoWindow", "115200", nullptr));
        cmbportbaud->setItemText(2, QApplication::translate("DemoWindow", "9600", nullptr));
        cmbportbaud->setItemText(3, QApplication::translate("DemoWindow", "2400", nullptr));
        cmbportbaud->setItemText(4, QApplication::translate("DemoWindow", "4800", nullptr));
        cmbportbaud->setItemText(5, QApplication::translate("DemoWindow", "1200", nullptr));

        cmbportbaud->setCurrentText(QApplication::translate("DemoWindow", "921600", nullptr));
        lineEdit_3->setText(QApplication::translate("DemoWindow", "5060", nullptr));
        label_12->setText(QApplication::translate("DemoWindow", "\346\263\242\347\211\271\347\216\207\357\274\232", nullptr));
        btnrefresh->setText(QApplication::translate("DemoWindow", "\345\210\267\346\226\260\344\270\262\345\217\243", nullptr));
        label->setText(QApplication::translate("DemoWindow", "\344\270\273\346\234\272IP\345\234\260\345\235\200\357\274\232", nullptr));
        cBox_commutechoose->setItemText(0, QApplication::translate("DemoWindow", "UDP\351\200\232\344\277\241", nullptr));
        cBox_commutechoose->setItemText(1, QApplication::translate("DemoWindow", "\344\270\262\345\217\243\351\200\232\344\277\241", nullptr));

        label_11->setText(QApplication::translate("DemoWindow", "\344\270\262\345\217\243\345\220\215\357\274\232", nullptr));
        Button_connect->setText(QApplication::translate("DemoWindow", "\350\277\236\346\216\245\347\275\221\347\273\234", nullptr));
        btnclose->setText(QApplication::translate("DemoWindow", "\346\211\223\345\274\200\344\270\262\345\217\243", nullptr));
        groupBox_3->setTitle(QApplication::translate("DemoWindow", "\347\212\266\346\200\201\346\214\207\347\244\272", nullptr));
        plainTextEdit->setPlainText(QApplication::translate("DemoWindow", "\346\232\202\346\234\252\350\277\236\346\216\245\357\274\201\n"
"", nullptr));
        label_4->setText(QApplication::translate("DemoWindow", "\350\277\220\350\241\214\347\212\266\346\200\201", nullptr));
        label_led->setText(QApplication::translate("DemoWindow", "LED", nullptr));
        btn_diagnosis->setText(QApplication::translate("DemoWindow", "\346\225\205\351\232\234\350\257\212\346\226\255", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab), QApplication::translate("DemoWindow", "\345\205\211\350\260\261", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab_2), QApplication::translate("DemoWindow", "\346\263\242\351\225\277", nullptr));
        btnclear->setText(QApplication::translate("DemoWindow", "\346\270\205\351\231\244\346\216\245\346\224\266", nullptr));
        btnsend->setText(QApplication::translate("DemoWindow", "\347\202\271\345\207\273\345\217\221\351\200\201", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab_3), QApplication::translate("DemoWindow", "\344\270\262\345\217\243", nullptr));
        groupBox_select->setTitle(QApplication::translate("DemoWindow", "\351\200\232\351\201\223\347\255\233\351\200\211\357\274\232", nullptr));
        label_2->setText(QApplication::translate("DemoWindow", "\346\211\200\345\261\236\351\200\232\351\201\223\357\274\232", nullptr));
        checkBox_1->setText(QApplication::translate("DemoWindow", "CH1", nullptr));
        checkBox_2->setText(QApplication::translate("DemoWindow", "CH2", nullptr));
        checkBox_3->setText(QApplication::translate("DemoWindow", "CH3", nullptr));
        checkBox_4->setText(QApplication::translate("DemoWindow", "CH4", nullptr));
        checkBox_5->setText(QApplication::translate("DemoWindow", "CH5", nullptr));
        checkBox_6->setText(QApplication::translate("DemoWindow", "CH6", nullptr));
        checkBox_7->setText(QApplication::translate("DemoWindow", "CH7", nullptr));
        Button_selectshow->setText(QApplication::translate("DemoWindow", "\351\200\232\351\201\223\347\255\233\351\200\211", nullptr));
        groupBox->setTitle(QApplication::translate("DemoWindow", "\345\217\202\346\225\260\350\256\276\347\275\256", nullptr));
        Button_reset->setText(QApplication::translate("DemoWindow", "\345\244\215\344\275\215", nullptr));
        comboBox_parameter_set->setItemText(0, QApplication::translate("DemoWindow", "\345\272\224\345\217\230", nullptr));

        label_5->setText(QApplication::translate("DemoWindow", "\345\217\202\346\225\2601:", nullptr));
        label_6->setText(QApplication::translate("DemoWindow", "\345\217\202\346\225\2602:", nullptr));
        label_7->setText(QApplication::translate("DemoWindow", "\346\225\260\346\215\256\351\207\207\351\233\206\351\242\221\347\216\207(Hz)", nullptr));
        label_8->setText(QApplication::translate("DemoWindow", "\344\277\235\345\255\230\346\227\266\351\227\264\351\227\264\351\232\224\357\274\210ms\357\274\211", nullptr));
        comboBox_3->setItemText(0, QApplication::translate("DemoWindow", "1000", nullptr));
        comboBox_3->setItemText(1, QApplication::translate("DemoWindow", "10", nullptr));
        comboBox_3->setItemText(2, QApplication::translate("DemoWindow", "100", nullptr));
        comboBox_3->setItemText(3, QApplication::translate("DemoWindow", "10000", nullptr));

        label_9->setText(QApplication::translate("DemoWindow", "\345\272\224\345\212\233\351\230\210\345\200\274\357\274\232", nullptr));
        textEdit_parameter_set3->setHtml(QApplication::translate("DemoWindow", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'SimSun'; font-size:13pt; font-weight:600; font-style:normal;\">\n"
"<p align=\"center\" style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><br /></p></body></html>", nullptr));
        label_10->setText(QApplication::translate("DemoWindow", "\350\207\263", nullptr));
        textEdit_parameter_set4->setHtml(QApplication::translate("DemoWindow", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'SimSun'; font-size:13pt; font-weight:600; font-style:normal;\">\n"
"<p align=\"center\" style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><br /></p></body></html>", nullptr));
        Button_parameter_set->setText(QApplication::translate("DemoWindow", "\347\241\256\350\256\244", nullptr));
        comboBox->setItemText(0, QApplication::translate("DemoWindow", "1", nullptr));
        comboBox->setItemText(1, QApplication::translate("DemoWindow", "10", nullptr));
        comboBox->setItemText(2, QApplication::translate("DemoWindow", "100", nullptr));

        groupBox_save->setTitle(QApplication::translate("DemoWindow", "\344\277\235\345\255\230\344\270\216\350\276\223\345\207\272", nullptr));
        Button_Save_dynamic->setText(QApplication::translate("DemoWindow", "\350\207\252\345\212\250\344\277\235\345\255\230", nullptr));
        Button_Save_Static->setText(QApplication::translate("DemoWindow", "\346\211\213\345\212\250\344\277\235\345\255\230", nullptr));
        Button_save_spectrum->setText(QApplication::translate("DemoWindow", "\345\205\211\350\260\261\344\277\235\345\255\230", nullptr));
    } // retranslateUi

};

namespace Ui {
    class DemoWindow: public Ui_DemoWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DEMOWINDOW_H
