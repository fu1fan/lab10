#include "setting.h"
#include "common.h"
#include <QMessageBox>

Setting::Setting(QWidget* parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	subjects_m = new QStringListModel(this);
	subjects_l = new QStringList();

	size_t count = subjects_db->get_subject_count();
	for (size_t i = 0; i < count; i++)
	{
		subjects_l->append(
			QString::fromStdString(
				subjects_db->get_subject_name(
					subjects_db->get_subject_id(i)
				)
			)
		);
	}

	subjects_m->setStringList(*subjects_l);
	ui.listView->setModel(subjects_m);

	connect(ui.listView, SIGNAL(clicked(const QModelIndex&)), this, SLOT(select_subject(const QModelIndex&)));

	items_tb = new QStandardItemModel(0, 2, this);
	items_tb->setHorizontalHeaderItem(0, new QStandardItem("评分项目"));
	items_tb->setHorizontalHeaderItem(1, new QStandardItem("权重（%）"));
	ui.tableView->setModel(items_tb);
	ui.tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);


	connect(ui.toolButton, SIGNAL(clicked()), this, SLOT(add_subject()));
	connect(ui.toolButton_2, SIGNAL(clicked()), this, SLOT(remove_subject()));

	ui.subjectCountLabel->setText("共计：" + QString::number(count));

	connect(items_tb, SIGNAL(itemChanged(QStandardItem*)), this, SLOT(change_subject_item()));

	connect(ui.toolButton_3, SIGNAL(clicked()), this, SLOT(add_subject_item()));
	connect(ui.toolButton_4, SIGNAL(clicked()), this, SLOT(remove_subject_item()));
	connect(ui.pushButton, SIGNAL(clicked()), this, SLOT(apply()));
}

Setting::~Setting()
{
	delete subjects_m;
	delete subjects_l;
	delete items_tb;
}

void Setting::select_subject(const QModelIndex& index)
{
	size_t subject_index = index.row();
	selected_subject_id = subjects_db->get_subject_id(subject_index);
	std::string id = subjects_db->get_subject_id(subject_index);

	ui.lineEdit->setText(QString::fromStdString(subjects_db->get_subject_code(id)));
	ui.lineEdit_2->setText(QString::fromStdString(subjects_db->get_subject_name(id)));

	auto weight_map = subjects_db->get_subject_items(id);
	// 清空表格
	items_tb->removeRows(0, items_tb->rowCount());
	int count_up = 0;
	for (auto it = weight_map.begin(); it != weight_map.end(); it++)
	{
		QList<QStandardItem*> items;
		items.append(new QStandardItem(QString::fromStdString(it->first)));
		items.append(new QStandardItem(QString::number(it->second)));
		items_tb->appendRow(items);
		count_up += it->second;
	}
	// 显示总权重
	ui.label_5->setText("总权重：" + QString::number(count_up) + "%");
	if (count_up == 100) {
		// 
		ui.label_5->setStyleSheet("color: green");
	}
	else {
		ui.label_5->setStyleSheet("color: red");
	}

	// 显示已录入学生数
	ui.studentCountLabel->setText("已录入学生数：" + QString::number(performance_db->get_student_count(id)));
}

void Setting::add_subject()
{
	//subjects_db->add_subject(code, name);
	//在列表中添加新项目
	subjects_l->append(QString::fromStdString("未命名科目"));
	subjects_m->setStringList(*subjects_l);
	ui.listView->setModel(subjects_m);

	//更新数据库
	std::string id = subjects_db->add_subject("未命名科目", "未命名科目");
	subjects_db->add_item(id, "成绩", 100);

	//更新界面
	//选中新项目
	int index = (int)subjects_db->get_subject_count() - 1;
	ui.listView->setCurrentIndex(subjects_m->index(index));
	select_subject(subjects_m->index(index));

	subjects_db->save();

	size_t count = subjects_db->get_subject_count();
	ui.subjectCountLabel->setText("共计：" + QString::number(count));
}

void Setting::remove_subject()
{
	QModelIndex index = ui.listView->currentIndex();
	if (index.isValid() == false)
	{
		return;
	}
	size_t subject_index = index.row();
	std::string id = subjects_db->get_subject_id(subject_index);
	if (check_existing_student(id)) {
		subjects_db->delete_subject(id);
		subjects_db->save();
		subjects_l->removeAt(subject_index);
		subjects_m->setStringList(*subjects_l);
		ui.listView->setModel(subjects_m);

		size_t count = subjects_db->get_subject_count();
		ui.subjectCountLabel->setText("共计：" + QString::number(count));
	}
}

void Setting::change_subject_item() {
	// 计算总权重
	int count_up = 0;
	for (int i = 0; i < items_tb->rowCount(); i++) {
		count_up += items_tb->item(i, 1)->text().toInt();
	}
	ui.label_5->setText("总权重：" + QString::number(count_up) + "%");
	if (count_up == 100) {
		// 
		ui.label_5->setStyleSheet("color: green");
	}
	else {
		ui.label_5->setStyleSheet("color: red");
	}
}

void Setting::add_subject_item() {
	QList<QStandardItem*> items;
	items.append(new QStandardItem("未命名项目"));
	items.append(new QStandardItem("0"));
	items_tb->appendRow(items);
}

void Setting::remove_subject_item() {
	QModelIndex index = ui.tableView->currentIndex();
	if (index.isValid() == false) {
		return;
	}
	items_tb->removeRow(index.row());
	change_subject_item();	// 重新计算总权重
}

bool Setting::check_existing_student(std::string subject_id) {
	// 检查是否有学生已经录入
	if (performance_db->get_student_count(subject_id) != 0)
	{
		QMessageBox::StandardButton reply;
		reply = QMessageBox::question(this, "警告", "已有学生录入，是否继续？",
			QMessageBox::Yes | QMessageBox::No);
		if (reply == QMessageBox::No) {
			return false;
		}
	}
	return true;
}
void Setting::apply() {
	// 先检查总权重是否为100
	int count_up = 0;
	std::string id = selected_subject_id;
	for (int i = 0; i < items_tb->rowCount(); i++) {
		count_up += items_tb->item(i, 1)->text().toInt();
	}
	if (count_up != 100) {
		QMessageBox::warning(this, "错误", "总权重不为100%");
		return;
	}

	// 检查是否有学生已经录入
	if (!check_existing_student(id)) {
		return;
	}

	// 更新数据库
	subjects_db->set_subject_code(id, ui.lineEdit->text().toStdString());
	subjects_db->set_subject_name(id, ui.lineEdit_2->text().toStdString());
	subjects_db->clear_items(id);
	for (int i = 0; i < items_tb->rowCount(); i++) {
		subjects_db->add_item(id, items_tb->item(i, 0)->text().toStdString(), items_tb->item(i, 1)->text().toInt());
	}
	subjects_db->save();
	QMessageBox::information(this, "成功", "保存成功");
	
	// 刷新课程列表
	size_t count = subjects_db->get_subject_count();
	subjects_l->clear();
	for (size_t i = 0; i < count; i++)
	{
		subjects_l->append(
			QString::fromStdString(
				subjects_db->get_subject_name(
					subjects_db->get_subject_id(i)
				)
			)
		);
	}
	subjects_m->setStringList(*subjects_l);
	ui.listView->setModel(subjects_m);
	// 重新选中当前课程
}
