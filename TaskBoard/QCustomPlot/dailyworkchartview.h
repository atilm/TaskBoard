#ifndef DAILYWORKCHARTVIEW_H
#define DAILYWORKCHARTVIEW_H

#include "QCustomPlot/qcustomplot.h"
#include <QObject>

class DailyWorkChartView : public QCustomPlot
{
public:
    DailyWorkChartView();
    virtual ~DailyWorkChartView();

protected:

    virtual void mouseMoveEvent(QMouseEvent *event);
    virtual QString getTimeString(double seconds, QCPBars *bars);
};

#endif // DAILYWORKCHARTVIEW_H
