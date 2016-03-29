#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <QSqlDatabase>
#include "taskentry.h"
#include "projectentry.h"

class DatabaseManager
{
public:
    DatabaseManager();
    virtual ~DatabaseManager();

    int size(const QString &filterString) const;

    TaskEntry getTaskEntry(const QString &filterString, int index) const;
    void addTaskEntry(TaskEntry entry);
    void updateTaskEntry(TaskEntry entry);
    void removeTaskEntry(int id);

    QStringList listOfProjects() const;
    ProjectEntry getProjectEntry(int index) const;
    void updateProjectEntry(ProjectEntry entry);

private:
    QSqlDatabase db;

    void openDatabase();
    QSqlQuery taskQuery(const QString &filterString) const;
    TaskEntry buildTaskEntry(const QSqlQuery &query) const;
    ProjectEntry buildProjectEntry(const QSqlQuery &query) const;
};

#endif // DATABASEMANAGER_H
