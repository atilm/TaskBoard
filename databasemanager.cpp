#include "databasemanager.h"
#include <QDebug>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlError>
#include <QMap>

DatabaseManager::DatabaseManager(QSqlDatabase *db) : maximumInt(2147483647)
{
    this->db = db;

    openDatabase();
}

DatabaseManager::~DatabaseManager()
{
    if(db->isOpen())
        db->close();

    delete db;
}

int DatabaseManager::size(TaskState state) const
{
    if(!db->isOpen()){
        qDebug() << "Database is not open.";
        return 0;
    }

    QSqlQuery query = taskQuery(state);
    if( query.last()){
        return query.at() + 1;
    }
    else{
        return 0;
    }
}

TaskEntry DatabaseManager::getTaskEntry(TaskState state, int index) const
{
    QSqlQuery query = taskQuery(state);
    if(query.seek(index)){
        return buildTaskEntry(query);
    }
    else{
        qDebug() << "getTaskEntry: SQL Error: " << query.lastError().text();
        return TaskEntry();
    }
}

QVector<TaskEntry> DatabaseManager::getTaskEntries(TaskState state) const
{
    QVector<TaskEntry> entries;

    QSqlQuery query = taskQuery(state);
    while(query.next()){
        TaskEntry entry = buildTaskEntry(query);
        entries.append(entry);
    }

    return entries;
}

void DatabaseManager::addTaskEntry(TaskEntry entry)
{
    int sortingIndex = getNewSortingIndex(static_cast<TaskState>(entry.state));

    QString s = QString("INSERT INTO tasks "
                        "(title, description, project, color, estimate, state, createdDate, closedDate, sortingOrder) "
                        "VALUES "
                        "(\"%1\", \"%2\", %3, %4, %5, %6, \"%7\", \"%8\", \"%9\")")
                .arg(entry.title).arg(entry.description)
                .arg(entry.projectIndex).arg(entry.colorIndex)
                .arg(entry.estimated_minutes).arg(entry.state)
                .arg(entry.createdString()).arg(entry.closedString())
                .arg(sortingIndex);

    QSqlQuery query;

    if(!query.exec(s))
        qDebug() << "addTaskEntry: SQL Error: " << query.lastError().text();
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
        qDebug() << "updateTaskEntry: SQL Error: " << query.lastError().text();
}

void DatabaseManager::updateTaskField(int id, const QString &fieldName, int value)
{
    QSqlQuery query;

    QString queryString = QString("UPDATE tasks "
                                  "SET %1=:value "
                                  "WHERE id=:id").arg(fieldName);

    query.prepare(queryString);

    query.bindValue(":id", id);
    query.bindValue(":value", value);

    if(!query.exec())
        qDebug() << "updateTaskField: SQL Error: " << query.lastError().text();
}

void DatabaseManager::removeTaskEntry(int id)
{
    QString s = QString("DELETE FROM tasks "
                        "WHERE id=%1")
                .arg(id);

    QSqlQuery query;

    if(!query.exec(s))
        qDebug() << "removeTaskEntry: SQL Error: " << query.lastError().text();
}

void DatabaseManager::setTaskState(int id, int state)
{
    QSqlQuery query;

    query.prepare("UPDATE tasks "
                  "SET state=:state "
                  "WHERE id=:id");

    query.bindValue(":id", id);
    query.bindValue(":state", state);

    if(!query.exec())
        qDebug() << "setTaskState: SQL Error: " << query.lastError().text();
}

void DatabaseManager::sortTaskIntoColumn(int id, int state, int index)
{
    QSqlQuery query;

    query.prepare("UPDATE tasks "
                  "SET state=:state, sortingOrder=:index, closedDate=:date "
                  "WHERE id=:id");

    query.bindValue(":id", id);
    query.bindValue(":state", state);
    query.bindValue(":index", index);
    query.bindValue(":date", getUpdatedClosedDate(id, state));

    if(!query.exec())
        qDebug() << "setTaskStateAndIndex: SQL Error: " << query.lastError().text();
}

