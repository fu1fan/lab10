#pragma once
#include "common.h"

using namespace std;

class Login : public QObject
{
public:
	Login() {
		db = new AccountDatabase();
	}
	~Login() {
		delete db;
	}
	bool verify(string username, string password, bool is_student) {
		return db->login_verify(username, password, is_student);
	}
private:
	AccountDatabase* db = nullptr;
};
