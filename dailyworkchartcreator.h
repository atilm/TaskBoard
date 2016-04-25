#ifndef DAILYWORKCHARTCREATOR_H
#define DAILYWORKCHARTCREATOR_H

#include <QVBoxLayout>
#include <QDateEdit>
#include <QString>
#include <QMap>
#include <QVector>
#include <QPushButton>

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

protected slots:
    void updatePlot();

private:
    QVector<QColor> colors;
    QVBoxLayout *controlsLayout;
    QDateEdit *beginEdit;
    QDateEdit *endEdit;
    QPushButton *updateButton;
    QCustomPlot *chartView;
    QMap<QString, QVector<double>> efforts;
    QVector<double> xTicks;
    QVector<QCPBars*> bars;
    int colorIndex;

    void buildControls();
    void formatAxes();
    void plotEfforts();
    void setXTics();
    void setYTics();
    void buildBars();
    void styleBar(QCPBars *bar);
    void nextColorIndex();
};

#endif // DAILYWORKCHARTCREATOR_H
