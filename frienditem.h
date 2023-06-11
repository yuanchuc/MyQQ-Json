#ifndef FRIENDITEM_H
#define FRIENDITEM_H

#include <QWidget>
#include<QMouseEvent>
#include<QMenu>
#include"tcpsocketclient.h"
#include"myproto.h"
#include"chatting.h"
#include"storageclass.hpp"
namespace Ui {
class friendItem;
}

class friendItem : public QWidget
{
    Q_OBJECT
public:
    explicit friendItem(TcpSocketClient* s,Info* psInfo,Info*fdInfo,QWidget *parent = nullptr);
    ~friendItem();
    void setFriendId(QString Id);
    void setStatus(QString status);
    QString getFriendId();
    //virtual void mousePressEvent(QMouseEvent *event);
    void mouseDoubleClickEvent(QMouseEvent* e);
private slots:
    void hasMsgDeal(MyProtoMsg* header);
    void onDisConnect();
private:
    TcpSocketClient* socket;
    Ui::friendItem *ui;
    Info* psInfo;
    Info* fdInfo;
private:

};

#endif // FRIENDITEM_H
