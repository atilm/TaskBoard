#include "dailyworkchartview.h"
#include <QToolTip>

DailyWorkChartView::DailyWorkChartView()
{

}

DailyWorkChartView::~DailyWorkChartView()
{

}

void DailyWorkChartView::mouseMoveEvent(QMouseEvent *event)
{
    QCPLayerable *layerable = layerableAt(event->pos(), false);
    QCPAbstractPlottable *plottable = qobject_cast<QCPAbstractPlottable*>(layerable);

    if(plottable){
        QString name = plottable->name();

        QToolTip::showText(event->screenPos().toPoint(), name, 0, QRect());
    }
    else
        QToolTip::showText(event->screenPos().toPoint(), "", 0, QRect());

    QCustomPlot::mouseMoveEvent(event);
}

