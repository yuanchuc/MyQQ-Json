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

QString logonView::GetMd5(const QString &value)
{
    QString md5;
    QByteArray bb;//相当于是QChar的一个vector<>
    bb = QCryptographicHash::hash(value.toUtf8(), QCryptographicHash::Md5);
    md5.append(bb.toHex());
    return md5;
}

void logonView::on_pushButton_clicked()
{
    QString UserName=ui->nameEdit->text();
    QString phone = ui->phoneEdit->text();
    QString Pwd=ui->pwdEdit->text();
    QString PwdSure=ui->pwdSureEdit->text();
    if(UserName.isEmpty()){
        QMessageBox::warning(this,"注册提示","昵称不能为空");
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
    //对密码进行加密
    QString newPass = GetMd5(Pwd);
    //------放入消息
    MyProtoMsg msg1;
    msg1.head.server = CMD_LOGON;
    msg1.body["UserPhone"] = Json::Value(phone.toStdString().c_str());
    msg1.body["UserName"] = Json::Value(UserName.toUtf8().data());
    msg1.body["UserPwd"] = Json::Value(newPass.toStdString().c_str());
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
                QString data = QString::fromLocal8Bit(header->body["data"].asCString());
                QString Id = QString::fromLocal8Bit(header->body["userId"].asCString());
                QString tip = data+"\n"+"你的账号\n"+Id;
                QMessageBox::information(this,"注册提示",tip);
                return;
            }else if(header->body["result"].asInt() == 0){
                QMessageBox::warning(this,"注册提示",QString::fromLocal8Bit(header->body["data"].asCString()));
                this->close();
            }
        }
    }
}
