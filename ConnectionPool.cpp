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
    // º”‘ÿ≈‰÷√Œƒº˛
    if (!parseJsonFile()) {
        return;
    }
    
    for (int i = 0; i < m_minSize; ++i) {
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
        
        m_ip = root["ip"].asString();
        m_port = root["port"].asInt();
        m_user = root["userName"].asString();
        m_passwd = root["password"].asString();
        m_dbName = root["dbName"].asString();
        m_minSize = root["minSize"].asInt();
        m_maxSize = root["maxSize"].asInt();
        m_maxIdleTime = root["maxIdleTime"].asInt();
        m_timeout = root["timeout"].asInt();
        return true;
        
    }
    return false;
}

void ConnectionPool::produceConnection()
{
    while (true)
    {
        unique_lock<mutex> locker(m_mutexQ);
        while (m_connectQ.size() >= m_minSize) {

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
        while (m_connectQ.size() > m_minSize) {
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
