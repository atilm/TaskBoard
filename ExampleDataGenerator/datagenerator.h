#ifndef DATAGENERATOR_H
#define DATAGENERATOR_H

#include "databasemanager.h"

#include <QDate>
#include <QDateTime>
#include <QMap>
#include <QString>
#include <QStringList>

#include <random>
using namespace std;

class DataGenerator
{
public:
    DataGenerator();
    ~DataGenerator();

    void generateData(const QDate &startDate,
                      const QDate &endDate);

protected:
    QSqlDatabase sqlite;
    DatabaseManager *db;
    random_device rd;
    mt19937 mt;

    /*! Map of projects and possible tasks

      keys: example project names
      values: lists of possible task name templates per project
     */
    QMap<QString, QStringList> projects;
    double todaysWorkLoad;
    int issueNumber;
    int currentTaskID;
    QDateTime now;

    void initializeProjectTemplates();
    bool isWorkDay(const QDate &date);
    void fillDay(const QDate &date);
    void addTask();
    int nextIssueNumber();
    int randomInt(int from, int to);
    int generateEstimation(int effort);
};

#endif // DATAGENERATOR_H
