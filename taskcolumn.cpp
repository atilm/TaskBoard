#include "taskcolumn.h"
#include "ui_taskcolumn.h"
#include "qlistviewdelegate.h"
#include <QDebug>

TaskColumn::TaskColumn(EditTaskDialog *editDialog,
                       QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TaskColumn)
{
    ui->setupUi(this);
    this->editDialog = editDialog;

    QListViewDelegate *delegate = new QListViewDelegate();
    ui->listView->setItemDelegate(delegate);

    connect(ui->addButton, SIGNAL(clicked(bool)),
            this, SLOT(handleAddClicked()));

    ui->listView->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->listView, SIGNAL(customContextMenuRequested(QPoint)),
            this, SLOT(handleListViewContextMenuRequested(QPoint)));
}

TaskColumn::~TaskColumn()
{
    delete ui;
    delete editDialog;
}

void TaskColumn::setTitle(const QString &title)
{
    ui->label->setText(title);
}

void TaskColumn::setModel(TaskModel *model)
{
    this->model = model;
    ui->listView->setModel(model);
}

void TaskColumn::setColumnState(TaskEntry::State state)
{
    columnState = state;
}

void TaskColumn::handleAddClicked()
{
    editDialog->clear();

    if(editDialog->exec()){
        TaskEntry task = editDialog->getTaskEntry();
        task.state = columnState;

        model->addTask(task);
    }
}

void TaskColumn::handleRemoveCurrent()
{
    model->removeRow(ui->listView->currentIndex().row());
}

void TaskColumn::handleListViewContextMenuRequested(QPoint point)
{
    QMenu menu(ui->listView);
    menu.addAction("Delete", this, SLOT(handleRemoveCurrent()));

    if(ui->listView->indexAt(point).isValid())
        menu.exec(ui->listView->mapToGlobal(point));
}
