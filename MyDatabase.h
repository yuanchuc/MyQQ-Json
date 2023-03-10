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
	bool test_connect();	//测试链接
	bool Get_connect_status();//获取链接状态
	void delete_table(const char* table);	//删除表结构
	void mysql_Query(const char* sql);//sql语句执行
	void mysql_create(const char* sql);	//创建表结构
	test* mysql_select(const char* sql);		//表数据查询
	void mysql_DML(const char * sql);	//添加表数据
	~MyDatabase();		//释放数据库资源
};