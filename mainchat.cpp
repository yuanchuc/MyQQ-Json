#include "mainchat.h"
#include "ui_mainchat.h"

MainChat::MainChat(QString UserId,TcpSocketClient* s,QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainChat)
{
    ui->setupUi(this);
    setTabWidget();
    initEvent();

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
        case CMD_FRIEND_MAKE_SURE:{
            if (QMessageBox::Yes == QMessageBox::question(this, QStringLiteral("响应"),
             QStringLiteral("wants to add you\n %1").arg(QString::fromLocal8Bit(header->body["friendUserId"].asCString())),
             QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes))
            {
                addNewItem(QString::fromLocal8Bit(header->body["friendUserId"].asCString()),QString(header->body["status"].asCString()));
                MyProtoMsg msg;
                msg.head.server = CMD_FRIEND_MAKE_SURE_RESULT;
                msg.body["result"] = Json::Value(1);
                msg.body["selfUserId"] = Json::Value(this->UserId.toStdString().c_str());
                msg.body["friendUserId"] = Json::Value(header->body["friendUserId"]);
                socket->onSendData(msg);
             }
        }break;
        case CMD_GET_FRIEND_RESULT:{
            qDebug()<<"clear"<<endl;
            ui->listWidget1->clear();
            qDebug()<<"clear"<<endl;
            for(auto info : header->body["friendInfo"]){
                addNewItem(QString(info["friendUserId"].asCString()),QString(info["status"].asCString()));
            }
        }break;
        case CMD_DEL_FRIEND_RESULT:{
            if(header->body["result"].asInt()==1){
                moveItemFriend(header->body["friendUserId"].asCString());
            }
            QMessageBox::information(this,"结果",QString::fromLocal8Bit(header->body["data"].asCString()));
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
            if(header->body["result"].asInt() == 1){
                addNewItem(QString::fromLocal8Bit(header->body["friendUserId"].asCString()),QString(header->body["status"].asCString()));
            }

        }break;
        case CMD_FRIEND_REDUCE:{
            moveItemFriend(header->body["friendUserId"].asCString());
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

void MainChat::initEvent()
{
    /*重要：设置QListWidget的contextMenuPolicy属性，不然不能显示右键菜单*/
    ui->listWidget1->setProperty("contextMenuPolicy", Qt::CustomContextMenu);
    /*初始化一个包含两个Action(Delete和ClearAll)的菜单*/
    popMenu_In_ListWidget_ = new QMenu(this);
    action_Delete_In_ListWidget_ = new QAction(tr("Delete"), this);
    popMenu_In_ListWidget_->addAction(action_Delete_In_ListWidget_);
    qDebug()<<"初始化成功";
    connect(this->action_Delete_In_ListWidget_, &QAction::triggered, this, &MainChat::onActionDelete);
    //绑定处理函数
    //*绑定右键显示菜单：在单击右键之后会执行槽函数， 槽函数中负责弹出右键菜单*/
    connect(ui->listWidget1, SIGNAL(customContextMenuRequested(const QPoint &)),
            this, SLOT(onCustomContextMenuRequested(const QPoint &)));
}

void MainChat::moveItemFriend(const char* friendId)
{
    for(int i=0;i<ui->listWidget1->count();i++){
        QListWidgetItem * item = ui->listWidget1->item(i);
        QWidget* widget = ui->listWidget1->itemWidget(item);
        QString friendUserId = widget->findChild<QLabel*>("userIdLable")->text();
        qDebug()<<friendUserId;
        //对Item进行移除
        if(friendUserId == friendId){
            ui->listWidget1->removeItemWidget(item);
            delete item;
            break;
        }
    }
}

void MainChat::addNewItem(QString friendId, QString status)
{
    friendItem* widget = new friendItem(socket,this->UserId,friendId,status,this);
    QListWidgetItem* item = new QListWidgetItem;
    item->setSizeHint(QSize(24, 36));
    ui->listWidget1->addItem(item);
    ui->listWidget1->setItemWidget(item, widget);
    widget->show();
    friendItems.push_back(widget);
}

void MainChat::onActionDelete()
{
    qDebug()<<"删除被点击";
    /*获取当前选中的Item*/
    QList<QListWidgetItem*> items = ui->listWidget1->selectedItems();
    if(items.count()>0){
        foreach(QListWidgetItem* var, items) {
            //获取item中的数据
            QWidget*widget = ui->listWidget1->itemWidget(var);
            qDebug()<<widget->findChild<QLabel*>("userIdLable")->text()<<"被删除";
            QString friendUserId = widget->findChild<QLabel*>("userIdLable")->text();
            //确定是否删除
            if (QMessageBox::Yes == QMessageBox::question(this, QStringLiteral("操作"),
             QStringLiteral("delete friend?\n %1").arg(friendUserId), QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes))
            {
                MyProtoMsg msg;
                msg.head.server = CMD_DEL_FRIEND;
                msg.body["friendUserId"] = Json::Value(friendUserId.toStdString().c_str());
                msg.body["selfUserId"] = Json::Value(this->UserId.toStdString().c_str());
                socket->onSendData(msg);
                ui->listWidget1->removeItemWidget(var);
                items.removeOne(var);
                delete var;
             }

         }
    }
}

//事件处理函数
void MainChat::onCustomContextMenuRequested(const QPoint &pos)
{
    /*弹出右键菜单*/
    popMenu_In_ListWidget_->exec(QCursor::pos());
}

void MainChat::on_insertFriendButton_clicked()
{
    FriendMaking * FM = new FriendMaking(UserId,socket);
    connect(FM,&FriendMaking::addNewFriend,[this](QString friendId,QString status){
        addNewItem(friendId,status);
    });
    //设置父窗口不能操作
    FM->setWindowModality(Qt::ApplicationModal);
    FM->show();
}
