#include "studentwindow.h"

StudentWindow::StudentWindow(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);

	lookup = new Lookup();
	ui.stackedWidget->addWidget(lookup);
	account = new Account();
	ui.stackedWidget->addWidget(account);

	buttonGroup = new QButtonGroup(this);
	buttonGroup->addButton(ui.overviewButton, 0);
	buttonGroup->addButton(ui.studentButton, 1);

	connect(buttonGroup, &QButtonGroup::buttonClicked, this, &StudentWindow::onButtonClicked);

	ui.stackedWidget->setCurrentIndex(0);
}

StudentWindow::~StudentWindow()
{}

void StudentWindow::onButtonClicked(QAbstractButton* button) {

}
