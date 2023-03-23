#ifndef _CONNECTIONPOOP_H_
#define _CONNECTIONPOOP_H_

#include<queue>
#include<mutex>
#include<condition_variable>
#include"json/json.h"
#include<fstream>
#include<thread>
#include"MysqlConn.h"
using namespace Json;
class ConnectionPool
{
public:
	static ConnectionPool* getConnectPool();
	ConnectionPool(const ConnectionPool& obj) = delete;
	ConnectionPool& operator = (const ConnectionPool& obj) = delete;
	shared_ptr<MysqlConn> getConnection();
	~ConnectionPool();
private:
	ConnectionPool();
	bool parseJsonFile();
	void produceConnection();
	void recycleConnection();
	void addConnection();
	string m_ip;
	string m_user;
	string m_passwd;
	string m_dbName;
	unsigned short m_port;
	int m_minSize;
	int m_maxSize;
	int m_timeout;
	int m_maxIdleTime;

	queue<MysqlConn*> m_connectQ;
	mutex m_mutexQ;
	condition_variable m_cond;
};

#endif // !_CONNECTIONPOOP_H_



