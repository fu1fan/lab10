#include "mainWindow.h"

SubjectsDatabase* subjects_db = nullptr;
PerformanceDatabase * performance_db = nullptr;

mainWindow::mainWindow(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);

	subjects_db = new SubjectsDatabase();
	performance_db = new PerformanceDatabase(subjects_db);

	overview = new Overview();
	ui.stackedWidget->addWidget(overview);
	setting = new Setting();
	ui.stackedWidget->addWidget(setting);
	input = new Input();
	ui.stackedWidget->addWidget(input);
	student = new Student();
	ui.stackedWidget->addWidget(student);

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
	delete student;
	delete buttonGroup;
}

void mainWindow::onButtonClicked(QAbstractButton* button)
{
	int id = buttonGroup->id(button);
	ui.stackedWidget->setCurrentIndex(id);
}
