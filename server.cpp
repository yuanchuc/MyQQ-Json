
#include"EasyTcpServer.hpp"
#include<thread>

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
        printf("client<%d> join\n", pClient->sockfd());
    }
    //用户退出时初始化数据
    virtual void OnNetLeave(ClientSocket* pClient) {
        _clientCount--;
        
        string updt;
        updt.append("update logininfo set cur_socket = null,status = 0 where id = '");
        updt.append(pClient->getUserID());
        updt.append("'");  
        MySQL.mysql_DML(updt.c_str());
        printf("client<%d> leave\n", pClient->sockfd());
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
        case CMD_GET_FRIEND: GetFriends(pClient, header);
            break;
        //default: {
        //    //printf("Socket=<%d>收到未定义消息,数据长度：%d\n", (int)pClient->sockfd(), header->dataLength);
        //    /*DataHeader ret;
        //    SendData(cSock, &ret); */
        //}
        //       break;
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
                string sql_2 = "update logininfo set cur_socket = %d ,status = 1 where id = '%s'";
                char targetString2[1024];
                snprintf(targetString2, sizeof(targetString2), sql_2.c_str(), std::to_string(pClient->sockfd()).c_str(),header->body["userId"].asCString());
                MySQL.mysql_DML(targetString2);
                pClient->setUserId(header->body["userId"].asCString());
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
        string sql_1 = "select * from logininfo where id = '%d'";

        char targetString1[1024];
        snprintf(targetString1, sizeof(targetString1), sql_1.c_str(), header->body["userId"].asCString());
        MYSQL_RES* res = MySQL.mysql_select(targetString1);
        //验证注冊信息
        bool isLogon = false;
        MYSQL_ROW rows;
        
        while (rows = mysql_fetch_row(res)) {
            //如果已经注册
            isLogon = true;
        }
        MyProtoMsg msg;
        msg.head.server = CMD_LOGON;
        if (isLogon == true) {
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
        //验证该用户是否存在
        string sql_1 = "select * from logininfo where id = '%s'";
        char targetString1[1024];
        snprintf(targetString1, sizeof(targetString1), sql_1.c_str(), header->body["friendUserId"].asCString());
        bool isExist = false;
        MYSQL_RES* res_1 = MySQL.mysql_select(targetString1);
        MYSQL_ROW rows_1;
        while (rows_1 = mysql_fetch_row(res_1)) {
            isExist = true;
        }

        //strncpy(ret.friendId, FM->friendId,sizeof(FM->friendId));
        if (isExist) {
            //双向验证好友信息
            bool isMakeFriend = false;
            string sql_2 = "select * from friendmaking where selfUserId = '%s' and friendUserId = '%s';";
            char targetString2[1024];
            snprintf(targetString2,sizeof(targetString2),sql_2.c_str(),header->body["selfUserId"].asCString(), header->body["friendUserId"].asCString());
            MYSQL_RES* res_2 = MySQL.mysql_select(targetString2);
            MYSQL_ROW rows_2;
            while (rows_2 = mysql_fetch_row(res_2)) {
                isMakeFriend = true;
            }

            string sql_3 = "select * from friendmaking where friendUserId = '%s' and selfUserId = '%s';";
            char targetString3[1024];
            snprintf(targetString3, sizeof(targetString3), sql_3.c_str(), header->body["selfUserId"].asCString(), header->body["friendUserId"].asCString());
            MYSQL_RES* res_3 = MySQL.mysql_select(targetString3);
            MYSQL_ROW rows_3;
            while (rows_3 = mysql_fetch_row(res_3)) {
                isMakeFriend = true;
            }

            if (!isMakeFriend) {
                //如果不是好友
                msg.body["result"] = 1;
                msg.body["data"] = Json::Value("Succeeded in adding a friend");

                string sql_4 = "insert into friendmaking values ('%s','%s',CURRENT_TIMESTAMP,UUID_SHORT());";
                char targetString4[1024];
                snprintf(targetString4, sizeof(targetString4), sql_4.c_str(), header->body["selfUserId"].asCString(), header->body["friendUserId"].asCString());

                MySQL.mysql_DML(targetString4);
                printf("insert successful");
            }
            else {
                //如果是好友 
                msg.body["result"] = 0;
                msg.body["data"] = Json::Value("You're already friends");
            }
        }
        else {
            //如果该用户不存在
            msg.body["result"] = 0;
            msg.body["data"] = Json::Value("The user does not exist");
            printf("ERROR,no exists\n");
        }
        pClient->SendData(&msg);
    }
    void GetFriends(ClientSocket* pClient, MyProtoMsg* header) {
        printf("recvClient<Socket=%d>requset:CMD_GET_FRIEND,dataLength:%d\n", (int)pClient->sockfd(), header->head.len);
        string sql_1 = "select * from friendmaking where selfUserId = '%s' or friendUserId = '%s';";
        char targetString1[1024];
        snprintf(targetString1, sizeof(targetString1), sql_1.c_str(), header->body["selfUserId"].asCString(), header->body["selfUserId"].asCString());
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
            root.body["friendInfo"].append(temp);
        }
        pClient->SendData(&root);
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
