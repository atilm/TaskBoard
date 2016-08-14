#ifndef DATAGENERATOR_H
#define DATAGENERATOR_H

#include "databasemanager.h"

#include <QDate>
#include <QMap>
#include <QString>

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

    /*! Map of projects and possible tasks

      keys: example project names
      values: lists of possible task name templates per project
     */
    QMap<QString, QStringList> projects;

    void initializeProjectTemplates();
    bool isWorkDay(const QDate &date);
    void fillDay(const QDate &date);
};

#endif // DATAGENERATOR_H
