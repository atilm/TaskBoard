#ifndef TASKENTRY_H
#define TASKENTRY_H

#include <QDateTime>
#include <QString>

class TaskEntry
{
public:

    enum State{
        todo = 1,
        today = 2,
        done = 3
    };

    TaskEntry();
    virtual ~TaskEntry();

    QString estimateString() const;
    QString effortString() const;
    QString createdString() const;
    QString closedString() const;

    void setEstimate(const QString &s);
    void setEffort(const QString &s);

    int id;
    QString title;
    QString description;
    QString projectShort;
    int projectIndex;
    int estimated_minutes;
    int effort_minutes;
    int colorIndex;
    QDateTime created;
    QDateTime closed;
    State state;
    int sortingOrder;

private:
    int stringToMinutes(const QString &s) const;
    QString minutesToString(int minutes) const;
    int getHours(int minutes) const;
    int getMinutes(int minutes) const;
};

#endif // TASKENTRY_H
