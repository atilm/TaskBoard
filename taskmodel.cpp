#include "taskmodel.h"
#include <QDebug>

TaskModel::TaskModel(DatabaseManager *db, QObject *parent)
    : QAbstractListModel(parent)
{
    this->db = db;
}

TaskModel::~TaskModel()
{

}

void TaskModel::setFilterString(const QString &s)
{
    filterString = s;
}

int TaskModel::rowCount(const QModelIndex &parent) const
{
    return db->size(filterString);
}

QVariant TaskModel::data(const QModelIndex &index, int role) const
{
    QVariant();
}

TaskEntry TaskModel::getTask(const QModelIndex &index) const
{
    return db->getTaskEntry(filterString, index.row());
}

void TaskModel::addTask(TaskEntry entry)
{
    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    db->addTaskEntry(entry);
    endInsertRows();
}

void TaskModel::updateTask(TaskEntry entry)
{
    db->updateTaskEntry(entry);
}

void TaskModel::removeRow(int row, const QModelIndex &parent)
{
    TaskEntry entry = db->getTaskEntry(filterString, row);

    beginRemoveRows(parent, row, row);
    db->removeTaskEntry(entry.id);
    endRemoveRows();
}

QStringList TaskModel::projectList() const
{
    return db->listOfProjects();
}

ProjectEntry TaskModel::getProject(int index) const
{
    return db->getProjectEntry(index);
}

void TaskModel::updateProject(ProjectEntry entry)
{
    db->updateProjectEntry(entry);
}


