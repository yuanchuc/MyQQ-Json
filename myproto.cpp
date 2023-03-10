#include"myproto.h"
//----------------------------------Э��ͷ��װ����----------------------------------
//pDataָ��һ���µ��ڴ棬��ҪpMsg�����ݶ�pData�������
void MyProtoEncode::headEncode(uint8_t* pData, MyProtoMsg* pMsg)
{
    //����Э��ͷ�汾��Ϊ1
    *pData = 1;
    ++pData; //��ǰ�ƶ�һ���ֽ�λ�õ�ħ��

    //����Э��ͷħ��
    *pData = MY_PROTO_MAGIC; //���ڼ�У�����ݣ�ֻҪ���ͷ��ͽ��ܷ���ħ����һ�£��������Ϊ��������
    ++pData; //��ǰ�ƶ�һ���ֽ�λ�ã���server�����ֶΣ�16λ��С��

    //����Э�����ţ�����ţ����ڱ�ʶЭ���еĲ�ͬ���񣬱������������ȡget ����set ���add ... ���ǲ�ͬ����������ָ����
    //�ⲿ���ã������pMsg�У���ʵ���Բ����޸ģ�ֱ�������õ�ַ
    *(uint16_t*)pData = pMsg->head.server; //ԭ���Ǵ���ת��Ϊ�����ֽ��򣨵���û��Ҫ�������в���鿴Ӧ�ò����ݵ�
    pData += 2; //��ǰ�ƶ������ֽڣ���len�����ֶ�

    //����Э��ͷ�����ֶΣ�Э��ͷ+Э����Ϣ�壩����ʵ����Ϣ��������Ѿ��������ˣ�����Ҳ����ֱ������
    *(uint32_t*)pData = pMsg->head.len; //ԭ��Ҳ�ǽ������ֽ���ת��������ν�ˡ�����IP�����Ҳ����
}

//Э����Ϣ���װ�����������pMsg����ֻ�в������ݣ�����JsonЭ���壬����ţ��汾�ţ����Ƕ���Ϣ�������޸ĳ�����Ϣ����ʱ��Ҫ���±���Э��
//len���س�����Ϣ�����ں���socket��������
uint8_t* MyProtoEncode::encode(MyProtoMsg* pMsg, uint32_t& len)
{
    uint8_t* pData = nullptr; //���ڿ����µĿռ䣬��ű���������
    Json::FastWriter fwriter; //��ȡJson::Value���ݣ�ת��Ϊ����д���ļ����ַ���

    //Э��Json�����л�
    string bodyStr = fwriter.write(pMsg->body);

    //������Ϣ���л��Ժ���³���
    len = MY_PROTO_HEAD_SIZE + (uint32_t)bodyStr.size();
    pMsg->head.len = len; //һ�����Э��ͷ��ʱ�����õ�
    //����һ���µĿռ䣬���ڱ�����Ϣ��������Բ��ã�ֱ��ʹ��ԭ���ռ�Ҳ���ԣ�
    pData = new uint8_t[len];
    //����Э��ͷ
    headEncode(pData, pMsg); //�����ڲ�û��ͨ������ָ���޸�pData�����ݣ��޸ĵ�����ʱ����
    //���Э����
    memcpy(pData + MY_PROTO_HEAD_SIZE, bodyStr.data(), bodyStr.size());

    return pData; //������Ϣ�ײ���ַ
}
//----------------------------------Э�������----------------------------------
//��ʼ��Э�����״̬
void MyProtoDecode::init()
{
    mCurParserStatus = ON_PARSER_INIT;
}

//��ս����õ���Ϣ����
void MyProtoDecode::clear()
{
    MyProtoMsg* pMsg = NULL;
    while (!mMsgQ.empty())
    {
        pMsg = mMsgQ.front();
        delete pMsg;
        mMsgQ.pop();
    }
}

//�жϽ����õ���Ϣ�����Ƿ�Ϊ��
bool MyProtoDecode::empty()
{
    return mMsgQ.empty();
}

//����һ����Ϣ
void MyProtoDecode::pop()
{
    mMsgQ.pop();
}

//��ȡһ�������õ���Ϣ
MyProtoMsg* MyProtoDecode::front()
{
    return mMsgQ.front();
}

