#ifndef EFFORTSTABLEMODEL_H
#define EFFORTSTABLEMODEL_H

#include <QAbstractTableModel>
#include <QList>

#include "databasemanager.h"
#include "effortstableitem.h"
#include "timeperiodchooser.h"

class EffortsTableModel : public QAbstractTableModel
{
public:
    EffortsTableModel();
    virtual ~EffortsTableModel();

    void injectDataBase(DatabaseManager *db);
    void injectTimeChooser(TimePeriodChooser *chooser);

    void reload(QDate begin, QDate end);
    void clear();
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;

protected:
    QList<EffortsTableItem> items;
    DatabaseManager *db;
    TimePeriodChooser *timeChooser;

    void appendProject(const QString &key, double effort);
    void appendTasks(const QString &key, QDate begin, QDate end);
};

#endif // EFFORTSTABLEMODEL_H
