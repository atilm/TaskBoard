#include "timedisplay.h"

TimeDisplay::TimeDisplay(QWidget *parent) : QLabel(parent)
{
    setMinutes(0);
}

TimeDisplay::~TimeDisplay()
{

}

void TimeDisplay::setMinutes(int minutes)
{
    int fullHours = minutes / 60;
    int restingMinutes = minutes % 60;

    QString s = QString("%1 h %2 min")
            .arg(fullHours)
            .arg(restingMinutes, 2, 10, QLatin1Char('0'));

    setText(s);
}

