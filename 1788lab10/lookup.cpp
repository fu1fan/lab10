#include "lookup.h"

Lookup::Lookup(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	account = new Account();
	// 放入tabview第二个页面
	ui.tabWidget->insertTab(2, account, "账号设置");
}

Lookup::~Lookup()
{}
