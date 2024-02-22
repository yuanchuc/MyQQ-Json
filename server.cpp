
#include"EasyTcpServer.hpp"
#include"ConnectionPool.h"
#include"IDGenerater.hpp"
#include<thread>
#include<ctime>
#include<signal.h>
bool g_bRun = true;

class MyServer:public EasyTcpServer
{
    //MyDatabase MySQL;
    ConnectionPool* pool = ConnectionPool::getConnectPool();                //���ݿ��
    NonLockIDGenerater*idGenerater = NonLockIDGenerater::GetInstance();    //����ID����(����ѩ���㷨)
public:
    MyServer() {
        shared_ptr<MysqlConn> MySQL = pool->getConnection();
        string updt = "update logininfo set cur_socket = null,status = 0;";
        MySQL->update(updt.c_str());
    }
    ~MyServer() {
        delete pool;
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
        case CMD_SEND_FRIEND_MSG:SendFriendMsg(pClient, header);
            break;
        case CMD_GET_FRIEND_MSG:GetFriendMsg(pClient, header);
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

        printf("OpMsg:  recvClient<Socket=%d>\nrequset:  <CMD_LOGIN>  ,dataLength:%d\n", (int)pClient->sockfd(), header->head.len);
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
        //------������Ϣ
        MyProtoMsg msg1;
        msg1.head.server = CMD_LOGIN_RESULT;
        while (MySQL->next()) {
            //����Ѿ�ע�ᣬ���ж�����
            isLogon = true;
            if (!strcmp(MySQL->value(2).c_str(), header->body["pwd"].asCString())) {
                pwd_flag = true;
                msg1.body["userName"] = Json::Value(MySQL->value(1).c_str());
                msg1.body["userPhone"] = Json::Value(MySQL->value(5).c_str());
                //����Ѿ�����
                if (!strcmp(MySQL->value(4).c_str(), "1")) {
                    isLogin = true;
                }
                break;
            }
        }
       
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
                    msgToFriend.body["cur_socket"] = Json::Value(to_string(pClient->sockfd()).c_str());
                    msgToFriend.body["userName"] = msg1.body["userPhone"];
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
        printf("OpMsg:  recvClient<Socket=%d>\nrequset:  <CMD_LOGON>  ,dataLength:%d\n", (int)pClient->sockfd(), header->head.len);
        
        MyProtoMsg msg;
        msg.head.server = CMD_LOGON_RESULT;
        shared_ptr<MysqlConn> MySQL = pool->getConnection();
        if (isLogon(header->body["UserPhone"].asCString())) {
            //�˺��Ѿ���ע��
            msg.body["result"] = Json::Value(0);
            msg.body["data"] = Json::Value("The phone has been registered");
        }
        else {
            string userName = header->body["UserName"].asString();
            string userPwd = header->body["UserPwd"].asString();
            string userPhone = header->body["UserPhone"].asString();
            string updt = "insert into logininfo values(null,'%s','%s',null,0,'%s');";
            char targetString[1024];
            snprintf(targetString, sizeof(targetString),updt.c_str(),userName.c_str(),userPwd.c_str(),userPhone.c_str());
            MySQL->update(targetString);
            //��ȡ�����Id
            string Id;
            getInfoByPhone(Id,userPhone.c_str());
            msg.body["result"] = Json::Value(1);
            msg.body["data"] = Json::Value("Account registration succeeded");
            msg.body["userId"] = Json::Value(Id.c_str());
        }
        pClient->SendData(&msg);
    }
    void FriendMaking(ClientSocket* pClient, MyProtoMsg* header) {
        printf("OpMsg:  recvClient<Socket=%d>\nrequset:  <CMD_FRIEND_MAKE>  ,dataLength:%d\n", (int)pClient->sockfd(), header->head.len);
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
                string sql_4 = "insert into verifymsg value(null,'%s','%s',CURRENT_TIMESTAMP,'%s',0)";
                char targetString4[1024];
                snprintf(targetString4, sizeof(targetString4),sql_4.c_str(),
                    header->body["friendUserId"].asCString(), header->body["selfUserId"].asCString(), 
                    header->body["extraMsg"].asCString());
                MySQL->update(targetString4);
                //��������
                char* curTime = measureTime();
                cout << curTime << endl;
                msgToFriend.body["friendUserId"] = Json::Value(header->body["selfUserId"]);
                msgToFriend.body["extraMsg"] = Json::Value(header->body["extraMsg"]);
                msgToFriend.body["insertDate"] = Json::Value(curTime);
                SendToFriendForUpdate(pClient, header, msgToFriend);
                delete[] curTime;
                printf("insert successful\n");
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
        printf("OpMsg:  recvClient<Socket=%d>\nrequset:  <CMD_FRIEND_MAKE_SURE_RESULT>  ,dataLength:%d\n", (int)pClient->sockfd(), header->head.len);
        //���ͬ����Ӻ���
        shared_ptr<MysqlConn> MySQL = pool->getConnection();
        if (header->body["result"].asInt() == 1) {
            //������֤��Ϣ״̬
            string sql_1 = "update verifymsg set status = 1 where selfUserId = '%s' and friendUserId = '%s'and status = 0";
            char targetString1[1024];
            snprintf(targetString1, sizeof(targetString1), sql_1.c_str(), header->body["selfUserId"].asCString(), header->body["friendUserId"].asCString());
            MySQL->update(targetString1);

            if (isFriend(header->body["selfUserId"].asCString(), header->body["friendUserId"].asCString())) {
                return;
            }

            MyProtoMsg msgToFriend;
            msgToFriend.head.server = CMD_FRIEND_ADD;
            msgToFriend.body["result"] = Json::Value(1);
            msgToFriend.body["data"] = Json::Value("Succeeded in adding a friend");
            long long recorldId = idGenerater->NextID();
            //ͬ������״̬
            string sql_2 = "insert into friendmaking values ('%s','%s',CURRENT_TIMESTAMP,'%lld');";
            char targetString2[1024];
            snprintf(targetString2, sizeof(targetString2), sql_2.c_str(),
                header->body["selfUserId"].asCString(), header->body["friendUserId"].asCString(),
                recorldId);
            MySQL->update(targetString2);
            
            string sql_3 = "insert into friendmaking values ('%s','%s',CURRENT_TIMESTAMP,'%lld');";
            char targetString3[1024];
            snprintf(targetString3, sizeof(targetString3), sql_2.c_str(),
                header->body["friendUserId"].asCString(), header->body["selfUserId"].asCString(),
                recorldId);
            MySQL->update(targetString3);
            //�������ݵ��ͻ���
            getSpFriendInfo(msgToFriend, header->body["friendUserId"].asCString(), header->body["selfUserId"].asCString());
            SendToFriendForUpdate(pClient, header, msgToFriend);
            //��ѯ���ѵ�ǰ״̬
            MyProtoMsg msgToSelf;
            msgToSelf.head.server = CMD_FRIEND_ADD;
            getSpFriendInfo(msgToSelf, header->body["selfUserId"].asCString(),header->body["friendUserId"].asCString());
            msgToSelf.body["result"] = Json::Value(1);
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
        printf("OpMsg:  recvClient<Socket=%d>\nrequset:  <CMD_GET_FRIEND>  ,dataLength:%d\n", (int)pClient->sockfd(), header->head.len);

        shared_ptr<MysqlConn> MySQL = pool->getConnection();
        string sql_1 = "select selfUserId,friendUserId,addDate,recordId,name,cur_socket,`status`,phone from friendmaking JOIN logininfo on friendUserId = id WHERE selfUserId = '%s';";
        char targetString1[1024];
        snprintf(targetString1, sizeof(targetString1), sql_1.c_str(), header->body["selfUserId"].asCString());
        MySQL->query(targetString1);
        MyProtoMsg root;
        root.head.server = CMD_GET_FRIEND_RESULT;
        while (MySQL->next()) {
            Json::Value temp;
            temp["friendUserId"] = Json::Value(MySQL->value(1).c_str());
            temp["addDate"] = Json::Value(MySQL->value(2).c_str());
            temp["recordId"] = Json::Value(MySQL->value(3).c_str());
            temp["userName"] = Json::Value(MySQL->value(4).c_str());
            temp["cur_socket"] = (MySQL->value(5).c_str() == nullptr) ? Json::Value("NULL") : Json::Value(MySQL->value(5).c_str());
            temp["status"] = Json::Value(MySQL->value(6).c_str());
            temp["phone"] = Json::Value(MySQL->value(7).c_str());
            root.body["friendInfo"].append(temp);
        }
        pClient->SendData(&root);
    }
    void DelFriend(ClientSocket* pClient, MyProtoMsg* header) {
        //����ɾ���¼�
        printf("OpMsg:  recvClient<Socket=%d>\nrequset:  <CMD_DEL_FRIEND>  ,dataLength:%d\n", (int)pClient->sockfd(), header->head.len);
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
        printf("OpMsg:  recvClient<Socket=%d>\nrequset:  <CMD_GET_VERIFY_MSG>  ,dataLength:%d\n", (int)pClient->sockfd(), header->head.len);
        shared_ptr<MysqlConn> MySQL = pool->getConnection();
        string sql_1 = "select * from verifymsg where selfUserId = '%s';";
        char targetString1[1024];
        snprintf(targetString1, sizeof(targetString1), sql_1.c_str(), header->body["selfUserId"].asCString());
        MySQL->query(targetString1);
        MyProtoMsg msg;
        msg.head.server = CMD_GET_VERIFY_MSG_RESULT;
        while (MySQL->next()) {
            Json::Value temp;
            temp["friendUserId"] = Json::Value(MySQL->value(2).c_str());
            temp["insertDate"] = Json::Value(MySQL->value(3).c_str());
            temp["extraMsg"] = Json::Value(MySQL->value(4).c_str());
            temp["status"] = Json::Value(MySQL->value(5).c_str());
            msg.body["info"].append(temp);
        }
        pClient->SendData(&msg);
    }
    void SendFriendMsg(ClientSocket* pClient, MyProtoMsg* header) {
        printf("OpMsg:  recvClient<Socket=%d>\nrequset:  <CMD_SEND_FRIEND_MSG>  ,dataLength:%d\n", (int)pClient->sockfd(), header->head.len);
        const char* content = header->body["content"].asCString();
        const char* userId = header->body["userId"].asCString();
        const char* friendId = header->body["friendUserId"].asCString();

        //���ݴ洢
        string recordId = getRecordId(userId, friendId);
        insertMsg(userId,content,recordId.c_str());
        //ת������
        MyProtoMsg msg;
        msg.head.server = CMD_RECV_FRIEND_MSG;
        if (!content) {
            return;
        }
        msg.body["content"] = Json::Value(content);
        msg.body["friendUserId"] = Json::Value(friendId);
        msg.body["userId"] = Json::Value(userId);
        SendToFriendForUpdate(pClient,header,msg);
        pClient->SendData(&msg);
    }
    void GetFriendMsg(ClientSocket* pClient, MyProtoMsg* header) {
        printf("OpMsg:  recvClient<Socket=%d>\nrequset:  <CMD_GET_FRIEND_MSG>  ,dataLength:%d\n", (int)pClient->sockfd(), header->head.len);
        const char* userId = header->body["userId"].asCString();
        const char* friendId = header->body["friendUserId"].asCString();
        string recordId = getRecordId(userId, friendId);
        Json::Value root = getRecordMsgById(recordId.c_str());


        //��������
        MyProtoMsg msg;
        msg.head.server = CMD_GET_FRIEND_MSG;
        msg.body = root;
        pClient->SendData(&msg);
    }
public:

    void SendToFriendForLogin(ClientSocket* pClient, MyProtoMsg& msgToFriend) {
        //֪ͨ��������
        shared_ptr<MysqlConn> MySQL = pool->getConnection();
        string sql = "select id,cur_socket,`status` from friendmaking JOIN logininfo on id = friendUserId WHERE selfUserId='%s' and status = 1;";
        char targetString[1024];
        snprintf(targetString, sizeof(targetString), sql.c_str(),pClient->getUserID().c_str());
        MySQL->query(targetString);
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
        snprintf(time, 1024, str.c_str(), p->tm_year + 1900, p->tm_mon + 1, p->tm_mday, p->tm_hour, p->tm_min, p->tm_sec);
        return time;
    }
    bool isFriend(const char * selfUserId,const char* friendUserId) {
        //˫����֤������Ϣ
        bool isMakeFriend = false;
        shared_ptr<MysqlConn> MySQL = pool->getConnection();
        string sql_2 = "select * from friendmaking where selfUserId = '%s' and friendUserId = '%s';";
        char targetString2[1024];
        snprintf(targetString2, sizeof(targetString2), sql_2.c_str(),
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
        string sql_1 = "select * from logininfo where phone = '%s'";
        char targetString1[1024];
        snprintf(targetString1, sizeof(targetString1), sql_1.c_str(), UserId);
        MySQL->query(targetString1);
        while (MySQL->next()) {
            //����Ѿ�ע��
            return true;
        }
        return false;
    }
    Json::Value getInfoById(const char* UserId) {
        shared_ptr<MysqlConn> MySQL = pool->getConnection();
        string sql_1 = "select * from logininfo where id = '%s';";
        char targetString1[1024];
        snprintf(targetString1, sizeof(targetString1), sql_1.c_str(), UserId);
        MySQL->query(targetString1);
        Json::Value info;
        while (MySQL->next()) {
            info["UserId"] = Json::Value(MySQL->value(0).c_str());
            info["UserName"] = Json::Value(MySQL->value(1).c_str());
            info["UserPwd"] = Json::Value(MySQL->value(2).c_str());
            info["UserSocket"] = Json::Value(MySQL->value(3).c_str());
            info["UserStatus"] = Json::Value(MySQL->value(4).c_str());
            info["UserPhone"] = Json::Value(MySQL->value(5).c_str());
        }
        return info;
    }
    void getInfoByPhone(string& Id, const char* UserPhone) {
        shared_ptr<MysqlConn> MySQL = pool->getConnection();
        string sql_1 = "select * from logininfo where phone = '%s';";
        char targetString1[1024];
        snprintf(targetString1, sizeof(targetString1), sql_1.c_str(), UserPhone);
        MySQL->query(targetString1);
        while (MySQL->next()) {
            Id = MySQL->value(0);
            //strcpy(Info[1], MySQL->value(1).c_str());
            //strcpy(Info[2], MySQL->value(2).c_str());
            //strcpy(Info[3], MySQL->value(3).c_str());
            //strcpy(Info[4], MySQL->value(4).c_str());
            //strcpy(Info[5], MySQL->value(5).c_str());
        }

    }
    void getSpFriendInfo(MyProtoMsg& msgToFriend,const char* selfId,const char* fdId) {
        shared_ptr<MysqlConn> MySQL = pool->getConnection();
        string sql_1 = "select selfUserId,friendUserId,addDate,recordId,name,cur_socket,`status`,phone from friendmaking JOIN logininfo on friendUserId = id WHERE selfUserId = '%s' and friendUserId = '%s';";
        char targetString1[1024];
        snprintf(targetString1, sizeof(targetString1), sql_1.c_str(), selfId,fdId);
        MySQL->query(targetString1);
        while (MySQL->next()) {
            Json::Value temp;
            msgToFriend.body["friendUserId"] = Json::Value(MySQL->value(1).c_str());
            msgToFriend.body["addDate"] = Json::Value(MySQL->value(2).c_str());
            msgToFriend.body["recordId"] = Json::Value(MySQL->value(3).c_str());
            msgToFriend.body["userName"] = Json::Value(MySQL->value(4).c_str());
            msgToFriend.body["cur_socket"] = (MySQL->value(5).c_str() == nullptr) ? Json::Value("NULL") : Json::Value(MySQL->value(5).c_str());
            msgToFriend.body["status"] = Json::Value(MySQL->value(6).c_str());
            msgToFriend.body["phone"] = Json::Value(MySQL->value(7).c_str());
        }
    }
    string getRecordId(const char * userId,const char*friendId) {
        shared_ptr<MysqlConn> MySQL = pool->getConnection();
        string sql = "select recordId from friendmaking where selfUserId = '%s' and friendUserId = '%s';";
        char targetString[1024];
        snprintf(targetString, sizeof(targetString), sql.c_str(),userId,friendId);
        MySQL->query(targetString);
        while (MySQL->next()) {
            return MySQL->value(0);
        }
        return string("");
    }
    void insertMsg(const char* userId, const char* content, const char* recordId) {
        shared_ptr<MysqlConn> MySQL = pool->getConnection();
        string sql = "insert into msgrecord(userId,content,recordId) values('%s','%s','%s');";
        char targetString[1024];
        snprintf(targetString, sizeof(targetString), sql.c_str(), userId, content,recordId);
        MySQL->query(targetString);
    }
    Json::Value getRecordMsgById(const char * recordId) {
        shared_ptr<MysqlConn> MySQL = pool->getConnection();
        string sql = "select userId,content,date from msgrecord where recordId = '%s';";
        char targetString[1024];
        snprintf(targetString, sizeof(targetString), sql.c_str(),recordId);
        MySQL->query(targetString);
        Json::Value root;
        while (MySQL->next()) {
            Json::Value temp;
            temp["userId"] = MySQL->value(0).c_str();
            temp["content"] = MySQL->value(1).c_str();
            temp["date"] = MySQL->value(2).c_str();
            root.append(temp);
        }
        return root;
    }
};
MyServer server;
void EXIT(int sig){
    cout<<"接收到退出信号sig = "<<sig<<endl;
    g_bRun = false;
    server.Close();
    exit(0);
}
int main() {
    signal(SIGINT,EXIT);
    signal(SIGTERM,EXIT);
    server.InitSocket();
    server.Bind(nullptr, 4567);
    server.Listen(15);
    server.Start(4);
    //std::thread t1(cmdThread);
    //t1.detach();    //�������̣߳���ֹ���߳��˳������߳���δ�˳���������
    
    while (g_bRun) {
        g_bRun = server.OnRun(); 
    }
    server.Close();
    printf("Message:  The task is completed. Procedure\n");
    return 0;
}
