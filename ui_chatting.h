/********************************************************************************
** Form generated from reading UI file 'chatting.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CHATTING_H
#define UI_CHATTING_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QFontComboBox>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Chatting
{
public:
    QWidget *widget_6;
    QVBoxLayout *verticalLayout_7;
    QWidget *widget_7;
    QHBoxLayout *horizontalLayout_11;
    QWidget *widget_8;
    QHBoxLayout *horizontalLayout_12;
    QFrame *frame_8;
    QVBoxLayout *verticalLayout_8;
    QListWidget *showListWidget;
    QFrame *frame_9;
    QHBoxLayout *horizontalLayout_13;
    QFontComboBox *fontCbx;
    QComboBox *sizeCbx;
    QToolButton *boldTBtn_3;
    QToolButton *italicTBtn_3;
    QToolButton *underlineTBtn_3;
    QToolButton *colorTBtn_3;
    QToolButton *saveTBtn_3;
    QToolButton *clearTBtn_3;
    QTextEdit *msgTextEdit;
    QTableWidget *usrTbWidget;
    QFrame *frame_10;
    QHBoxLayout *horizontalLayout_14;
    QSpacerItem *horizontalSpacer_7;
    QPushButton *btnSend;
    QSpacerItem *horizontalSpacer_8;
    QLabel *usrNumLbl_3;
    QSpacerItem *horizontalSpacer_9;
    QPushButton *btnClose;
    QWidget *widget;
    QHBoxLayout *horizontalLayout_15;
    QWidget *widget_9;
    QVBoxLayout *verticalLayout_9;

    void setupUi(QWidget *Chatting)
    {
        if (Chatting->objectName().isEmpty())
            Chatting->setObjectName(QString::fromUtf8("Chatting"));
        Chatting->resize(787, 415);
        widget_6 = new QWidget(Chatting);
        widget_6->setObjectName(QString::fromUtf8("widget_6"));
        widget_6->setGeometry(QRect(0, 0, 786, 414));
        verticalLayout_7 = new QVBoxLayout(widget_6);
        verticalLayout_7->setObjectName(QString::fromUtf8("verticalLayout_7"));
        verticalLayout_7->setContentsMargins(0, 0, 0, 0);
        widget_7 = new QWidget(widget_6);
        widget_7->setObjectName(QString::fromUtf8("widget_7"));
        widget_7->setMinimumSize(QSize(0, 0));
        horizontalLayout_11 = new QHBoxLayout(widget_7);
        horizontalLayout_11->setObjectName(QString::fromUtf8("horizontalLayout_11"));
        horizontalLayout_11->setContentsMargins(0, 0, 0, 0);
        widget_8 = new QWidget(widget_7);
        widget_8->setObjectName(QString::fromUtf8("widget_8"));
        horizontalLayout_12 = new QHBoxLayout(widget_8);
        horizontalLayout_12->setObjectName(QString::fromUtf8("horizontalLayout_12"));
        horizontalLayout_12->setContentsMargins(0, 0, 0, 0);
        frame_8 = new QFrame(widget_8);
        frame_8->setObjectName(QString::fromUtf8("frame_8"));
        frame_8->setFrameShape(QFrame::Box);
        verticalLayout_8 = new QVBoxLayout(frame_8);
        verticalLayout_8->setObjectName(QString::fromUtf8("verticalLayout_8"));
        verticalLayout_8->setContentsMargins(0, 0, 0, 0);
        showListWidget = new QListWidget(frame_8);
        showListWidget->setObjectName(QString::fromUtf8("showListWidget"));

        verticalLayout_8->addWidget(showListWidget);

        frame_9 = new QFrame(frame_8);
        frame_9->setObjectName(QString::fromUtf8("frame_9"));
        frame_9->setFrameShape(QFrame::Box);
        horizontalLayout_13 = new QHBoxLayout(frame_9);
        horizontalLayout_13->setObjectName(QString::fromUtf8("horizontalLayout_13"));
        fontCbx = new QFontComboBox(frame_9);
        fontCbx->setObjectName(QString::fromUtf8("fontCbx"));

        horizontalLayout_13->addWidget(fontCbx);

        sizeCbx = new QComboBox(frame_9);
        sizeCbx->addItem(QString());
        sizeCbx->addItem(QString());
        sizeCbx->addItem(QString());
        sizeCbx->addItem(QString());
        sizeCbx->addItem(QString());
        sizeCbx->addItem(QString());
        sizeCbx->addItem(QString());
        sizeCbx->addItem(QString());
        sizeCbx->addItem(QString());
        sizeCbx->addItem(QString());
        sizeCbx->addItem(QString());
        sizeCbx->addItem(QString());
        sizeCbx->addItem(QString());
        sizeCbx->addItem(QString());
        sizeCbx->addItem(QString());
        sizeCbx->setObjectName(QString::fromUtf8("sizeCbx"));

        horizontalLayout_13->addWidget(sizeCbx);

        boldTBtn_3 = new QToolButton(frame_9);
        boldTBtn_3->setObjectName(QString::fromUtf8("boldTBtn_3"));
        boldTBtn_3->setMinimumSize(QSize(33, 33));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/images/bold.png"), QSize(), QIcon::Normal, QIcon::Off);
        boldTBtn_3->setIcon(icon);
        boldTBtn_3->setIconSize(QSize(26, 26));
        boldTBtn_3->setCheckable(true);

        horizontalLayout_13->addWidget(boldTBtn_3);

        italicTBtn_3 = new QToolButton(frame_9);
        italicTBtn_3->setObjectName(QString::fromUtf8("italicTBtn_3"));
        italicTBtn_3->setMinimumSize(QSize(33, 33));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/images/italic.png"), QSize(), QIcon::Normal, QIcon::Off);
        italicTBtn_3->setIcon(icon1);
        italicTBtn_3->setIconSize(QSize(26, 26));
        italicTBtn_3->setCheckable(true);

        horizontalLayout_13->addWidget(italicTBtn_3);

        underlineTBtn_3 = new QToolButton(frame_9);
        underlineTBtn_3->setObjectName(QString::fromUtf8("underlineTBtn_3"));
        underlineTBtn_3->setMinimumSize(QSize(33, 33));
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/images/under.png"), QSize(), QIcon::Normal, QIcon::Off);
        underlineTBtn_3->setIcon(icon2);
        underlineTBtn_3->setIconSize(QSize(26, 26));
        underlineTBtn_3->setCheckable(true);

        horizontalLayout_13->addWidget(underlineTBtn_3);

        colorTBtn_3 = new QToolButton(frame_9);
        colorTBtn_3->setObjectName(QString::fromUtf8("colorTBtn_3"));
        colorTBtn_3->setMinimumSize(QSize(33, 33));
        QIcon icon3;
        icon3.addFile(QString::fromUtf8(":/images/color.png"), QSize(), QIcon::Normal, QIcon::Off);
        colorTBtn_3->setIcon(icon3);
        colorTBtn_3->setIconSize(QSize(26, 26));

        horizontalLayout_13->addWidget(colorTBtn_3);

        saveTBtn_3 = new QToolButton(frame_9);
        saveTBtn_3->setObjectName(QString::fromUtf8("saveTBtn_3"));
        saveTBtn_3->setMinimumSize(QSize(33, 33));
        QIcon icon4;
        icon4.addFile(QString::fromUtf8(":/images/save.png"), QSize(), QIcon::Normal, QIcon::Off);
        saveTBtn_3->setIcon(icon4);
        saveTBtn_3->setIconSize(QSize(26, 26));

        horizontalLayout_13->addWidget(saveTBtn_3);

        clearTBtn_3 = new QToolButton(frame_9);
        clearTBtn_3->setObjectName(QString::fromUtf8("clearTBtn_3"));
        clearTBtn_3->setMinimumSize(QSize(33, 33));
        QIcon icon5;
        icon5.addFile(QString::fromUtf8(":/images/clear.png"), QSize(), QIcon::Normal, QIcon::Off);
        clearTBtn_3->setIcon(icon5);
        clearTBtn_3->setIconSize(QSize(26, 26));

        horizontalLayout_13->addWidget(clearTBtn_3);


        verticalLayout_8->addWidget(frame_9);

        msgTextEdit = new QTextEdit(frame_8);
        msgTextEdit->setObjectName(QString::fromUtf8("msgTextEdit"));
        msgTextEdit->setMaximumSize(QSize(16777215, 100));

        verticalLayout_8->addWidget(msgTextEdit);


        horizontalLayout_12->addWidget(frame_8);


        horizontalLayout_11->addWidget(widget_8);

        usrTbWidget = new QTableWidget(widget_7);
        if (usrTbWidget->columnCount() < 1)
            usrTbWidget->setColumnCount(1);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        usrTbWidget->setHorizontalHeaderItem(0, __qtablewidgetitem);
        usrTbWidget->setObjectName(QString::fromUtf8("usrTbWidget"));
        usrTbWidget->setMinimumSize(QSize(180, 0));
        usrTbWidget->setMaximumSize(QSize(180, 16777215));
        usrTbWidget->setSelectionMode(QAbstractItemView::SingleSelection);
        usrTbWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
        usrTbWidget->setShowGrid(false);

        horizontalLayout_11->addWidget(usrTbWidget);


        verticalLayout_7->addWidget(widget_7);

        frame_10 = new QFrame(widget_6);
        frame_10->setObjectName(QString::fromUtf8("frame_10"));
        frame_10->setFrameShape(QFrame::Box);
        horizontalLayout_14 = new QHBoxLayout(frame_10);
        horizontalLayout_14->setObjectName(QString::fromUtf8("horizontalLayout_14"));
        horizontalLayout_14->setContentsMargins(0, 0, 0, 0);
        horizontalSpacer_7 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_14->addItem(horizontalSpacer_7);

        btnSend = new QPushButton(frame_10);
        btnSend->setObjectName(QString::fromUtf8("btnSend"));

        horizontalLayout_14->addWidget(btnSend);

        horizontalSpacer_8 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_14->addItem(horizontalSpacer_8);

        usrNumLbl_3 = new QLabel(frame_10);
        usrNumLbl_3->setObjectName(QString::fromUtf8("usrNumLbl_3"));

        horizontalLayout_14->addWidget(usrNumLbl_3);

        horizontalSpacer_9 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_14->addItem(horizontalSpacer_9);

        btnClose = new QPushButton(frame_10);
        btnClose->setObjectName(QString::fromUtf8("btnClose"));

        horizontalLayout_14->addWidget(btnClose);


        verticalLayout_7->addWidget(frame_10);

        widget = new QWidget(Chatting);
        widget->setObjectName(QString::fromUtf8("widget"));
        widget->setGeometry(QRect(190, 518, 786, 0));
        horizontalLayout_15 = new QHBoxLayout(widget);
        horizontalLayout_15->setObjectName(QString::fromUtf8("horizontalLayout_15"));
        horizontalLayout_15->setContentsMargins(0, 0, 0, 0);
        widget_9 = new QWidget(Chatting);
        widget_9->setObjectName(QString::fromUtf8("widget_9"));
        widget_9->setGeometry(QRect(190, 90, 786, 0));
        verticalLayout_9 = new QVBoxLayout(widget_9);
        verticalLayout_9->setObjectName(QString::fromUtf8("verticalLayout_9"));
        verticalLayout_9->setContentsMargins(0, 0, 0, 0);

        retranslateUi(Chatting);

        sizeCbx->setCurrentIndex(4);


        QMetaObject::connectSlotsByName(Chatting);
    } // setupUi

    void retranslateUi(QWidget *Chatting)
    {
        Chatting->setWindowTitle(QCoreApplication::translate("Chatting", "Form", nullptr));
        sizeCbx->setItemText(0, QCoreApplication::translate("Chatting", "8", nullptr));
        sizeCbx->setItemText(1, QCoreApplication::translate("Chatting", "9", nullptr));
        sizeCbx->setItemText(2, QCoreApplication::translate("Chatting", "10", nullptr));
        sizeCbx->setItemText(3, QCoreApplication::translate("Chatting", "11", nullptr));
        sizeCbx->setItemText(4, QCoreApplication::translate("Chatting", "12", nullptr));
        sizeCbx->setItemText(5, QCoreApplication::translate("Chatting", "13", nullptr));
        sizeCbx->setItemText(6, QCoreApplication::translate("Chatting", "14", nullptr));
        sizeCbx->setItemText(7, QCoreApplication::translate("Chatting", "15", nullptr));
        sizeCbx->setItemText(8, QCoreApplication::translate("Chatting", "16", nullptr));
        sizeCbx->setItemText(9, QCoreApplication::translate("Chatting", "17", nullptr));
        sizeCbx->setItemText(10, QCoreApplication::translate("Chatting", "18", nullptr));
        sizeCbx->setItemText(11, QCoreApplication::translate("Chatting", "19", nullptr));
        sizeCbx->setItemText(12, QCoreApplication::translate("Chatting", "20", nullptr));
        sizeCbx->setItemText(13, QCoreApplication::translate("Chatting", "21", nullptr));
        sizeCbx->setItemText(14, QCoreApplication::translate("Chatting", "22", nullptr));

#if QT_CONFIG(tooltip)
        boldTBtn_3->setToolTip(QCoreApplication::translate("Chatting", "\345\212\240\347\262\227", nullptr));
#endif // QT_CONFIG(tooltip)
        boldTBtn_3->setText(QCoreApplication::translate("Chatting", "...", nullptr));
#if QT_CONFIG(tooltip)
        italicTBtn_3->setToolTip(QCoreApplication::translate("Chatting", "\345\200\276\346\226\234", nullptr));
#endif // QT_CONFIG(tooltip)
        italicTBtn_3->setText(QCoreApplication::translate("Chatting", "...", nullptr));
#if QT_CONFIG(tooltip)
        underlineTBtn_3->setToolTip(QCoreApplication::translate("Chatting", "\344\270\213\345\210\222\347\272\277", nullptr));
#endif // QT_CONFIG(tooltip)
        underlineTBtn_3->setText(QCoreApplication::translate("Chatting", "...", nullptr));
#if QT_CONFIG(tooltip)
        colorTBtn_3->setToolTip(QCoreApplication::translate("Chatting", "\346\233\264\346\224\271\345\255\227\344\275\223\351\242\234\350\211\262", nullptr));
#endif // QT_CONFIG(tooltip)
        colorTBtn_3->setText(QCoreApplication::translate("Chatting", "...", nullptr));
#if QT_CONFIG(tooltip)
        saveTBtn_3->setToolTip(QCoreApplication::translate("Chatting", "\344\277\235\345\255\230\350\201\212\345\244\251\350\256\260\345\275\225", nullptr));
#endif // QT_CONFIG(tooltip)
        saveTBtn_3->setText(QCoreApplication::translate("Chatting", "...", nullptr));
#if QT_CONFIG(tooltip)
        clearTBtn_3->setToolTip(QCoreApplication::translate("Chatting", "\346\270\205\347\251\272\350\201\212\345\244\251\350\256\260\345\275\225", nullptr));
#endif // QT_CONFIG(tooltip)
        clearTBtn_3->setText(QCoreApplication::translate("Chatting", "...", nullptr));
        QTableWidgetItem *___qtablewidgetitem = usrTbWidget->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QCoreApplication::translate("Chatting", "\347\224\250\346\210\267\345\220\215", nullptr));
        btnSend->setText(QCoreApplication::translate("Chatting", "\345\217\221\351\200\201", nullptr));
        usrNumLbl_3->setText(QCoreApplication::translate("Chatting", "\345\234\250\347\272\277\344\272\272\346\225\260\357\274\2320\344\272\272", nullptr));
        btnClose->setText(QCoreApplication::translate("Chatting", "\351\200\200\345\207\272", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Chatting: public Ui_Chatting {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CHATTING_H
