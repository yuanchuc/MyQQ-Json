#include "chatting.h"
#include "ui_chatting.h"

Chatting::Chatting(TcpSocketClient* s,Info* const psInfo,Info* const fdInfo,QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Chatting),socket(s),psInfo(psInfo),fdInfo(fdInfo)
{
    ui->setupUi(this);
    connect(socket,&TcpSocketClient::connected,this,&Chatting::Connected);
    connect(socket,&TcpSocketClient::disConnect,this,&Chatting::onDisConnect);
    connect(socket,&TcpSocketClient::hasMsg,this,&Chatting::hasMsgDeal);
    qDebug()<<"cur_user:"<<this->psInfo->getCharUserId()<<" friend_user:"<<fdInfo->getCharUserId()<<endl;
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
    QString userId = this->psInfo->getUserId();
    QString friendId = this->fdInfo->getUserId();
    msg.head.server = CMD_GET_FRIEND_MSG;
    msg.body["userId"] = Json::Value(userId.toStdString().c_str());
    msg.body["friendUserId"] = Json::Value(friendId.toStdString().c_str());
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
        //1.解析数据
        Json::Value msg = header->body;
        QString userId = msg["userId"].asCString();
        QString content = msg["content"].asCString();

        //2.获取当前时间
        QDateTime current_date_time =QDateTime::currentDateTime();
        QString current_date =current_date_time.toString("yyyy-MM-dd hh:mm:ss");

        //3.添加条目
        if(fdInfo->getUserId()==QString(userId)){
            addNewItem(fdInfo->getUserName(),content,current_date);
        }else{
            addNewItem(psInfo->getUserName(),content,current_date);
        }
        ui->msgTextEdit->clear();
    }break;
    }
}

void Chatting::onDisConnect()
{

}

void Chatting::on_btnSend_clicked()
{
    QString content = ui->msgTextEdit->toPlainText();
    QString userId = this->psInfo->getUserId();
    QString friendId = this->fdInfo->getUserId();
    if(content.isEmpty()){
        return;
    }
    MyProtoMsg msg;
    msg.head.server = CMD_SEND_FRIEND_MSG;
    msg.body["content"] = Json::Value(content.toStdString().c_str());
    msg.body["userId"] = Json::Value(userId.toStdString().c_str());
    msg.body["friendUserId"] = Json::Value(friendId.toStdString().c_str());
    socket->onSendData(msg);
}
