#pragma once

#include <QWidget>
#include "ui_setting.h"
#include <QStringListModel>
#include <QStandardItemModel>

class Setting : public QWidget
{
	Q_OBJECT

public:
	Setting(QWidget *parent = nullptr);
	~Setting();

private:
	Ui::SettingClass ui;
	QStringListModel *subjects_m;
	QStringList *subjects_l;
	QStandardItemModel *items_tb;
	bool check_existing_student(std::string subject_id);

public slots:
	void select_subject(const QModelIndex &index);
	void add_subject();
	void remove_subject();
	void change_subject_item();
	void add_subject_item();
	void remove_subject_item();
	void apply();
	//void remove_subject();
};
