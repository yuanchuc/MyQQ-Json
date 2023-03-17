#include "verifymsg.h"
#include "ui_verifymsg.h"

VerifyMsg::VerifyMsg(TcpSocketClient* s,QString UserId,QWidget *parent) :
    QWidget(parent),
    ui(new Ui::VerifyMsg)
{
    ui->setupUi(this);
    //setAttribute(Qt::WA_DeleteOnClose);
    this->socket = s;
    this->UserId = UserId;
    connect(socket,&TcpSocketClient::hasMsg,this,&VerifyMsg::hasMsgDeal);
    initVerifyMsg();
}

VerifyMsg::~VerifyMsg()
{
    delete ui;
}

void VerifyMsg::hasMsgDeal(MyProtoMsg *header)
{
    qDebug()<<"verifymsg:recv server"<<header->head.server;
    switch (header->head.server) {
        case CMD_FRIEND_MAKE_SURE:{
            QString friendId = QString(header->body["friendUserId"].asCString());
            QString extraMsg = QString(header->body["extraMsg"].asCString());
            QString insertDate = QString(header->body["insertDate"].asCString());
            addNewItem(friendId,extraMsg,insertDate);
        }break;
        case CMD_GET_VERIFY_MSG_RESULT:{
            cout<<header->body<<endl;
            for(auto info : header->body["info"]){
                addNewItem(QString(info["friendUserId"].asCString()),
                        QString(info["extraMsg"].asCString()),
                        QString(info["insertDate"].asCString()),
                        QString(info["status"].asCString()));
            }
        }break;
    }
}

void VerifyMsg::addNewItem(QString friendId, QString extraMsg, QString insertDate, QString status)
{
    VerifyMsgItem* widget = new VerifyMsgItem(socket,UserId,friendId,extraMsg,insertDate,status);
    QListWidgetItem* item = new QListWidgetItem;
    item->setSizeHint(QSize(50, 90));
    ui->listWidget->insertItem(0,item);
    ui->listWidget->setItemWidget(item, widget);
    widget->show();
}

void VerifyMsg::initVerifyMsg()
{
    MyProtoMsg msg;
    msg.head.server = CMD_GET_VERIFY_MSG;
    msg.body["selfUserId"] = Json::Value(this->UserId.toLocal8Bit().data());
    socket->onSendData(msg);
}
