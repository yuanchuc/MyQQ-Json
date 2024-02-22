#ifndef CHATTING_H
#define CHATTING_H
#include<QDateTime>
#include <QWidget>
#include"content.h"
#include<tcpsocketclient.h>
#include"storageclass.hpp"

namespace Ui {
class Chatting;
}

class Chatting : public QWidget
{
    Q_OBJECT

public:
    explicit Chatting(TcpSocketClient* s,Info* const psInfo,Info* const fdInfo,QWidget *parent = nullptr);
    ~Chatting();

private slots:

    void on_btnClose_clicked();
    void Connected();
    void hasMsgDeal(MyProtoMsg* header);
    void onDisConnect();
    void on_btnSend_clicked();
private:
    Ui::Chatting *ui;
    void addNewItem(QString name,QString content,QString time);   //添加对应的好友Item
    void addUsrItem(QString name);
    void getMsg();
private:
    TcpSocketClient* socket;
    Info* const psInfo;
    Info* const fdInfo;
};

#endif // CHATTING_H
