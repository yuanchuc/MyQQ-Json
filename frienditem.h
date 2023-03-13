#ifndef FRIENDITEM_H
#define FRIENDITEM_H

#include <QWidget>
#include<QMouseEvent>
#include<QMenu>
#include"tcpsocketclient.h"
#include"myproto.h"
namespace Ui {
class friendItem;
}

class friendItem : public QWidget
{
    Q_OBJECT
public:
    explicit friendItem(TcpSocketClient* s,QString selfId,QString userId,QString result,QWidget *parent = nullptr);
    ~friendItem();
    void setFriendId(QString Id);
    void setStatus(QString result);
    QString getFriendId();
    //virtual void mousePressEvent(QMouseEvent *event);
private slots:

private:
    TcpSocketClient* socket;
    Ui::friendItem *ui;
    QString userId;
    QString result;
    QString selfId;

private:

};

#endif // FRIENDITEM_H
