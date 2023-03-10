       #ifndef TCPSOCKETCLIENT_H
#define TCPSOCKETCLIENT_H

#include <QObject>
#include <QTcpSocket>
#include <QTimer>
#include <QThread>
#include<QMessageBox>
#include"myproto.h"
class TcpSocketClient :public QObject
{
    Q_OBJECT
public:
    TcpSocketClient();
    ~TcpSocketClient();
    void onSendData(MyProtoMsg& msg1);
    void startConnect(const QString& strAddressIP, quint16 iPort);
signals:
    void connected();
    void disConnect();
    void hasMsg(MyProtoMsg* header);
private:
    QTcpSocket *m_TcpSocket;

    QString m_strAddressIP;
    quint16 m_iPort;
    QTimer *m_timerConnect;
    QTimer *m_timerSend;
private slots:
    void onConnect();
    void onDisConnect();
    void onErrorString(QAbstractSocket::SocketError errorString);

    void onRecvData();
public:
    bool b_isConnectState;
};

#endif // TCPSOCKETCLIENT_H

