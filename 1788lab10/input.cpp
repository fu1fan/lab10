#include "input.h"
#include <string>
#include <QInputDialog>
#include <QMessageBox>


Input::Input(QWidget* parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	subjects_m = new QStringListModel(this);
	subjects_l = new QStringList();

	connect(ui.comboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(select_subject(int)));
	connect(ui.studentsView, SIGNAL(clicked(const QModelIndex&)), this, SLOT(select_student(const QModelIndex&)));

	items_tb = new QStandardItemModel(0, 2, this);
	items_tb->setHorizontalHeaderItem(0, new QStandardItem("评分项目"));
	items_tb->setHorizontalHeaderItem(1, new QStandardItem("分数"));
	ui.tableView->setModel(items_tb);
	ui.tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

	connect(items_tb, SIGNAL(itemChanged(QStandardItem*)), this, SLOT(update_performance()));

	connect(ui.lineEdit_3, SIGNAL(textChanged(const QString&)), this, SLOT(update_student_info()));
	connect(ui.lineEdit_4, SIGNAL(textChanged(const QString&)), this, SLOT(update_student_info()));

	connect(ui.toolButton_7, SIGNAL(clicked()), this, SLOT(remove_student()));
	connect(ui.toolButton_8, SIGNAL(clicked()), this, SLOT(add_student()));
	student_ids = vector<std::string>();
}


Input::~Input()
{
	delete subjects_m;
	delete subjects_l;
	delete items_tb;
}

void Input::select_subject(int index)
{
	//ui.subjectsView->selectionModel()->selectedIndexes();
	//清空
	if (index == -1)//无科目
	{
		subjects_l->clear();
		subjects_m->setStringList(*subjects_l);
		ui.studentsView->setModel(subjects_m);
		return;
	}
	subjects_l->clear();
	std::string id = subjects_db->get_subject_id(index);
	size_t count = performance_db->get_all_student_cout();
	size_t ex = 0;

	std::string student_id;

	student_ids.clear();

	for (size_t i = 0; i < count; i++)
	{
		student_id = performance_db->get_student_id(i);
		if (not (performance_db->student_has_subject(student_id, id)))
		{
			continue;
		}
		ex++;
		subjects_l->append(
			QString::fromStdString(
				performance_db->get_student_name(student_id)
			)
		);
		student_ids.push_back(student_id);
	}

	subjects_m->setStringList(*subjects_l);
	ui.studentsView->setModel(subjects_m);
	// 刷新列表
	ui.label_12->setText("共计：" + QString::number(ex));
}

void Input::update_subjects() {
	ui.comboBox->clear();
	if (subjects_db->get_subject_count() == 0)
	{
		ui.comboBox->addItem("目前无科目");
	}
	else
	{
		for (json::iterator it = subjects_db->begin(); it != subjects_db->end(); ++it)
		{
			ui.comboBox->addItem(" " + QString::fromStdString(subjects_db->get_subject_name(it)) + "（" + QString::fromStdString(subjects_db->get_subject_code(it)) + "）");
		}
		select_subject(ui.comboBox->currentIndex());
	}
	// 清空
	ui.lineEdit_3->clear();
	ui.lineEdit_4->clear();
	ui.label_10->setText("总分：");
	// 清空表格
	items_tb->removeRows(0, items_tb->rowCount());
}

void Input::select_student(const QModelIndex& index)
{
	if (!index.isValid()) {
		return;
	}
	std::string subject_id = subjects_db->get_subject_id(ui.comboBox->currentIndex());
	auto r = index.row();
	std::string student_id = student_ids[index.row()];
	std::string student_name = performance_db->get_student_name(student_id);

	// 读取当前科目评分权重map
	std::map<std::string, int> items = subjects_db->get_subject_items(subject_id);
	// 遍历map
	for (auto it = items.begin(); it != items.end(); ++it) {
		// 依次将绩项替换map中的第二项
		items[it->first] = performance_db->get_score(student_id, subject_id, it->first);
	}
	ui.lineEdit_3->setText(QString::fromStdString(student_id));
	ui.lineEdit_4->setText(QString::fromStdString(student_name));

	// 清空表格
	items_tb->removeRows(0, items_tb->rowCount());
	// 遍历map
	for (auto it = items.begin(); it != items.end(); ++it)
	{
		QList<QStandardItem*> items;
		items.append(new QStandardItem(QString::fromStdString(it->first)));
		items.append(new QStandardItem(QString::number(it->second)));
		items_tb->appendRow(items);
	}

	// 计算总分
	ui.label_10->setText("总分：" + QString::number(performance_db->calculate_score(student_id, subject_id)));
}

