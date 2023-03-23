#include "logonview.h"
#include "ui_logonview.h"

logonView::logonView(TcpSocketClient*s,QWidget *parent) :
    QWidget(parent),
    ui(new Ui::logonView)
{
    ui->setupUi(this);
    this->socket = s;
    //接收到消息时
    connect(socket,&TcpSocketClient::hasMsg,this,&logonView::hasMsgDeal);
}

logonView::~logonView()
{
    delete ui;
}

void logonView::on_pushButton_clicked()
{
    QString UserId=ui->userIdEdit->text();
    QString Pwd=ui->pwdEdit->text();
    QString PwdSure=ui->pwdSureEdit->text();
    if(UserId.isEmpty()){
        QMessageBox::warning(this,"注册提示","账号不能为空");
        return;
    }
    if(Pwd.isEmpty()){
        QMessageBox::warning(this,"注册提示","密码不能为空");
        return;
    }
    if(PwdSure.isEmpty()){
        QMessageBox::warning(this,"注册提示","确认密码不能为空");
        return;
    }
    if(Pwd!=PwdSure){
        QMessageBox::warning(this,"注册提示","两次输入的密码不一致");
        return;
    }
    MyProtoMsg msg1;
    //------放入消息
    msg1.head.server = CMD_LOGON;
    msg1.body["userId"] = Json::Value(UserId.toStdString().c_str());
    msg1.body["pwd"] = Json::Value(Pwd.toStdString().c_str());
    if(socket->b_isConnectState){
        this->socket->onSendData(msg1);
    }else{
        QMessageBox::warning(this,"注册提示","网络未连接");
    }

}

void logonView::hasMsgDeal(MyProtoMsg* header)
{
    switch (header->head.server) {
        case CMD_LOGON_RESULT:{
            if(header->body["result"].asInt() == 1){
                QMessageBox::information(this,"注册提示",QString::fromLocal8Bit(header->body["data"].asCString()));
                return;
            }else if(header->body["result"].asInt() == 0){
                QMessageBox::warning(this,"注册提示",QString::fromLocal8Bit(header->body["data"].asCString()));
                this->close();
            }
        }
    }
}
