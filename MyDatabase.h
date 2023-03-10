#pragma once
#include<iostream>
#ifdef _WIN32
#include<mysql.h>
#else
#include<mysql/mysql.h>
#endif // DEBUG


#include"memory.h"
using namespace std;

class MyDatabase {
	MYSQL* m_sock;
	const char* host;
	const char* user ;
	const char* pass;
	const char* db ;
	unsigned int nport;
	bool connect_status;
	int Affect;
public:
	explicit MyDatabase();
	MYSQL* Get_sock();
	bool UserInfo_connect();	//��������
	bool Get_connect_status();//��ȡ����״̬
	void delete_table(const char* table);	//ɾ����ṹ
	void mysql_Query(const char* sql);//sql���ִ��
	void mysql_create(const char* sql);	//������ṹ
	MYSQL_RES* mysql_select(const char* sql);		//�����ݲ�ѯ
	void mysql_DML(const char * sql);	//��ӱ�����
	MYSQL_RES* res;//�����
	~MyDatabase();		//�ͷ����ݿ���Դ
};