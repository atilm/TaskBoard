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
    todoModel->setColumnType(ToDo);
    todayModel->setColumnType(Today);
    doneModel->setColumnType(Done);

    todoColumn->setModel(todoModel);
    todayColumn->setModel(todayModel);
    doneColumn->setModel(doneModel);

    connectModels(todoModel, todayModel, doneModel);
}

void MainWindow::setup()
{
    setWindowTitle("Task Board");
}

void MainWindow::connectModels(TaskModel *todoModel,
                               TaskModel *todayModel,
                               TaskModel *doneModel)
{
    connect(todoModel, SIGNAL(itemDropped()), todayModel, SLOT(sendAllDataChanged()));
    connect(todoModel, SIGNAL(itemDropped()), doneModel, SLOT(sendAllDataChanged()));

    connect(todayModel, SIGNAL(itemDropped()), todoModel, SLOT(sendAllDataChanged()));
    connect(todayModel, SIGNAL(itemDropped()), doneModel, SLOT(sendAllDataChanged()));

    connect(doneModel, SIGNAL(itemDropped()), todoModel, SLOT(sendAllDataChanged()));
    connect(doneModel, SIGNAL(itemDropped()), todayModel, SLOT(sendAllDataChanged()));
}
