#include "ConnectionPool.h"

ConnectionPool* ConnectionPool::getConnectPool()
{
    static ConnectionPool pool;
    return &pool;
}

shared_ptr<MysqlConn> ConnectionPool::getConnection()
{
    unique_lock<mutex> locker(m_mutexQ);
    while(m_connectQ.empty()) {
        if (cv_status::timeout == m_cond.wait_for(locker, chrono::milliseconds(m_timeout))) {
            if (m_connectQ.empty()) {
                //return nullptr;
                continue;
            }
        }
    }
    shared_ptr<MysqlConn> connptr(m_connectQ.front(), [this](MysqlConn* conn) {
        lock_guard<mutex> locker(m_mutexQ);
        conn->refreshAliveTime();
        m_connectQ.push(conn);
        });
    m_connectQ.pop();
    m_cond.notify_all();
    return connptr;
}

ConnectionPool::~ConnectionPool()
{
    while (!m_connectQ.empty()) {
        MysqlConn* conn = m_connectQ.front();
        m_connectQ.pop();
        delete conn;
    }
}

ConnectionPool::ConnectionPool()
{
    printf("Procedure: The database thread pool is starting\n");
    // ���������ļ�
    if (!parseJsonFile()) {
        return;
    }
    
    for (int i = 0; i < m_minSize; ++i) {
    	printf("%d号数据连接。\n",i);
        addConnection();
        
    }
    thread producer(&ConnectionPool::produceConnection, this);
    thread recycler(&ConnectionPool::recycleConnection,this);
    producer.detach();
    recycler.detach();
    printf("Procedure: The database thread pool is started successfully.\n");
}

bool ConnectionPool::parseJsonFile()
{
    ifstream ifs("dbconf.json");
    Reader rd;
    Value root;
    rd.parse(ifs, root);
    
    if (root.isObject()) {
        Value sys = root["Linux"];
        cout << sys << endl;
        m_ip = sys["ip"].asString();
        m_port = sys["port"].asInt();
        m_user = sys["userName"].asString();
        m_passwd = sys["password"].asString();
        m_dbName = sys["dbName"].asString();
        m_minSize = sys["minSize"].asInt();
        m_maxSize = sys["maxSize"].asInt();
        m_maxIdleTime = sys["maxIdleTime"].asInt();
        m_timeout = sys["timeout"].asInt();
        return true;
        
    }
    return false;
}

void ConnectionPool::produceConnection()
{
    while (true)
    {
        unique_lock<mutex> locker(m_mutexQ);
        while (m_connectQ.size() >= (size_t)m_minSize) {

            m_cond.wait(locker);
        }
        addConnection();
        m_cond.notify_all();
    }
}

void ConnectionPool::recycleConnection()
{
    while (true)
    {
        this_thread::sleep_for(chrono::milliseconds(500));
        lock_guard<mutex> locker(m_mutexQ);
        while (m_connectQ.size() > (size_t)m_minSize) {
            MysqlConn* conn = m_connectQ.front();
            if (conn->getAliueveTime() >= m_maxIdleTime) {
                m_connectQ.pop();
                delete conn;
            }
            else {
                break;
            }
        }
    }
}

void ConnectionPool::addConnection()
{
    MysqlConn* conn = new MysqlConn;
    
    conn->connect(m_user, m_passwd, m_dbName, m_ip, m_port);
    
    conn->refreshAliveTime();
    m_connectQ.push(conn);
}
