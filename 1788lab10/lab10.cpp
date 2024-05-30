#include "lab10.h"
#include <QMessageBox>
#include "common.h"

SubjectsDatabase* subjects_db = nullptr;
PerformanceDatabase* performance_db = nullptr;
AccountDatabase* account_db = nullptr;

lab10::lab10(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);

    subjects_db = new SubjectsDatabase();
    performance_db = new PerformanceDatabase(subjects_db);
	account_db = new AccountDatabase();

    main_window = new mainWindow();
    
    connect(ui.loginButton, SIGNAL(clicked()), this, SLOT(login_slot()));
}

lab10::~lab10()
{
    delete main_window;
}

void lab10::login_slot() {
    string username = ui.accountEdit->text().toStdString();
	string password = ui.passwordEdit->text().toStdString();
	bool is_student = ui.roleBox->currentIndex() == 1;
    if (account_db->login_verify(username, password, is_student)) {
        main_window->show();
        this->hide();
	}
    else {
		// 弹窗
        QMessageBox::warning(this, "错误", "用户名或密码错误");
	}
}
