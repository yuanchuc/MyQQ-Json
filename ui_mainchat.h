/********************************************************************************
** Form generated from reading UI file 'mainchat.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINCHAT_H
#define UI_MAINCHAT_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainChat
{
public:
    QLabel *userIdLabel;
    QWidget *layoutWidget;
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QLabel *statusLabel;
    QTabWidget *tabWidget;
    QWidget *tab_1;
    QListWidget *listWidget1;
    QWidget *tab_2;
    QListWidget *listWidget1_2;
    QWidget *tab_3;
    QWidget *layoutWidget1;
    QHBoxLayout *horizontalLayout_2;
    QPushButton *insertFriendButton;
    QSpacerItem *horizontalSpacer;
    QPushButton *joinGroupButton;
    QSpacerItem *horizontalSpacer_2;
    QPushButton *createGroupButton;
    QPushButton *verifyMsgButton;
    QLabel *userNameLabel;

    void setupUi(QWidget *MainChat)
    {
        if (MainChat->objectName().isEmpty())
            MainChat->setObjectName(QString::fromUtf8("MainChat"));
        MainChat->resize(322, 694);
        userIdLabel = new QLabel(MainChat);
        userIdLabel->setObjectName(QString::fromUtf8("userIdLabel"));
        userIdLabel->setGeometry(QRect(10, 10, 121, 21));
        layoutWidget = new QWidget(MainChat);
        layoutWidget->setObjectName(QString::fromUtf8("layoutWidget"));
        layoutWidget->setGeometry(QRect(0, 670, 159, 19));
        horizontalLayout = new QHBoxLayout(layoutWidget);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        label = new QLabel(layoutWidget);
        label->setObjectName(QString::fromUtf8("label"));

        horizontalLayout->addWidget(label);

        statusLabel = new QLabel(layoutWidget);
        statusLabel->setObjectName(QString::fromUtf8("statusLabel"));

        horizontalLayout->addWidget(statusLabel);

        tabWidget = new QTabWidget(MainChat);
        tabWidget->setObjectName(QString::fromUtf8("tabWidget"));
        tabWidget->setGeometry(QRect(0, 109, 331, 441));
        tabWidget->setTabPosition(QTabWidget::North);
        tabWidget->setElideMode(Qt::ElideNone);
        tabWidget->setDocumentMode(false);
        tabWidget->setTabsClosable(false);
        tabWidget->setMovable(true);
        tabWidget->setTabBarAutoHide(false);
        tab_1 = new QWidget();
        tab_1->setObjectName(QString::fromUtf8("tab_1"));
        listWidget1 = new QListWidget(tab_1);
        listWidget1->setObjectName(QString::fromUtf8("listWidget1"));
        listWidget1->setGeometry(QRect(0, 0, 321, 421));
        tabWidget->addTab(tab_1, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName(QString::fromUtf8("tab_2"));
        listWidget1_2 = new QListWidget(tab_2);
        listWidget1_2->setObjectName(QString::fromUtf8("listWidget1_2"));
        listWidget1_2->setGeometry(QRect(0, 0, 301, 401));
        tabWidget->addTab(tab_2, QString());
        tab_3 = new QWidget();
        tab_3->setObjectName(QString::fromUtf8("tab_3"));
        tabWidget->addTab(tab_3, QString());
        layoutWidget1 = new QWidget(MainChat);
        layoutWidget1->setObjectName(QString::fromUtf8("layoutWidget1"));
        layoutWidget1->setGeometry(QRect(0, 630, 309, 30));
        horizontalLayout_2 = new QHBoxLayout(layoutWidget1);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        horizontalLayout_2->setContentsMargins(0, 0, 0, 0);
        insertFriendButton = new QPushButton(layoutWidget1);
        insertFriendButton->setObjectName(QString::fromUtf8("insertFriendButton"));

        horizontalLayout_2->addWidget(insertFriendButton);

        horizontalSpacer = new QSpacerItem(18, 18, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer);

        joinGroupButton = new QPushButton(layoutWidget1);
        joinGroupButton->setObjectName(QString::fromUtf8("joinGroupButton"));

        horizontalLayout_2->addWidget(joinGroupButton);

        horizontalSpacer_2 = new QSpacerItem(18, 18, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_2);

        createGroupButton = new QPushButton(layoutWidget1);
        createGroupButton->setObjectName(QString::fromUtf8("createGroupButton"));

        horizontalLayout_2->addWidget(createGroupButton);

        verifyMsgButton = new QPushButton(MainChat);
        verifyMsgButton->setObjectName(QString::fromUtf8("verifyMsgButton"));
        verifyMsgButton->setGeometry(QRect(0, 590, 93, 28));
        userNameLabel = new QLabel(MainChat);
        userNameLabel->setObjectName(QString::fromUtf8("userNameLabel"));
        userNameLabel->setGeometry(QRect(10, 30, 121, 21));

        retranslateUi(MainChat);

        tabWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(MainChat);
    } // setupUi

    void retranslateUi(QWidget *MainChat)
    {
        MainChat->setWindowTitle(QCoreApplication::translate("MainChat", "Form", nullptr));
        userIdLabel->setText(QCoreApplication::translate("MainChat", "<html><head/><body><p><span style=\" font-size:12pt;\">TextLabel</span></p></body></html>", nullptr));
        label->setText(QCoreApplication::translate("MainChat", "\345\275\223\345\211\215\346\234\215\345\212\241\345\231\250\347\212\266\346\200\201\357\274\232", nullptr));
        statusLabel->setText(QCoreApplication::translate("MainChat", "<html><head/><body><p><span style=\" color:#00ff00;\">\345\234\250\347\272\277</span></p></body></html>", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab_1), QCoreApplication::translate("MainChat", "\345\245\275\345\217\213", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab_2), QCoreApplication::translate("MainChat", "\347\276\244\347\273\204", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab_3), QCoreApplication::translate("MainChat", "\347\251\272\351\227\264", nullptr));
        insertFriendButton->setText(QCoreApplication::translate("MainChat", "\346\267\273\345\212\240\345\245\275\345\217\213", nullptr));
        joinGroupButton->setText(QCoreApplication::translate("MainChat", "\345\212\240\345\205\245\347\276\244\350\201\212", nullptr));
        createGroupButton->setText(QCoreApplication::translate("MainChat", "\345\210\233\345\273\272\347\276\244\350\201\212", nullptr));
        verifyMsgButton->setText(QCoreApplication::translate("MainChat", "\351\252\214\350\257\201\346\266\210\346\201\257", nullptr));
        userNameLabel->setText(QCoreApplication::translate("MainChat", "<html><head/><body><p><span style=\" font-size:12pt;\">TextLabel</span></p></body></html>", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainChat: public Ui_MainChat {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINCHAT_H
