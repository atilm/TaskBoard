#ifndef BURNUPCHARTCREATOR_H
#define BURNUPCHARTCREATOR_H

#include <QComboBox>
#include <QRadioButton>
#include <QVBoxLayout>
#include <QDate>
#include <QVector>

#include "projectanalyzer.h"
#include "QCustomPlot/qcustomplot.h"

class BurnUpChartCreator : public ProjectAnalyzer
{
    Q_OBJECT
public:
    BurnUpChartCreator(DatabaseManager *db, QObject *parent = 0);
    virtual ~BurnUpChartCreator();

    QString getActionText() const;
    void updateView();

public slots:
    void showControls();

protected slots:
    void updatePlot();

private:
    QVector<DatabaseManager::DayEffort> efforts;
    QVector<double> dates;
    QVector<double> accumulatedHours;
    QVector<double> xTicks;
    QVector<QString> xTickLabels;
    QVector<double> yTicks;
    QVBoxLayout *controlsLayout;
    QComboBox *projectChooser;
    QRadioButton *showAllDaysButton;
    QRadioButton *showRecordDaysButton;
    QCustomPlot *chartView;

    void buildControls();
    void updateProjectChooser();
    void updateEfforts();
    void readEfforts();
    void buildPlottableData();
    void buildAllDates();
    void buildRecordDates();
    void formatAxes();
    void plotEfforts();
    void setXTicks();
    void buildAllDayTicks();
    void buildRecordDayTicks();
    void setYTicks();
};

#endif // BURNUPCHARTCREATOR_H
