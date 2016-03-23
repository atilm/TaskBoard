#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    DatabaseManager *db = new DatabaseManager();

    MainWindow w;

    w.injectColumnWidgets(new TaskColumn(new EditTaskDialog(new EditProjectDialog())),
                          new TaskColumn(new EditTaskDialog(new EditProjectDialog())),
                          new TaskColumn(new EditTaskDialog(new EditProjectDialog())));

    w.injectModels(new TaskModel(db),
                   new TaskModel(db),
                   new TaskModel(db));


    w.show();

    return a.exec();
}
