#include "databasemanager.h"
#include <QDebug>
#include <QDir>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlError>
#include <QStandardPaths>
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

TaskEntry DatabaseManager::getTaskEntry(int id) const
{
    QString queryText = QString("SELECT"
                                " tasks.id, tasks.title, tasks.description, projects.id, projects.short, "
                                " tasks.estimate, tasks.color, tasks.createdDate, tasks.closedDate, tasks.sortingOrder, tasks.state"
                                " FROM tasks"
                                " JOIN projects ON tasks.project = projects.id"
                                " WHERE tasks.id=%1").arg(id);

    QSqlQuery query(queryText);

    if(query.next())
        return buildTaskEntry(query);
    else{
        qDebug() << "getTaskEntry: SQL Error: " << query.lastError().text();
        return TaskEntry();
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

    QSqlQuery query;

    query.prepare("INSERT INTO tasks "
                  "(title, description, project, color, estimate, state, createdDate, closedDate, sortingOrder) "
                  "VALUES "
                  "(:title, :description, :project, :color, :estimate, :state, :createdDate, :closedDate, :sortingOrder)");

    query.bindValue(":title", entry.title);
    query.bindValue(":description", entry.description);
    query.bindValue(":project", entry.projectIndex);
    query.bindValue(":color", entry.colorIndex);
    query.bindValue(":estimate", entry.estimated_minutes);
    query.bindValue(":state", entry.state);
    query.bindValue(":createdDate", entry.createdString());
    query.bindValue(":closedDate", entry.closedString());
    query.bindValue(":sortingOrder", sortingIndex);

    if(!query.exec())
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
    QSqlQuery removeRecords;
    QSqlQuery removeTask;

    removeRecords.prepare("DELETE FROM records "
                          "WHERE task=:id");

    removeTask.prepare("DELETE FROM tasks "
                       "WHERE id=:id");

    removeRecords.bindValue(":id", id);
    removeTask.bindValue(":id", id);

    executeQuery(removeRecords);
    executeQuery(removeTask);
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

    return projects;
}

QList<ProjectEntry> DatabaseManager::getActiveProjects() const
{
    QString queryText = QString("SELECT id, short, name, description"
                                " FROM projects"
                                " WHERE hidden == 0");

    QSqlQuery query(queryText);

    QList<ProjectEntry> list;

    while(query.next()){
        list.append(buildProjectEntry(query));
    }

    return list;
}

void DatabaseManager::addProjectEntry(ProjectEntry entry)
{
    QSqlQuery query;

    query.prepare("INSERT INTO projects "
                  "(short, name, description, hidden) "
                  "VALUES "
                  "(:short, :name, :description, :hidden)");

    query.bindValue(":short", entry.shortSign);
    query.bindValue(":name", entry.title);
    query.bindValue(":description", entry.description);
    query.bindValue(":hidden", 0);

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

QMap<QString, double> DatabaseManager::getSummedProjectEfforts(QDate begin, QDate end) const
{
    QMap<QString, double> map;

    QString queryString = QString("SELECT projects.name, SUM(records.time) FROM records"
                                  " JOIN tasks ON records.task = tasks.id"
                                  " JOIN projects ON tasks.project = projects.id"
                                  " WHERE records.date BETWEEN \"%1\" AND \"%2\""
                                  " GROUP BY tasks.project")
            .arg(begin.toString("yyyy-MM-dd"))
            .arg(end.toString("yyyy-MM-dd"));

    QSqlQuery query(queryString);

    while(query.next()){
        map[query.value(0).toString()] = query.value(1).toDouble();
    }

    return map;
}

QList<QPair<QString, double> > DatabaseManager::getSummedTaskEfforts(const QString &project,
                                                    QDate begin, QDate end) const
{
    QList<QPair<QString, double>> efforts;

    QString queryString = QString("SELECT tasks.title, SUM(records.time) FROM records"
                                  " JOIN tasks ON records.task = tasks.id"
                                  " JOIN projects ON tasks.project = projects.id"
                                  " WHERE projects.name = \"%1\""
                                  " AND( records.date BETWEEN \"%2\" AND \"%3\" ) "
                                  " GROUP BY tasks.id")
            .arg(project)
            .arg(begin.toString("yyyy-MM-dd"))
            .arg(end.toString("yyyy-MM-dd"));

    QSqlQuery query(queryString);

    while(query.next()){
        QPair<QString, double> p(query.value(0).toString(), query.value(1).toDouble());
        efforts.append(p);
    }

    return efforts;
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
                                  "WHERE tasks.closedDate > \"%1\" "
                                  "AND tasks.closedDate <= \"%2\" "
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
        e.estimation = query.value(1).toInt();
        e.effort = query.value(2).toInt();

        if(e.isValid())
            errors.append(e);
    }

    return errors;
}

void DatabaseManager::openDatabase()
{
    QStringList locations = QStandardPaths::standardLocations(QStandardPaths::AppDataLocation);

    QDir appDir(locations.first());
    if(!appDir.exists()){
        appDir.mkdir(appDir.absolutePath());
    }

    db->setDatabaseName(locations.first() + "/tasks.db");

    if(!db->open()){
        qDebug() << "Error: " << db->lastError();
        return;
    }

    if(!dataBaseIsInitialized())
        createTables();
}

bool DatabaseManager::dataBaseIsInitialized()
{
    return db->tables().contains("tasks");
}

void DatabaseManager::createTables()
{
    QSqlQuery createProjects;
    QSqlQuery createTasks;
    QSqlQuery createRecords;

    createProjects.prepare("CREATE TABLE projects("
                           " id INTEGER PRIMARY KEY,"
                           " short TEXT,"
                           " name TEXT,"
                           " description TEXT,"
                           " hidden INTEGER)");

    createTasks.prepare("CREATE TABLE tasks("
                        " id INTEGER PRIMARY KEY,"
                        " title TEXT,"
                        " description TEXT,"
                        " project INTEGER,"
                        " estimate INTEGER,"
                        " state INTEGER,"
                        " color INTEGER,"
                        " createdDate TEXT,"
                        " closedDate TEXT,"
                        " sortingOrder INTEGER)");

    createRecords.prepare("CREATE TABLE records("
                          " id INTEGER PRIMARY KEY,"
                          " task INTEGER,"
                          " time INTEGER,"
                          " date TEXT,"
                          " startTime TEXT)");

    executeQuery(createProjects);
    executeQuery(createTasks);
    executeQuery(createRecords);
}

QSqlQuery DatabaseManager::taskQuery(TaskState state) const
{
    QString tempString = QString("SELECT"
                                 " tasks.id, tasks.title, tasks.description, projects.id, projects.short, "
                                 " tasks.estimate, tasks.color, tasks.createdDate, tasks.closedDate, tasks.sortingOrder, tasks.state"
                                 " FROM tasks"
                                 " JOIN projects ON tasks.project = projects.id"
                                 " WHERE tasks.state = %1").arg(state);

    if(state == TaskState::Done){
        QDate fourteenDaysAgo = QDate::currentDate().addDays(-14);
        tempString += QString(" AND tasks.closedDate >= \"%1\"")
                .arg(fourteenDaysAgo.toString("yyyy-MM-dd"));
    }

    tempString += " ORDER BY tasks.sortingOrder";

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
    entry.setState(query.record().value(10).toInt());

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
    else
        emit dataChanged(taskID);
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

void DatabaseManager::executeQuery(QSqlQuery &query)
{
    if(!query.exec()){
        qDebug() << "Error: " << query.lastQuery();
        qDebug() << query.lastError();
    }
}




