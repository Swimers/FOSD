/********************************************************************************
** Form generated from reading UI file 'logwidget.ui'
**
** Created by: Qt User Interface Compiler version 5.12.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_LOGWIDGET_H
#define UI_LOGWIDGET_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QFrame>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_LogWidget
{
public:
    QFrame *frame;
    QPushButton *btn_log;
    QLineEdit *edit_pw;
    QLabel *label_title;
    QCheckBox *check_pw;
    QLabel *label;
    QLabel *label_2;
    QComboBox *comboBox;
    QToolButton *toolButton;

    void setupUi(QWidget *LogWidget)
    {
        if (LogWidget->objectName().isEmpty())
            LogWidget->setObjectName(QString::fromUtf8("LogWidget"));
        LogWidget->setWindowModality(Qt::NonModal);
        LogWidget->resize(722, 571);
        LogWidget->setLayoutDirection(Qt::LeftToRight);
        LogWidget->setStyleSheet(QString::fromUtf8("*\n"
"{\n"
" font-size:24px;\n"
" font-family:Century Gothic;\n"
"}\n"
"#LogWidget{\n"
"background:url(:/resource/resource_log/1.png)\n"
"}\n"
"\n"
"QFrame\n"
"{\n"
"background:rgba(0,0,0,0.8);\n"
"border-radius:15px;\n"
"}\n"
"#btn_log\n"
"{\n"
"background:red;\n"
"color:white;\n"
"border-radius:15px;\n"
"}\n"
"#btn_log:hover{\n"
"color:#333;\n"
"border-radius:15px;\n"
"background:#49ebff;\n"
"}\n"
"QToolButton\n"
"{\n"
"background:red;\n"
"border-radius:60px;\n"
"\n"
"}\n"
"QLineEdit{\n"
"background:transparent;\n"
"border:none;\n"
"color:#717072;\n"
"border-bottom:1px solid #717072;\n"
"}\n"
"QLabel\n"
"{\n"
" color:#fff;\n"
" background:transparent;\n"
" font-size:30px;\n"
" font-style:MingLiU-ExtB;\n"
"}\n"
"QCheckBox{\n"
"font-size:17px;\n"
"background:rgba(85,170,255,0);\n"
"color:grey;\n"
"font-style:MingLiU-ExtB;\n"
"}\n"
"QComboBox{\n"
"background:transparent;\n"
"border:none;\n"
"color:#717072;\n"
"border-bottom:1px solid #717072;\n"
"}\n"
"QComboBox QAbstractItemView\n"
"{\n"
"color:#717072;\n"
"bo"
                        "rder: 1px solid white;\n"
"}\n"
"QMessageBox\n"
"{\n"
"background-color:white;\n"
"}\n"
"QMessageBox QPushButton\n"
"{\n"
"background-color:rgb(197, 199, 200);\n"
"\n"
"}\n"
"QMessageBox QLabel\n"
"{\n"
"color:black;\n"
"}"));
        frame = new QFrame(LogWidget);
        frame->setObjectName(QString::fromUtf8("frame"));
        frame->setGeometry(QRect(130, 90, 471, 421));
        frame->setFrameShape(QFrame::StyledPanel);
        frame->setFrameShadow(QFrame::Raised);
        btn_log = new QPushButton(frame);
        btn_log->setObjectName(QString::fromUtf8("btn_log"));
        btn_log->setGeometry(QRect(30, 360, 421, 51));
        edit_pw = new QLineEdit(frame);
        edit_pw->setObjectName(QString::fromUtf8("edit_pw"));
        edit_pw->setGeometry(QRect(30, 270, 421, 41));
        edit_pw->setEchoMode(QLineEdit::Password);
        label_title = new QLabel(frame);
        label_title->setObjectName(QString::fromUtf8("label_title"));
        label_title->setGeometry(QRect(190, 60, 81, 41));
        check_pw = new QCheckBox(frame);
        check_pw->setObjectName(QString::fromUtf8("check_pw"));
        check_pw->setGeometry(QRect(30, 320, 201, 31));
        check_pw->setTristate(false);
        label = new QLabel(frame);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(30, 150, 191, 31));
        label_2 = new QLabel(frame);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(30, 240, 151, 31));
        comboBox = new QComboBox(frame);
        comboBox->setObjectName(QString::fromUtf8("comboBox"));
        comboBox->setGeometry(QRect(30, 190, 421, 41));
        toolButton = new QToolButton(LogWidget);
        toolButton->setObjectName(QString::fromUtf8("toolButton"));
        toolButton->setGeometry(QRect(300, 30, 121, 121));
        toolButton->setLayoutDirection(Qt::LeftToRight);
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/resource/resource_log/2.png"), QSize(), QIcon::Normal, QIcon::Off);
        toolButton->setIcon(icon);
        toolButton->setIconSize(QSize(50, 50));
        toolButton->setPopupMode(QToolButton::DelayedPopup);
        toolButton->setToolButtonStyle(Qt::ToolButtonIconOnly);
        toolButton->setArrowType(Qt::NoArrow);
        QWidget::setTabOrder(edit_pw, check_pw);
        QWidget::setTabOrder(check_pw, btn_log);
        QWidget::setTabOrder(btn_log, toolButton);

        retranslateUi(LogWidget);

        QMetaObject::connectSlotsByName(LogWidget);
    } // setupUi

    void retranslateUi(QWidget *LogWidget)
    {
        LogWidget->setWindowTitle(QApplication::translate("LogWidget", "Form", nullptr));
        btn_log->setText(QApplication::translate("LogWidget", "\347\231\273\345\275\225", nullptr));
        edit_pw->setText(QString());
        edit_pw->setPlaceholderText(QApplication::translate("LogWidget", "\345\257\206\347\240\201", nullptr));
        label_title->setText(QApplication::translate("LogWidget", "\347\231\273\345\275\225", nullptr));
        check_pw->setText(QApplication::translate("LogWidget", "\350\256\260\344\275\217\345\257\206\347\240\201", nullptr));
        label->setText(QApplication::translate("LogWidget", "\347\224\250\346\210\267\345\220\215", nullptr));
        label_2->setText(QApplication::translate("LogWidget", "\345\257\206\347\240\201", nullptr));
        toolButton->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class LogWidget: public Ui_LogWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LOGWIDGET_H
