#include "timercontroller.h"
#include "taskcolumn.h"
#include <QMessageBox>
#include <stdexcept>

TimerController::TimerController(QTimer *timer, QObject *parent) : QObject(parent)
{
    this->timer = timer;
    timerAction = nullptr;
    db = nullptr;
    display = nullptr;

    connect(timer, SIGNAL(timeout()), this, SLOT(handleTimerTick()));
}

TimerController::~TimerController()
{
    delete display;
    delete timer;
}

void TimerController::injectToolBar(QToolBar *toolBar)
{
    this->toolBar = toolBar;
    toolBar->insertWidget(toolBar->actions().at(1), display);
}

void TimerController::injectDatabase(DatabaseManager *db)
{
    this->db = db;
}

void TimerController::injectDisplay(TimeDisplay *display)
{
    this->display = display;
}

void TimerController::injectTimerAction(QAction *timerAction)
{
    this->timerAction = timerAction;

    connect(timerAction, SIGNAL(toggled(bool)), this, SLOT(handleTimerToggled(bool)));
}

void TimerController::handleCurrentTaskChanged(int current, int previous)
{
    if(timer->isActive()){
        logTimeRecord(previous);
        startTime = QDateTime::currentDateTime();
        timer->stop();
        timer->start();
        handleTimerTick();
    }
}

void TimerController::handleTimerToggled(bool on)
{
    if(on){
        if(TaskColumn::getCurrentTaskID() == -1){
            timerAction->blockSignals(true);
            timerAction->setChecked(false);
            timerAction->blockSignals(false);

            QMessageBox::information(0, "No Task Selected", "Please select a task first.");

            return;
        }

        startTime = QDateTime::currentDateTime();
        timer->start(60000);
    }
    else{
        timer->stop();
        logTimeRecord(TaskColumn::getCurrentTaskID());
        display->setMinutes(0);
    }
}

void TimerController::handleTimerTick()
{
    display->setMinutes(elapsedMinutes());
}

int TimerController::elapsedMinutes()
{
    // +1 sec so that floor division yields 1 after 60 secs:
    return (startTime.secsTo(QDateTime::currentDateTime()) + 1) / 60;
}

void TimerController::logTimeRecord(int taskID)
{
    if(elapsedMinutes() != 0)
        db->addRecord(taskID, startTime, elapsedMinutes());
}

