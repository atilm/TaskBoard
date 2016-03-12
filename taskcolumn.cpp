#include "taskcolumn.h"
#include "ui_taskcolumn.h"
#include "qlistviewdelegate.h"

TaskColumn::TaskColumn(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TaskColumn)
{
    ui->setupUi(this);

    QListViewDelegate *delegate = new QListViewDelegate();
    ui->listView->setItemDelegate(delegate);
}

TaskColumn::~TaskColumn()
{
    delete ui;
}

void TaskColumn::setTitle(const QString &title)
{
    ui->label->setText(title);
}

void TaskColumn::setModel(QAbstractListModel *model)
{
    ui->listView->setModel(model);
}
