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

int TaskEntry::getHours(int minutes) const
{
    return minutes / 60;
}

int TaskEntry::getMinutes(int minutes) const
{
    return minutes % 60;
}

