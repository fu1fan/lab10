#pragma once

#include <QWidget>
#include "ui_account.h"

class Account : public QWidget
{
	Q_OBJECT

public:
	Account(QWidget *parent = nullptr);
	~Account();

private:
	Ui::AccountClass ui;

public slots:
	void update_account();
	void set_username();
	void set_password();
};
