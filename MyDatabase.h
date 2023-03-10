#pragma once
#include<iostream>
#include <winsock2.h>
#include<mysql.h>
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
	bool test_connect();	//��������
	bool Get_connect_status();//��ȡ����״̬
	void delete_table(const char* table);	//ɾ����ṹ
	void mysql_Query(const char* sql);//sql���ִ��
	void mysql_create(const char* sql);	//������ṹ
	test* mysql_select(const char* sql);		//�����ݲ�ѯ
	void mysql_DML(const char * sql);	//��ӱ�����
	~MyDatabase();		//�ͷ����ݿ���Դ
};