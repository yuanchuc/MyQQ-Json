#ifndef FRIENDMAKING_H
#define FRIENDMAKING_H

#include <QWidget>
#include<QTcpSocket>
#include"tcpsocketclient.h"
#include"storageclass.hpp"
namespace Ui {
class FriendMaking;
}

class FriendMaking : public QWidget
{
    Q_OBJECT
signals:
    //void addNewFriend(QString friendId,QString status);
public:
    explicit FriendMaking(Info *psInfo,TcpSocketClient*s,QWidget *parent = nullptr);
    ~FriendMaking();

private slots:
    void on_insertButton_clicked();
    //void hasMsgDeal(MyProtoMsg* header);//接收服务器消息信号处理
private:
    Ui::FriendMaking *ui;
    TcpSocketClient* socket;
    Info *psInfo;
};

#endif // FRIENDMAKING_H
