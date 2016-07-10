#ifndef DAILYWORKCHARTCREATOR_H
#define DAILYWORKCHARTCREATOR_H

#include <QVBoxLayout>
#include <QDateEdit>
#include <QString>
#include <QMap>
#include <QVector>
#include <QPushButton>

#include "projectanalyzer.h"
#include "timeperiodchooser.h"
#include "QCustomPlot/dailyworkchartview.h"

class DailyWorkChartCreator : public ProjectAnalyzer
{
    Q_OBJECT
public:
    DailyWorkChartCreator(DatabaseManager *db,
                          TimePeriodChooser *timeChooser,
                          QObject *parent = 0);
    virtual ~DailyWorkChartCreator();

    QString getActionText() const;
    void updateView();

public slots:
    void showControls();

protected slots:
    void updatePlot();

private:
    QVector<QColor> colors;
    QVBoxLayout *controlsLayout;
    TimePeriodChooser *timeChooser;
    DailyWorkChartView *chartView;
    QMap<QString, QVector<double>> efforts;
    QVector<double> xTicks;
    QVector<QCPBars*> bars;
    int colorIndex;

    // For outside legend:
    QCPLayoutGrid *subLayout;
    QCPLayoutElement *dummyElement;

    void buildControls();
    void formatAxes();
    void plotEfforts();
    void showOutsideLegend();
    void setXTics();
    void setYTics();
    void buildBars();
    void styleBar(QCPBars *bar);
    void nextColorIndex();
};

#endif // DAILYWORKCHARTCREATOR_H
