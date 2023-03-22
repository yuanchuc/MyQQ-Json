#ifndef LOGONVIEW_H
#define LOGONVIEW_H

#include <QWidget>
#include <QMessageBox>
#include"tcpsocketclient.h"
namespace Ui {
class logonView;
}

class logonView : public QWidget
{
    Q_OBJECT

public:
    explicit logonView(TcpSocketClient*s,QWidget *parent = nullptr);
    ~logonView();

private slots:
    void on_pushButton_clicked();
    void hasMsgDeal(MyProtoMsg* header);
private:
    Ui::logonView *ui;
    TcpSocketClient*socket;
};

#endif // LOGONVIEW_H