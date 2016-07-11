#include "sqlitetablemodel.h"
#include <QDebug>

SQLiteTableModel::SQLiteTableModel(QObject *parent, QSqlDatabase db)
    : QSqlTableModel(parent, db)
{

}

SQLiteTableModel::~SQLiteTableModel()
{

}

void SQLiteTableModel::setCheckable(int column)
{
    checkableColumns.insert(column);
}

Qt::ItemFlags SQLiteTableModel::flags(const QModelIndex &index) const
{
    if(checkableColumns.contains(index.column())){
        return QSqlTableModel::flags(index) | Qt::ItemIsUserCheckable;
    }
    else{
        return QSqlTableModel::flags(index);
    }
}

QVariant SQLiteTableModel::data(const QModelIndex &idx, int role) const
{
    if(!idx.isValid())
        return QVariant();

    QVariant value;

    if(checkableColumns.contains(idx.column()) && role == Qt::CheckStateRole){
        value = QSqlTableModel::data(idx, Qt::DisplayRole);
        int b = value.toInt();
        value = (b == 0) ? false : true;
    }
    else if(checkableColumns.contains(idx.column()) && role != Qt::CheckStateRole){
        value = QVariant();
    }
    else{
        value = QSqlTableModel::data(idx, role);
    }

    return value;
}

bool SQLiteTableModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if(!index.isValid())
        return false;

    if(checkableColumns.contains(index.column()) && role == Qt::CheckStateRole){
        bool isChecked = data(index, Qt::CheckStateRole).toBool();

        if(isChecked)
            return QSqlTableModel::setData(index, 0, Qt::EditRole);
        else
            return QSqlTableModel::setData(index, 1, Qt::EditRole);
    }
    else{
        return QSqlTableModel::setData(index, value, role);
    }
}

