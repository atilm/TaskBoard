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

void MainWindow::injectModels(TaskModel *todoModel,
                              TaskModel *todayModel,
                              TaskModel *doneModel)
{
    ui->todoColumn->setModel(todoModel);
    ui->todayColumn->setModel(todayModel);
    ui->doneColumn->setModel(doneModel);
}

void MainWindow::setup()
{
    setWindowTitle("Task Board");
    ui->todoColumn->setTitle("To do");
    ui->todayColumn->setTitle("To do today");
    ui->doneColumn->setTitle("Done");
}
