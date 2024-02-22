/********************************************************************************
** Form generated from reading UI file 'verifymsg.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_VERIFYMSG_H
#define UI_VERIFYMSG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_VerifyMsg
{
public:
    QTabWidget *tabWidget;
    QWidget *tab_3;
    QListWidget *listWidget;
    QWidget *tab_4;
    QLabel *label;

    void setupUi(QWidget *VerifyMsg)
    {
        if (VerifyMsg->objectName().isEmpty())
            VerifyMsg->setObjectName(QString::fromUtf8("VerifyMsg"));
        VerifyMsg->resize(554, 593);
        tabWidget = new QTabWidget(VerifyMsg);
        tabWidget->setObjectName(QString::fromUtf8("tabWidget"));
        tabWidget->setGeometry(QRect(0, 40, 561, 491));
        tabWidget->setIconSize(QSize(32, 32));
        tab_3 = new QWidget();
        tab_3->setObjectName(QString::fromUtf8("tab_3"));
        listWidget = new QListWidget(tab_3);
        listWidget->setObjectName(QString::fromUtf8("listWidget"));
        listWidget->setGeometry(QRect(0, 0, 551, 471));
        tabWidget->addTab(tab_3, QString());
        tab_4 = new QWidget();
        tab_4->setObjectName(QString::fromUtf8("tab_4"));
        tabWidget->addTab(tab_4, QString());
        label = new QLabel(VerifyMsg);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(20, 0, 72, 15));

        retranslateUi(VerifyMsg);

        tabWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(VerifyMsg);
    } // setupUi

    void retranslateUi(QWidget *VerifyMsg)
    {
        VerifyMsg->setWindowTitle(QCoreApplication::translate("VerifyMsg", "Form", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab_3), QCoreApplication::translate("VerifyMsg", "\345\245\275\345\217\213\351\252\214\350\257\201", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab_4), QCoreApplication::translate("VerifyMsg", "\347\276\244\347\263\273\347\273\237\346\266\210\346\201\257", nullptr));
        label->setText(QCoreApplication::translate("VerifyMsg", "\351\252\214\350\257\201\346\266\210\346\201\257", nullptr));
    } // retranslateUi

};

namespace Ui {
    class VerifyMsg: public Ui_VerifyMsg {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_VERIFYMSG_H
