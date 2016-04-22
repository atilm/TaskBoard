#include "timercontroller.h"

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

void TimerController::handleTimerToggled(bool on)
{
    if(on){
        startTime = QDateTime::currentDateTime();
        timer->start(1000);
    }
    else{
        timer->stop();
        display->setMinutes(0);
    }
}

void TimerController::handleTimerTick()
{
    int seconds = startTime.secsTo(QDateTime::currentDateTime());
    display->setMinutes(seconds);
}

