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

void TaskModel::setColumnType(TaskState columnType)
{
    this->columnType = columnType;
}

int TaskModel::rowCount(const QModelIndex &parent) const
{
    return db->size(columnType);
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
    return db->getTaskEntry(columnType, index.row());
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
    TaskEntry entry = db->getTaskEntry(columnType, row);

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

    int insertionIndex;

    if(columnType == Done){
        insertionIndex = 0;
    }
    else{
        insertionIndex = parent.row();
    }


    beginInsertRows(QModelIndex(), insertionIndex, insertionIndex);
    db->insertIntoColumn(columnType, insertionIndex, id);
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


