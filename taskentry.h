#ifndef TASKENTRY_H
#define TASKENTRY_H

#include <QDateTime>
#include <QString>

class TaskEntry
{
public:
    TaskEntry();
    virtual ~TaskEntry();

    QString estimateString() const;
    QString effortString() const;

    QString title;
    QString description;
    QString projectShort;
    int estimated_minutes;
    int colorIndex;
    QDateTime created;
    QDateTime closed;

private:
    int getHours(int minutes) const;
    int getMinutes(int minutes) const;
};

#endif // TASKENTRY_H
