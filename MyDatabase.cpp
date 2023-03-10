#include"MyDatabase.h"

MyDatabase:: MyDatabase() {
	m_sock = new MYSQL;
	mysql_init(m_sock);
	host = { "localhost" };
	user = { "root" };
	pass = { "123456" };
	db = { "test" };
	nport = 3306;
	connect_status = false;
	Affect = 0;
}
MYSQL* MyDatabase::Get_sock() {
	return m_sock;
}
bool MyDatabase::test_connect(){
	if (!mysql_real_connect(m_sock, host, user, pass, db, nport, NULL, CLIENT_MULTI_STATEMENTS)) {
		connect_status = false;
	}
	else {
		connect_status = true;
	}
	return connect_status;
}
void MyDatabase::delete_table(const char* table) {
	string sql = "drop table " + (string)table;
	mysql_Query(sql.c_str());
}
bool MyDatabase::Get_connect_status()
{
	return connect_status;
}
void MyDatabase::mysql_Query(const char* sql) {
	mysql_autocommit(m_sock, 0);
	int ret = mysql_query(m_sock, sql);
	/*int ret1 = mysql_query(m_sock, "delete from test");*/
	Affect = mysql_affected_rows(m_sock);
	
	if (ret == 0)
	{
		// 提交事务
		cout << sql << "执行成功" << endl;
		mysql_commit(m_sock);
		
	}
	else
	{
		//回滚事务（回到原状态）
		cout << sql << "执行失败" << endl;
		cout << "错误原因:" << mysql_error(m_sock) << endl;
		mysql_rollback(m_sock);
	}
}
void MyDatabase::mysql_create(const char* sql)
{
	mysql_Query(sql);
}
void MyDatabase::mysql_DML(const char* sql)
{
	mysql_Query(sql);
	if (Affect <= 0) {
		cout << sql << "实际上执行失败" << endl;
		return;
	}
	
}
test* MyDatabase::mysql_select(const char * sql) {
	test* t=new struct test[10];
	mysql_Query(sql);
	// 4. 取出结果集
	MYSQL_RES * res = mysql_store_result(m_sock);
	if (res == NULL)
	{
		printf("mysql_store_result() 失败了, 原因: %s\n", mysql_error(m_sock));
		return nullptr;
	}
	// 5. 得到结果集中的列数
	int num = mysql_num_fields(res);

	// 6. 得到所有列的名字, 并且输出
	MYSQL_FIELD* fields = mysql_fetch_fields(res);
	for (int i = 0; i < num; ++i)
	{
		cout<< fields[i].name<<'\t';
	}
	printf("\n");
	//得到结果集中的行数
	MYSQL_ROW rows;
	int i = 0;
	while (rows = mysql_fetch_row(res)) {
		for (int i = 0; i < num; i++) {
			cout << rows[i] << '\t';
		}
		strcpy(t[i].no, rows[0]);
		strcpy(t[i].name, rows[1]);
		strcpy(t[i++].age, rows[2]);
		cout << endl;
	}
	return t;
}
MyDatabase::~MyDatabase()
{	
	mysql_close(m_sock);
}

