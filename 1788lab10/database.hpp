#pragma once

#include <winsqlite/winsqlite3.h>
#include <string>
#include <vector>
#include <iostream>
#include <common.h>

// 简单封装sqlite3，实现增删改查
class Database {
public:
	Database(const std::string& dbPath) {
		int rc = sqlite3_open(dbPath.c_str(), &db);
		if (rc) {
			std::cerr << "Can't open database: " << sqlite3_errmsg(db) << std::endl;
			sqlite3_close(db);
		}
	}

	~Database() {
		sqlite3_close(db);
	}

	bool exec(const std::string& sql) {
		char* errMsg = nullptr;
		int rc = sqlite3_exec(db, sql.c_str(), nullptr, nullptr, &errMsg);
		if (rc != SQLITE_OK) {
			std::cerr << "SQL error: " << errMsg << std::endl;
			sqlite3_free(errMsg);
			return false;
		}
		return true;
	}

	std::vector<std::vector<std::string>> query(const std::string& sql) {
		std::vector<std::vector<std::string>> result;
		char* errMsg = nullptr;
		int rc = sqlite3_exec(db, sql.c_str(), callback, &result, &errMsg);
		if (rc != SQLITE_OK) {
			std::cerr << "SQL error: " << errMsg << std::endl;
			sqlite3_free(errMsg);
		}
		return result;
	}
private:
	static int callback(void* data, int argc, char** argv, char** azColName) {
		auto result = static_cast<std::vector<std::vector<std::string>>*>(data);
		std::vector<std::string> row;
		for (int i = 0; i < argc; i++) {
			row.push_back(argv[i]);
		}
		result->push_back(row);
		return 0;
	}

	sqlite3* db;
};

// 针对程序功能再一层封装
class ProgramDatabase {
public:
	ProgramDatabase(){
		account_db = new Database("account.db");
		performance_db = new Database("performance.db");
	}

	void init() {
		// 在account.db中创建表：学生账号和教师账号
		account_db->exec("CREATE TABLE IF NOT EXISTS student (id INTEGER PRIMARY KEY AUTOINCREMENT, username TEXT, password TEXT)");
		account_db->exec("CREATE TABLE IF NOT EXISTS teacher (id INTEGER PRIMARY KEY AUTOINCREMENT, username TEXT, password TEXT)");
		// 在performance.db中创建表：科目成绩占比设置
		performance_db->exec("CREATE TABLE IF NOT EXISTS subjects (id INTEGER PRIMARY KEY AUTOINCREMENT, name TEXT)");
	}

	bool login_verify(const std::string& username, const std::string& password, bool is_student) {
		std::string table = is_student ? "student" : "teacher";
		std::string sql = "SELECT * FROM " + table + " WHERE username='" + username + "' AND password='" + password + "'";
		auto result = account_db->query(sql);
		return !result.empty();
	}

	std::vector<std::vector<std::string>> query_exsiting_subjects() {
		// 查询performance.db中已有的课程（数据表名为课程名）
		return performance_db->query("SELECT name FROM sqlite_master WHERE type='table'");
	}

	bool create_subject(const std::string& subject, const preformance_composition& pc) {
		if (subject == "subjects") return false; // 防止表名为"subjects"

		// 遍历字典，将数据库中不存在的科目成绩占比设置插入数据库
		for (const auto& [key, value] : pc) {
			std::string sql = "SELECT * FROM subjects WHERE name='" + key + "'";
			auto result = performance_db->query(sql);
			if (result.empty()) {
				performance_db->exec("INSERT INTO subjects (name, percentage) VALUES ('" + key + "', " + std::to_string(value) + ")");
			}
		}

		// 插入新科目，设置成绩占比
		performance_db->exec("INSERT INTO subjects (name) VALUES ('" + subject + "')");
		for (const auto& [key, value] : pc) {
			performance_db->exec("ALTER TABLE " + subject + " ADD COLUMN " + key + " INTEGER");
		}

		return true;
	}
private:
	Database* account_db;
	Database* performance_db;
};
