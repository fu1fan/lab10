#include "lab10.h"
#include <QMessageBox>

lab10::lab10(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);

	login = new Login();
    
    connect(ui.loginButton, SIGNAL(clicked()), this, SLOT(login_slot()));
}

lab10::~lab10()
{
    delete login;
    delete main_window;
}

void lab10::login_slot() {
    string username = ui.accountEdit->text().toStdString();
	string password = ui.passwordEdit->text().toStdString();
	bool is_student = ui.roleBox->currentIndex() == 1;
    if (login->verify(username, password, is_student)) {
		main_window = new mainWindow();
        main_window->show();
        this->hide();
	}
    else {
		// 弹窗
        QMessageBox::warning(this, "错误", "用户名或密码错误");
	}
}
