#ifndef LOGINVIEW_H
#define LOGINVIEW_H
//g++ server.cpp MyDatabase.o  -pthread -std=c++11
//-I /usr/include/mysql -L /usr/lib64/mysql -lmysqlclient
//-o server
#include <QMainWindow>
#include <QTcpSocket>
#include<QMessageBox>
#include<QDebug>
#include<QFile>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include<QTimer>
#include<QGraphicsOpacityEffect>
#include<QPropertyAnimation>
#include<QMovie>
#include <QMouseEvent>
#include<QToolButton>
#include <QJsonDocument>
#include<QCryptographicHash>
#include <QJsonParseError>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonArray>

#include"mainchat.h"
#include"logonview.h"
#include"tcpsocketclient.h"
#include"myproto.h"
#include"storageclass.hpp"


//用于转化IP地址
#include <QHostAddress>

namespace Ui {
class LoginView;
}

class LoginView : public QMainWindow
{
    Q_OBJECT

public:
    explicit LoginView(QWidget *parent = nullptr);
    ~LoginView();
    QString GetMd5(const QString &value);
private slots:
    void on_loginButton_clicked();  //登入按钮
    void on_logonButton_clicked();  //注册按钮
    void Connected();               //连接服务器信号处理
    void hasMsgDeal(MyProtoMsg* header);//接收服务器消息信号处理
    void onDisConnect();                //断开服务器消息信号处理
    void on_loginIdInput_editTextChanged(const QString &arg1);  //Id输入框信息变化处理
    void on_autoLoginCheck_stateChanged(int arg1);  //自动登入状态处理
    void windowclosed();//关闭
    void windowmin();   //最小化
    void on_cancelLoginButton_clicked();//停止登入按钮

private:
    Ui::LoginView *ui;
    QString IP;
    QString port;
    TcpSocketClient * socket;
    QSqlDatabase db;
    QGraphicsOpacityEffect* m_pOpacityClose;
    QPropertyAnimation * m_pAnimationClose;
    QPropertyAnimation * m_pAnimationOpen;
    QPropertyAnimation * m_pAnimationlogin;
    QPoint uiLoginPos;
private:
    //处理鼠标移动窗口
    void mousePressEvent(QMouseEvent* event);
    void mouseMoveEvent(QMouseEvent* event);
    void mouseReleaseEvent(QMouseEvent *event);
    //事件过滤器
    bool eventFilter(QObject *obj, QEvent *event);
    QPoint last_mouse_position_;
private:
    QPixmap minPix;
    QPixmap closePix;
    bool loginStatus;
private:
    void createToolButton();    //创建最小化和关闭按钮
    void readComBoxItem();      //获取已经登入的账号
    void writeQComBoxItem();    //保存登入成功的账号信息
    void connectDatabase();     //连接sqlite数据库
    void initAnimation();       //初始化动画
    void loginViewReduce();     //登入视图还原
    void loginViewMove();       //登入视图动画启动
    void mainViewOpen();        //主窗口出现动画
    void mainViewClose();       //主窗口关闭动画
    void createNewFrame(Info* io); //创建登入后窗口

};

#endif // LOGINVIEW_H
