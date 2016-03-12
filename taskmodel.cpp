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

int TaskModel::rowCount(const QModelIndex &parent) const
{
    return db->size("SELECT * FROM tasks");
}

QVariant TaskModel::data(const QModelIndex &index, int role) const
{
    if(index.isValid() && role == Qt::DisplayRole)
        return db->getEntry("SELECT title FROM tasks", index.row());
    else
        return QVariant();
}


