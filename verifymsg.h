#ifndef VERIFYMSG_H
#define VERIFYMSG_H

#include <QWidget>
#include"tcpsocketclient.h"
#include"verifymsgitem.h"
namespace Ui {
class VerifyMsg;
}

class VerifyMsg : public QWidget
{
    Q_OBJECT

public:
    explicit VerifyMsg(TcpSocketClient* s,QString UserId,QWidget *parent = nullptr);
    ~VerifyMsg();
public slots:
    void hasMsgDeal(MyProtoMsg* header);
public:
    void addNewItem(QString friendId,QString extraMsg,QString insertDate,QString status ="0");
    void initVerifyMsg();
private:

    Ui::VerifyMsg *ui;
    TcpSocketClient* socket;
    QString UserId;

};

#endif // VERIFYMSG_H
