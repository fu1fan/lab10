#pragma once

#include <QWidget>
#include "ui_overview.h"
#include <QBarCategoryAxis>
#include <QBarSeries>
#include <QBarSet>
#include <QValueAxis>

class Overview : public QWidget
{
	Q_OBJECT

public:
	Overview(QWidget *parent = nullptr);
	~Overview();

private:
	Ui::OverviewClass ui;
	QChart* chart;
	static const QStringList categories;
	QBarSet* set0;
	QBarSeries* series;
	QBarCategoryAxis* axisX;
	QValueAxis* axisY;

public slots:
	void update_data();
	void update_subjects();
};