//�������ֽ����н�������Э����Ϣ,len��socket����recv����
bool MyProtoDecode::parser(void* data, size_t len)
{
    if (len <= 0)
        return false;

    uint32_t curLen = 0; //���ڱ���δ�����������ֽ������ȣ��Ƕ�vector)
    uint32_t parserLen = 0; //����vector���Ѿ���������ɵ��ֽ�����һ���������vector������
    uint8_t* curData = NULL; //ָ��data,��ǰδ�����������ֽ���

    curData = (uint8_t*)data;

    //����ǰҪ�����������ֽ���д�뵽vector��    
    while (len--)
    {
        mCurReserved.push_back(*curData);
        ++curData;
    }

    curLen = mCurReserved.size();
    curData = (uint8_t*)&mCurReserved[0]; //��ȡ�����׵�ַ

    //ֻҪ����δ�����������ֽ������ͳ�������
    while (curLen > 0)
    {
        bool parserBreak = false;

        //����ͷ��
        if (ON_PARSER_INIT == mCurParserStatus || //ע�⣺��ʶ�����ã�������û����ȫ�ﵽ����ȴ���һ�ν��������Ժ��������ͷ��
            ON_PARSER_BODY == mCurParserStatus) //���Խ���ͷ������
        {
            if (!parserHead(&curData, curLen, parserLen, parserBreak))
                return false;
            if (parserBreak)
                break; //�˳�ѭ�����ȴ���һ�����ݵ��һ�����ͷ��
        }

        //�������Э��ͷ����ʼ����Э����
        if (ON_PARSER_HEAD == mCurParserStatus)
        {
            if (!parserBody(&curData, curLen, parserLen, parserBreak))
                return false;
            if (parserBreak)
                break;
        }

        //����ɹ���������Ϣ���Ͱ���������Ϣ����
        if (ON_PARSER_BODY == mCurParserStatus)
        {
            MyProtoMsg* pMsg = NULL;
            pMsg = new MyProtoMsg;
            *pMsg = mCurMsg;
            mMsgQ.push(pMsg);
        }

        if (parserLen > 0)
        {
            //ɾ���Ѿ��������������ֽ���
            mCurReserved.erase(mCurReserved.begin(), mCurReserved.begin() + parserLen);
        }

        return true;
    }
}

//���ڽ�����Ϣͷ
bool MyProtoDecode::parserHead(uint8_t** curData, uint32_t& curLen,
    uint32_t& parserLen, bool& parserBreak)
{
    if (curLen < MY_PROTO_HEAD_SIZE)
    {
        parserBreak = true; //��������û��ͷ������û�취��������������
        return true; //�������ݻ������õģ�����û�з��ֳ�������true���ȴ�һ�����ݵ��ˣ��ٽ���ͷ�������ڱ�־û�䣬һ�ỹ�ǽ���ͷ��
    }

    uint8_t* pData = *curData;

    //�������ֽ����У���������Э���ʽ���ݡ�������MyProtoMsg mCurMsg; //��ǰ�����е�Э����Ϣ��
    //���������汾��
    mCurMsg.head.version = *pData;
    pData++;
    //����������У���ħ��
    mCurMsg.head.magic = *pData;
    pData++;

    //�ж�У����Ϣ
    if (MY_PROTO_MAGIC != mCurMsg.head.magic)
        return false; //���ݳ���

    //���������
    mCurMsg.head.server = *(uint16_t*)pData;
    pData += 2;

    //����Э����Ϣ�峤��
    mCurMsg.head.len = *(uint32_t*)pData;

    //�ж����ݳ����Ƿ񳬹�ָ���Ĵ�С
    if (mCurMsg.head.len > MY_PROTO_MAX_SIZE)
        return false;

    //������ָ����ǰ�ƶ�����Ϣ��λ��,������Ϣͷ��С
    (*curData) += MY_PROTO_HEAD_SIZE;
    curLen -= MY_PROTO_HEAD_SIZE;
    parserLen += MY_PROTO_HEAD_SIZE;
    mCurParserStatus = ON_PARSER_HEAD;

    return true;
}

//���ڽ�����Ϣ��
bool MyProtoDecode::parserBody(uint8_t** curData, uint32_t& curLen,
    uint32_t& parserLen, bool& parserBreak)
{
    uint32_t JsonSize = mCurMsg.head.len - MY_PROTO_HEAD_SIZE; //��Ϣ��Ĵ�С
    if (curLen < JsonSize)
    {
        parserBreak = true; //���ݻ�û����ȫ������ǻ�Ҫ�ȴ�һ�����ݵ��ˣ��ٽ�����Ϣ�塣���ڱ�־û�䣬һ�ỹ�ǽ�����Ϣ��
        return true;
    }

    Json::Reader reader; //Json������
    if (!reader.parse((char*)(*curData),
        (char*)((*curData) + JsonSize), mCurMsg.body, false)) //false��ʾ����ע��
        return false; //�������ݵ�body��

    //����ָ����ǰ�ƶ�
    (*curData) += JsonSize;
    curLen -= JsonSize;
    parserLen += JsonSize;
    mCurParserStatus = ON_PARSER_BODY;

    return true;
}
