#pragma once

#include <QMainWindow>
#include <qbuttongroup.h>
#include "ui_mainWindow.h"
#include "overview.h"
#include "setting.h"
#include "input.h"
#include "account.h"
#include "common.h"

class mainWindow : public QMainWindow
{
	Q_OBJECT

public:
	mainWindow(QWidget *parent = nullptr);
	~mainWindow();

private:
	Ui::mainWindowClass ui;
	Overview* overview;
	Setting* setting;
	Input* input;
	Account* account;
	QButtonGroup* buttonGroup;
public slots:
	void onButtonClicked(QAbstractButton* button);
};
