#pragma once

#include <QWidget>
#include "ui_student.h"

class Student : public QWidget
{
	Q_OBJECT

public:
	Student(QWidget *parent = nullptr);
	~Student();

private:
	Ui::StudentClass ui;
};
