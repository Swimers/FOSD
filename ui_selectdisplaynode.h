/********************************************************************************
** Form generated from reading UI file 'selectdisplaynode.ui'
**
** Created by: Qt User Interface Compiler version 5.12.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SELECTDISPLAYNODE_H
#define UI_SELECTDISPLAYNODE_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_SelectDisplayNode
{
public:
    QWidget *widget;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label;
    QComboBox *comboBox;
    QSpacerItem *horizontalSpacer_4;
    QLineEdit *editNum;
    QPushButton *btnFind;
    QHBoxLayout *horizontalLayout;
    QLabel *label_2;
    QLineEdit *x_node;
    QLineEdit *y_node;
    QLineEdit *z_node;
    QHBoxLayout *horizontalLayout_3;
    QSpacerItem *horizontalSpacer_2;
    QPushButton *ok;
    QSpacerItem *horizontalSpacer;
    QPushButton *cancel;
    QSpacerItem *horizontalSpacer_3;

    void setupUi(QDialog *SelectDisplayNode)
    {
        if (SelectDisplayNode->objectName().isEmpty())
            SelectDisplayNode->setObjectName(QString::fromUtf8("SelectDisplayNode"));
        SelectDisplayNode->resize(486, 231);
        widget = new QWidget(SelectDisplayNode);
        widget->setObjectName(QString::fromUtf8("widget"));
        widget->setGeometry(QRect(9, 9, 471, 211));
        verticalLayout = new QVBoxLayout(widget);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        label = new QLabel(widget);
        label->setObjectName(QString::fromUtf8("label"));
        QFont font;
        font.setBold(true);
        font.setWeight(75);
        label->setFont(font);

        horizontalLayout_2->addWidget(label);

        comboBox = new QComboBox(widget);
        comboBox->setObjectName(QString::fromUtf8("comboBox"));
        comboBox->setMinimumSize(QSize(0, 30));

        horizontalLayout_2->addWidget(comboBox);

        horizontalSpacer_4 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_4);

        editNum = new QLineEdit(widget);
        editNum->setObjectName(QString::fromUtf8("editNum"));
        editNum->setMinimumSize(QSize(0, 30));

        horizontalLayout_2->addWidget(editNum);

        btnFind = new QPushButton(widget);
        btnFind->setObjectName(QString::fromUtf8("btnFind"));
        btnFind->setMinimumSize(QSize(0, 30));

        horizontalLayout_2->addWidget(btnFind);

        horizontalLayout_2->setStretch(0, 10);
        horizontalLayout_2->setStretch(1, 10);
        horizontalLayout_2->setStretch(3, 10);
        horizontalLayout_2->setStretch(4, 10);

        verticalLayout->addLayout(horizontalLayout_2);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        label_2 = new QLabel(widget);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setFont(font);

        horizontalLayout->addWidget(label_2);

        x_node = new QLineEdit(widget);
        x_node->setObjectName(QString::fromUtf8("x_node"));
        x_node->setMinimumSize(QSize(0, 30));

        horizontalLayout->addWidget(x_node);

        y_node = new QLineEdit(widget);
        y_node->setObjectName(QString::fromUtf8("y_node"));
        y_node->setMinimumSize(QSize(0, 30));

        horizontalLayout->addWidget(y_node);

        z_node = new QLineEdit(widget);
        z_node->setObjectName(QString::fromUtf8("z_node"));
        z_node->setMinimumSize(QSize(0, 30));

        horizontalLayout->addWidget(z_node);


        verticalLayout->addLayout(horizontalLayout);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_2);

        ok = new QPushButton(widget);
        ok->setObjectName(QString::fromUtf8("ok"));
        ok->setMinimumSize(QSize(0, 30));

        horizontalLayout_3->addWidget(ok);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer);

        cancel = new QPushButton(widget);
        cancel->setObjectName(QString::fromUtf8("cancel"));
        cancel->setMinimumSize(QSize(0, 30));

        horizontalLayout_3->addWidget(cancel);

        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_3);

        horizontalLayout_3->setStretch(0, 20);
        horizontalLayout_3->setStretch(1, 8);
        horizontalLayout_3->setStretch(2, 8);
        horizontalLayout_3->setStretch(3, 8);
        horizontalLayout_3->setStretch(4, 8);

        verticalLayout->addLayout(horizontalLayout_3);


        retranslateUi(SelectDisplayNode);

        QMetaObject::connectSlotsByName(SelectDisplayNode);
    } // setupUi

    void retranslateUi(QDialog *SelectDisplayNode)
    {
        SelectDisplayNode->setWindowTitle(QApplication::translate("SelectDisplayNode", "Dialog", nullptr));
        label->setText(QApplication::translate("SelectDisplayNode", "\347\273\223\347\202\271\347\274\226\345\217\267:", nullptr));
        btnFind->setText(QApplication::translate("SelectDisplayNode", "\346\237\245\350\257\242", nullptr));
        label_2->setText(QApplication::translate("SelectDisplayNode", "\347\273\223\347\202\271\345\235\220\346\240\207:", nullptr));
        ok->setText(QApplication::translate("SelectDisplayNode", "\347\241\256\345\256\232", nullptr));
        cancel->setText(QApplication::translate("SelectDisplayNode", "\345\217\226\346\266\210", nullptr));
    } // retranslateUi

};

namespace Ui {
    class SelectDisplayNode: public Ui_SelectDisplayNode {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SELECTDISPLAYNODE_H
