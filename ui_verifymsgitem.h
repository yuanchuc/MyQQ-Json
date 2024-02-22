/********************************************************************************
** Form generated from reading UI file 'verifymsgitem.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_VERIFYMSGITEM_H
#define UI_VERIFYMSGITEM_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_VerifyMsgItem
{
public:
    QLabel *friendIdLabel;
    QLabel *Label;
    QLabel *extraMsgLable;
    QLabel *addTimeLable;
    QPushButton *agreeButton;
    QPushButton *refuseButton;
    QLabel *statusLabel;

    void setupUi(QWidget *VerifyMsgItem)
    {
        if (VerifyMsgItem->objectName().isEmpty())
            VerifyMsgItem->setObjectName(QString::fromUtf8("VerifyMsgItem"));
        VerifyMsgItem->resize(561, 100);
        friendIdLabel = new QLabel(VerifyMsgItem);
        friendIdLabel->setObjectName(QString::fromUtf8("friendIdLabel"));
        friendIdLabel->setGeometry(QRect(4, 3, 111, 21));
        Label = new QLabel(VerifyMsgItem);
        Label->setObjectName(QString::fromUtf8("Label"));
        Label->setGeometry(QRect(16, 58, 81, 21));
        extraMsgLable = new QLabel(VerifyMsgItem);
        extraMsgLable->setObjectName(QString::fromUtf8("extraMsgLable"));
        extraMsgLable->setGeometry(QRect(89, 58, 321, 21));
        addTimeLable = new QLabel(VerifyMsgItem);
        addTimeLable->setObjectName(QString::fromUtf8("addTimeLable"));
        addTimeLable->setGeometry(QRect(160, 5, 151, 16));
        agreeButton = new QPushButton(VerifyMsgItem);
        agreeButton->setObjectName(QString::fromUtf8("agreeButton"));
        agreeButton->setGeometry(QRect(440, 20, 93, 28));
        refuseButton = new QPushButton(VerifyMsgItem);
        refuseButton->setObjectName(QString::fromUtf8("refuseButton"));
        refuseButton->setGeometry(QRect(440, 50, 93, 28));
        statusLabel = new QLabel(VerifyMsgItem);
        statusLabel->setObjectName(QString::fromUtf8("statusLabel"));
        statusLabel->setGeometry(QRect(464, 37, 61, 21));

        retranslateUi(VerifyMsgItem);

        QMetaObject::connectSlotsByName(VerifyMsgItem);
    } // setupUi

    void retranslateUi(QWidget *VerifyMsgItem)
    {
        VerifyMsgItem->setWindowTitle(QCoreApplication::translate("VerifyMsgItem", "Form", nullptr));
        friendIdLabel->setText(QCoreApplication::translate("VerifyMsgItem", "<html><head/><body><p><br/></p></body></html>", nullptr));
        Label->setText(QCoreApplication::translate("VerifyMsgItem", "<html><head/><body><p>\351\231\204\345\212\240\346\266\210\346\201\257\357\274\232</p></body></html>", nullptr));
        extraMsgLable->setText(QCoreApplication::translate("VerifyMsgItem", "<html><head/><body><p><br/></p><p><br/></p></body></html>", nullptr));
        addTimeLable->setText(QString());
        agreeButton->setText(QCoreApplication::translate("VerifyMsgItem", "\345\220\214\346\204\217", nullptr));
        refuseButton->setText(QCoreApplication::translate("VerifyMsgItem", "\346\213\222\347\273\235", nullptr));
        statusLabel->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class VerifyMsgItem: public Ui_VerifyMsgItem {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_VERIFYMSGITEM_H
