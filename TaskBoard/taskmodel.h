#ifndef TASKMODEL_H
#define TASKMODEL_H

#include <QAbstractListModel>
#include "databasemanager.h"
#include "taskentry.h"
#include "projectentry.h"

class TaskModel : public QAbstractListModel
{
    Q_OBJECT
public:
    TaskModel(DatabaseManager *db, QObject* parent = 0);
    virtual ~TaskModel();

    virtual void setColumnType(TaskState columnType);
    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;
    virtual QVariant data(const QModelIndex &index, int role) const;
    virtual Qt::ItemFlags flags(const QModelIndex &index) const;
    virtual TaskEntry getTask(const QModelIndex &index) const;
    virtual void addTask(TaskEntry entry);
    virtual void updateTask(TaskEntry entry);
    virtual void removeRow(int row, const QModelIndex &parent = QModelIndex());

    virtual QStringList mimeTypes() const;
    virtual QMimeData *mimeData(const QModelIndexList &indexes) const;
    virtual bool dropMimeData(const QMimeData *data, Qt::DropAction action,
                              int row, int column, const QModelIndex &parent);

    virtual QStringList projectList() const;
    virtual QList<ProjectEntry> getActiveProjects() const;
    virtual ProjectEntry getProject(int index) const;
    virtual void updateProject(ProjectEntry entry);
    virtual void addProject(ProjectEntry entry);

signals:
    void itemDropped();

public slots:
    void sendAllDataChanged();
    void emitTaskChanged(int taskIndex);

private:
    DatabaseManager *db;
    TaskState columnType;

    QString queryString();
};

#endif // TASKMODEL_H
