/********************************************************************************
** Form generated from reading UI file 'frienditem.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FRIENDITEM_H
#define UI_FRIENDITEM_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_friendItem
{
public:
    QLabel *userIcon;
    QLabel *userIdLable;
    QLabel *statusLabel;

    void setupUi(QWidget *friendItem)
    {
        if (friendItem->objectName().isEmpty())
            friendItem->setObjectName(QString::fromUtf8("friendItem"));
        friendItem->resize(283, 34);
        userIcon = new QLabel(friendItem);
        userIcon->setObjectName(QString::fromUtf8("userIcon"));
        userIcon->setGeometry(QRect(0, 0, 31, 31));
        userIcon->setPixmap(QPixmap(QString::fromUtf8(":/userIcon.png")));
        userIcon->setScaledContents(true);
        userIdLable = new QLabel(friendItem);
        userIdLable->setObjectName(QString::fromUtf8("userIdLable"));
        userIdLable->setGeometry(QRect(40, 6, 121, 21));
        statusLabel = new QLabel(friendItem);
        statusLabel->setObjectName(QString::fromUtf8("statusLabel"));
        statusLabel->setGeometry(QRect(240, 9, 31, 16));

        retranslateUi(friendItem);

        QMetaObject::connectSlotsByName(friendItem);
    } // setupUi

    void retranslateUi(QWidget *friendItem)
    {
        friendItem->setWindowTitle(QCoreApplication::translate("friendItem", "Form", nullptr));
        userIcon->setText(QString());
        userIdLable->setText(QCoreApplication::translate("friendItem", "<html><head/><body><p><span style=\" font-size:10pt;\">TextLabel</span></p></body></html>", nullptr));
        statusLabel->setText(QCoreApplication::translate("friendItem", "<html><head/><body><p><span style=\" color:#ff0000;\">\347\246\273\347\272\277</span></p></body></html>", nullptr));
    } // retranslateUi

};

namespace Ui {
    class friendItem: public Ui_friendItem {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FRIENDITEM_H
