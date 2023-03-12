#ifndef MAINCHAT_H
#define MAINCHAT_H

#include <QWidget>
#include<QTcpSocket>
#include<vector>
#include"tcpsocketclient.h"
#include"friendmaking.h"
#include"frienditem.h"
namespace Ui {
class MainChat;
}

class MainChat : public QWidget
{
    Q_OBJECT

public:
    explicit MainChat(QString UserId,TcpSocketClient* s,QWidget *parent = nullptr);
    ~MainChat();
private slots:
    void Connected();
    void hasMsgDeal(MyProtoMsg* header);
    void onDisConnect();
    void on_insertFriendButton_clicked();

private:
    Ui::MainChat *ui;
    TcpSocketClient* socket;
    QString UserId;
    vector<friendItem*> friendItems;
private:
    void setTabWidget();
    void initFriend();
    void addNewItem(QString friendId,QString result);
};

#endif // MAINCHAT_H
