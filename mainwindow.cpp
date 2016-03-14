#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "taskcolumn.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setup();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::injectColumnWidgets(TaskColumn *todoColumn,
                                     TaskColumn *todayColumn,
                                     TaskColumn *doneColumn)
{
    this->todoColumn = todoColumn;
    this->todayColumn = todayColumn;
    this->doneColumn = doneColumn;

    ui->horizontalLayout->replaceWidget(ui->todoColumn, todoColumn);
    ui->horizontalLayout->replaceWidget(ui->todayColumn, todayColumn);
    ui->horizontalLayout->replaceWidget(ui->doneColumn, doneColumn);

    todoColumn->setTitle("To do");
    todayColumn->setTitle("To do today");
    doneColumn->setTitle("Done");

    todoColumn->setColumnState(TaskEntry::todo);
    todayColumn->setColumnState(TaskEntry::today);
    doneColumn->setColumnState(TaskEntry::done);
}

void MainWindow::injectModels(TaskModel *todoModel,
                              TaskModel *todayModel,
                              TaskModel *doneModel)
{
    todoModel->setFilterString("tasks.state = 1");
    todayModel->setFilterString("tasks.state = 2");
    doneModel->setFilterString("tasks.state = 3");

    todoColumn->setModel(todoModel);
    todayColumn->setModel(todayModel);
    doneColumn->setModel(doneModel);
}

void MainWindow::setup()
{
    setWindowTitle("Task Board");
}
