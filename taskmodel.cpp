#include "taskmodel.h"
#include <QDataStream>
#include <QDebug>
#include <QMimeData>

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

Qt::ItemFlags TaskModel::flags(const QModelIndex &index) const
{
    Qt::ItemFlags defaultFlags = QAbstractListModel::flags(index);

    if (index.isValid())
        return Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled | defaultFlags;
    else
        return Qt::ItemIsDropEnabled | defaultFlags;
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

QStringList TaskModel::mimeTypes() const
{
    QStringList types;
    types << "text/plain";
    return types;
}

QMimeData *TaskModel::mimeData(const QModelIndexList &indexes) const
{
    QMimeData *mimeData = new QMimeData();

    TaskEntry entry = getTask(indexes.first());

    mimeData->setText(QString("%1").arg(entry.id));

    return mimeData;
}

bool TaskModel::dropMimeData(const QMimeData *data, Qt::DropAction action,
                             int row, int column, const QModelIndex &parent)
{
    int id = data->text().toInt();

    int state;

    if(filterString.contains("1"))
        state = 1;
    else if(filterString.contains("2"))
        state = 2;
    else if(filterString.contains("3"))
        state = 3;
    else
        return false;

    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    db->setTaskState(id, state);
    endInsertRows();

    emit itemDropped();

    return true;
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

void TaskModel::addProject(ProjectEntry entry)
{
    db->addProjectEntry(entry);
}

void TaskModel::sendAllDataChanged()
{
    QModelIndex top = index(0);
    QModelIndex bottom = index(rowCount()-1);

    emit dataChanged(top, bottom);
}


