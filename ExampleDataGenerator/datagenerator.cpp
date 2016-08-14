#include "datagenerator.h"

DataGenerator::DataGenerator()
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
}

