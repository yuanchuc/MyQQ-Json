#ifndef FRIENDMAKING_H
#define FRIENDMAKING_H

#include <QWidget>
#include<QTcpSocket>
#include"tcpsocketclient.h"
namespace Ui {
class FriendMaking;
}

class FriendMaking : public QWidget
{
    Q_OBJECT
signals:
    void addNewFriend();
public:
    explicit FriendMaking(QString UserId,TcpSocketClient*s,QWidget *parent = nullptr);
    ~FriendMaking();

private slots:
    void on_insertButton_clicked();
    void hasMsgDeal(MyProtoMsg* header);//接收服务器消息信号处理
private:
    Ui::FriendMaking *ui;
    TcpSocketClient* socket;
    QString UserId;
};

#endif // FRIENDMAKING_H
