#ifndef TASKMODEL_H
#define TASKMODEL_H

#include <QAbstractListModel>
#include "databasemanager.h"
#include "taskentry.h"

class TaskModel : public QAbstractListModel
{
public:
    TaskModel(DatabaseManager *db, QObject* parent = 0);
    virtual ~TaskModel();

    virtual void setFilterString(const QString &s);
    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;
    virtual QVariant data(const QModelIndex &index, int role) const;
    virtual TaskEntry getTask(const QModelIndex &index) const;
    virtual void addTask(TaskEntry entry);
    virtual void updateTask(TaskEntry entry);
    virtual void removeRow(int row, const QModelIndex &parent = QModelIndex());

private:
    DatabaseManager *db;
    QString filterString;

    QString queryString();
};

#endif // TASKMODEL_H
