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

int DatabaseManager::size(const QString &filterString) const
{
    if(!db.isOpen()){
        qDebug() << "Database is not open.";
        return 0;
    }

    QSqlQuery query = taskQuery(filterString);
    if( query.last()){
        return query.at() + 1;
    }
    else{
        qDebug() << "error: " << query.lastError();
        return 0;
    }
}

TaskEntry DatabaseManager::getTaskEntry(const QString &filterString, int index) const
{
    QSqlQuery query = taskQuery(filterString);
    if(query.seek(index)){
        return buildTaskEntry(query);
    }
    else{
        qDebug() << "SQL Error: " << query.lastError().text();
        return TaskEntry();
    }
}

void DatabaseManager::openDatabase()
{
    db.setDatabaseName("tasks.db");

    if(!db.open())
        qDebug() << "Error: " << db.lastError();
}

QSqlQuery DatabaseManager::taskQuery(const QString &filterString) const
{
    QString s("SELECT"
              " tasks.title, tasks.description, projects.short,"
              " tasks.estimate, tasks.color, tasks.createdDate, tasks.closedDate"
              " FROM tasks"
              " JOIN projects ON tasks.project = projects.id");

    if(!filterString.isEmpty())
        s += QString(" WHERE %1").arg(filterString);

    return QSqlQuery(s);
}

TaskEntry DatabaseManager::buildTaskEntry(const QSqlQuery &query) const
{
    TaskEntry entry;

    entry.title = query.record().value(0).toString();
    entry.description = query.record().value(1).toString();
    entry.projectShort = query.record().value(2).toString();
    entry.estimated_minutes = query.record().value(3).toInt();
    entry.colorIndex = query.record().value(4).toInt();
    entry.created = QDateTime::fromString(query.record().value(5).toString());
    entry.closed = QDateTime::fromString(query.record().value(6).toString());

    return entry;
}




