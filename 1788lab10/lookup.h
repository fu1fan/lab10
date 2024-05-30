#pragma once

#include <QWidget>
#include "ui_lookup.h"
#include "account.h"

class Lookup : public QWidget
{
	Q_OBJECT

public:
	Lookup(QWidget *parent = nullptr);
	~Lookup();

private:
	Ui::LookupClass ui;
	Account* account;
};
