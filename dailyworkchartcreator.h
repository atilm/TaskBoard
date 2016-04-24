#ifndef DAILYWORKCHARTCREATOR_H
#define DAILYWORKCHARTCREATOR_H

#include <QVBoxLayout>
#include <QDateEdit>
#include <QString>
#include <QMap>
#include <QVector>

#include "projectanalyzer.h"
#include "QCustomPlot/qcustomplot.h"

class DailyWorkChartCreator : public ProjectAnalyzer
{
    Q_OBJECT
public:
    DailyWorkChartCreator(DatabaseManager *db, QObject *parent = 0);
    virtual ~DailyWorkChartCreator();

    QString getActionText() const;

public slots:
    void showControls();

private:
    QVBoxLayout *controlsLayout;
    QDateEdit *beginEdit;
    QDateEdit *endEdit;
    QCustomPlot *chartView;
    QMap<QString, QVector<double>> efforts;

    void buildControls();
    void updatePlot();
};

#endif // DAILYWORKCHARTCREATOR_H
