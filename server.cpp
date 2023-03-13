
#include"EasyTcpServer.hpp"
#include<thread>

bool g_bRun = true;
void cmdThread() {
    //3������������
    while (true)
    {
        char cmdBuf[256] = {};
        scanf("%s", cmdBuf);
        //4��������
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
            //���ӳɹ���������ֵ��ʼ��
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
    //�û��˳�ʱ��ʼ������
    virtual void OnNetLeave(ClientSocket* pClient) {
        _clientCount--;
        //֪ͨ����������
        MyProtoMsg msgToFriend;
        msgToFriend.head.server = CMD_FRIEND_LOGOUT;
        msgToFriend.body["friendId"] = Json::Value(pClient->getUserID().c_str());
        SendToFriendForLogin(pClient, msgToFriend);
        //��ʼ������
        string updt = "update logininfo set cur_socket = null,status = 0 where id = '%s';";
        char targetString[1024];
        snprintf(targetString, sizeof(targetString), updt.c_str(), pClient->getUserID()); 
        MySQL.mysql_DML(targetString);
        printf("client<%d> leave\n", (int)pClient->sockfd());
    }
    //������Ϣ
    virtual void OnNetMsg(ClientSocket* pClient, MyProtoMsg* header) {
        _msgCount++;
       
        switch (header->head.server)
        {
        //�����ж�
        case CMD_LOGIN: LoginFun(pClient, header);
                      break;
        case CMD_LOGON: LogonFun(pClient, header);
                       break;
        case CMD_FRIEND_MAKE: FriendMaking(pClient, header);
            break;
        case CMD_GET_FRIEND: GetFriends(pClient, header);
            break;
        case CMD_DEL_FRIEND: DelFriend(pClient, header);
        }
    }
private:
    void DataInit() {
        string updt;
        updt.append("update logininfo set cur_socket = null,status = 0");
        MySQL.mysql_DML(updt.c_str());
    }
    //�û�����ʱ���д���
    void LoginFun(ClientSocket* pClient, MyProtoMsg* header) {

        printf("recvClient<Socket=%d>requset:CMD_LOGIN,dataLength:%d\n", (int)pClient->sockfd(), header->head.len);
        //�����ж��û������Ƿ���ȷ�Ĺ���
        string sql_1 = "select * from logininfo where id = '%s';";
        char targetString1[1024];
        snprintf(targetString1, sizeof(targetString1), sql_1.c_str(),header->body["userId"].asCString());
        MYSQL_RES* res = MySQL.mysql_select(targetString1);
        //��֤�������ȷ��
        bool pwd_flag = false;
        bool isLogin = false;
        MYSQL_ROW rows;
        while (rows = mysql_fetch_row(res)) {
            printf("password = %s\n", rows[1]);
            //����Ѿ�ע�ᣬ���ж�����
            if (!strcmp(rows[1], header->body["pwd"].asCString())) {
                pwd_flag = true;
                //����Ѿ�����
                if (!strcmp(rows[3], "1")) {
                    isLogin = true;
                }
                break;
            }
        }
        //------������Ϣ
        MyProtoMsg msg1;
        msg1.head.server = CMD_LOGIN_RESULT;
        //������ȷ
        if (pwd_flag == true) {
            //�˺��Ѿ�����
            if (isLogin) {
                msg1.body["result"] = Json::Value(0);
                msg1.body["data"] = Json::Value("Account has been logged in");
            }
            else {
                msg1.body["result"] = Json::Value(1);
                msg1.body["data"] = Json::Value("Login success");
                //���ݿ����ݸ���
                string sql_2 = "update logininfo set cur_socket = %s ,status = 1 where id = '%s'";
                char targetString2[1024];
                snprintf(targetString2, sizeof(targetString2), sql_2.c_str(), std::to_string((int)pClient->sockfd()).c_str(),header->body["userId"].asCString());
                MySQL.mysql_DML(targetString2);
                pClient->setUserId(header->body["userId"].asCString());
                //֪ͨ��������
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

        //�����ж��û������Ƿ���ȷ�Ĺ���
        printf("recvClient<Socket=%d>requset:CMD_LOGON,dataLength:%d\n", (int)pClient->sockfd(), header->head.len);
        string sql_1 = "select * from logininfo where id = '%d'";

        char targetString1[1024];
        snprintf(targetString1, sizeof(targetString1), sql_1.c_str(), header->body["userId"].asCString());
        MYSQL_RES* res = MySQL.mysql_select(targetString1);
        //��֤ע����Ϣ
        bool isLogon = false;
        MYSQL_ROW rows;
        
        while (rows = mysql_fetch_row(res)) {
            //����Ѿ�ע��
            isLogon = true;
        }
        MyProtoMsg msg;
        msg.head.server = CMD_LOGON;
        if (isLogon == true) {
            //�˺��Ѿ���ע��
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
        //��֤���û��Ƿ����
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
            //˫����֤������Ϣ
            bool isMakeFriend = false;
            string sql_2 = "select * from friendmaking where (selfUserId = '%s' and friendUserId = '%s') or (friendUserId = '%s' and selfUserId = '%s');";
            char targetString2[1024];
            snprintf(targetString2,sizeof(targetString2),sql_2.c_str(),header->body["selfUserId"].asCString(),
                header->body["friendUserId"].asCString(), header->body["selfUserId"].asCString(), 
                header->body["friendUserId"].asCString());
            MYSQL_RES* res_2 = MySQL.mysql_select(targetString2);
            MYSQL_ROW rows_2;
            while (rows_2 = mysql_fetch_row(res_2)) {
                isMakeFriend = true;
            }

            if (!isMakeFriend) {
                //������Ǻ���
                msg.body["result"] = 1;
                msg.body["data"] = Json::Value("Succeeded in adding a friend");
                string sql_4 = "insert into friendmaking values ('%s','%s',CURRENT_TIMESTAMP,UUID_SHORT());";
                char targetString4[1024];
                snprintf(targetString4, sizeof(targetString4), sql_4.c_str(), header->body["selfUserId"].asCString(), header->body["friendUserId"].asCString());
                MySQL.mysql_DML(targetString4);
                //��Ӧ����
                MyProtoMsg msgToFriend;
                msgToFriend.head.server = CMD_FRIEND_ADD;
                msgToFriend.body["friendUserId"] = Json::Value(header->body["selfUserId"]);
                SendToFriendForUpdate(pClient, header, msgToFriend);
                printf("insert successful");
            }
            else {
                //����Ǻ��� 
                msg.body["result"] = 0;
                msg.body["data"] = Json::Value("You're already friends");
            }
        }
        else {
            //������û�������
            msg.body["result"] = 0;
            msg.body["data"] = Json::Value("The user does not exist");
            printf("ERROR,no exists\n");
        }
        pClient->SendData(&msg);
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
        //����ɾ���¼�
        printf("recvClient<Socket=%d>requset:CMD_DEL_FRIEND,dataLength:%d\n", (int)pClient->sockfd(), header->head.len);
        string sql = "delete from friendmaking where (selfUserId = '%s' and friendUserId = '%s') or (selfUserId = '%s' and friendUserId = '%s');";
        char targetString[1024];
        snprintf(targetString, sizeof(targetString), 
            sql.c_str(), header->body["selfUserId"].asCString(), 
            header->body["friendUserId"].asCString(), header->body["friendUserId"].asCString(), 
            header->body["selfUserId"].asCString());
        bool ret = MySQL.mysql_DML(targetString);
        if (ret) {
            //�������
            MyProtoMsg msgToself;
            msgToself.head.server = CMD_DEL_FRIEND_RESULT;
            msgToself.body["result"] = Json::Value(1);
            msgToself.body["friendUserId"] = Json::Value(header->body["friendUserId"]);
            msgToself.body["data"] = Json::Value("Deleted successfully");
            pClient->SendData(&msgToself);
            //֪ͨ���ѱ�ɾ��
            MyProtoMsg msgToFriend;
            msgToFriend.head.server = CMD_FRIEND_REDUCE;
            msgToFriend.body["friendUserId"] = Json::Value(header->body["selfUserId"]);
            SendToFriendForUpdate(pClient, header, msgToFriend);
        }
        else {
            //�������
            MyProtoMsg msgToself;
            msgToself.head.server = CMD_DEL_FRIEND_RESULT;
            msgToself.body["result"] = Json::Value(0);
            msgToself.body["friendUserId"] = Json::Value(header->body["friendUserId"]);
            msgToself.body["data"] = Json::Value("ERROR,Deletion failure");
            pClient->SendData(&msgToself);
        }
        
        
    }
public:
    void SendToFriendForLogin(ClientSocket* pClient, MyProtoMsg& msgToFriend) {
        //֪ͨ��������
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
        //֪ͨ����ӵ����ߺ���
        string sql_1 = "select cur_socket from logininfo where id = '%s' and status = 1;";
        char targetString1[1024];
        snprintf(targetString1, sizeof(targetString1), sql_1.c_str(), header->body["friendUserId"].asCString());
        MYSQL_RES* res_1 = MySQL.mysql_select(targetString1);
        MYSQL_ROW rows_1;
        while (rows_1 = mysql_fetch_row(res_1)) {
            pClient->SendData(&msgToFriend, atoi(rows_1[0]));
        }
    }

};

int main() {
    MyServer server;
    server.InitSocket();
    server.Bind(nullptr, 4567);
    server.Listen(15);
    server.Start(4);
    //std::thread t1(cmdThread);
    //t1.detach();    //�������̣߳���ֹ���߳��˳������߳���δ�˳���������
    
    while (g_bRun) {
        server.OnRun(); 
    }
    server.Close();
    printf("The task is completed. Procedure\n");
    return 0;
}
