#ifndef BURNUPCHARTCREATOR_H
#define BURNUPCHARTCREATOR_H

#include <QComboBox>
#include <QVBoxLayout>

#include "projectanalyzer.h"
#include "QCustomPlot/qcustomplot.h"

class BurnUpChartCreator : public ProjectAnalyzer
{
public:
    BurnUpChartCreator(DatabaseManager *db, QObject *parent = 0);
    virtual ~BurnUpChartCreator();

    QString getActionText() const;
    void updateView() {}

public slots:

private:
    QVBoxLayout *controlsLayout;
    QComboBox *projectChooser;
    QCustomPlot *chartView;

    void buildControls();
};

#endif // BURNUPCHARTCREATOR_H
