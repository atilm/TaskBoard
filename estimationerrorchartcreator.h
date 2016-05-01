#ifndef ESTIMATIONERRORCHARTCREATOR_H
#define ESTIMATIONERRORCHARTCREATOR_H

#include <QDateEdit>
#include <QObject>
#include <QPushButton>

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
    QPushButton *updateButton;
    QVBoxLayout *controlsLayout;
    QCustomPlot *chartView;

    virtual void buildControls();
};

#endif // ESTIMATIONERRORCHARTCREATOR_H
