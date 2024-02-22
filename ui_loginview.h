/********************************************************************************
** Form generated from reading UI file 'loginview.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_LOGINVIEW_H
#define UI_LOGINVIEW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_LoginView
{
public:
    QWidget *centralWidget;
    QFrame *loginBackgroundFrame;
    QWidget *loginViewWidget;
    QPushButton *loginButton;
    QLineEdit *loginPwdInput;
    QLabel *label_4;
    QComboBox *loginIdInput;
    QLabel *label_3;
    QCheckBox *autoLoginCheck;
    QCheckBox *remPwdCheck;
    QPushButton *logonButton;
    QLabel *title;
    QWidget *layoutWidget;
    QHBoxLayout *horizontalLayout;
    QLabel *label_2;
    QLabel *statusLable;
    QLabel *backgroud;
    QToolButton *minButton;
    QToolButton *closeButton;
    QPushButton *cancelLoginButton;

    void setupUi(QMainWindow *LoginView)
    {
        if (LoginView->objectName().isEmpty())
            LoginView->setObjectName(QString::fromUtf8("LoginView"));
        LoginView->resize(558, 441);
        centralWidget = new QWidget(LoginView);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        loginBackgroundFrame = new QFrame(centralWidget);
        loginBackgroundFrame->setObjectName(QString::fromUtf8("loginBackgroundFrame"));
        loginBackgroundFrame->setGeometry(QRect(0, 0, 561, 441));
        loginBackgroundFrame->setStyleSheet(QString::fromUtf8(""));
        loginBackgroundFrame->setFrameShape(QFrame::StyledPanel);
        loginBackgroundFrame->setFrameShadow(QFrame::Raised);
        loginViewWidget = new QWidget(loginBackgroundFrame);
        loginViewWidget->setObjectName(QString::fromUtf8("loginViewWidget"));
        loginViewWidget->setGeometry(QRect(0, 160, 561, 291));
        loginViewWidget->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 255);"));
        loginButton = new QPushButton(loginViewWidget);
        loginButton->setObjectName(QString::fromUtf8("loginButton"));
        loginButton->setGeometry(QRect(170, 200, 241, 41));
        loginPwdInput = new QLineEdit(loginViewWidget);
        loginPwdInput->setObjectName(QString::fromUtf8("loginPwdInput"));
        loginPwdInput->setGeometry(QRect(170, 110, 241, 41));
        loginPwdInput->setEchoMode(QLineEdit::Password);
        loginPwdInput->setClearButtonEnabled(true);
        label_4 = new QLabel(loginViewWidget);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setGeometry(QRect(120, 120, 31, 16));
        loginIdInput = new QComboBox(loginViewWidget);
        loginIdInput->setObjectName(QString::fromUtf8("loginIdInput"));
        loginIdInput->setGeometry(QRect(170, 60, 241, 41));
        loginIdInput->setEditable(true);
        loginIdInput->setDuplicatesEnabled(true);
        label_3 = new QLabel(loginViewWidget);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setGeometry(QRect(120, 70, 31, 16));
        autoLoginCheck = new QCheckBox(loginViewWidget);
        autoLoginCheck->setObjectName(QString::fromUtf8("autoLoginCheck"));
        autoLoginCheck->setGeometry(QRect(170, 160, 91, 19));
        remPwdCheck = new QCheckBox(loginViewWidget);
        remPwdCheck->setObjectName(QString::fromUtf8("remPwdCheck"));
        remPwdCheck->setGeometry(QRect(310, 160, 91, 19));
        logonButton = new QPushButton(loginViewWidget);
        logonButton->setObjectName(QString::fromUtf8("logonButton"));
        logonButton->setGeometry(QRect(420, 60, 81, 31));
        title = new QLabel(loginBackgroundFrame);
        title->setObjectName(QString::fromUtf8("title"));
        title->setGeometry(QRect(220, 60, 121, 81));
        layoutWidget = new QWidget(loginBackgroundFrame);
        layoutWidget->setObjectName(QString::fromUtf8("layoutWidget"));
        layoutWidget->setGeometry(QRect(0, 10, 159, 19));
        horizontalLayout = new QHBoxLayout(layoutWidget);
        horizontalLayout->setSpacing(6);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        label_2 = new QLabel(layoutWidget);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        horizontalLayout->addWidget(label_2);

        statusLable = new QLabel(layoutWidget);
        statusLable->setObjectName(QString::fromUtf8("statusLable"));

        horizontalLayout->addWidget(statusLable);

        backgroud = new QLabel(loginBackgroundFrame);
        backgroud->setObjectName(QString::fromUtf8("backgroud"));
        backgroud->setGeometry(QRect(0, 0, 561, 441));
        backgroud->setStyleSheet(QString::fromUtf8("border-image: url(:/background.gif);"));
        minButton = new QToolButton(loginBackgroundFrame);
        minButton->setObjectName(QString::fromUtf8("minButton"));
        minButton->setGeometry(QRect(480, 0, 41, 41));
        QFont font;
        font.setFamily(QString::fromUtf8("Agency FB"));
        font.setPointSize(9);
        minButton->setFont(font);
        minButton->setIconSize(QSize(100, 100));
        minButton->setPopupMode(QToolButton::DelayedPopup);
        minButton->setToolButtonStyle(Qt::ToolButtonIconOnly);
        closeButton = new QToolButton(loginBackgroundFrame);
        closeButton->setObjectName(QString::fromUtf8("closeButton"));
        closeButton->setGeometry(QRect(520, 0, 41, 41));
        closeButton->setIconSize(QSize(40, 40));
        closeButton->setCheckable(false);
        closeButton->setArrowType(Qt::NoArrow);
        cancelLoginButton = new QPushButton(loginBackgroundFrame);
        cancelLoginButton->setObjectName(QString::fromUtf8("cancelLoginButton"));
        cancelLoginButton->setGeometry(QRect(210, 340, 141, 41));
        cancelLoginButton->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
"font: 16pt \"Algerian\";\n"
"cancelLoginButton{border:1px solid white;background:transparent;}"));
        cancelLoginButton->setFlat(true);
        backgroud->raise();
        loginViewWidget->raise();
        title->raise();
        layoutWidget->raise();
        minButton->raise();
        closeButton->raise();
        cancelLoginButton->raise();
        LoginView->setCentralWidget(centralWidget);

        retranslateUi(LoginView);

        QMetaObject::connectSlotsByName(LoginView);
    } // setupUi

    void retranslateUi(QMainWindow *LoginView)
    {
        LoginView->setWindowTitle(QCoreApplication::translate("LoginView", "LoginView", nullptr));
        loginButton->setText(QCoreApplication::translate("LoginView", "\347\231\273\345\205\245", nullptr));
        label_4->setText(QCoreApplication::translate("LoginView", "\345\257\206\347\240\201", nullptr));
        label_3->setText(QCoreApplication::translate("LoginView", "\350\264\246\345\217\267", nullptr));
        autoLoginCheck->setText(QCoreApplication::translate("LoginView", "\350\207\252\345\212\250\347\231\273\345\205\245", nullptr));
        remPwdCheck->setText(QCoreApplication::translate("LoginView", "\350\256\260\344\275\217\345\257\206\347\240\201", nullptr));
        logonButton->setText(QCoreApplication::translate("LoginView", "\346\263\250\345\206\214", nullptr));
        title->setText(QCoreApplication::translate("LoginView", "<html><head/><body><p><span style=\" font-size:36pt; color:#ffffff;\">MyQQ</span></p></body></html>", nullptr));
        label_2->setText(QCoreApplication::translate("LoginView", "<html><head/><body><p><span style=\" color:#ffffff;\">\345\275\223\345\211\215\346\234\215\345\212\241\345\231\250\347\212\266\346\200\201\357\274\232</span></p></body></html>", nullptr));
        statusLable->setText(QCoreApplication::translate("LoginView", "<html><head/><body><p><span style=\" color:#ff0000;\">\347\246\273\347\272\277</span></p></body></html>", nullptr));
        backgroud->setText(QString());
        minButton->setText(QString());
        closeButton->setText(QString());
        cancelLoginButton->setText(QCoreApplication::translate("LoginView", "\345\217\226\346\266\210\347\231\273\345\205\245", nullptr));
    } // retranslateUi

};

namespace Ui {
    class LoginView: public Ui_LoginView {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LOGINVIEW_H
