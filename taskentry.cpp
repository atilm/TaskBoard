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
    int hours = getHours(estimated_minutes);
    int minutes = getMinutes(estimated_minutes);

    if(hours == 0)
        return QString("%1 m").arg(minutes);
    else
        return QString("%1 h %2 m").arg(hours).arg(minutes);
}

QString TaskEntry::effortString() const
{
    return QString("0 m");
}

QString TaskEntry::createdString() const
{
    return created.toString();
}

QString TaskEntry::closedString() const
{
    return closed.toString();
}

void TaskEntry::setEstimate(const QString &s)
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

    estimated_minutes = minutes;
}

int TaskEntry::getHours(int minutes) const
{
    return minutes / 60;
}

int TaskEntry::getMinutes(int minutes) const
{
    return minutes % 60;
}

