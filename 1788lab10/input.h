#pragma once

#include <QWidget>
#include "ui_input.h"

class Input : public QWidget
{
	Q_OBJECT

public:
	Input(QWidget *parent = nullptr);
	~Input();

private:
	Ui::InputClass ui;
};
