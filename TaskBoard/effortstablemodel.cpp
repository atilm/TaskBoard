#include "effortstablemodel.h"
#include <QDebug>
#include <QPair>

EffortsTableModel::EffortsTableModel()
{

}

EffortsTableModel::~EffortsTableModel()
{

}

void EffortsTableModel::injectDataBase(DatabaseManager *db)
{
    this->db = db;
}

void EffortsTableModel::injectTimeChooser(TimePeriodChooser *chooser)
{
    this->timeChooser = chooser;
}

void EffortsTableModel::reload(QDate begin, QDate end)
{
    clear();

    QMap<QString, double> projects = db->getSummedProjectEfforts(begin, end);

    foreach(QString key, projects.keys()){
        appendProject(key, projects.value(key));
        appendTasks(key, begin, end);
    }
}

void EffortsTableModel::clear()
{
    if(items.isEmpty())
        return;

    beginRemoveRows(QModelIndex(), 0, rowCount()-1);
    items.clear();
    endRemoveRows();
}

QVariant EffortsTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(role != Qt::DisplayRole)
        return QVariant();

    if(orientation == Qt::Vertical)
        return QVariant();
    else{
        switch(section){
        case 0:
            return tr("Name");
        case 1:
            return tr("Effort [h]");
        default:
            return QVariant();
        }
    }
}

int EffortsTableModel::rowCount(const QModelIndex &parent) const
{
    return items.count();
}

int EffortsTableModel::columnCount(const QModelIndex &parent) const
{
    return 2;
}

QVariant EffortsTableModel::data(const QModelIndex &index, int role) const
{
    if(!index.isValid())
        return QVariant();

    int row = index.row();
    int column = index.column();

    if(role == Qt::DisplayRole){
        return items.at(row).getText(column);
    }
    else if(role == Qt::BackgroundColorRole){
        return items.at(row).getColor();
    }
    else{
        return QVariant();
    }
}

void EffortsTableModel::appendProject(const QString &key, double effort)
{
    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    EffortsTableItem item(key, effort / 60.0, EffortsTableItem::Project);
    items.append(item);
    endInsertRows();
}

void EffortsTableModel::appendTasks(const QString &key, QDate begin, QDate end)
{
    QList<QPair<QString, double>> entries = db->getSummedTaskEfforts(key, begin, end);

    foreach(auto e, entries){
        beginInsertRows(QModelIndex(), rowCount(), rowCount());
        EffortsTableItem item(e.first, e.second / 60.0);
        items.append(item);
        endInsertRows();
    }
}

