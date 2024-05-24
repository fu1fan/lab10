#pragma once

#include <QWidget>
#include "ui_setting.h"

class Setting : public QWidget
{
	Q_OBJECT

public:
	Setting(QWidget *parent = nullptr);
	~Setting();

private:
	Ui::SettingClass ui;
};
