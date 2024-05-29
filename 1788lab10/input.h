#pragma once

#include <QWidget>
#include "ui_input.h"
#include "common.h"
#include <QStringListModel>
#include <QStringList>
#include <qstandarditemmodel.h>
#include <vector>

class Input : public QWidget
{
	Q_OBJECT

public:
	Input(QWidget *parent = nullptr);
	~Input();

private:
	Ui::InputClass ui;
	
	QStringListModel* subjects_m;
	QStringList* subjects_l;

	QStandardItemModel* items_tb;
	vector<std::string> student_ids;


public slots:
	void select_subject(int index);
	void update_subjects();
	void select_student(const QModelIndex& index);
	void remove_student();
	void add_student();
	void update_performance();
	void update_student_info();
};
