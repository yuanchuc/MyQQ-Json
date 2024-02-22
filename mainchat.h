#ifndef MAINCHAT_H
#define MAINCHAT_H

#include <QWidget>
#include<QTcpSocket>
#include<vector>
#include<QMenuBar>
#include<QMenu>

#include <QToolBar>
#include<QCryptographicHash>
#include"tcpsocketclient.h"
#include"friendmaking.h"
#include"frienditem.h"
#include"verifymsg.h"
#include"storageclass.hpp"
namespace Ui {
class MainChat;
}

class MainChat : public QWidget
{
    Q_OBJECT

public:
    explicit MainChat(Info* const psInfo,TcpSocketClient* s,QWidget *parent = nullptr);
    ~MainChat();
    QString GetMd5(const QString &value);
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
    Info* const psInfo;
private:
    void setTabWidget();//初始化TabWidget
    void initFriend();  //初始化好友
    void initEvent();   //初始化右键菜单事件
    void moveItemFriend(const char* friendId);          //删除对应的好友Item
    void addNewItem(Info * const fdInfo);   //添加对应的好友Item
    void onActionDelete();  //菜单删除操作
    Info* setPersonInfo(Json::Value info);
private:
    QMenu *popMenu_In_ListWidget_;/*弹出菜单*/
    QAction *action_Delete_In_ListWidget_;/*菜单上的Action*/
    VerifyMsg* VM;
};

#endif // MAINCHAT_H
