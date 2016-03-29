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

void DatabaseManager::addTaskEntry(TaskEntry entry)
{
    QString s = QString("INSERT INTO tasks "
                        "(title, description, project, color, estimate, state, createdDate, closedDate) "
                        "VALUES "
                        "(\"%1\", \"%2\", %3, %4, %5, %6, \"%7\", \"%8\")")
                .arg(entry.title).arg(entry.description)
                .arg(entry.projectIndex).arg(entry.colorIndex)
                .arg(entry.estimated_minutes).arg(entry.state)
                .arg(entry.createdString()).arg(entry.closedString());

    QSqlQuery query;

    if(!query.exec(s))
        qDebug() << "SQL Error: " << query.lastError().text();
}

void DatabaseManager::updateTaskEntry(TaskEntry entry)
{
    QSqlQuery query;

    query.prepare("UPDATE tasks "
                  "SET title=:title, description=:desc, project=:pro, "
                  "color=:color, estimate=:est "
                  "WHERE id=:id");

    query.bindValue(":id", entry.id);
    query.bindValue(":title", entry.title);
    query.bindValue(":desc", entry.description);
    query.bindValue(":pro", entry.projectIndex);
    query.bindValue(":color", entry.colorIndex);
    query.bindValue(":est", entry.estimated_minutes);

    if(!query.exec())
        qDebug() << "SQL Error: " << query.lastError().text();
}

void DatabaseManager::removeTaskEntry(int id)
{
    QString s = QString("DELETE FROM tasks "
                        "WHERE id=%1")
                .arg(id);

    QSqlQuery query;

    if(!query.exec(s))
        qDebug() << "SQL Error: " << query.lastError().text();
}

QStringList DatabaseManager::listOfProjects() const
{
    QStringList projects;

    QSqlQuery query("SELECT name FROM projects");

    while(query.next()){
        projects.append(query.value(0).toString());
    }

    return projects;
}

void DatabaseManager::addProjectEntry(ProjectEntry entry)
{
    QSqlQuery query;

    query.prepare("INSERT INTO projects "
                  "(short, name, description) "
                  "VALUES "
                  "(:short, :name, :description)");

    query.bindValue(":short", entry.shortSign);
    query.bindValue(":name", entry.title);
    query.bindValue(":description", entry.description);

    if(!query.exec())
        qDebug() << "SQL Error: " << query.lastError().text();
}

ProjectEntry DatabaseManager::getProjectEntry(int index) const
{
    QString queryText = QString("SELECT id, short, name, description FROM projects"
                                " WHERE id = %1").arg(index);

    QSqlQuery query(queryText);

    if(query.next()){
        return buildProjectEntry(query);
    }
    else{
        qDebug() << "SQL Error: " << query.lastError().text();
        return ProjectEntry();
    }
}

void DatabaseManager::updateProjectEntry(ProjectEntry entry)
{
    QSqlQuery query;

    query.prepare("UPDATE projects "
                  "SET short=:short, name=:name, description=:desc "
                  "WHERE id=:id");

    query.bindValue(":id", entry.id);
    query.bindValue(":short", entry.shortSign);
    query.bindValue(":name", entry.title);
    query.bindValue(":desc", entry.description);

    if(!query.exec())
        qDebug() << "SQL Error: " << query.lastError().text();
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
              " tasks.id, tasks.title, tasks.description, projects.id, projects.short, "
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

    entry.id = query.record().value(0).toInt();
    entry.title = query.record().value(1).toString();
    entry.description = query.record().value(2).toString();
    entry.projectIndex = query.record().value(3).toInt();
    entry.projectShort = query.record().value(4).toString();
    entry.estimated_minutes = query.record().value(5).toInt();
    entry.colorIndex = query.record().value(6).toInt();
    entry.created = QDateTime::fromString(query.record().value(7).toString());
    entry.closed = QDateTime::fromString(query.record().value(8).toString());

    return entry;
}

ProjectEntry DatabaseManager::buildProjectEntry(const QSqlQuery &query) const
{
    ProjectEntry entry;

    entry.id = query.value(0).toInt();
    entry.shortSign = query.value(1).toString();
    entry.title = query.value(2).toString();
    entry.description = query.value(3).toString();

    return entry;
}




