#ifndef MAINCHAT_H
#define MAINCHAT_H

#include <QWidget>
#include<QTcpSocket>
#include"tcpsocketclient.h"
#include"friendmaking.h"
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
    void on_pushButton_clicked();

private:
    Ui::MainChat *ui;
    TcpSocketClient* socket;
    QString UserId;

private:
    void setTabWidget();
    void initFriend();
};

#endif // MAINCHAT_H
