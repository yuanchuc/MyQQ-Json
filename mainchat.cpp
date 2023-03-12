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
    qDebug()<<"MainChat:header->server = "<<header->head.server;
    switch (header->head.server) {
        case CMD_GET_FRIEND_RESULT:{
            qDebug()<<"clear"<<endl;
            ui->listWidget1->clear();
            qDebug()<<"clear"<<endl;
            for(auto info : header->body["friendInfo"]){
                addNewItem(QString(info["friendUserId"].asCString()),QString(info["status"].asCString()));
            }
        }break;
        case CMD_FRIEND_LOGIN:{
            for(auto item:friendItems){
                if(header->body["friendId"].asString()==item->getFriendId().toStdString()){
                    item->setStatus("1");
                    break;
                }
            }
        }break;

        case CMD_FRIEND_LOGOUT:{
            for(auto item:friendItems){
                if(header->body["friendId"].asString()==item->getFriendId().toStdString()){
                    item->setStatus("0");
                    break;
                }
            }
        }break;

        case CMD_FRIEND_ADD:{
            addNewItem(QString(header->body["friendUserId"].asCString()),"1");
        }break;

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
    cout<<"initFriend"<<endl;
     MyProtoMsg msg;
     msg.head.server = CMD_GET_FRIEND;
     msg.body["selfUserId"] = this->UserId.toStdString().c_str();
    if(socket->b_isConnectState){
        socket->onSendData(msg);
    }  
}

void MainChat::addNewItem(QString friendId, QString result)
{
    friendItem* widget = new friendItem(socket,this->UserId,friendId,result,this);
    QListWidgetItem* item = new QListWidgetItem;
    item->setSizeHint(QSize(24, 36));
    ui->listWidget1->addItem(item);
    ui->listWidget1->setItemWidget(item, widget);
    widget->show();
    friendItems.push_back(widget);
}

void MainChat::on_insertFriendButton_clicked()
{
    FriendMaking * FM = new FriendMaking(UserId,socket);
    connect(FM,&FriendMaking::addNewFriend,this,&MainChat::initFriend);
    //设置父窗口不能操作
    FM->setWindowModality(Qt::ApplicationModal);
    FM->show();
}
