#include "burnupchartcreator.h"
#include "dailyworkchartcreator.h"
#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    DatabaseManager *db = new DatabaseManager();

    TimerController *timerController = new TimerController(new QTimer());

    timerController->injectDisplay(new TimeDisplay());
    timerController->injectDatabase(db);

    StatisticsWindow *statsWindow = new StatisticsWindow();
    statsWindow->addAnalyzer(new BurnUpChartCreator(db));
    statsWindow->addAnalyzer(new DailyWorkChartCreator(db));

    MainWindow w(timerController, statsWindow, 0);

    w.injectColumnWidgets(new TaskColumn(new EditTaskDialog(new EditProjectDialog())),
                          new TaskColumn(new EditTaskDialog(new EditProjectDialog())),
                          new TaskColumn(new EditTaskDialog(new EditProjectDialog())));

    w.injectModels(new TaskModel(db),
                   new TaskModel(db),
                   new TaskModel(db));


    w.show();

    return a.exec();
}
