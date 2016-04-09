#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "taskcolumn.h"
#include "taskmodel.h"
#include "databasemanager.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void injectColumnWidgets(TaskColumn *todoColumn,
                             TaskColumn *todayColumn,
                             TaskColumn *doneColumn);
    void injectModels(TaskModel *todoModel,
                      TaskModel *todayModel,
                      TaskModel *doneModel);

private:
    Ui::MainWindow *ui;
    TaskColumn *todoColumn;
    TaskColumn *todayColumn;
    TaskColumn *doneColumn;

    void setup();
    void connectModels(TaskModel *todoModel,
                       TaskModel *todayModel,
                       TaskModel *doneModel);
};

#endif // MAINWINDOW_H
