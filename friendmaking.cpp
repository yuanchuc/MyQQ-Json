#include "friendmaking.h"
#include "ui_friendmaking.h"

FriendMaking::FriendMaking(QString UserId,TcpSocketClient*s,QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FriendMaking)
{
    ui->setupUi(this);
    socket = s;
    this->UserId = UserId;
    //接收到消息时
    connect(socket,&TcpSocketClient::hasMsg,this,&FriendMaking::hasMsgDeal);
}

FriendMaking::~FriendMaking()
{
    socket->disconnect();

    delete ui;

}

void FriendMaking::on_insertButton_clicked()
{
    QString UserId = ui->newFriendId->text();
    if(UserId == this->UserId){
        QMessageBox::information(this,"添加提示","不允许添加自己");
        return;
    }
    MyProtoMsg msg;
    msg.head.server = CMD_FRIEND_MAKE;
    msg.body["friendUserId"] = UserId.toUtf8().data();
    msg.body["selfUserId"] = this->UserId.toUtf8().data();
    socket->onSendData(msg);
}

void FriendMaking::hasMsgDeal(MyProtoMsg* header)
{
    switch (header->head.server) {
        case CMD_FRIEND_MAKE_RESULT:{
            QMessageBox::information(this,"添加提示",QString::fromLocal8Bit(header->body["data"].asCString()));
            qDebug()<<"data="<<QString::fromLocal8Bit(header->body["data"].asCString())<<endl;
            if(header->body["result"].asInt()==1){
                emit addNewFriend();
            }
        }
    }
}
