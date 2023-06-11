#include "chatting.h"
#include "ui_chatting.h"

Chatting::Chatting(TcpSocketClient* s,Info* psInfo,Info*fdInfo,QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Chatting)
{
    ui->setupUi(this);
    this->socket = s;
    this->psInfo = psInfo;
    this->fdInfo = fdInfo;
    connect(socket,&TcpSocketClient::connected,this,&Chatting::Connected);
    connect(socket,&TcpSocketClient::disConnect,this,&Chatting::onDisConnect);
    connect(socket,&TcpSocketClient::hasMsg,this,&Chatting::hasMsgDeal);
    getMsg();
}

Chatting::~Chatting()
{
    delete ui;
}

void Chatting::addNewItem(QString name, QString content, QString time)
{
    Content* widget = new Content(name,content,time,this);
    QListWidgetItem* item = new QListWidgetItem;
    item->setSizeHint(QSize(24, 50));
    ui->showListWidget->addItem(item);
    ui->showListWidget->setItemWidget(item, widget);
    widget->show();
}

void Chatting::addUsrItem(QString name)
{
    QTableWidgetItem *usr = new QTableWidgetItem("张三");
    ui->usrTbWidget->insertRow(0);
    ui->usrTbWidget->setItem(0,0,usr);
}

void Chatting::getMsg()
{
    MyProtoMsg msg;
    char* userId = psInfo->getCharUserId();
    char* friendId = fdInfo->getCharUserId();
    msg.head.server = CMD_GET_FRIEND_MSG;
    msg.body["userId"] = Json::Value(userId);
    msg.body["friendId"] = Json::Value(friendId);
    socket->onSendData(msg);
}

void Chatting::on_btnClose_clicked()
{
    this->close();
}

void Chatting::Connected()
{

}

void Chatting::hasMsgDeal(MyProtoMsg *header)
{
    qDebug()<<"Chatting:header->server = "<<header->head.server;
    switch (header->head.server) {
    case CMD_GET_FRIEND_MSG:{
        Json::Value data = header->body;
        for(int i=0;i<ui->showListWidget->count();){
            QListWidgetItem * item = ui->showListWidget->item(i);
            ui->showListWidget->removeItemWidget(item);
            delete item;
        }
        for(auto msg:data){
            QString userId = msg["userId"].asCString();
            QString content = msg["content"].asCString();
            QString date = msg["date"].asCString();
            if(psInfo->getUserId()==QString(userId)){
                addNewItem(psInfo->getUserName(),content,date);
            }else{
                addNewItem(fdInfo->getUserName(),content,date);
            }

        }
    }break;
    case CMD_RECV_FRIEND_MSG:{
        Json::Value msg = header->body;
        QString userId = msg["userId"].asCString();
        QString content = msg["content"].asCString();
        if(fdInfo->getUserId()==QString(userId)){
            QDateTime current_date_time =QDateTime::currentDateTime();
            QString current_date =current_date_time.toString("yyyy.MM.dd hh:mm:ss");
            addNewItem(fdInfo->getUserName(),content,current_date);
        }
    }break;
    }
}

void Chatting::onDisConnect()
{

}

void Chatting::on_btnSend_clicked()
{
    QString content = ui->msgTextEdit->toPlainText();
    char* userId = psInfo->getCharUserId();
    char* friendId = fdInfo->getCharUserId();
    if(content.isEmpty()){
        return;
    }
    MyProtoMsg msg;
    msg.head.server = CMD_SEND_FRIEND_MSG;
    msg.body["content"] = Json::Value(content.toStdString().c_str());
    msg.body["userId"] = Json::Value(userId);
    msg.body["friendId"] = Json::Value(friendId);
    socket->onSendData(msg);
}
