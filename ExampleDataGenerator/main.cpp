#include <QCoreApplication>

#include "datagenerator.h"

#include <QDate>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    DataGenerator generator;
    QDate today = QDate::currentDate();
    QDate oneWeekAgo = today.addDays(-7);

    generator.generateData(oneWeekAgo, today);

    return 0;
}

