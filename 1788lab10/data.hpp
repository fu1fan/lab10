#pragma once

#include "nlohmann/json.hpp"
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include "common.h"

using json = nlohmann::json;
using namespace std;

class AccountDatabase
{
public:
	AccountDatabase() {
		// 读取accounts.json文件
		std::ifstream i("accounts.json");
		accounts = new json();
		try {
			i >> *accounts;
			// 检查是否整体为一个字典，如果不是则初始化为空字典
			if (!accounts->is_object()) {
				*accounts = json::object();
				(*accounts)["student"] = json::object();
				(*accounts)["teacher"] = json::object();
#ifdef ENABLE_ADMIN
				(*accounts)["admin"] = "admin";
#endif // ENABLE_ADMIN

				save();
			}
		}
		catch (json::parse_error) {
			*accounts = json::object();
			save();
		}
	}
	~AccountDatabase() {
		// 写入accounts.json文件
		std::ofstream o("accounts.json");
		o << *accounts;
		delete accounts;
	}
	void save() {
		// 写入accounts.json文件
		std::ofstream o("accounts.json");
		o << *accounts;
	}
	bool login_verify(std::string username, std::string password, bool is_student=false) {
		//在students或teatures中查找，储存格式为{"username": "password"}
		std::string role = is_student ? "student" : "teacher";
		if (accounts->find(role) == accounts->end()) {
			return false;
		}
		if ((*accounts)[role].find(username) == (*accounts)[role].end()) {
			return false;
		}
		return (*accounts)[role][username] == password;
	}
	void set_password(std::string username, std::string password, bool is_student = false) {
		//在students或teatures中查找，储存格式为{"username": "password"}
		std::string role = is_student ? "student" : "teacher";
		(*accounts)[role][username] = password;
	}
private:
	json *accounts;
};
/*
科目json文件数据样例
{
	"1":["MATH01", "高等数学1", {
		"平时成绩": 20,
		"期中考试": 30,
		"期末考试": 50
	}]
}
1.科目编号
MATH01.科目代码
高等数学1.科目名称
平时成绩.评分项
*/
class SubjectsDatabase
{
public:
	SubjectsDatabase() {
		// 读取subjects.json文件
		std::ifstream i("subjects.json");
		subjects = new json();
		try {
			i >> *subjects;
			// 检查是否整体为一个字典，如果不是则初始化为空字典
			if (!subjects->is_object()) {
				*subjects = json::object();
				save();
			}
		}
		catch (json::parse_error) {
			*subjects = json::object();
			save();
		}	
	}
	~SubjectsDatabase() {
		// 写入subjects.json文件
		std::ofstream o("subjects.json");
		o << *subjects;
		delete subjects;
	}
	void save() {
		// 写入subjects.json文件
		std::ofstream o("subjects.json");
		o << *subjects;
	}

	void add_subject(std::string subject_code, std::string subject_name) {
		// 生成课程id
		std::string subject_id = std::to_string(subjects->size() + 1);
		(*subjects)[subject_id] = { subject_code, subject_name, json::object() };
	}
	void add_item(std::string subject_id, std::string item_name, int weight) {
		(*subjects)[subject_id][2][item_name] = weight;
	}
	void delete_subject(std::string subject_id) {
		subjects->erase(subject_id);
	}
	void delete_item(std::string subject_id, std::string item_name) {
		(*subjects)[subject_id][2].erase(item_name);
	}
	void set_item_weight(std::string subject_id, std::string item_name, int weight) {
		(*subjects)[subject_id][2][item_name] = weight;
	}

