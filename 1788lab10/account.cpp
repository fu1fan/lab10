#include "account.h"
#include "common.h"
#include <QMessageBox>

Account::Account(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	connect(ui.pushButton, SIGNAL(clicked()), this, SLOT(set_username()));
	connect(ui.pushButton_2, SIGNAL(clicked()), this, SLOT(set_password()));
}

Account::~Account()
{}

void Account::set_username()
{
	if (account_db->is_student()) {
		// 禁止修改学生账号
		QMessageBox::warning(this, "错误", "学生账号无法修改");
		return;
	}
	string new_username = ui.lineEdit->text().toStdString();
	bool is_student = account_db->is_student();
	if (account_db->exist(new_username, is_student)) {
		// 弹窗
		QMessageBox::warning(this, "错误", "用户名已存在");
		return;
	}
	account_db->set_username(new_username, is_student);
	QMessageBox::information(this, "成功", "用户名修改成功");
}

void Account::set_password()
{
	// 先验证旧密码
	string old_password = ui.lineEdit_2->text().toStdString();
	bool is_student = account_db->is_student();
	if (!account_db->login_verify(account_db->get_username(), old_password, is_student)) {
		// 弹窗
		QMessageBox::warning(this, "错误", "密码错误");
		return;
	}
	// 判断是否一致
	string new_password = ui.lineEdit_3->text().toStdString();
	string new_password_confirm = ui.lineEdit_4->text().toStdString();

	if (new_password != new_password_confirm) {
		// 弹窗
		QMessageBox::warning(this, "错误", "两次密码不一致");
		return;
	}
	account_db->set_password(account_db->get_username(), new_password, is_student);
	QMessageBox::information(this, "成功", "密码修改成功");

	// 清空输入框
	ui.lineEdit_2->clear();
	ui.lineEdit_3->clear();
	ui.lineEdit_4->clear();
}

void Account::update_account()
{
	ui.lineEdit->setText(QString::fromStdString(account_db->get_username()));
	
}
