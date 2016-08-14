#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "taskcolumn.h"
#include "version.h"

MainWindow::MainWindow(TimerController *timerController,
                       StatisticsWindow *statsWindow,
                       SettingsDialog *settings,
                       QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->timerController = timerController;
    timerController->injectToolBar(ui->mainToolBar);
    timerController->injectTimerAction(ui->actionTimer);

    this->statsWindow = statsWindow;

    this->settings = settings;

    todoColumn = nullptr;
    todayColumn = nullptr;
    doneColumn = nullptr;

    setup();
}

MainWindow::~MainWindow()
{
    delete ui;
    delete about;
    delete spacer;
    delete timerController;
    delete todoColumn;
    delete todayColumn;
    delete doneColumn;
    delete statsWindow;
    delete settings;
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

void MainWindow::handleActionStatistics()
{
    statsWindow->show();
}

void MainWindow::setup()
{
    about = new AboutBox(this);

    setWindowTitle(QString("%1 %2")
                   .arg("Task Board")
                   .arg(Version::versionNumberString()));

    // Add an expanding spacer widget to right align the about button
    spacer = new QWidget();
    spacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    ui->mainToolBar->insertWidget(ui->actionAbout, spacer);

    connect(ui->actionStatistics, SIGNAL(triggered(bool)), this, SLOT(handleActionStatistics()));
    connect(ui->actionSettings, SIGNAL(triggered(bool)), settings, SLOT(exec()));
    connect(ui->actionAbout, SIGNAL(triggered(bool)), about, SLOT(show()));
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

    connect(todoColumn, SIGNAL(currentTaskIDChanged(int,int)), todoModel, SLOT(sendAllDataChanged()));
    connect(todoColumn, SIGNAL(currentTaskIDChanged(int,int)), todayModel, SLOT(sendAllDataChanged()));
    connect(todoColumn, SIGNAL(currentTaskIDChanged(int,int)), doneModel, SLOT(sendAllDataChanged()));

    connect(todayColumn, SIGNAL(currentTaskIDChanged(int,int)), todoModel, SLOT(sendAllDataChanged()));
    connect(todayColumn, SIGNAL(currentTaskIDChanged(int,int)), todayModel, SLOT(sendAllDataChanged()));
    connect(todayColumn, SIGNAL(currentTaskIDChanged(int,int)), doneModel, SLOT(sendAllDataChanged()));

    connect(doneColumn, SIGNAL(currentTaskIDChanged(int,int)), todoModel, SLOT(sendAllDataChanged()));
    connect(doneColumn, SIGNAL(currentTaskIDChanged(int,int)), todayModel, SLOT(sendAllDataChanged()));
    connect(doneColumn, SIGNAL(currentTaskIDChanged(int,int)), doneModel, SLOT(sendAllDataChanged()));

    connect(todoColumn, SIGNAL(currentTaskIDChanged(int,int)), timerController, SLOT(handleCurrentTaskChanged(int, int)));
    connect(todayColumn, SIGNAL(currentTaskIDChanged(int,int)), timerController, SLOT(handleCurrentTaskChanged(int, int)));
    connect(doneColumn, SIGNAL(currentTaskIDChanged(int,int)), timerController, SLOT(handleCurrentTaskChanged(int, int)));
}

