#ifndef MAINCHAT_H
#define MAINCHAT_H

#include <QWidget>
#include<QTcpSocket>
#include<vector>
#include<QMenuBar>
#include<QMenu>

#include <QToolBar>
#include"tcpsocketclient.h"
#include"friendmaking.h"
#include"frienditem.h"
#include"verifymsg.h"
namespace Ui {
class MainChat;
}

class MainChat : public QWidget
{
    Q_OBJECT

public:
    explicit MainChat(QString UserId,QString pwd,TcpSocketClient* s,QWidget *parent = nullptr);
    ~MainChat();
private slots:
    void Connected();
    void hasMsgDeal(MyProtoMsg* header);
    void onDisConnect();
    void on_insertFriendButton_clicked();
    void onCustomContextMenuRequested(const QPoint &pos);
    void on_verifyMsgButton_clicked();

private:
    Ui::MainChat *ui;
    TcpSocketClient* socket;
    QString UserId;
    QString pwd;
private:
    void setTabWidget();//初始化TabWidget
    void initFriend();  //初始化好友
    void initEvent();   //初始化右键菜单事件
    void moveItemFriend(const char* friendId);          //删除对应的好友Item
    void addNewItem(QString friendId,QString status);   //添加对应的好友Item
    void onActionDelete();  //菜单删除操作
private:
    QMenu *popMenu_In_ListWidget_;/*弹出菜单*/
    QAction *action_Delete_In_ListWidget_;/*菜单上的Action*/
    VerifyMsg* VM;
};

#endif // MAINCHAT_H
