#include "mainchat.h"
#include "ui_mainchat.h"

MainChat::MainChat(QString UserId,TcpSocketClient* s,QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainChat)
{
    ui->setupUi(this);
    setTabWidget();


    //初始化成员
    this->UserId = UserId;
    socket = s;
    ui->userIdLabel->setText(this->UserId);
    connect(socket,&TcpSocketClient::connected,this,&MainChat::Connected);
    connect(socket,&TcpSocketClient::disConnect,this,&MainChat::onDisConnect);
    connect(socket,&TcpSocketClient::hasMsg,this,&MainChat::hasMsgDeal);
    initFriend();
}

MainChat::~MainChat()
{
    delete ui;
}

void MainChat::Connected()
{
    QPalette pa;
    pa.setColor(QPalette::WindowText,Qt::green);
    ui->statusLabel->setText("在线");
    ui->statusLabel->setPalette(pa);
}

void MainChat::hasMsgDeal(MyProtoMsg* header)
{
    qDebug()<<"header->server = "<<header->head.server;
    switch (header->head.server) {
        case CMD_GET_FRIEND_RESULT:{
            ui->listWidget1->clear();
            for(auto info : header->body["friendInfo"]){
                ui->listWidget1->addItem(QString(info["friendUserId"].asCString()));
            }
        }
    }
}

void MainChat::onDisConnect()
{
    QPalette pa;
    pa.setColor(QPalette::WindowText,Qt::red);
    ui->statusLabel->setText("离线");
    ui->statusLabel->setPalette(pa);
}

void MainChat::setTabWidget()
{
    //设置tabWidget 的Tab自动拉宽
    ui->tabWidget->setStyleSheet(QString("QTabBar::tab{width:%1}QTabBar::tab{height:60}").arg(ui->tabWidget->width()/ui->tabWidget->count()));
}

void MainChat::initFriend()
{
     MyProtoMsg msg;
     msg.head.server = CMD_GET_FRIEND;
     msg.body["selfUserId"] = this->UserId.toStdString().c_str();

    if(socket->b_isConnectState){
        socket->onSendData(msg);
    }
}

void MainChat::on_pushButton_clicked()
{
    FriendMaking * FM = new FriendMaking(UserId,socket);
    connect(FM,&FriendMaking::addNewFriend,this,&MainChat::initFriend);
    //设置父窗口不能操作
    FM->setWindowModality(Qt::ApplicationModal);
    FM->show();
}
