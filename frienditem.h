#ifndef FRIENDITEM_H
#define FRIENDITEM_H

#include <QWidget>
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
private slots:

private:
    TcpSocketClient* socket;
    Ui::friendItem *ui;
    QString userId;
    QString result;
    QString selfId;
};

#endif // FRIENDITEM_H