void DatabaseManager::insertIntoColumn(TaskState state, int beforeRow, int taskId)
{
    int currentIndex = 0;
    int previousIndex = 0;
    int insertionIndex = maximumInt / 2;

    if(beforeRow != -1){

        getSortingIndices(state, beforeRow, currentIndex, previousIndex);

        if(currentIndex - previousIndex < 2){
            rearrangeSortingOrder(state);
            getSortingIndices(state, beforeRow, currentIndex, previousIndex);
        }

        insertionIndex = (previousIndex + currentIndex) / 2;
    }

    sortTaskIntoColumn(taskId, state, insertionIndex);
}

QStringList DatabaseManager::listOfProjects() const
{
    QStringList projects;

    QSqlQuery query("SELECT name FROM projects");

    while(query.next()){
        projects.append(query.value(0).toString());
    }

    qDebug() << "projects: " << projects.count();

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

void DatabaseManager::addRecord(int taskID, QDateTime startTime, int minutes)
{
    performRecordQuery("INSERT INTO records "
                       "(task, time, date, startTime) "
                       "VALUES "
                       "(:task, :time, :date, :startTime)",
                       taskID,
                       startTime,
                       minutes);
}

int DatabaseManager::getEffortForTask(int taskID) const
{
    QString queryText = QString("SELECT SUM(time) FROM records "
                                "WHERE task = %1").arg(taskID);

    QSqlQuery query(queryText);

    if(query.next()){
        return query.record().value(0).toInt();
    }
    else{
        qDebug() << "SQL Error: " << query.lastError().text();
        return 0;
    }
}

QMap<QString, QVector<double>> DatabaseManager::getProjectEfforts(QDate begin, QDate end)
{
    QMap<QString, QVector<double>> efforts;

    int days = begin.daysTo(end) + 1;

    QDate date = begin;

    for(int day=0; day<days; day++){
        QMap<QString, double> currentDayEffort = getProjectEfforts(date);

        QMapIterator<QString, double> it(currentDayEffort);

        while(it.hasNext()){
            it.next();

            if(!efforts.contains(it.key()))
                efforts[it.key()] = QVector<double>(days);

            efforts[it.key()][day] = it.value() / 60.0; // value in hours
        }

        date = date.addDays(1);
    }

    return efforts;
}

QMap<QString, double> DatabaseManager::getProjectEfforts(QDate date)
{
    QMap<QString, double> map;

    QString queryString = QString("SELECT projects.name, SUM(records.time) FROM records"
                                  " JOIN tasks ON records.task = tasks.id"
                                  " JOIN projects ON tasks.project = projects.id"
                                  " WHERE records.date = \"%1\""
                                  " GROUP BY tasks.project")
            .arg(date.toString("yyyy-MM-dd"));

    QSqlQuery query(queryString);

    while(query.next()){
        map[query.value(0).toString()] = query.value(1).toDouble();
    }

    return map;
}

QVector<DatabaseManager::DayEffort> DatabaseManager::getDayEffortsOfProject(int projectID)
{
    QString queryString = QString("SELECT records.date, SUM(records.time) FROM records "
                                  "JOIN tasks ON records.task = tasks.id "
                                  "WHERE tasks.project=%1 "
                                  "GROUP BY records.date "
                                  "ORDER BY records.date").arg(projectID);

    QSqlQuery query(queryString);

    QVector<DayEffort> efforts;

    while(query.next()){
        DayEffort entry;
        QString dateString = query.record().value(0).toString();
        entry.date = QDateTime::fromString(dateString, "yyyy-MM-dd");
        entry.effortMinutes = query.record().value(1).toInt();
        efforts.append(entry);
    }

    return efforts;
}

QVector<DatabaseManager::EstimationError> DatabaseManager::getEstimationErrors(QDate begin, QDate end)
{
    QString queryString = QString("SELECT tasks.title, tasks.estimate, SUM(records.time) FROM records "
                                  "JOIN tasks ON records.task = tasks.id "
                                  "WHERE records.date > \"%1\" "
                                  "AND records.date <= \"%2\" "
                                  "AND tasks.closedDate != \"\" "
                                  "GROUP BY records.task");

    queryString = queryString
                  .arg(begin.toString("yyyy-MM-dd"))
                  .arg(end.toString("yyyy-MM-dd"));

    QSqlQuery query(queryString);

    QVector<EstimationError> errors;

    while(query.next()){
        EstimationError e;
        e.taskTitle = query.value(0).toString();
        int estimate = query.value(1).toInt();
        int effort = query.value(2).toInt();
        e.estimationError = effort - estimate;

        errors.append(e);
    }

    return errors;
}

void DatabaseManager::openDatabase()
{
    db->setDatabaseName("tasks.db");

    if(!db->open())
        qDebug() << "Error: " << db->lastError();
}

QSqlQuery DatabaseManager::taskQuery(TaskState state) const
{
    QString tempString = QString("SELECT"
                                 " tasks.id, tasks.title, tasks.description, projects.id, projects.short, "
                                 " tasks.estimate, tasks.color, tasks.createdDate, tasks.closedDate, tasks.sortingOrder"
                                 " FROM tasks"
                                 " JOIN projects ON tasks.project = projects.id"
                                 " WHERE tasks.state = %1").arg(state);

    if(state == TaskState::Done){
        QDate sevenDaysAgo = QDate::currentDate().addDays(-7);
        tempString += QString(" AND tasks.closedDate > \"%1\""
                              " ORDER BY tasks.closedDate DESC")
                .arg(sevenDaysAgo.toString("yyyy-MM-dd"));
    }
    else{
        tempString += QString(" ORDER BY tasks.sortingOrder");
    }

    return QSqlQuery(tempString);
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
    entry.created = QDateTime::fromString(query.record().value(7).toString(), "yyyy-MM-dd");
    entry.closed = QDateTime::fromString(query.record().value(8).toString(), "yyyy-MM-dd");
    entry.sortingOrder = query.record().value(9).toInt();

    entry.effort_minutes = getEffortForTask(entry.id);

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

void DatabaseManager::rearrangeSortingOrder(TaskState state)
{
    QVector<TaskEntry> entries = getTaskEntries(state);

    int rows = entries.count();
    int upperLimit = maximumInt; // = 2^31-1 i.e. biggest signed integer

    int step = upperLimit / (rows + 1);

    for(int i=0; i<rows; i++)
        updateTaskField(entries[i].id, "sortingOrder", step * (i+1));
}

void DatabaseManager::getSortingIndices(TaskState state, int row, int &currentIndex, int &previousIndex)
{
    TaskEntry entry = getTaskEntry(state, row);
    currentIndex = entry.sortingOrder;

    if(row == 0){
        previousIndex = 0;
    }
    else{
        TaskEntry previousEntry = getTaskEntry(state, row - 1);
        previousIndex = previousEntry.sortingOrder;
    }
}

int DatabaseManager::getNewSortingIndex(TaskState state)
{
    if(size(state) == 0)
        return maximumInt / 2;
    else{
        TaskEntry entry = getTaskEntry(state, 0);
        return entry.sortingOrder / 2;
    }
}

int DatabaseManager::getTodaysEffort(int taskID)
{
    QString queryString = QString("SELECT time FROM records "
                                  "WHERE task=%1 AND date=\"%2\"")
            .arg(taskID)
            .arg(QDateTime::currentDateTime().toString(QString("yyyy-MM-dd")));

    QSqlQuery query(queryString);

    if(query.next())
        return query.value(0).toInt();
    else
        return 0;
}

void DatabaseManager::performRecordQuery(const QString &queryString, int taskID,
                                         QDateTime startTime, int minutes)
{
    QSqlQuery query;

    query.prepare(queryString);

    query.bindValue(":task", taskID);
    query.bindValue(":time", minutes);
    query.bindValue(":date", startTime.toString(QString("yyyy-MM-dd")));
    query.bindValue(":startTime", startTime.toString(QString("hh:mm")));

    if(!query.exec())
        qDebug() << "SQL Error: " << query.lastError().text();
}

QString DatabaseManager::getUpdatedClosedDate(int id, int state)
{
    if(state == Done){
        QString dateString = getClosedDateString(id);
        if(!dateString.isEmpty())
            return dateString;
        else
            return QDateTime::currentDateTime().toString(QString("yyyy-MM-dd"));
    }
    else
        return QString();
}

QString DatabaseManager::getClosedDateString(int id)
{
    QString queryString = QString("SELECT closedDate FROM tasks "
                                  "WHERE id=%1").arg(id);

    QSqlQuery query(queryString);

    QString dateString;

    if(query.next()){
        dateString = query.value(0).toString().trimmed();
    }

    return dateString;
}




