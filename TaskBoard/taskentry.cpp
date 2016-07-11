#include "taskentry.h"
#include <QDebug>

TaskEntry::TaskEntry()
{

}

TaskEntry::~TaskEntry()
{

}

QString TaskEntry::estimateString() const
{
    return minutesToString(estimated_minutes);
}

QString TaskEntry::effortString() const
{
    return minutesToString(effort_minutes);
}

QString TaskEntry::createdString() const
{
    return created.toString(QString("yyyy-MM-dd"));
}

QString TaskEntry::closedString() const
{
    return closed.toString(QString("yyyy-MM-dd"));
}

void TaskEntry::setEstimate(const QString &s)
{
    estimated_minutes = stringToMinutes(s);
}

void TaskEntry::setEffort(const QString &s)
{
    effort_minutes = stringToMinutes(s);
}

void TaskEntry::setState(int value)
{
    switch(value){
    case TaskEntry::done:
        state = TaskEntry::done;
        break;
    case TaskEntry::today:
        state = TaskEntry::today;
        break;
    case TaskEntry::todo:
        state = TaskEntry::todo;
        break;
    default:
        state = TaskEntry::todo;
    }
}

int TaskEntry::stringToMinutes(const QString &s) const
{
    // Capture one or more digits
    // followed by zero or more whitespace
    // followed by 'h'
    QRegExp hourExp("(\\d+)\\s*h");

    // Capture one or more digits
    // followed by zero or more whitespace
    // followed by 'm'
    QRegExp minuteExp("(\\d+)\\s*m");

    int minutes = 0;

    if( hourExp.indexIn(s) > -1 )
        minutes += hourExp.cap(1).toInt() * 60;

    if( minuteExp.indexIn(s) > -1)
        minutes += minuteExp.cap(1).toInt();

    return minutes;
}

QString TaskEntry::minutesToString(int minutes) const
{
    int hours = getHours(minutes);
    int mins = getMinutes(minutes);

    if(hours == 0)
        return QString("%1 m").arg(mins);
    else
        return QString("%1 h %2 m").arg(hours).arg(mins);
}

int TaskEntry::getHours(int minutes) const
{
    return minutes / 60;
}

int TaskEntry::getMinutes(int minutes) const
{
    return minutes % 60;
}

