#include "datagenerator.h"

DataGenerator::DataGenerator()
    : mt(rd())
{
    sqlite = QSqlDatabase::addDatabase("QSQLITE");
    db = new DatabaseManager(&sqlite);
}

DataGenerator::~DataGenerator()
{
    delete db;
}

void DataGenerator::generateData(const QDate &startDate,
                                 const QDate &endDate)
{
    initializeProjectTemplates();

    QDate date = startDate;
    issueNumber = 0;
    currentTaskID = 0;

    while(date <= endDate){
        if(isWorkDay(date)){
            fillDay(date);
        }

        date = date.addDays(1);
    }
}

void DataGenerator::initializeProjectTemplates()
{
    QStringList organization;
    organization << "Group meeting" << "Task planning"
                 << "Phone call";

    QStringList training;
    training << "Read literature" << "Training course"
             << "Spontaneous discussions";

    QStringList projectOne;
    projectOne << "Implement Feature #%1" << "Fix Issue #%1";

    QStringList projectTwo;
    projectTwo << "Implement Feature #%1" << "Fix Issue #%1"
               << "Installation Support" << "User Training";

    QStringList projectThree;
    projectThree << "Implement Feature #%1" << "Fix Issue #%1"
                 << "Installation Support" << "User Training";


    ProjectEntry org;
    org.title = "Organization";
    org.shortSign = "ORG";

    ProjectEntry train;
    train.title = "Training";
    train.shortSign = "TR";

    ProjectEntry one;
    one.title = "Internal Library Development";
    one.shortSign = "IL";

    ProjectEntry two;
    two.title = "Task Board Development";
    two.shortSign = "TB";

    ProjectEntry three;
    three.title = "Requirements Manager Development";
    three.shortSign = "RM";

    db->addProjectEntry(org);
    db->addProjectEntry(train);
    db->addProjectEntry(one);
    db->addProjectEntry(two);
    db->addProjectEntry(three);

    projects.insert(org.title, organization);
    projects.insert(train.title, training);
    projects.insert(one.title, projectOne);
    projects.insert(two.title, projectTwo);
    projects.insert(three.title, projectThree);
}

bool DataGenerator::isWorkDay(const QDate &date)
{
    // 1 = Monday to 7 = Sunday
    return date.dayOfWeek() < 6;
}

void DataGenerator::fillDay(const QDate &date)
{
    todaysWorkLoad = 0;

    now = QDateTime(date, QTime(8, 0, 0));

    while(todaysWorkLoad < 7*60){
        addTask();
    }
}

void DataGenerator::addTask()
{
    int estimation = randomInt(10, 2*60);
    int actualEffort = generateActualEffort(estimation);

    TaskEntry entry;

    int projectIndex = randomInt(0, projects.keys().count()-1);
    entry.projectIndex = projectIndex;

    QString projectName = projects.keys().at(projectIndex);

    int titleIndex = randomInt(0, projects[projectName].count()-1);
    QString title = projects[projectName].at(titleIndex);
    entry.title = title.arg(nextIssueNumber());

    entry.colorIndex = randomInt(0, 7);
    entry.estimated_minutes = estimation;
    entry.state = TaskEntry::done;
    entry.created = now;
    entry.closed = now.addSecs(actualEffort * 60);

    db->addTaskEntry(entry);
    currentTaskID++;

    db->addRecord(currentTaskID, now, actualEffort);
    todaysWorkLoad += actualEffort;

    now = entry.closed;
}

int DataGenerator::nextIssueNumber()
{
    return ++issueNumber;
}

int DataGenerator::randomInt(int from, int to)
{
    uniform_int_distribution<int> dist(from, to);
    return dist(mt);
}

int DataGenerator::generateActualEffort(int estimation)
{
    double doubleEstimation = estimation;
    uniform_real_distribution<double> dist(0.5*doubleEstimation, 1.5*doubleEstimation);
    return (int)round(dist(mt));
}

