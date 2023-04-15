#ifndef VERIFYMSG_H
#define VERIFYMSG_H

#include <QWidget>
#include"tcpsocketclient.h"
#include"verifymsgitem.h"
#include"storageclass.hpp"
namespace Ui {
class VerifyMsg;
}

class VerifyMsg : public QWidget
{
    Q_OBJECT

public:
    explicit VerifyMsg(TcpSocketClient* s,Info *psInfo,QWidget *parent = nullptr);
    ~VerifyMsg();
public slots:
    void hasMsgDeal(MyProtoMsg* header);
public:
    void addNewItem(QString friendId,QString extraMsg,QString insertDate,QString status ="0");
    void initVerifyMsg();
private:

    Ui::VerifyMsg *ui;
    TcpSocketClient* socket;
    Info * psInfo;
};

#endif // VERIFYMSG_H
