#include "verifymsgitem.h"
#include "ui_verifymsgitem.h"

VerifyMsgItem::VerifyMsgItem(TcpSocketClient* s,QString UserId,QString friendId, QString extraMsg, QString insertDate, QString status,QWidget *parent) :
    QWidget(parent),
    ui(new Ui::VerifyMsgItem)
{
    ui->setupUi(this);
    this->UserId = UserId;
    this->friendId = friendId;
    this->socket = s;
    ui->friendIdLabel->setText(friendId);
    ui->extraMsgLable->setText(extraMsg);
    ui->addTimeLable->setText(insertDate);
    if(status != "0" ){
        ui->agreeButton->hide();
        ui->refuseButton->hide();
        ui->statusLabel->show();
    }
    if(status == "0"){
        ui->agreeButton->show();
        ui->refuseButton->show();
        ui->statusLabel->hide();
    }
    if(status == "1"){
        ui->statusLabel->setText("已同意");
    }
    if(status == "-1"){
        ui->statusLabel->setText("已拒绝");
    }
}

VerifyMsgItem::~VerifyMsgItem()
{
    delete ui;
}

void VerifyMsgItem::on_agreeButton_clicked()
{
    MyProtoMsg msg;
    msg.head.server = CMD_FRIEND_MAKE_SURE_RESULT;
    msg.body["result"] = Json::Value(1);
    msg.body["selfUserId"] = Json::Value(this->UserId.toStdString().c_str());
    msg.body["friendUserId"] = Json::Value(this->friendId.toUtf8().data());
    if(socket->b_isConnectState){
        socket->onSendData(msg);
        ui->agreeButton->hide();
        ui->refuseButton->hide();
        ui->statusLabel->show();
        ui->statusLabel->setText("已同意");
    }

}

void VerifyMsgItem::on_refuseButton_clicked()
{
    MyProtoMsg msg;
    msg.head.server = CMD_FRIEND_MAKE_SURE_RESULT;
    msg.body["result"] = Json::Value(-1);
    msg.body["selfUserId"] = Json::Value(this->UserId.toStdString().c_str());
    msg.body["friendUserId"] = Json::Value(this->friendId.toUtf8().data());
    if(socket->b_isConnectState){
        socket->onSendData(msg);
        ui->agreeButton->hide();
        ui->refuseButton->hide();
        ui->statusLabel->show();
        ui->statusLabel->setText("已拒绝");
    }

}
