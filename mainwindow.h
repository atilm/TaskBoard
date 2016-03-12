#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
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

    void injectModels(TaskModel *todoModel,
                      TaskModel *todayModel,
                      TaskModel *doneModel);

private:
    Ui::MainWindow *ui;

    void setup();
};

#endif // MAINWINDOW_H