	std::string* get_subject_ids() {
		std::string *ids = new std::string[subjects->size()];
		int i = 0;
		for (json::iterator it = subjects->begin(); it != subjects->end(); ++it) {
			ids[i++] = it.key();
		}
		return ids;
	}
	std::string get_subject_id(size_t index) {
		int i = 0;
		for (json::iterator it = subjects->begin(); it != subjects->end(); ++it) {
			if (i == index) {
				return it.key();
			}
			i++;
		}
		return "";
	}
	size_t get_subject_count() {
		return subjects->size();
	}
	std::string get_subject_name(std::string subject_id) {
		return (*subjects)[subject_id][1];
	}
	std::string get_subject_name(json::iterator subject_id) {
		return (*subjects)[subject_id.key()][1];
	}
	std::string get_subject_code(std::string subject_id) {
		return (*subjects)[subject_id][0];
	}
	std::string get_subject_code(json::iterator subject_id) {
		return (*subjects)[subject_id.key()][0];
	}
	std::map<std::string, int> get_subject_items(std::string subject_id) {
		std::map<std::string, int> items;
		for (json::iterator it = (*subjects)[subject_id][2].begin(); it != (*subjects)[subject_id][2].end(); ++it) {
			items[it.key()] = it.value();
		}
		return items;
	}
	json::iterator begin() {
		return subjects->begin();
	}
	json::iterator end() {
		return subjects->end();
	}
private:
	json *subjects;
};
/*
格式说明
{
    "2023111788":["傅逸凡", {
        "1":{
            "平时成绩": 100,
            "期中成绩": 100,
            "期末成绩": 100
        }
    }]
}
2023111788.学号
傅逸凡.姓名
1.科目编号
*/
class PerformanceDatabase
{
public:
	PerformanceDatabase(SubjectsDatabase* subjects_db) {
		// 读取performance.json文件
		std::ifstream i("performance.json");
		performance = new json();
		try {
			i >> *performance;
			// 检查是否整体为一个字典，如果不是则初始化为空字典
			if (!performance->is_object()) {
				*performance = json::object();
				save();
			}
		}
		catch (json::parse_error) {
			*performance = json::object();
			save();
		}
		this->subjects_db = subjects_db;
	}
	~PerformanceDatabase() {
		// 写入performance.json文件
		std::ofstream o("performance.json");
		o << *performance;
		delete performance;
	}
	void save() {
		// 写入performance.json文件
		std::ofstream o("performance.json");
		o << *performance;
	}
	string get_student_id(int index) {
		int i = 0;
		for (json::iterator it = performance->begin(); it != performance->end(); ++it) {
			if (i == index) {
				return it.key();
			}
			i++;
		}
		return "";
	}
	string get_student_id(json::iterator index) {
		return index.key();
	}
	void add_student(std::string student_id, std::string student_name) {
		(*performance)[student_id] = { student_name, json::object() };
	}
	void add_subject(std::string student_id, std::string subject_id) {
		(*performance)[student_id][1][subject_id] = json::object();
	}
	void delete_student(std::string student_id) {
		performance->erase(student_id);
	}
	void delete_subject(std::string student_id, std::string subject_id) {
		(*performance)[student_id][1].erase(subject_id);
	}
	void set_score(std::string student_id, std::string subject_id, std::string item_name, int score) {
		(*performance)[student_id][1][subject_id][item_name] = score;
	}
	std::string get_student_name(std::string student_id) {
		return (*performance)[student_id][0];
	}
	std::string get_student_name(json::iterator student_id) {
		return (*performance)[student_id.key()][0];
	}
	int get_score(std::string student_id, std::string subject_id, std::string item_name) {
		try {
			return (*performance)[student_id][1][subject_id][item_name];
		}
		catch (json::out_of_range) {
			return -1;
		}
	}
	int get_score(json::iterator student_id, std::string subject_id, std::string item_name) {
		auto t = student_id.key();
		try {
			return (*performance)[student_id.key()][1][subject_id][item_name];
		}
		catch (json::out_of_range) {
			return -1;
		}
	}
	json::iterator begin() {
		return performance->begin();
	}
	json::iterator end() {
		return performance->end();
	}

	float calculate_score(std::string student_id, std::string subject_id) {
		std::map<std::string, int> items = subjects_db->get_subject_items(subject_id);
		float total_score = 0;
		float total_weight = 0;
		for (std::map<std::string, int>::iterator it = items.begin(); it != items.end(); ++it) {
			total_score += get_score(student_id, subject_id, it->first) * it->second;
			total_weight += it->second;
		}
		return total_score / total_weight;
	}

	float calculate_score(json::iterator student_id, std::string subject_id) {
		std::map<std::string, int> items = subjects_db->get_subject_items(subject_id);
		float total_score = 0;
		float total_weight = 0;
		for (std::map<std::string, int>::iterator it = items.begin(); it != items.end(); ++it) {
			int socre = get_score(student_id, subject_id, it->first);
			if (socre == -1) {
				return -1;
			}
			total_score += socre * it->second;
			total_weight += it->second;
		}
		return total_score / total_weight;
	}

private:
	json *performance;
	SubjectsDatabase *subjects_db;
};