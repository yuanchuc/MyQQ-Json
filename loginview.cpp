#include "loginview.h"
#include "ui_loginview.h"

LoginView::LoginView(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::LoginView)
{
    ui->setupUi(this);
    //安装事件过滤器
    ui->loginIdInput->installEventFilter(this);
    //初始化动画和框架内容
    ui->loginIdInput->setFocus();
    initAnimation();
    createToolButton();
    mainViewOpen();
    connectDatabase();
    //初始化
    readComBoxItem();

    //获取ip地址和端口号
    IP = "39.98.92.216";
    //IP = "127.0.0.1";
    port = "4567";
    //socket启动
    socket = new TcpSocketClient;
    socket->startConnect(IP,port.toShort());
    //连接服务器
    connect(socket,&TcpSocketClient::connected,this,&LoginView::Connected);
    //接收到消息时
    connect(socket,&TcpSocketClient::hasMsg,this,&LoginView::hasMsgDeal);
    //服务器断开连接时
    connect(socket,&TcpSocketClient::disConnect,this,&LoginView::onDisConnect);
    qDebug()<<ui->autoLoginCheck->checkState();
    if(ui->autoLoginCheck->checkState()==Qt::Checked) ui->loginButton->click();

}

LoginView::~LoginView()
{
    delete ui;
}

QString LoginView::GetMd5(const QString &value)
{
    QString md5;
    QByteArray bb;//相当于是QChar的一个vector<>
    bb = QCryptographicHash::hash(value.toUtf8(), QCryptographicHash::Md5);
    md5.append(bb.toHex());
    return md5;
}
void LoginView::on_loginButton_clicked()
{
    loginViewMove();
    ui->cancelLoginButton->show();
    loginStatus = true;

    QTimer::singleShot(2000,this,[&](){
        if(!loginStatus){
            ui->cancelLoginButton->hide();
            return;
        }

        QString userId = ui->loginIdInput->currentText();
        //MD5加密比对
        QString pwd =GetMd5(ui->loginPwdInput->text());
        MyProtoMsg msg1;
        //------放入消息
        msg1.head.server = CMD_LOGIN;
        qDebug()<<userId;
        msg1.body["userId"] = Json::Value(userId.toStdString().c_str()) ;
        msg1.body["pwd"] = Json::Value(pwd.toStdString().c_str());
        if(!socket->b_isConnectState) {
            loginViewReduce();
            ui->cancelLoginButton->hide();
            QMessageBox::warning(this,"登入提示","当前网络未连接");
            return;
        }else{
            socket->onSendData(msg1);
        }
    });
}

void LoginView::on_logonButton_clicked()
{

    logonView* logonview = new logonView(socket);
    //设置父窗口不能操作
    logonview->setWindowModality(Qt::ApplicationModal);
    logonview->show();
}

void LoginView::Connected()
{
    QPalette pa;
    pa.setColor(QPalette::WindowText,Qt::green);
    ui->statusLable->setText("在线");
    ui->statusLable->setPalette(pa);
}

void LoginView::hasMsgDeal(MyProtoMsg* header)
{
      switch (header->head.server) {
        case CMD_LOGIN_RESULT:{

            if(header->body["result"]==-1){
                QMessageBox::warning(this,"登入提示",QString::fromLocal8Bit(header->body["data"].asCString()));
                loginViewReduce();
                return;
            }else if(header->body["result"]==0){
                QMessageBox::warning(this,"登入提示",QString::fromLocal8Bit(header->body["data"].asCString()));
                loginViewReduce();
                return;
            }else if(header->body["result"]==1){
                Info* io =new Info(ui->loginIdInput->currentText(),header->body["userName"].asCString());
                io->setPwd(ui->loginPwdInput->text());
                io->setPhone(header->body["userPhone"].asCString());
                writeQComBoxItem();
                createNewFrame(io);
            }
        }break;
    }
}

void LoginView::onDisConnect()
{
    QPalette pa;
    pa.setColor(QPalette::WindowText,Qt::red);
    ui->statusLable->setText("离线");
    ui->statusLable->setPalette(pa);
}

void LoginView::readComBoxItem()
{
    //查询到所有登入过的账号

    int lastLogin = -1;
    int pos=0;
    QString Pwd = "";
    QString sql("select * from logininfo");
    QSqlQuery query(sql);
    auto pComboBoxA =ui->loginIdInput;

    while(query.next()){
        QString tempId = query.value(0).toString();
        QString last = query.value(4).toString();
        QString remPwd = query.value(2).toString();
        QString pwd = query.value(1).toString();
        pComboBoxA->addItem(tempId);
        if(last == "1"){
            lastLogin = pos;
            if(remPwd == "1"){
                Pwd = pwd;
                qDebug()<<Pwd;
            }
        }
        pos++;
    }
    //qDebug()<<Pwd;
    pComboBoxA->setCurrentIndex(lastLogin);
    ui->loginPwdInput->setText(Pwd);

}

