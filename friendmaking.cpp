#include "friendmaking.h"
#include "ui_friendmaking.h"

FriendMaking::FriendMaking(Info *psInfo,TcpSocketClient*s,QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FriendMaking)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose);
    socket = s;
    this->psInfo = psInfo;
    //接收到消息时
    //connect(socket,&TcpSocketClient::hasMsg,this,&FriendMaking::hasMsgDeal);
}

FriendMaking::~FriendMaking()
{
    //disconnect(socket,&TcpSocketClient::hasMsg,this,&FriendMaking::hasMsgDeal);
    delete ui;
}

void FriendMaking::on_insertButton_clicked()
{
    QString UserId = ui->newFriendId->text();
    if(UserId == psInfo->getUserId()){
        QMessageBox::information(this,"添加提示","不允许添加自己");
        return;
    }
    MyProtoMsg msg;
    msg.head.server = CMD_FRIEND_MAKE;
    msg.body["friendUserId"] = UserId.toUtf8().data();
    msg.body["selfUserId"] = psInfo->getUserId().toUtf8().data();
    msg.body["extraMsg"] = Json::Value(ui->extraMsgEdit->toPlainText().toUtf8().data());
    socket->onSendData(msg);
    this->close();
}

//void FriendMaking::hasMsgDeal(MyProtoMsg* header)
//{
//    switch (header->head.server) {
//        case CMD_FRIEND_MAKE_RESULT:{
//            QMessageBox::information(this,"添加提示",QString::fromLocal8Bit(header->body["data"].asCString()));
//            qDebug()<<"data="<<QString::fromLocal8Bit(header->body["data"].asCString())<<endl;
//            if(header->body["result"].asInt()==1){
//                emit addNewFriend(QString::fromLocal8Bit(header->body["friendUserId"].asCString()),
//                        QString::fromLocal8Bit(header->body["status"].asCString()));
//            }
//        }
//    }
//}
