#ifndef _MYPROTO_H_
#define _MYPROTO_H_

#include <cstdint>
#include<string>
#include<queue>
#include <iostream>
#include"json/json.h"
using namespace std;
const uint8_t MY_PROTO_MAGIC = 8; //Э��ħ����ͨ��ħ�����м򵥶Ա�У�飬Ҳ������֮ǰѧ��CRCУ���滻
const uint32_t MY_PROTO_MAX_SIZE = 10 * 1024 * 1024; //10MЭ�����������
const uint32_t MY_PROTO_HEAD_SIZE = 8; //Э��ͷ��С
//server
enum CMD {
    CMD_LOGON,
    CMD_LOGON_RESULT,
    CMD_LOGIN,
    CMD_LOGIN_RESULT,
    CMD_LOGOUT,
    CMD_LOGOUT_RESULT,
    CMD_NEW_USER_JOIN,
    CMD_ERROR,
    CMD_FRIEND_MAKE,
    CMD_FRIEND_MAKE_RESULT,
    CMD_GET_FRIEND,
    CMD_GET_FRIEND_RESULT,
    CMD_DEL_FRIEND,
    CMD_DEL_FRIEND_RESULT,
    CMD_FRIEND_LOGIN,
    CMD_FRIEND_LOGOUT,
    CMD_FRIEND_ADD,
    CMD_FRIEND_REDUCE
};
//Э��ͷ��
struct MyProtoHead
{
    uint8_t version; //Э��汾��
    uint8_t magic; //Э��ħ��
    uint16_t server; //Э�鸴�õķ���ţ����ڱ�ʶЭ���еĲ�ͬ���񣬱������������ȡget ����set ���add ... ���ǲ�ͬ����������ָ����
    uint32_t len; //Э�鳤�ȣ�Э��ͷ��+�䳤jsonЭ����=�ܳ��ȣ�
};

//Э����Ϣ��
struct MyProtoMsg
{
    MyProtoHead head; //Э��ͷ
    Json::Value body; //Э����
};

//<----ʵ��Э���װ����----->
//Э���װ��
class MyProtoEncode
{
public:
    //Э����Ϣ���װ�����������pMsg����ֻ�в������ݣ�����JsonЭ���壬����ţ����Ƕ���Ϣ�������޸ĳ�����Ϣ����ʱ��Ҫ���±���Э��
    uint8_t* encode(MyProtoMsg* pMsg, uint32_t& len); //���س�����Ϣ�����ں���socket��������
private:
    //Э��ͷ��װ����
    void headEncode(uint8_t* pData, MyProtoMsg* pMsg);
};


//<----ʵ��Э���������----->
typedef enum MyProtoParserStatus //Э�������״̬
{
    ON_PARSER_INIT = 0, //��ʼ״̬
    ON_PARSER_HEAD = 1, //����ͷ��
    ON_PARSER_BODY = 2, //��������
}MyProtoParserStatus;
//Э�������
class MyProtoDecode
{
private:
    MyProtoMsg mCurMsg; //��ǰ�����е�Э����Ϣ��
    queue<MyProtoMsg*> mMsgQ; //�����õ�Э����Ϣ����
    vector<uint8_t> mCurReserved; //δ�����������ֽ��������Ի�������û�н��������ݣ����ֽڣ�
    MyProtoParserStatus mCurParserStatus; //��ǰ���ܷ�����״̬
public:
    void init(); //��ʼ��Э�����״̬
    void clear(); //��ս����õ���Ϣ����
    bool empty(); //�жϽ����õ���Ϣ�����Ƿ�Ϊ��
    void pop();  //����һ����Ϣ

    MyProtoMsg* front(); //��ȡһ�������õ���Ϣ
    bool parser(void* data, size_t len); //�������ֽ����н�������Э����Ϣ��len�������е��ֽ������ȣ�ͨ��socket���Ի�ȡ
private:
    bool parserHead(uint8_t** curData, uint32_t& curLen,
        uint32_t& parserLen, bool& parserBreak); //���ڽ�����Ϣͷ
    bool parserBody(uint8_t** curData, uint32_t& curLen,
        uint32_t& parserLen, bool& parserBreak); //���ڽ�����Ϣ��
};

#endif // !1

