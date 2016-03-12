#include "databasemanager.h"
#include <QDebug>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlError>

DatabaseManager::DatabaseManager()
{
    db = QSqlDatabase::addDatabase("QSQLITE");

    openDatabase();
}

DatabaseManager::~DatabaseManager()
{
    if(db.isOpen())
        db.close();
}

int DatabaseManager::size(const QString &queryString) const
{
    if(!db.isOpen()){
        qDebug() << "Database is not open.";
        return 0;
    }

    QSqlQuery query;
    if(query.exec(queryString)){
        query.last();
        return query.at() + 1;
    }
    else{
        qDebug() << "error: " << query.lastError();
        return 0;
    }
}

QString DatabaseManager::getEntry(const QString &queryString, int index) const
{
    QSqlQuery query(queryString);
    if(query.seek(index)){
        return query.value(0).toString();
    }
    else{
        qDebug() << "SQL Error: " << query.lastError().text();
        return QString();
    }
}

void DatabaseManager::openDatabase()
{
    db.setDatabaseName("tasks.db");

    if(!db.open())
        qDebug() << "Error: " << db.lastError();
}




