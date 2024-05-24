#pragma once

#include <QMainWindow>
#include "ui_student.h"

class Student : public QMainWindow
{
	Q_OBJECT

public:
	Student(QWidget *parent = nullptr);
	~Student();

private:
	Ui::StudentClass ui;
};
