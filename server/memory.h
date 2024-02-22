#pragma once
#include<string>
#include"MysqlConn.h"
#include<memory>
using namespace std;

//个人信息用户表
struct LoginInfo {
	static const char* const tableName;
	string id;
	string name;
	string pwd;
	string cur_socket;
	string status;
	string phone;
	LoginInfo operator = (shared_ptr<MysqlConn> mysql) {
		id = mysql->value(0);
		name = mysql->value(1);
		pwd = mysql->value(2);
		cur_socket = mysql->value(3);
		status = mysql->value(4);
		phone = mysql->value(5);
	}
	string generateInsertSQL() {
		char target[1024];
		string sql = "insert into %s (name,pwd,cur_socket,status,phone) value ( '%s','%s','%s','%s','%s');";
		snprintf(target, sizeof target, id.c_str(), name.c_str(), pwd.c_str(), cur_socket.c_str(), status.c_str(), phone.c_str());
		return static_cast<string>(target);
	}
	// Getters
	string getId() const {
		return id;
	}

	string getName() const {
		return name;
	}

	string getPwd() const {
		return pwd;
	}

	string getCurSocket() const {
		return cur_socket;
	}

	string getStatus() const {
		return status;
	}

	string getPhone() const {
		return phone;
	}
	bool ToNull(const char * value,string target) {
		if (!value) {
			target= "null";
			return true;
		}
		return false;
	}
	// Setters
	void setId(const char* value) {
		if (ToNull(value, id)) return;
		id = value;
	}

	void setName(const char* value) {
		if (ToNull(value, name)) return;
		name = value;
	}

	void setPwd(const char* value) {
		if (ToNull(value, pwd)) return;
		pwd = value;
	}

	void setCurSocket(const char* value) {
		if (ToNull(value, cur_socket)) return;
		cur_socket = value;
	}

	void setStatus(const char* value) {
		if (ToNull(value, status)) return;
		status = value;
	}

	void setPhone(const char* value) {
		if (ToNull(value, phone)) return;
		phone = value;
	}
};
const char* const LoginInfo::tableName = "logininfo";

//好友表
struct FriendMaking {
	static const char* const tableName;
	string selfUserId;
	string friendUserId;
	string addDate;
	string recordId;
	FriendMaking operator = (shared_ptr<MysqlConn> mysql) {
		selfUserId = mysql->value(0);
		friendUserId = mysql->value(1);
		addDate = mysql->value(2);
		recordId = mysql->value(3);
	}
};
const char* const FriendMaking::tableName = "friendmaking";

//验证好友添加表
struct VerifyMsg {
	static const char* const tableName;
	string id;
	string selfUserId;
	string friendUserid;
	string insertDate;
	string extraMsg;
	string status;
	VerifyMsg operator = (shared_ptr<MysqlConn> mysql) {
		id = mysql->value(0);
		selfUserId = mysql->value(1);
		friendUserid = mysql->value(2);
		insertDate = mysql->value(3);
		extraMsg = mysql->value(4);
		status = mysql->value(5);
	}
};
const char* const VerifyMsg::tableName = "verifymsg";

//好友消息表
struct VerifyMsg {
	static const char* const tableName;
	string id;
	string userId;
	string content;
	string date;
	string recordId;
	VerifyMsg operator = (shared_ptr<MysqlConn> mysql) {
		id = mysql->value(0);
		userId = mysql->value(1);
		content = mysql->value(2);
		date = mysql->value(3);
		recordId = mysql->value(4);
	}
};
const char* const VerifyMsg::tableName = "verifymsg";