void LoginView::writeQComBoxItem()
{
    //初始状态
    QString id = ui->loginIdInput->currentText();
    QString pwd = ui->loginPwdInput->text();
    int remStatus = 0;
    int autoStatus=0;
    int lastLogin = 1;
    if(ui->remPwdCheck->checkState()==Qt::Checked){
        remStatus = 1;
    }
    if(ui->autoLoginCheck->checkState()==Qt::Checked){
        autoStatus = 1;
    }
    //处理状态
    bool hasValue = false;
    QString sql = QString("select * from logininfo where userId = '%1';").arg(id);
    qDebug()<<sql;
    QSqlQuery query(sql);
    while(query.next()){
        qDebug()<<"go on";

        hasValue = true;
    }
    //初始化最后一次登入
    QString sql_init = QString("update logininfo set lastLogin = 0");
    QSqlQuery query_init(sql_init);
    //对用户登入信息进行更新
    QString sql_temp;
    if(hasValue){
        sql_temp = QString("update logininfo set remPwdStatus = %1,autoLogin = %2,lastLogin = %3 where userId = %4").arg(remStatus).arg(autoStatus).arg(lastLogin).arg(id);
        QSqlQuery query_temp(sql_temp);
    }else{
        sql_temp = QString("insert into logininfo values('%1','%2','%3','%4','%5');").arg(id).arg(pwd).arg(remStatus).arg(autoStatus).arg(lastLogin);
        QSqlQuery query_temp(sql_temp);
    }
    qDebug()<<sql_temp;
}

void LoginView::connectDatabase()
{
    //连接数据库
    this->db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("QQ.db");//设置一个数据库，创建一个数据库文件
    //判断数据库是否打开成功
    bool ok;
    ok = db.open();
    if(!ok)
    {
        qDebug()<<"数据库打开失败";
        db.lastError();
    }
    else
    {
        qDebug()<<"数据库打开成功";
    }
    //如果不存在则创建数据表
    QSqlQuery *m_pQuery = new QSqlQuery(db);
    if( m_pQuery->exec(QString("select count(*) from sqlite_master where type='table' and name='logininfo'")))  //关键的判断
    {
        m_pQuery->next();
        int x = m_pQuery->value(0).toInt();
        if( x == 0 )
        {
            QString strSql  = QString::fromLocal8Bit("create table logininfo([userId] VARCHAR(255) NOT NULL UNIQUE, \
                                                     [pwd] VARCHAR(255) NOT NULL, \
                                                     [remPwdStatus] SMALLINT NOT NULL DEFAULT 0, \
                                                     [autoLogin] SMALLINT NOT NULL DEFAULT 0, \
                                                     [lastLogin] SMALLINT NOT NULL DEFAULT 0);");
            if(!m_pQuery->exec( strSql ) )
            {
                QSqlError str = m_pQuery->lastError();
                //QMessageBox::information( this, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("警报信息数据库表创建失败!"));
                qDebug()<<"警报信息数据库表创建失败"<<endl;
            }
        }
    }
}
//初始化
void LoginView::initAnimation()
{
    //隐藏按钮
    ui->cancelLoginButton->hide();
    //去边框化
    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setWindowFlags(Qt::X11BypassWindowManagerHint | Qt::FramelessWindowHint);
    //背景动画
    QMovie * movie = new QMovie ("../MyQQ/background.gif");
    ui->backgroud->setMovie(movie);
    movie->start();
    //登入动画
    m_pAnimationlogin = new QPropertyAnimation();
    m_pAnimationlogin->setTargetObject(ui->loginViewWidget);    //设置使用动画的控件
    m_pAnimationlogin->setEasingCurve(QEasingCurve::Linear); //设置动画效果
    //主窗口关闭动画
    m_pAnimationOpen = new QPropertyAnimation(this, "windowOpacity");
    //主窗口关闭动画
    m_pAnimationClose = new QPropertyAnimation(this, "windowOpacity");
}

void LoginView::loginViewReduce()
{
    ui->cancelLoginButton->hide();
    m_pAnimationlogin->stop();
    m_pAnimationlogin->setPropertyName("pos");    //指定动画属性名
    m_pAnimationlogin->setDuration(250);    //设置动画时间（单位：毫秒）
    m_pAnimationlogin->setStartValue(ui->loginViewWidget->pos());  //设置动画起始位置在label控件当前的pos
    m_pAnimationlogin->setEndValue(uiLoginPos); //设置动画结束位置
    m_pAnimationlogin->start();   //启动动画
}

