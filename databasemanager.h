#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <QSqlDatabase>
#include "taskentry.h"
#include "projectentry.h"

enum TaskState{
    ToDo = 1,
    Today = 2,
    Done = 3
};

class DatabaseManager
{
public:
    DatabaseManager();
    virtual ~DatabaseManager();

    int size(TaskState state) const;

    TaskEntry getTaskEntry(TaskState state, int index) const;
    void addTaskEntry(TaskEntry entry);
    void updateTaskEntry(TaskEntry entry);
    void removeTaskEntry(int id);
    void setTaskState(int id, int state);
    void insertIntoColumn(int state, int beforeRow, int taskId);

    QStringList listOfProjects() const;
    void addProjectEntry(ProjectEntry entry);
    ProjectEntry getProjectEntry(int index) const;
    void updateProjectEntry(ProjectEntry entry);

private:
    QSqlDatabase db;

    void openDatabase();
    QSqlQuery taskQuery(TaskState state) const;
    TaskEntry buildTaskEntry(const QSqlQuery &query) const;
    ProjectEntry buildProjectEntry(const QSqlQuery &query) const;
};

#endif // DATABASEMANAGER_H
