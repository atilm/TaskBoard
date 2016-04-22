#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <QVector>
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
    QVector<TaskEntry> getTaskEntries(TaskState state) const;
    void addTaskEntry(TaskEntry entry);
    void updateTaskEntry(TaskEntry entry);
    void updateTaskField(int id, const QString &fieldName, int value);
    void removeTaskEntry(int id);
    void setTaskState(int id, int state);
    void setTaskStateAndIndex(int id, int state, int index);
    void insertIntoColumn(TaskState state, int beforeRow, int taskId);

    QStringList listOfProjects() const;
    void addProjectEntry(ProjectEntry entry);
    ProjectEntry getProjectEntry(int index) const;
    void updateProjectEntry(ProjectEntry entry);

    void addRecord(int taskID, int minutes);
    int getEffortForTask(int taskID) const;

private:
    QSqlDatabase db;
    const int maximumInt;

    void openDatabase();
    QSqlQuery taskQuery(TaskState state) const;
    TaskEntry buildTaskEntry(const QSqlQuery &query) const;
    ProjectEntry buildProjectEntry(const QSqlQuery &query) const;
    void rearrangeSortingOrder(TaskState state);
    void getSortingIndices(TaskState state, int row, int &currentIndex, int &previousIndex);
    int getNewSortingIndex(TaskState state);
};

#endif // DATABASEMANAGER_H
