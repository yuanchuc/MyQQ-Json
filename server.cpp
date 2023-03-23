
#include"EasyTcpServer.hpp"
#include"ConnectionPool.h"
#include<thread>
#include<ctime>
bool g_bRun = true;

class MyServer:public EasyTcpServer
{
    //MyDatabase MySQL;
    ConnectionPool* pool = ConnectionPool::getConnectPool();
public:
    MyServer() {

    }
    ~MyServer() {
        
    }
public:
    virtual void OnNetJoin(ClientSocket* pClient) {
        _clientCount++;
        printf("Message:  client<%d> join\n", (int)pClient->sockfd());
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
        shared_ptr<MysqlConn> MySQL = pool->getConnection();
        string updt = "update logininfo set cur_socket = null,status = 0 where id = '%s';";
        char targetString[1024];
        snprintf(targetString, sizeof(targetString), updt.c_str(), pClient->getUserID().c_str()); 
        MySQL->update(targetString);
        printf("Message:  client<%d> leave\n", (int)pClient->sockfd());
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
        shared_ptr<MysqlConn> MySQL = pool->getConnection();
        string updt;
        updt.append("update logininfo set cur_socket = null,status = 0");
        MySQL->update(updt.c_str());
    }
    //�û�����ʱ���д���
    void LoginFun(ClientSocket* pClient, MyProtoMsg* header) {

        printf("OpMsg:  recvClient<Socket=%d>requset:  <CMD_LOGIN>  ,dataLength:%d\n", (int)pClient->sockfd(), header->head.len);
        //�����ж��û������Ƿ���ȷ�Ĺ���
        shared_ptr<MysqlConn> MySQL = pool->getConnection();
        string sql_1 = "select * from logininfo where id = '%s';";
        char targetString1[1024];
        snprintf(targetString1, sizeof(targetString1), sql_1.c_str(),header->body["userId"].asCString());
        MySQL->query(targetString1);
        //��֤�������ȷ��
        bool pwd_flag = false;
        bool isLogin = false;
        bool isLogon = false;
        while (MySQL->next()) {
            //����Ѿ�ע�ᣬ���ж�����
            isLogon = true;
            if (!strcmp(MySQL->value(1).c_str(), header->body["pwd"].asCString())) {
                pwd_flag = true;
                //����Ѿ�����
                if (!strcmp(MySQL->value(3).c_str(), "1")) {
                    isLogin = true;
                }
                break;
            }
        }
        //------������Ϣ
        MyProtoMsg msg1;
        msg1.head.server = CMD_LOGIN_RESULT;
        //��ע��
        if (isLogon) {
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
                    snprintf(targetString2, sizeof(targetString2), sql_2.c_str(), std::to_string((int)pClient->sockfd()).c_str(), header->body["userId"].asCString());
                    MySQL->update(targetString2);
                    pClient->setUserId(header->body["userId"].asCString());
                    //֪ͨ��������
                    MyProtoMsg msgToFriend;
                    msgToFriend.head.server = CMD_FRIEND_LOGIN;
                    msgToFriend.body["friendId"] = Json::Value(header->body["userId"].asCString());
                    SendToFriendForLogin(pClient, msgToFriend);
                }
            }
            else {
                msg1.body["result"] = Json::Value(-1);
                msg1.body["data"] = Json::Value("Password error");
            }
        }
        else {
            msg1.body["result"] = Json::Value(-1);
            msg1.body["data"] = Json::Value("This account is not registered");
        }
        
        pClient->SendData(&msg1);
    }
    void LogonFun(ClientSocket* pClient, MyProtoMsg* header) {

        //�����ж��û������Ƿ���ȷ�Ĺ���
        printf("OpMsg:  recvClient<Socket=%d>requset:  <CMD_LOGON>  ,dataLength:%d\n", (int)pClient->sockfd(), header->head.len);
        
        MyProtoMsg msg;
        msg.head.server = CMD_LOGON_RESULT;
        shared_ptr<MysqlConn> MySQL = pool->getConnection();
        if (isLogon(header->body["userId"].asCString())) {
            //�˺��Ѿ���ע��
            msg.body["result"] = Json::Value(0);
            msg.body["data"] = Json::Value("The account has been registered");
        }
        else {
            string updt = "insert into logininfo values('%s','%s',null,0);";
            char targetString[1024];
            snprintf(targetString, sizeof(targetString),updt.c_str(),header->body["userId"].asCString(), header->body["pwd"].asCString());
            MySQL->update(targetString);
            msg.body["result"] = Json::Value(1);
            msg.body["data"] = Json::Value("Account registration succeeded");
        }
        pClient->SendData(&msg);
    }
    void FriendMaking(ClientSocket* pClient, MyProtoMsg* header) {
        
        printf("OpMsg:  recvClient<Socket=%d>requset:  <CMD_FRIEND_MAKE>  ,dataLength:%d\n", (int)pClient->sockfd(), header->head.len);
        MyProtoMsg msg;
        msg.head.server = CMD_FRIEND_MAKE_RESULT;
        //������û�����
        shared_ptr<MysqlConn> MySQL = pool->getConnection();
        if (isExist(header->body["friendUserId"].asCString())) {
            msg.body["friendUserId"] = Json::Value(header->body["friendUserId"]);
            bool isfriend = isFriend(header->body["selfUserId"].asCString(), header->body["friendUserId"].asCString());
            //�����������
            if (!isfriend) {
                //��Ӧ����
                MyProtoMsg msgToFriend;
                msgToFriend.head.server = CMD_FRIEND_MAKE_SURE;
                //��ѯ����״̬
                if (isLogin(header->body["friendUserId"].asCString())) {
                    msgToFriend.body["status"] = Json::Value("1");
                }
                else {
                    msgToFriend.body["status"] = Json::Value("0");
                }
                //�洢���������Ϣ
                string sql_4 = "insert into verifymsg value('%s','%s',CURRENT_TIMESTAMP,'%s',0)";
                char targetString4[1024];
                snprintf(targetString4, sizeof(targetString4), sql_4.c_str(), header->body["friendUserId"].asCString(), header->body["selfUserId"].asCString(), header->body["extraMsg"].asCString());
                MySQL->update(targetString4);
                //��������
                char* curTime = measureTime();
                cout << curTime << endl;
                msgToFriend.body["friendUserId"] = Json::Value(header->body["selfUserId"]);
                msgToFriend.body["extraMsg"] = Json::Value(header->body["extraMsg"]);
                msgToFriend.body["insertDate"] = Json::Value(curTime);
                SendToFriendForUpdate(pClient, header, msgToFriend);
                delete[] curTime;
                printf("insert successful");
            }
            else {
                //����Ǻ��� 
                msg.body["result"] = 0;
                msg.body["data"] = Json::Value("You're already friends");
                pClient->SendData(&msg);
            }
        }
        else {
            //������û�������
            msg.body["result"] = 0;
            msg.body["data"] = Json::Value("The user does not exist");
            printf("ERROR,no exists\n");
            pClient->SendData(&msg);
        }
        
    }
    void FriendSure(ClientSocket* pClient, MyProtoMsg* header)   {
        printf("OpMsg:  recvClient<Socket=%d>requset:  <CMD_FRIEND_MAKE_SURE_RESULT>  ,dataLength:%d\n", (int)pClient->sockfd(), header->head.len);
        //���ͬ����Ӻ���
        shared_ptr<MysqlConn> MySQL = pool->getConnection();
        if (header->body["result"].asInt() == 1) {
            //������֤��Ϣ״̬
            string sql_2 = "update verifymsg set status = 1 where selfUserId = '%s' and friendUserId = '%s'and status = 0";
            char targetString2[1024];
            snprintf(targetString2, sizeof(targetString2), sql_2.c_str(), header->body["selfUserId"].asCString(), header->body["friendUserId"].asCString());
            MySQL->update(targetString2);

            if (isFriend(header->body["selfUserId"].asCString(), header->body["friendUserId"].asCString())) {
                return;
            }

            MyProtoMsg msgToFriend;
            msgToFriend.head.server = CMD_FRIEND_ADD;
            msgToFriend.body["status"] = Json::Value("1");
            msgToFriend.body["result"] = Json::Value(1);
            msgToFriend.body["friendUserId"] = Json::Value(header->body["selfUserId"]);
            msgToFriend.body["data"] = Json::Value("Succeeded in adding a friend");
            //ͬ������״̬
            string sql_1 = "insert into friendmaking values ('%s','%s',CURRENT_TIMESTAMP,UUID_SHORT());";
            char targetString1[1024];
            snprintf(targetString1, sizeof(targetString1), sql_1.c_str(), header->body["selfUserId"].asCString(), header->body["friendUserId"].asCString());
            MySQL->update(targetString1);
            
            //�������ݵ��ͻ���
            SendToFriendForUpdate(pClient, header, msgToFriend);
            //��ѯ���ѵ�ǰ״̬
            
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
            //������֤��Ϣ״̬
            string sql_1 = "update verifymsg set status = -1 where selfUserId = '%s' and friendUserId = '%s'and status = 0";
            char targetString1[1024];
            snprintf(targetString1, sizeof(targetString1), sql_1.c_str(), header->body["selfUserId"].asCString(), header->body["friendUserId"].asCString());
            MySQL->update(targetString1);
        }
    }
    void GetFriends(ClientSocket* pClient, MyProtoMsg* header) {
        printf("OpMsg:  recvClient<Socket=%d>requset:  <CMD_GET_FRIEND>  ,dataLength:%d\n", (int)pClient->sockfd(), header->head.len);
        shared_ptr<MysqlConn> MySQL = pool->getConnection();
        string sql_1 = "select selfUserId,friendUserId,addDate,recordId,cur_socket,`status`\
            from friendmaking JOIN logininfo on id != '%s' \
            WHERE (selfUserId=id or friendUserId = id)  and (selfUserId='%s' or friendUserId = '%s');";
        char targetString1[1024];
        snprintf(targetString1, sizeof(targetString1), sql_1.c_str(), header->body["selfUserId"].asCString(), header->body["selfUserId"].asCString(), header->body["selfUserId"].asCString());
        MySQL->query(targetString1);
        MyProtoMsg root;
        root.head.server = CMD_GET_FRIEND_RESULT;
        while (MySQL->next()) {
            Json::Value temp;
            if (!strcmp(MySQL->value(0).c_str(), header->body["selfUserId"].asCString())) {
                temp["friendUserId"] = Json::Value(MySQL->value(1).c_str());
            }
            else {
                temp["friendUserId"] = Json::Value(MySQL->value(0).c_str());
            }
            temp["addDate"] = Json::Value(MySQL->value(2).c_str());
            temp["recordId"] = Json::Value(MySQL->value(3).c_str());
            if (MySQL->value(4).c_str() == nullptr) {
                temp["cur_socket"] = Json::Value("NULL");
            }
            else {
                temp["cur_socket"] = Json::Value(MySQL->value(4).c_str());
            }
            
            temp["status"] = Json::Value(MySQL->value(5).c_str());
            root.body["friendInfo"].append(temp);
        }
        pClient->SendData(&root);
    }
    void DelFriend(ClientSocket* pClient, MyProtoMsg* header) {
        //����ɾ���¼�
        printf("OpMsg:  recvClient<Socket=%d>requset:  <CMD_DEL_FRIEND>  ,dataLength:%d\n", (int)pClient->sockfd(), header->head.len);
        shared_ptr<MysqlConn> MySQL = pool->getConnection();
        string sql = "delete from friendmaking where (selfUserId = '%s' and friendUserId = '%s') or (selfUserId = '%s' and friendUserId = '%s');";
        char targetString[1024];
        snprintf(targetString, sizeof(targetString), 
            sql.c_str(), header->body["selfUserId"].asCString(), 
            header->body["friendUserId"].asCString(), header->body["friendUserId"].asCString(), 
            header->body["selfUserId"].asCString());
        bool ret = MySQL->update(targetString);
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
    void GetVerifyMsg(ClientSocket* pClient, MyProtoMsg* header) {
        printf("OpMsg:  recvClient<Socket=%d>requset:  <CMD_GET_VERIFY_MSG>  ,dataLength:%d\n", (int)pClient->sockfd(), header->head.len);
        shared_ptr<MysqlConn> MySQL = pool->getConnection();
        string sql_1 = "select * from verifymsg where selfUserId = '%s';";
        char targetString1[1024];
        snprintf(targetString1, sizeof(targetString1), sql_1.c_str(), header->body["selfUserId"].asCString());
        MySQL->query(targetString1);
        MyProtoMsg msg;
        msg.head.server = CMD_GET_VERIFY_MSG_RESULT;
        while (MySQL->next()) {
            Json::Value temp;
            temp["friendUserId"] = Json::Value(MySQL->value(1).c_str());
            temp["insertDate"] = Json::Value(MySQL->value(2).c_str());
            temp["extraMsg"] = Json::Value(MySQL->value(3).c_str());
            temp["status"] = Json::Value(MySQL->value(4).c_str());
            msg.body["info"].append(temp);
        }
        pClient->SendData(&msg);
    }
public:
    void SendToFriendForLogin(ClientSocket* pClient, MyProtoMsg& msgToFriend) {
        //֪ͨ��������
        shared_ptr<MysqlConn> MySQL = pool->getConnection();
        string sql_3 = "select id,cur_socket,`status`\
                                from friendmaking JOIN logininfo on id != '%s' \
                                WHERE (selfUserId=id or friendUserId = id)  and (selfUserId='%s' or friendUserId = '%s') and status = 1;";
        char targetString3[1024];
        snprintf(targetString3, sizeof(targetString3), sql_3.c_str(),pClient->getUserID().c_str(), pClient->getUserID().c_str(), pClient->getUserID().c_str());
        MySQL->query(targetString3);
        while (MySQL->next()) {
            pClient->SendData(&msgToFriend, atoi(MySQL->value(1).c_str()));
        }
    }
    void SendToFriendForUpdate(ClientSocket* pClient, MyProtoMsg* header,MyProtoMsg& msgToFriend) {
        //֪ͨ����ӵ����ߺ���
        shared_ptr<MysqlConn> MySQL = pool->getConnection();
        string sql_1 = "select cur_socket from logininfo where id = '%s' and status = 1;";
        char targetString1[1024];
        snprintf(targetString1, sizeof(targetString1), sql_1.c_str(), header->body["friendUserId"].asCString());
        MySQL->query(targetString1);
        while (MySQL->next()) {
            pClient->SendData(&msgToFriend, atoi(MySQL->value(0).c_str()));
        }
    }
    char* measureTime() {
        time_t nowtime;
        time(&nowtime); //��ȡ1900��1��1��0��0��0�뵽���ھ���������
        tm* p = localtime(&nowtime);
        string str = "%04d:%02d:%02d %02d:%02d:%02d";
        char* time = new char[1024];
        snprintf(time, sizeof(time), str.c_str(), p->tm_year + 1900, p->tm_mon + 1, p->tm_mday, p->tm_hour, p->tm_min, p->tm_sec);
        return time;
    }
    bool isFriend(const char * selfUserId,const char* friendUserId) {
        //˫����֤������Ϣ
        bool isMakeFriend = false;
        shared_ptr<MysqlConn> MySQL = pool->getConnection();
        string sql_2 = "select * from friendmaking where (selfUserId = '%s' and friendUserId = '%s') or (friendUserId = '%s' and selfUserId = '%s');";
        char targetString2[1024];
        snprintf(targetString2, sizeof(targetString2), sql_2.c_str(),
            selfUserId, friendUserId,
            selfUserId, friendUserId);
        MySQL->query(targetString2);

        while (MySQL->next()) {
            isMakeFriend = true;
        }
        return isMakeFriend;
    }
    bool isExist(const char * UserId) {
        shared_ptr<MysqlConn> MySQL = pool->getConnection();
        string sql_1 = "select * from logininfo where id = '%s'";
        char targetString1[1024];
        snprintf(targetString1, sizeof(targetString1), sql_1.c_str(), UserId);
        bool isExist = false;
        MySQL->query(targetString1);
        while (MySQL->next()) {
            isExist = true;
        }
        return isExist;
    }
    bool isLogin(const char * UserId) {
        shared_ptr<MysqlConn> MySQL = pool->getConnection();
        string sql = "select status from logininfo where id = '%s'";
        char targetString[1024];
        snprintf(targetString, sizeof(targetString), sql.c_str(), UserId);
        MySQL->query(targetString);
        while (MySQL->next()) {
            if (!strcmp(MySQL->value(0).c_str(),"1")) return true;
            else return false;
        }
        return false;
    }
    bool isLogon(const char* UserId) {
        shared_ptr<MysqlConn> MySQL = pool->getConnection();
        string sql_1 = "select * from logininfo where id = '%s'";
        char targetString1[1024];
        snprintf(targetString1, sizeof(targetString1), sql_1.c_str(), UserId);
        MySQL->query(targetString1);
        while (MySQL->next()) {
            //����Ѿ�ע��
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
    //t1.detach();    //�������̣߳���ֹ���߳��˳������߳���δ�˳���������
    
    while (g_bRun) {
        server.OnRun(); 
    }
    server.Close();
    printf("Message:  The task is completed. Procedure\n");
    return 0;
}
