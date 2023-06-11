#include "frienditem.h"
#include "ui_frienditem.h"

friendItem::friendItem(TcpSocketClient* s,Info* psInfo,Info*fdInfo,QWidget *parent) :
    QWidget(parent),
    ui(new Ui::friendItem)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose);
    this->socket = s;
    this->fdInfo = fdInfo;
    this->psInfo = psInfo;
    setFriendId(fdInfo->getUserId());
    setStatus(fdInfo->getStatus());
    //服务器断开连接时
    connect(socket,&TcpSocketClient::disConnect,this,&friendItem::onDisConnect);
    connect(socket,&TcpSocketClient::hasMsg,this,&friendItem::hasMsgDeal);
}


friendItem::~friendItem()
{
    delete ui;
}

void friendItem::setFriendId(QString Id)
{
    ui->userIdLable->setText(Id);
}

void friendItem::setStatus(QString status)
{
    QPalette pa;
    fdInfo->setStatus(status);

    if(status=="1"){
        pa.setBrush(QPalette::Text,Qt::green);
        ui->statusLabel->setText("在线");
    }else{
        pa.setBrush(QPalette::Text,Qt::red);
        ui->statusLabel->setText("离线");
    }
    ui->statusLabel->setPalette(pa);
}

QString friendItem::getFriendId()
{
    return fdInfo->getUserId();
}

void friendItem::mouseDoubleClickEvent(QMouseEvent *e)
{
    Chatting* chatView = new Chatting(this->socket,this->psInfo,this->fdInfo);
    chatView->show();
}

void friendItem::hasMsgDeal(MyProtoMsg *header)
{
    switch (header->head.server) {
        case CMD_FRIEND_LOGIN:{
            if(header->body["friendId"].asCString()==ui->userIdLable->text()){
                setStatus("1");
                psInfo->setCurSock(header->body["cur_socket"].asCString());
                break;
            }
        }break;

        case CMD_FRIEND_LOGOUT:{
            if(header->body["friendId"].asCString()==ui->userIdLable->text()){
                setStatus("0");
                psInfo->setCurSock(nullptr);
                break;
            }
        }break;
    }
}

void friendItem::onDisConnect()
{
    setStatus("0");
}
