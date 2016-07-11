#ifndef SQLITETABLEMODEL_H
#define SQLITETABLEMODEL_H

#include <QSet>
#include <QSqlTableModel>

class SQLiteTableModel : public QSqlTableModel
{
public:
    SQLiteTableModel(QObject *parent, QSqlDatabase db);
    virtual ~SQLiteTableModel();

    void setCheckable(int column);

    Qt::ItemFlags flags(const QModelIndex &index) const;
    QVariant data(const QModelIndex &idx, int role) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role);

private:
    QSet<int> checkableColumns;
};

#endif // SQLITETABLEMODEL_H
