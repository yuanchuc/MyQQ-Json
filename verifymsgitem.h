#ifndef VERIFYMSGITEM_H
#define VERIFYMSGITEM_H

#include <QWidget>
#include"tcpsocketclient.h"
namespace Ui {
class VerifyMsgItem;
}

class VerifyMsgItem : public QWidget
{
    Q_OBJECT

public:
    explicit VerifyMsgItem(TcpSocketClient* s,QString UserId,QString friendId, QString extraMsg, QString insertDate, QString status,QWidget *parent = nullptr);
    ~VerifyMsgItem();

private slots:
    void on_agreeButton_clicked();

    void on_refuseButton_clicked();
private:
    Ui::VerifyMsgItem *ui;
    QString UserId;
    QString friendId;
    TcpSocketClient*socket;
};

#endif // VERIFYMSGITEM_H