void LoginView::loginViewMove()
{

    m_pAnimationlogin->setPropertyName("pos");    //指定动画属性名
    m_pAnimationlogin->setDuration(250);    //设置动画时间（单位：毫秒）
    uiLoginPos = ui->loginViewWidget->pos();
    m_pAnimationlogin->setStartValue(ui->loginViewWidget->pos());  //设置动画起始位置在label控件当前的pos
    m_pAnimationlogin->setEndValue(ui->loginViewWidget->pos() + QPoint(0, 2100)); //设置动画结束位置
    m_pAnimationlogin->start();   //启动动画
}

void LoginView::mainViewOpen()
{
    //主窗口关闭动画
    m_pAnimationOpen->setDuration(500);
    m_pAnimationOpen->setStartValue(0);
    m_pAnimationOpen->setEndValue(1);
    m_pAnimationOpen->start();
}


void LoginView::mainViewClose()
{
    //主窗口关闭动画
    m_pAnimationClose->setDuration(500);
    m_pAnimationClose->setStartValue(1);
    m_pAnimationClose->setEndValue(0);
    m_pAnimationClose->start();
}

void LoginView::createNewFrame(Info* io)
{

    MainChat* MC = new MainChat(io,socket);
    QTimer::singleShot(500,this,[&](){
        this->hide();
    });
    mainViewClose();
    //disconnect(socket,&TcpSocketClient::hasMsg,this,&LoginView::hasMsgDeal);
    MC->show();
}

void LoginView::on_loginIdInput_editTextChanged(const QString &arg1)
{
    QString sql = QString("select pwd,remPwdStatus,autoLogin from logininfo where userId = '%1';").arg(arg1);
    QSqlQuery query(sql);
    QString pwd;
    bool isRemPwd = false;
    bool isAutoLogin = false;
    while(query.next()){
        pwd = query.value(0).toString();
        QString remPwd = query.value(1).toString();
        QString autoLogin = query.value(2).toString();
        if(remPwd == "1") isRemPwd = true;
        if(autoLogin == "1") isAutoLogin = true;
    }
    if(isRemPwd){
        ui->remPwdCheck->setCheckState(Qt::CheckState::Checked);
        ui->loginPwdInput->setText(pwd);
    }else{
        ui->loginPwdInput->clear();
        ui->remPwdCheck->setCheckState(Qt::CheckState::Unchecked);
    }
    if(isAutoLogin){
        ui->autoLoginCheck->setCheckState(Qt::CheckState::Checked);
    }else{
        ui->autoLoginCheck->setCheckState(Qt::CheckState::Unchecked);
    }
}

void LoginView::on_autoLoginCheck_stateChanged(int arg1)
{
    qDebug()<<arg1;
    if(arg1==2){
        ui->remPwdCheck->setCheckState(Qt::CheckState::Checked);
    }
}

void LoginView::windowclosed()
{
    this->close();
}

void LoginView::windowmin()
{
    this->showMinimized();//最小化
}

void LoginView::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
        last_mouse_position_ = event->globalPos() - this->pos();
}

void LoginView::mouseMoveEvent(QMouseEvent *event)
{
    if((event->buttons()==Qt::LeftButton) && (!this->isMaximized()) && (last_mouse_position_!=QPoint())){
            move(event->globalPos() - last_mouse_position_);
    }
}

void LoginView::mouseReleaseEvent(QMouseEvent *event)
{
    last_mouse_position_ = QPoint();
}

bool LoginView::eventFilter(QObject *obj, QEvent *event)
{
    if(obj == ui->loginIdInput){
        if(event->type() == QEvent::MouseMove){
            return true;
        }
    }
    return QWidget::eventFilter(obj,event);

}

void LoginView::createToolButton()
{
     //获取最小化、关闭按钮图标
    minPix  = style()->standardPixmap(QStyle::SP_TitleBarMinButton);
    closePix = style()->standardPixmap(QStyle::SP_TitleBarCloseButton);
    minPix.scaled(100,100);
    //设置最小化、关闭按钮图标
    ui->minButton->setIcon(minPix);
    ui->closeButton->setIcon(closePix);

    //设置鼠标移至按钮上的提示信息
    ui->minButton->setToolTip(tr("最小化"));
    ui->closeButton->setToolTip(tr("关闭"));
    //设置最小化、关闭按钮的样式
    ui->minButton->setStyleSheet("background-color:transparent;");
    ui->closeButton->setStyleSheet("background-color:transparent;");
    //每个按钮对应的信号槽
    connect(ui->closeButton, SIGNAL(clicked()), this, SLOT(windowclosed()));
    connect(ui->minButton, SIGNAL(clicked()), this, SLOT(windowmin()));

}

void LoginView::on_cancelLoginButton_clicked()
{
    loginStatus = false;
    loginViewReduce();
    ui->cancelLoginButton->hide();
}

