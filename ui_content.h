/********************************************************************************
** Form generated from reading UI file 'content.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CONTENT_H
#define UI_CONTENT_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Content
{
public:
    QLabel *timeLabel;
    QLabel *nameLabel;
    QLabel *contentLabel;

    void setupUi(QWidget *Content)
    {
        if (Content->objectName().isEmpty())
            Content->setObjectName(QString::fromUtf8("Content"));
        Content->resize(453, 66);
        timeLabel = new QLabel(Content);
        timeLabel->setObjectName(QString::fromUtf8("timeLabel"));
        timeLabel->setGeometry(QRect(180, 0, 72, 15));
        nameLabel = new QLabel(Content);
        nameLabel->setObjectName(QString::fromUtf8("nameLabel"));
        nameLabel->setGeometry(QRect(10, 30, 81, 16));
        contentLabel = new QLabel(Content);
        contentLabel->setObjectName(QString::fromUtf8("contentLabel"));
        contentLabel->setGeometry(QRect(100, 30, 311, 21));

        retranslateUi(Content);

        QMetaObject::connectSlotsByName(Content);
    } // setupUi

    void retranslateUi(QWidget *Content)
    {
        Content->setWindowTitle(QCoreApplication::translate("Content", "Form", nullptr));
        timeLabel->setText(QCoreApplication::translate("Content", "TextLabel", nullptr));
        nameLabel->setText(QCoreApplication::translate("Content", "TextLabel:", nullptr));
        contentLabel->setText(QCoreApplication::translate("Content", "TextLabel", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Content: public Ui_Content {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CONTENT_H