void Input::remove_student()
{
	auto cur_index = ui.studentsView->currentIndex();
	if (!cur_index.isValid()) {
		return;
	}
	auto subject_id = subjects_db->get_subject_id(ui.comboBox->currentIndex());
	if (!cur_index.isValid()) {
		return;
	}
	std::string student_id = student_ids[cur_index.row()];
	performance_db->remove_student_subject(student_id, subject_id);
	performance_db->save();
	select_subject(ui.comboBox->currentIndex());
	select_subject(ui.comboBox->currentIndex());// 此程序依靠bug实现刷新
}

void Input::add_student()
{
	std::string student_id;
	std::string student_name;
	// 弹出对话框输入学号姓名
	student_id = QInputDialog::getText(this, "输入学号", "请输入学号").toStdString();
	student_name = QInputDialog::getText(this, "输入姓名", "请输入姓名").toStdString();
	// 检查是否为空
	if (student_id.empty() || student_name.empty()) {
		// 弹出警告
		QMessageBox::warning(this, "警告", "学号或姓名为空");
		return;
	}
	// 检查是否已存在
	if (performance_db->has_student_id(student_id)) {
		if (student_name != performance_db->get_student_name(student_id))
		{
			// 弹出警告
			QMessageBox::warning(this, "警告", "学号已存在，姓名不一致");
			return;
		}
		if (performance_db->student_has_subject(student_id, subjects_db->get_subject_id(ui.comboBox->currentIndex())))
		{
			// 弹出警告
			QMessageBox::warning(this, "警告", "学号已存在，科目已录入");
			return;
		}
	}
	else {
		// 说明学生不存在
		performance_db->add_student(student_id, student_name);
	}
	performance_db->add_student_subject(student_id, subjects_db->get_subject_id(ui.comboBox->currentIndex()));

	select_subject(ui.comboBox->currentIndex());

	performance_db->save();
}

void Input::update_performance()
{
	std::string subject_id = subjects_db->get_subject_id(ui.comboBox->currentIndex());
	// 不能同过下标获取学生id
	std::string student_id = student_ids[ui.studentsView->currentIndex().row()];
	// 读取当前科目评分权重map
	std::map<std::string, int> items = subjects_db->get_subject_items(subject_id);
	for (int i = 0; i < items_tb->rowCount(); i++)
	{
		std::string item = items_tb->item(i, 0)->text().toStdString();
		float value = items_tb->item(i, 1)->text().toFloat();
		performance_db->set_score(student_id, subject_id, item, value);
	}
	ui.label_10->setText("总分：" + QString::number(performance_db->calculate_score(student_id, subject_id)));

	performance_db->save();
}

void Input::update_student_info()
{
	auto cur_index = ui.studentsView->currentIndex();
	if (!cur_index.isValid()) {
		return;
	}
	std::string student_id = student_ids[cur_index.row()];

	// 设置id
	if (student_id!= ui.lineEdit_3->text().toStdString()){
		//非空检查
		if (ui.lineEdit_3->text().isEmpty()) {
			return;
		}
		performance_db->set_student_id(student_id, ui.lineEdit_3->text().toStdString());
		performance_db->save();
		return;
	}
	// 设置姓名
	if (performance_db->get_student_name(student_id) != ui.lineEdit_4->text().toStdString()) {
		//非空检查
		if (ui.lineEdit_4->text().isEmpty()) {
			return;
		}
		performance_db->set_student_name(student_id, ui.lineEdit_4->text().toStdString());
	
	}
	
	performance_db->save();

	// 更新列表
	select_subject(ui.comboBox->currentIndex());
	ui.studentsView->setCurrentIndex(cur_index);
	select_student(cur_index);
}
