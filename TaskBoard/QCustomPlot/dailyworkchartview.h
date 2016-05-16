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
};

#endif // DAILYWORKCHARTVIEW_H
