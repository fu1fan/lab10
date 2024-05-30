#pragma once

#include <QMainWindow>
#include "ui_studentwindow.h"
#include "lookup.h"
#include "account.h"
#include <QButtonGroup>

class StudentWindow : public QMainWindow
{
	Q_OBJECT

public:
	StudentWindow(QWidget *parent = nullptr);
	~StudentWindow();
	Lookup* lookup;
	Account* account;
	QButtonGroup* buttonGroup;

private:
	Ui::StudentWindowClass ui;

public slots:
	void onButtonClicked(QAbstractButton* button);
};
