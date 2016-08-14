#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "aboutbox.h"
#include "taskcolumn.h"
#include "taskmodel.h"
#include "databasemanager.h"
#include "timercontroller.h"
#include "settingsdialog.h"
#include "statisticswindow.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(TimerController *timerController,
                        StatisticsWindow *statsWindow,
                        SettingsDialog *settings,
                        QWidget *parent = 0);
    ~MainWindow();

    void injectColumnWidgets(TaskColumn *todoColumn,
                             TaskColumn *todayColumn,
                             TaskColumn *doneColumn);
    void injectModels(TaskModel *todoModel,
                      TaskModel *todayModel,
                      TaskModel *doneModel);

private slots:
    void handleActionStatistics();

private:
    Ui::MainWindow *ui;
    QWidget *spacer;
    AboutBox *about;
    TaskColumn *todoColumn;
    TaskColumn *todayColumn;
    TaskColumn *doneColumn;
    TimerController *timerController;
    StatisticsWindow *statsWindow;
    SettingsDialog *settings;

    void setup();
    void connectModels(TaskModel *todoModel,
                       TaskModel *todayModel,
                       TaskModel *doneModel);
};

#endif // MAINWINDOW_H
