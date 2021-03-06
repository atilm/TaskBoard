#include "taskcolumn.h"
#include "ui_taskcolumn.h"
#include "qlistviewdelegate.h"

#include <QDebug>
#include <QKeyEvent>
#include <QMessageBox>

int TaskColumn::currentTaskID = -1;

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
    ui->listView->setDragEnabled(true);
    ui->listView->setAcceptDrops(true);
    ui->listView->setDropIndicatorShown(true);
    ui->listView->installEventFilter(this);

    connect(ui->listView, SIGNAL(customContextMenuRequested(QPoint)),
            this, SLOT(handleListViewContextMenuRequested(QPoint)));
    connect(ui->listView, SIGNAL(clicked(QModelIndex)),
            this, SLOT(handleTaskClicked(QModelIndex)));
    connect(ui->listView, SIGNAL(doubleClicked(QModelIndex)),
            this, SLOT(handleTaskDoubleClicked(QModelIndex)));
}

TaskColumn::~TaskColumn()
{
    delete ui;
    delete editDialog;
}

int TaskColumn::getCurrentTaskID()
{
    return currentTaskID;
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

bool TaskColumn::eventFilter(QObject *, QEvent *event)
{
    if(event->type() == QEvent::KeyPress){
        return true;
    }
    else{
        return false;
    }
}

void TaskColumn::handleAddClicked()
{
    editDialog->clear();
    editDialog->setTaskModel(model);

    if(editDialog->exec()){
        TaskEntry task = editDialog->getTaskEntry();
        task.state = columnState;
        if(columnState == TaskEntry::done)
            task.closed = QDateTime::currentDateTime();

        model->addTask(task);
    }
}

void TaskColumn::handleRemoveCurrent()
{
    if(QMessageBox::question(this,
                             tr("Remove Task"),
                             tr("Do you really want to remove this task and all it's records?"))
            == QMessageBox::Yes)
    {
        model->removeRow(ui->listView->currentIndex().row());
    }
}

void TaskColumn::handleTaskClicked(QModelIndex index)
{
    int previous = currentTaskID;
    currentTaskID = model->getTask(index).id;

    emit currentTaskIDChanged(currentTaskID, previous);
}

void TaskColumn::handleTaskDoubleClicked(QModelIndex index)
{
    editDialog->clear();
    editDialog->setTaskModel(model);
    editDialog->setTaskEntry(model->getTask(index));

    if(editDialog->exec())
        model->updateTask(editDialog->getTaskEntry());
}

void TaskColumn::handleListViewContextMenuRequested(QPoint point)
{
    QMenu menu(ui->listView);
    menu.addAction("Delete", this, SLOT(handleRemoveCurrent()));

    if(ui->listView->indexAt(point).isValid())
        menu.exec(ui->listView->mapToGlobal(point));
}
