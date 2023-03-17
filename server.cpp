
#include"EasyTcpServer.hpp"
#include<thread>
#include<ctime>
bool g_bRun = true;
void cmdThread() {
    //3输入请求命令
    while (true)
    {
        char cmdBuf[256] = {};
        scanf("%s", cmdBuf);
        //4处理请求
        if (0 == strcmp(cmdBuf, "exit")) {
            g_bRun = false;
            printf("recv exit,task is over \n");
            return;
        }
        else {
            printf("EPPOR , If the command is not supported, enter it again\n");
        }
    }
}
class MyServer:public EasyTcpServer
{
    MyDatabase MySQL;
    
public:
    MyServer() {
        if (MySQL.UserInfo_connect()) {
            printf("The server is successfully connected to the database\n");
            //连接成功，进行数值初始化
            DataInit();
        }
        else {
            printf("The server failed to connect to the database\n");
        }
    }
    ~MyServer() {
        
    }
public:
    virtual void OnNetJoin(ClientSocket* pClient) {
        _clientCount++;
        printf("client<%d> join\n", (int)pClient->sockfd());
    }
    //用户退出时初始化数据
    virtual void OnNetLeave(ClientSocket* pClient) {
        _clientCount--;
        //通知好友已下线
        MyProtoMsg msgToFriend;
        msgToFriend.head.server = CMD_FRIEND_LOGOUT;
        msgToFriend.body["friendId"] = Json::Value(pClient->getUserID().c_str());
        SendToFriendForLogin(pClient, msgToFriend);
        //初始化数据
        string updt = "update logininfo set cur_socket = null,status = 0 where id = '%s';";
        char targetString[1024];
        snprintf(targetString, sizeof(targetString), updt.c_str(), pClient->getUserID()); 
        MySQL.mysql_DML(targetString);
        printf("client<%d> leave\n", (int)pClient->sockfd());
    }
    //处理信息
    virtual void OnNetMsg(ClientSocket* pClient, MyProtoMsg* header) {
        _msgCount++;
       
        switch (header->head.server)
        {
        //登入判断
        case CMD_LOGIN: LoginFun(pClient, header);
                      break;
        case CMD_LOGON: LogonFun(pClient, header);
                       break;
        case CMD_FRIEND_MAKE: FriendMaking(pClient, header);
            break;
        case CMD_FRIEND_MAKE_SURE_RESULT: FriendSure(pClient, header);
            break;
        case CMD_GET_FRIEND: GetFriends(pClient, header);
            break;
        case CMD_DEL_FRIEND: DelFriend(pClient, header); 
            break;
        case CMD_GET_VERIFY_MSG:GetVerifyMsg(pClient, header);
            break;
        }
    }
private:
    void DataInit() {
        string updt;
        updt.append("update logininfo set cur_socket = null,status = 0");
        MySQL.mysql_DML(updt.c_str());
    }
    //用户登入时进行处理
    void LoginFun(ClientSocket* pClient, MyProtoMsg* header) {

        printf("recvClient<Socket=%d>requset:CMD_LOGIN,dataLength:%d\n", (int)pClient->sockfd(), header->head.len);
        //忽略判断用户密码是否正确的过程
        string sql_1 = "select * from logininfo where id = '%s';";
        char targetString1[1024];
        snprintf(targetString1, sizeof(targetString1), sql_1.c_str(),header->body["userId"].asCString());
        MYSQL_RES* res = MySQL.mysql_select(targetString1);
        //验证密码的正确性
        bool pwd_flag = false;
        bool isLogin = false;
        MYSQL_ROW rows;
        while (rows = mysql_fetch_row(res)) {
            printf("password = %s\n", rows[1]);
            //如果已经注册，则判断密码
            if (!strcmp(rows[1], header->body["pwd"].asCString())) {
                pwd_flag = true;
                //如果已经登入
                if (!strcmp(rows[3], "1")) {
                    isLogin = true;
                }
                break;
            }
        }
        //------放入消息
        MyProtoMsg msg1;
        msg1.head.server = CMD_LOGIN_RESULT;
        //密码正确
        if (pwd_flag == true) {
            //账号已经登入
            if (isLogin) {
                msg1.body["result"] = Json::Value(0);
                msg1.body["data"] = Json::Value("Account has been logged in");
            }
            else {
                msg1.body["result"] = Json::Value(1);
                msg1.body["data"] = Json::Value("Login success");
                //数据库数据更新
                string sql_2 = "update logininfo set cur_socket = %s ,status = 1 where id = '%s'";
                char targetString2[1024];
                snprintf(targetString2, sizeof(targetString2), sql_2.c_str(), std::to_string((int)pClient->sockfd()).c_str(),header->body["userId"].asCString());
                MySQL.mysql_DML(targetString2);
                pClient->setUserId(header->body["userId"].asCString());
                //通知好友上线
                MyProtoMsg msgToFriend;
                msgToFriend.head.server = CMD_FRIEND_LOGIN;
                msgToFriend.body["friendId"] = Json::Value(header->body["userId"].asCString());
                SendToFriendForLogin(pClient,msgToFriend);
            }
        }
        else {
            msg1.body["result"] = Json::Value(-1);
            msg1.body["data"] = Json::Value("Password error");
        }
        pClient->SendData(&msg1);
    }
    void LogonFun(ClientSocket* pClient, MyProtoMsg* header) {

        //忽略判断用户密码是否正确的过程
        printf("recvClient<Socket=%d>requset:CMD_LOGON,dataLength:%d\n", (int)pClient->sockfd(), header->head.len);
        
        MyProtoMsg msg;
        msg.head.server = CMD_LOGON;
        if (isLogon(header->body["userId"].asCString())) {
            //账号已经被注册
            msg.body["result"] = Json::Value(0);
            msg.body["data"] = Json::Value("The account has been registered");
            printf("ERROR:UserName is logoned\n");
        }
        else {
            string updt = "insert into logininfo values('%s','%s',null,0);";
            char targetString[1024];
            snprintf(targetString, sizeof(targetString), header->body["userId"].asCString(), header->body["pwd"].asCString());
            MySQL.mysql_DML(targetString);
            //ret.result = 1;
            printf("SUCCESS:UserName is logoning\n");
            msg.body["result"] = Json::Value(1);
            msg.body["data"] = Json::Value("Account registration succeeded");
        }
        pClient->SendData(&msg);
    }
    void FriendMaking(ClientSocket* pClient, MyProtoMsg* header) {
        
        printf("recvClient<Socket=%d>requset:CMD_FRIEND_MAKE,dataLength:%d\n", (int)pClient->sockfd(), header->head.len);
        MyProtoMsg msg;
        msg.head.server = CMD_FRIEND_MAKE_RESULT;
        //如果该用户存在
        if (isExist(header->body["friendUserId"].asCString())) {
            msg.body["friendUserId"] = Json::Value(header->body["friendUserId"]);
            bool isfriend = isFriend(header->body["selfUserId"].asCString(), header->body["friendUserId"].asCString());
            //如果不是朋友
            if (!isfriend) {
                //响应更新
                MyProtoMsg msgToFriend;
                msgToFriend.head.server = CMD_FRIEND_MAKE_SURE;
                //查询登入状态
                if (isLogin(header->body["friendUserId"].asCString())) {
                    msgToFriend.body["status"] = Json::Value("1");
                }
                else {
                    msgToFriend.body["status"] = Json::Value("0");
                }
                //存储好友添加信息
                string sql_4 = "insert into verifymsg value('%s','%s',CURRENT_TIMESTAMP,'%s',0)";
                char targetString4[1024];
                snprintf(targetString4, sizeof(targetString4), sql_4.c_str(), header->body["friendUserId"].asCString(), header->body["selfUserId"].asCString(), header->body["extraMsg"].asCString());
                MySQL.mysql_DML(targetString4);
                //补充数据
                cout << measureTime() << endl;
                msgToFriend.body["friendUserId"] = Json::Value(header->body["selfUserId"]);
                msgToFriend.body["extraMsg"] = Json::Value(header->body["extraMsg"]);
                msgToFriend.body["insertDate"] = Json::Value(measureTime());
                SendToFriendForUpdate(pClient, header, msgToFriend);
                printf("insert successful");
            }
            else {
                //如果是好友 
                msg.body["result"] = 0;
                msg.body["data"] = Json::Value("You're already friends");
                pClient->SendData(&msg);
            }
        }
        else {
            //如果该用户不存在
            msg.body["result"] = 0;
            msg.body["data"] = Json::Value("The user does not exist");
            printf("ERROR,no exists\n");
            pClient->SendData(&msg);
        }
        
    }
    void FriendSure(ClientSocket* pClient, MyProtoMsg* header)   {
        printf("recvClient<Socket=%d>requset:CMD_FRIEND_MAKE_SURE_RESULT,dataLength:%d\n", (int)pClient->sockfd(), header->head.len);
        //如果同意添加好友
        if (header->body["result"].asInt() == 1) {
            //处理验证消息状态
            string sql_2 = "update verifymsg set status = 1 where selfUserId = '%s' and friendUserId = '%s'and status = 0";
            char targetString2[1024];
            snprintf(targetString2, sizeof(targetString2), sql_2.c_str(), header->body["selfUserId"].asCString(), header->body["friendUserId"].asCString());
            MySQL.mysql_DML(targetString2);

            if (isFriend(header->body["selfUserId"].asCString(), header->body["friendUserId"].asCString())) {
                return;
            }

            MyProtoMsg msgToFriend;
            msgToFriend.head.server = CMD_FRIEND_ADD;
            msgToFriend.body["status"] = Json::Value("1");
            msgToFriend.body["result"] = Json::Value(1);
            msgToFriend.body["friendUserId"] = Json::Value(header->body["selfUserId"]);
            msgToFriend.body["data"] = Json::Value("Succeeded in adding a friend");
            //同步好友状态
            string sql_1 = "insert into friendmaking values ('%s','%s',CURRENT_TIMESTAMP,UUID_SHORT());";
            char targetString1[1024];
            snprintf(targetString1, sizeof(targetString1), sql_1.c_str(), header->body["selfUserId"].asCString(), header->body["friendUserId"].asCString());
            MySQL.mysql_DML(targetString1);
            
            //发送数据到客户端
            SendToFriendForUpdate(pClient, header, msgToFriend);
            //查询好友当前状态
            
            MyProtoMsg msgToSelf;
            msgToSelf.head.server = CMD_FRIEND_ADD;
            if (isLogin(header->body["friendUserId"].asCString())) {
                msgToSelf.body["status"] = Json::Value("1");
            }
            else
            {
                msgToSelf.body["status"] = Json::Value("0");
            }
            msgToSelf.body["result"] = Json::Value(1);
            msgToSelf.body["friendUserId"] = Json::Value(header->body["friendUserId"]);
            msgToSelf.body["data"] = Json::Value("Succeeded in adding a friend");
            pClient->SendData(&msgToSelf);
        }
        else {
            //处理验证消息状态
            string sql_1 = "update verifymsg set status = -1 where selfUserId = '%s' and friendUserId = '%s'and status = 0";
            char targetString1[1024];
            snprintf(targetString1, sizeof(targetString1), sql_1.c_str(), header->body["selfUserId"].asCString(), header->body["friendUserId"].asCString());
            MySQL.mysql_DML(targetString1);
        }
    }
    void GetFriends(ClientSocket* pClient, MyProtoMsg* header) {
        printf("recvClient<Socket=%d>requset:CMD_GET_FRIEND,dataLength:%d\n", (int)pClient->sockfd(), header->head.len);
        string sql_1 = "select selfUserId,friendUserId,addDate,recordId,cur_socket,`status`\
            from friendmaking JOIN logininfo on id != '%s' \
            WHERE (selfUserId=id or friendUserId = id)  and (selfUserId='%s' or friendUserId = '%s');";
        char targetString1[1024];
        snprintf(targetString1, sizeof(targetString1), sql_1.c_str(), header->body["selfUserId"].asCString(), header->body["selfUserId"].asCString(), header->body["selfUserId"].asCString());
        MYSQL_RES* res_1 = MySQL.mysql_select(targetString1);
        MYSQL_ROW rows_1;
        
        MyProtoMsg root;
        root.head.server = CMD_GET_FRIEND_RESULT;
        while (rows_1 = mysql_fetch_row(res_1)) {
            Json::Value temp;
            if (!strcmp(rows_1[0], header->body["selfUserId"].asCString())) {
                temp["friendUserId"] = Json::Value(rows_1[1]);
            }
            else {
                temp["friendUserId"] = Json::Value(rows_1[0]);
            }
            temp["addDate"] = Json::Value(rows_1[2]);
            temp["recordId"] = Json::Value(rows_1[3]);
            if (rows_1[4] == nullptr) {
                temp["cur_socket"] = Json::Value("NULL");
            }
            else {
                temp["cur_socket"] = Json::Value(rows_1[4]);
            }
            
            temp["status"] = Json::Value(rows_1[5]);
            root.body["friendInfo"].append(temp);
        }
        pClient->SendData(&root);
    }
    void DelFriend(ClientSocket* pClient, MyProtoMsg* header) {
        //处理删除事件
        printf("recvClient<Socket=%d>requset:CMD_DEL_FRIEND,dataLength:%d\n", (int)pClient->sockfd(), header->head.len);
        string sql = "delete from friendmaking where (selfUserId = '%s' and friendUserId = '%s') or (selfUserId = '%s' and friendUserId = '%s');";
        char targetString[1024];
        snprintf(targetString, sizeof(targetString), 
            sql.c_str(), header->body["selfUserId"].asCString(), 
            header->body["friendUserId"].asCString(), header->body["friendUserId"].asCString(), 
            header->body["selfUserId"].asCString());
        bool ret = MySQL.mysql_DML(targetString);
        if (ret) {
            //反馈结果
            MyProtoMsg msgToself;
            msgToself.head.server = CMD_DEL_FRIEND_RESULT;
            msgToself.body["result"] = Json::Value(1);
            msgToself.body["friendUserId"] = Json::Value(header->body["friendUserId"]);
            msgToself.body["data"] = Json::Value("Deleted successfully");
            pClient->SendData(&msgToself);
            //通知好友被删除
            MyProtoMsg msgToFriend;
            msgToFriend.head.server = CMD_FRIEND_REDUCE;
            msgToFriend.body["friendUserId"] = Json::Value(header->body["selfUserId"]);
            SendToFriendForUpdate(pClient, header, msgToFriend);
        }
        else {
            //反馈结果
            MyProtoMsg msgToself;
            msgToself.head.server = CMD_DEL_FRIEND_RESULT;
            msgToself.body["result"] = Json::Value(0);
            msgToself.body["friendUserId"] = Json::Value(header->body["friendUserId"]);
            msgToself.body["data"] = Json::Value("ERROR,Deletion failure");
            pClient->SendData(&msgToself);
        }
        
        
    }
    void GetVerifyMsg(ClientSocket* pClient, MyProtoMsg* header) {
        string sql_1 = "select * from verifymsg where selfUserId = '%s';";
        char targetString1[1024];
        snprintf(targetString1, sizeof(targetString1), sql_1.c_str(), header->body["selfUserId"].asCString());
        MYSQL_RES* res_1 = MySQL.mysql_select(targetString1);
        MYSQL_ROW rows_1;
        MyProtoMsg msg;
        msg.head.server = CMD_GET_VERIFY_MSG_RESULT;
        while (rows_1 = mysql_fetch_row(res_1)) {
            Json::Value temp;
            temp["friendUserId"] = Json::Value(rows_1[1]);
            temp["insertDate"] = Json::Value(rows_1[2]);
            temp["extraMsg"] = Json::Value(rows_1[3]);
            temp["status"] = Json::Value(rows_1[4]);
            msg.body["info"].append(temp);
        }
        pClient->SendData(&msg);
    }
public:
    void SendToFriendForLogin(ClientSocket* pClient, MyProtoMsg& msgToFriend) {
        //通知好友上线
        string sql_3 = "select id,cur_socket,`status`\
                                from friendmaking JOIN logininfo on id != '%s' \
                                WHERE (selfUserId=id or friendUserId = id)  and (selfUserId='%s' or friendUserId = '%s') and status = 1;";
        char targetString3[1024];
        snprintf(targetString3, sizeof(targetString3), sql_3.c_str(),pClient->getUserID().c_str(), pClient->getUserID().c_str(), pClient->getUserID().c_str());
        MYSQL_RES* res_3 = MySQL.mysql_select(targetString3);
        MYSQL_ROW rows_3;
        while (rows_3 = mysql_fetch_row(res_3)) {
            pClient->SendData(&msgToFriend, atoi(rows_3[1]));
        }
    }
    void SendToFriendForUpdate(ClientSocket* pClient, MyProtoMsg* header,MyProtoMsg& msgToFriend) {
        //通知已添加的在线好友
        string sql_1 = "select cur_socket from logininfo where id = '%s' and status = 1;";
        char targetString1[1024];
        snprintf(targetString1, sizeof(targetString1), sql_1.c_str(), header->body["friendUserId"].asCString());
        MYSQL_RES* res_1 = MySQL.mysql_select(targetString1);
        MYSQL_ROW rows_1;
        while (rows_1 = mysql_fetch_row(res_1)) {
            pClient->SendData(&msgToFriend, atoi(rows_1[0]));
        }
    }
    char* measureTime() {
        time_t nowtime;
        time(&nowtime); //获取1900年1月1日0点0分0秒到现在经过的秒数
        tm* p = localtime(&nowtime);
        string str = "%04d:%02d:%02d %02d:%02d:%02d";
        char time[1024];
        snprintf(time, sizeof(time), str.c_str(), p->tm_year + 1900, p->tm_mon + 1, p->tm_mday, p->tm_hour, p->tm_min, p->tm_sec);
        return time;
    }
    bool isFriend(const char * selfUserId,const char* friendUserId) {
        //双向验证好友信息
        bool isMakeFriend = false;
        string sql_2 = "select * from friendmaking where (selfUserId = '%s' and friendUserId = '%s') or (friendUserId = '%s' and selfUserId = '%s');";
        char targetString2[1024];
        snprintf(targetString2, sizeof(targetString2), sql_2.c_str(),
            selfUserId, friendUserId,
            selfUserId, friendUserId);
        MYSQL_RES* res_2 = MySQL.mysql_select(targetString2);
        MYSQL_ROW rows_2;
        while (rows_2 = mysql_fetch_row(res_2)) {
            isMakeFriend = true;
        }
        return isMakeFriend;
    }
    bool isExist(const char * UserId) {
        string sql_1 = "select * from logininfo where id = '%s'";
        char targetString1[1024];
        snprintf(targetString1, sizeof(targetString1), sql_1.c_str(), UserId);
        bool isExist = false;
        MYSQL_RES* res_1 = MySQL.mysql_select(targetString1);
        MYSQL_ROW rows_1;
        while (rows_1 = mysql_fetch_row(res_1)) {
            isExist = true;
        }
        return isExist;
    }
    bool isLogin(const char * UserId) {
        string sql = "select status from logininfo where id = '%s'";
        char targetString[1024];
        snprintf(targetString, sizeof(targetString), sql.c_str(), UserId);
        MYSQL_RES* res = MySQL.mysql_select(targetString);
        MYSQL_ROW rows;
        while (rows = mysql_fetch_row(res)) {
            if (!strcmp(rows[0],"1")) return true;
            else return false;
        }
    }
    bool isLogon(const char* UserId) {
        string sql_1 = "select * from logininfo where id = '%d'";
        char targetString1[1024];
        snprintf(targetString1, sizeof(targetString1), sql_1.c_str(), UserId);
        MYSQL_RES* res = MySQL.mysql_select(targetString1);
        MYSQL_ROW rows;
        while (rows = mysql_fetch_row(res)) {
            //如果已经注册
            return true;
        }
        return false;
    }
};

int main() {
        
    MyServer server;
    server.InitSocket();
    server.Bind(nullptr, 4567);
    server.Listen(15);
    server.Start(4);
    //std::thread t1(cmdThread);
    //t1.detach();    //分离主线程，防止主线程退出后，子线程仍未退出导致问题
    
    while (g_bRun) {
        server.OnRun(); 
    }
    server.Close();
    printf("The task is completed. Procedure\n");
    return 0;
}
