#include "frienditem.h"
#include "ui_frienditem.h"

friendItem::friendItem(TcpSocketClient* s,QString selfId,QString userId,QString result,QWidget *parent) :
    QWidget(parent),
    ui(new Ui::friendItem)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose);
    this->socket = s;
    this->userId = userId;
    this->result = result;
    this->selfId = selfId;
    setFriendId(userId);
    setStatus(result);
}


friendItem::~friendItem()
{
    delete ui;
}

void friendItem::setFriendId(QString Id)
{
    ui->userIdLable->setText(Id);
}

void friendItem::setStatus(QString result)
{
    QPalette pa;


    if(result=="1"){
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
    return this->userId;
}
