#include "tcpsocketclient.h"

#include <QDebug>
TcpSocketClient::TcpSocketClient()
    : b_isConnectState(false)
    , m_strAddressIP("127.0.0.1"), m_iPort(0)
{
    m_TcpSocket = new QTcpSocket();
    connect(m_TcpSocket, SIGNAL(connected()),       this, SLOT(onConnect()));
    connect(m_TcpSocket, SIGNAL(disconnected()) ,   this, SLOT(onDisConnect()));
    connect(m_TcpSocket, SIGNAL(readyRead()),       this, SLOT(onRecvData()));
    connect(m_TcpSocket, SIGNAL(error(QAbstractSocket::SocketError)),
            this, SLOT(onErrorString(QAbstractSocket::SocketError)), Qt::DirectConnection);

    m_timerConnect = new QTimer();
    connect(m_timerConnect, &QTimer::timeout, [=](){
        if(b_isConnectState)
            return;
        // 这一部分又问题，实际状态有很多种
        //if(m_TcpSocket->state() == QAbstractSocket::ConnectingState)
        // 改为 等于未连接状态才开始连接，杜绝因为其他原因导致的连接
        if(m_TcpSocket->state() != QAbstractSocket::UnconnectedState)
            return;
        m_TcpSocket->waitForConnected();
        m_TcpSocket->connectToHost(m_strAddressIP, m_iPort);

    });
}

TcpSocketClient::~TcpSocketClient()
{
    m_TcpSocket->abort();
}

void TcpSocketClient::startConnect(const QString &strAddressIP, quint16 iPort)
{
    m_strAddressIP = strAddressIP;
    m_iPort = iPort;
    m_timerConnect->start(1000);
}

void TcpSocketClient::onConnect()
{
    b_isConnectState = true;
    qDebug() << "onConnect";
    emit connected();

    m_timerConnect->stop();
}

void TcpSocketClient::onDisConnect()
{
    b_isConnectState = false;
    qDebug() << "onDisConnect";
    emit disConnect();
    m_timerConnect->start(1000);
}

void TcpSocketClient::onErrorString(QAbstractSocket::SocketError errorString)
{
    qDebug() << errorString;
}

void TcpSocketClient::onRecvData()
{
    if(m_TcpSocket->state() != QAbstractSocket::ConnectedState)
        return;

    if(m_TcpSocket->bytesAvailable() < 0)
    {
        m_TcpSocket->disconnectFromHost();
        m_TcpSocket->waitForDisconnected(3000);
    }
    while (m_TcpSocket->bytesAvailable() > 0)
    {
        unsigned int len;
        char* Buf= new char[1024];
        len = (unsigned int)m_TcpSocket->read(Buf,1024);

        MyProtoDecode myDecode;
        myDecode.init();
        if (!myDecode.parser(Buf, len))
        {
            qDebug() << "parser msg failed!" << endl;
        }
        else
        {
            qDebug() << "parser msg successful!" << endl;
        }
        //------解析消息
        MyProtoMsg* pMsg = nullptr;
        while (!myDecode.empty())
        {
            pMsg = myDecode.front();
            cout<<"server = "<<pMsg->head.server<<endl;
            cout<<"body = "<<pMsg->body<<endl;
            emit hasMsg(pMsg);
            myDecode.pop();
        }

    }
}

void TcpSocketClient::onSendData(MyProtoMsg& msg1)
{
    if(m_TcpSocket->state() != QAbstractSocket::ConnectedState)
        return;
    uint32_t len = 0;
    uint8_t* pData = nullptr;
    MyProtoEncode myEncode;


    pData = myEncode.encode(&msg1, len);
    qint64 iSendDataLength = m_TcpSocket->write((const char*)pData,len);
    if(!m_TcpSocket->flush() || iSendDataLength < 0)
    {
        m_TcpSocket->disconnectFromHost();
        m_TcpSocket->waitForDisconnected(3000);
    }
}


