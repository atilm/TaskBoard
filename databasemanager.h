#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <QSqlDatabase>

class DatabaseManager
{
public:
    DatabaseManager();
    virtual ~DatabaseManager();

    int size(const QString &queryString) const;
    QString getEntry(const QString &queryString, int index) const;

private:
    QSqlDatabase db;

    void openDatabase();
};

#endif // DATABASEMANAGER_H
