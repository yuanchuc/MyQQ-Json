/********************************************************************************
** Form generated from reading UI file 'friendmaking.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FRIENDMAKING_H
#define UI_FRIENDMAKING_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_FriendMaking
{
public:
    QLineEdit *newFriendId;
    QPushButton *insertButton;
    QLabel *label;
    QLabel *label_2;
    QTextEdit *extraMsgEdit;
    QLabel *label_3;

    void setupUi(QWidget *FriendMaking)
    {
        if (FriendMaking->objectName().isEmpty())
            FriendMaking->setObjectName(QString::fromUtf8("FriendMaking"));
        FriendMaking->resize(287, 315);
        newFriendId = new QLineEdit(FriendMaking);
        newFriendId->setObjectName(QString::fromUtf8("newFriendId"));
        newFriendId->setGeometry(QRect(69, 70, 161, 31));
        insertButton = new QPushButton(FriendMaking);
        insertButton->setObjectName(QString::fromUtf8("insertButton"));
        insertButton->setGeometry(QRect(100, 250, 71, 41));
        label = new QLabel(FriendMaking);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(100, 10, 101, 41));
        label_2 = new QLabel(FriendMaking);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(27, 75, 31, 21));
        extraMsgEdit = new QTextEdit(FriendMaking);
        extraMsgEdit->setObjectName(QString::fromUtf8("extraMsgEdit"));
        extraMsgEdit->setGeometry(QRect(70, 130, 161, 101));
        label_3 = new QLabel(FriendMaking);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setGeometry(QRect(0, 130, 72, 15));

        retranslateUi(FriendMaking);

        QMetaObject::connectSlotsByName(FriendMaking);
    } // setupUi

    void retranslateUi(QWidget *FriendMaking)
    {
        FriendMaking->setWindowTitle(QCoreApplication::translate("FriendMaking", "Form", nullptr));
        insertButton->setText(QCoreApplication::translate("FriendMaking", "\346\267\273\345\212\240", nullptr));
        label->setText(QCoreApplication::translate("FriendMaking", "<html><head/><body><p><span style=\" font-size:14pt;\">\346\267\273\345\212\240\345\245\275\345\217\213</span></p></body></html>", nullptr));
        label_2->setText(QCoreApplication::translate("FriendMaking", "<html><head/><body><p><span style=\" font-size:10pt;\">\350\264\246\345\217\267</span></p></body></html>", nullptr));
        label_3->setText(QCoreApplication::translate("FriendMaking", "\351\231\204\345\212\240\344\277\241\346\201\257", nullptr));
    } // retranslateUi

};

namespace Ui {
    class FriendMaking: public Ui_FriendMaking {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FRIENDMAKING_H
