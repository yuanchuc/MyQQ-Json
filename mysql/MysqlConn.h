#ifndef _MYSQLCONN_H_
#define _MYSQLCONN_H_

#include<iostream>
#include<chrono>
using namespace std;
using namespace chrono;
#ifdef _WIN32
#include<mysql.h>
#else
#include<mysql/mysql.h>
#endif // DEBUG

class MysqlConn
{
public:
	//��ʼ�����ݿ�����
	MysqlConn();
	//�ͷ����ݿ�����
	~MysqlConn();
	//�������ݿ�
	bool connect(string user, string passwd, string dbName, string ip, unsigned short port = 3306);
	//�������ݿ�
	bool update(string sql);
	//��ѯ���ݿ�
	bool query(string sql);
	//������ѯ�õ��Ľ����
	bool next();
	//�õ�������е��ֶ�ֵ
	string value(int index);
	//�������
	bool transaction();
	//�ύ����
	bool commit();
	//����ع�
	bool roolback();
	//ˢ����ʼ�Ŀ���ʱ���
	void refreshAliveTime();
	long long getAliueveTime();
private:
	void freeResult();
	MYSQL* m_conn = nullptr;
	MYSQL_RES* m_result = nullptr;
	MYSQL_ROW m_row = nullptr;
	steady_clock::time_point m_alivetime;
};


#endif // !_MYSQLCONN_H_

