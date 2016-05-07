#ifndef TIMERCONTROLLER_H
#define TIMERCONTROLLER_H

#include <QAction>
#include <QObject>

#include "databasemanager.h"
#include "timedisplay.h"
#include <QToolBar>
#include <QTimer>
#include <QTime>

class TimerController : public QObject
{
    Q_OBJECT
public:
    explicit TimerController(QTimer *timer, QObject *parent = 0);
    virtual ~TimerController();

    void injectToolBar(QToolBar *toolBar);
    void injectDatabase(DatabaseManager *db);
    void injectDisplay(TimeDisplay *display);
    void injectTimerAction(QAction *timerAction);

signals:

public slots:
    void handleCurrentTaskChanged(int current, int previous);

private slots:
    void handleTimerToggled(bool on);
    void handleTimerTick();

private:
    QDateTime startTime;
    QToolBar *toolBar;
    QAction *timerAction;
    DatabaseManager *db;
    TimeDisplay *display;
    QTimer *timer;

    int elapsedMinutes();
    void logTimeRecord(int taskID);
};

#endif // TIMERCONTROLLER_H
