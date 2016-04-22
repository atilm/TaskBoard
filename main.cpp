#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    DatabaseManager *db = new DatabaseManager();

    TimerController *timerController = new TimerController(new QTimer());

    timerController->injectDisplay(new TimeDisplay());
    timerController->injectDatabase(db);

    MainWindow w(timerController, 0);

    w.injectColumnWidgets(new TaskColumn(new EditTaskDialog(new EditProjectDialog())),
                          new TaskColumn(new EditTaskDialog(new EditProjectDialog())),
                          new TaskColumn(new EditTaskDialog(new EditProjectDialog())));

    w.injectModels(new TaskModel(db),
                   new TaskModel(db),
                   new TaskModel(db));


    w.show();

    return a.exec();
}
