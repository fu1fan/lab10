#include "mainWindow.h"

mainWindow::mainWindow(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);

	overview = new Overview();
	ui.stackedWidget->addWidget(overview);
	setting = new Setting();
	ui.stackedWidget->addWidget(setting);
	input = new Input();
	ui.stackedWidget->addWidget(input);
	account = new Account();
	ui.stackedWidget->addWidget(account);

	buttonGroup = new QButtonGroup(this);
	buttonGroup->addButton(ui.overviewButton, 0);
	buttonGroup->addButton(ui.settingButton, 1);
	buttonGroup->addButton(ui.inputButton, 2);
	buttonGroup->addButton(ui.studentButton, 3);

	connect(buttonGroup, &QButtonGroup::buttonClicked, this, &mainWindow::onButtonClicked);

	ui.stackedWidget->setCurrentIndex(0);
}

mainWindow::~mainWindow()
{
	delete overview;
	delete setting;
	delete input;
	delete account;
	delete buttonGroup;
}

void mainWindow::onButtonClicked(QAbstractButton* button)
{
	int id = buttonGroup->id(button);
	ui.stackedWidget->setCurrentIndex(id);
	if (id == 0) {
		overview->update_subjects();
	}
	else if (id == 2) {
		input->update_subjects();
	}
	else if (id == 3) {
		account->update_account();
	}
}
