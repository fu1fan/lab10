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
};
