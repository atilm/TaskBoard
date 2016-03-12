#ifndef TASKENTRY_H
#define TASKENTRY_H

#include <QDateTime>
#include <QString>

class TaskEntry
{
public:
    TaskEntry();
    virtual ~TaskEntry();

    QString title;
    QString description;
    QString projectShort;
    int estimated_minutes;
    int colorIndex;
    QDateTime created;
    QDateTime closed;
};

#endif // TASKENTRY_H
