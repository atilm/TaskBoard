#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <QSqlDatabase>
#include "taskentry.h"

class DatabaseManager
{
public:
    DatabaseManager();
    virtual ~DatabaseManager();

    int size(const QString &filterString) const;
    TaskEntry getTaskEntry(const QString &filterString, int index) const;

private:
    QSqlDatabase db;

    void openDatabase();
    QSqlQuery taskQuery(const QString &filterString) const;
    TaskEntry buildTaskEntry(const QSqlQuery &query) const;
};

#endif // DATABASEMANAGER_H
