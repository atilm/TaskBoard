#ifndef DAILYWORKCHARTCREATOR_H
#define DAILYWORKCHARTCREATOR_H

#include <QVBoxLayout>
#include <QDateEdit>

#include "projectanalyzer.h"

class DailyWorkChartCreator : public ProjectAnalyzer
{
    Q_OBJECT
public:
    DailyWorkChartCreator(QObject *parent = 0);
    virtual ~DailyWorkChartCreator();

    QString getActionText() const;

public slots:

private:
    QVBoxLayout *controlsLayout;
    QDateEdit *beginEdit;
    QDateEdit *endEdit;

    void buildControls();
};

#endif // DAILYWORKCHARTCREATOR_H
