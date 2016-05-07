#ifndef ESTIMATIONERRORCHARTCREATOR_H
#define ESTIMATIONERRORCHARTCREATOR_H

#include <QDateEdit>
#include <QObject>
#include <QPushButton>
#include <QRadioButton>
#include <QSpinBox>

#include "projectanalyzer.h"
#include "QCustomPlot/qcustomplot.h"

class EstimationErrorChartCreator : public ProjectAnalyzer
{
    Q_OBJECT
public:
    EstimationErrorChartCreator(DatabaseManager *db, QObject *parent = 0);
    virtual ~EstimationErrorChartCreator();

    virtual QString getActionText() const;
    virtual void updateView();

public slots:
    void showControls();

protected slots:
    void updatePlot();

protected:
    QDateEdit *beginEdit;
    QDateEdit *endEdit;
    QSpinBox *binChooser;
    QRadioButton *minutesButton;
    QRadioButton *percentButton;
    QPushButton *updateButton;
    QVBoxLayout *controlsLayout;
    QCustomPlot *chartView;

    QVector<DatabaseManager::EstimationError> errors;
    QVector<double> xTicks;
    QVector<double> barHeights;

    virtual void buildControls();
    void calculateData();
    QVector<double> extractDataFromErrors();
    void formatAxes();
    void plotHistogram();
    void setXTicks();
    void setYTicks();
};

#endif // ESTIMATIONERRORCHARTCREATOR_H