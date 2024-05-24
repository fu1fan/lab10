#include "overview.h"
#include "common.h"
#include <string>

const QStringList Overview::categories = { "0-60", "60-70", "70-80", "80-90", "90-100" };

static float counts[5] = { 0 };

Overview::Overview(QWidget* parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	set0 = new QBarSet("人数");
	series = new QBarSeries();
	axisX = new QBarCategoryAxis();
	axisY = new QValueAxis();

	this->chart = new QChart();
	this->chart->setTitle("成绩直方图");
	this->chart->setAnimationOptions(QChart::SeriesAnimations);

	*set0 << counts[0] << counts[1] << counts[2] << counts[3] << counts[4];

	this->series->append(set0);

	this->chart->addSeries(series);

	this->axisX->append(categories);
	this->chart->addAxis(axisX, Qt::AlignBottom);
	this->series->attachAxis(axisX);

	axisY->setLabelFormat("%d");
	this->chart->addAxis(axisY, Qt::AlignLeft);
	this->series->attachAxis(axisY);

	this->chart->legend()->setVisible(true);
	this->chart->legend()->setAlignment(Qt::AlignBottom);

	ui.graphicsView->setChart(chart);

	connect(ui.loadButton, &QPushButton::clicked, this, &Overview::update_data);

	if (subjects_db->get_subject_count() == 0)
	{
		ui.subjectBox->addItem("目前无科目");
	}
	else
	{
		for (json::iterator it = subjects_db->begin(); it != subjects_db->end(); ++it)
		{
			ui.subjectBox->addItem(" " + QString::fromStdString(subjects_db->get_subject_name(it)) + "（" + QString::fromStdString(subjects_db->get_subject_code(it)) + "）");
		}
		update_data();
	}
}

Overview::~Overview()
{
	delete chart;
	delete set0;
	delete series;
	delete axisX;
	delete axisY;

}

void Overview::update_data()
{
	// 清空counts
	for (int i = 0; i < 10; i++)
	{
		counts[i] = 0;
	}
	string subject_it = subjects_db->get_subject_id(ui.subjectBox->currentIndex());
	for (json::iterator it = performance_db->begin(); it != performance_db->end(); ++it)
	{
		float score = performance_db->calculate_score(it, subject_it);
		if (score < 60)
		{
			counts[0]++;
		}
		else if (score < 70)
		{
			counts[1]++;
		}
		else if (score < 80)
		{
			counts[2]++;
		}
		else if (score < 90)
		{
			counts[3]++;
		}
		else
		{
			counts[4]++;
		}
	}
	axisY->setRange(0, (int)(*std::max_element(counts, counts + 10) * 1.2) + 1);
	
	// 给set0重新赋值
	set0->remove(0, 10);
	*set0 << counts[0] << counts[1] << counts[2] << counts[3] << counts[4];
	
	this->chart->update();
}
