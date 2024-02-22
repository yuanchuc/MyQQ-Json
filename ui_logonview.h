/********************************************************************************
** Form generated from reading UI file 'logonview.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_LOGONVIEW_H
#define UI_LOGONVIEW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_logonView
{
public:
    QLabel *label;
    QLineEdit *nameEdit;
    QLineEdit *pwdEdit;
    QLabel *label_2;
    QLabel *label_3;
    QLineEdit *pwdSureEdit;
    QLabel *label_4;
    QPushButton *pushButton;
    QLineEdit *phoneEdit;
    QLabel *label_5;

    void setupUi(QWidget *logonView)
    {
        if (logonView->objectName().isEmpty())
            logonView->setObjectName(QString::fromUtf8("logonView"));
        logonView->resize(433, 320);
        label = new QLabel(logonView);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(150, 10, 111, 31));
        nameEdit = new QLineEdit(logonView);
        nameEdit->setObjectName(QString::fromUtf8("nameEdit"));
        nameEdit->setGeometry(QRect(150, 66, 131, 31));
        pwdEdit = new QLineEdit(logonView);
        pwdEdit->setObjectName(QString::fromUtf8("pwdEdit"));
        pwdEdit->setGeometry(QRect(151, 156, 131, 31));
        pwdEdit->setEchoMode(QLineEdit::Password);
        label_2 = new QLabel(logonView);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(110, 70, 31, 21));
        label_3 = new QLabel(logonView);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setGeometry(QRect(110, 160, 31, 21));
        pwdSureEdit = new QLineEdit(logonView);
        pwdSureEdit->setObjectName(QString::fromUtf8("pwdSureEdit"));
        pwdSureEdit->setGeometry(QRect(150, 200, 131, 31));
        pwdSureEdit->setEchoMode(QLineEdit::Password);
        label_4 = new QLabel(logonView);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setGeometry(QRect(81, 207, 61, 16));
        pushButton = new QPushButton(logonView);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        pushButton->setGeometry(QRect(150, 270, 121, 31));
        phoneEdit = new QLineEdit(logonView);
        phoneEdit->setObjectName(QString::fromUtf8("phoneEdit"));
        phoneEdit->setGeometry(QRect(152, 111, 131, 31));
        label_5 = new QLabel(logonView);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        label_5->setGeometry(QRect(92, 116, 51, 20));

        retranslateUi(logonView);

        QMetaObject::connectSlotsByName(logonView);
    } // setupUi

    void retranslateUi(QWidget *logonView)
    {
        logonView->setWindowTitle(QCoreApplication::translate("logonView", "Form", nullptr));
        label->setText(QCoreApplication::translate("logonView", "<html><head/><body><p><span style=\" font-size:16pt;\">\346\263\250\345\206\214\350\264\246\345\217\267</span></p></body></html>", nullptr));
        label_2->setText(QCoreApplication::translate("logonView", "\346\230\265\347\247\260", nullptr));
        label_3->setText(QCoreApplication::translate("logonView", "\345\257\206\347\240\201", nullptr));
        label_4->setText(QCoreApplication::translate("logonView", "\347\241\256\350\256\244\345\257\206\347\240\201", nullptr));
        pushButton->setText(QCoreApplication::translate("logonView", "\347\241\256\350\256\244\346\263\250\345\206\214", nullptr));
        label_5->setText(QCoreApplication::translate("logonView", "\346\211\213\346\234\272\345\217\267", nullptr));
    } // retranslateUi

};

namespace Ui {
    class logonView: public Ui_logonView {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LOGONVIEW_H
