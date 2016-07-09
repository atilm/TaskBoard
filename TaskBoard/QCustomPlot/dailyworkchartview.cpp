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
    QCPBars *bars = qobject_cast<QCPBars*>(layerable);

    if(bars){
        QString name = bars->name();
        double seconds = xAxis->pixelToCoord(event->pos().x());

        QString timeString = getTimeString(seconds, bars);

        QString display = QString("%1: %2").arg(name).arg(timeString);

        QToolTip::showText(event->screenPos().toPoint(), display, 0, QRect());
    }
    else
        QToolTip::showText(event->screenPos().toPoint(), "", 0, QRect());

    QCustomPlot::mouseMoveEvent(event);
}

QString DailyWorkChartView::getTimeString(double seconds, QCPBars *bars)
{
    QCPBarDataMap *data = bars->data();

    QCPBarDataMapIterator it(*data);

    QDateTime day;
    QDateTime cursorTime = QDateTime::fromTime_t(seconds);
    double hours = 0;

    while(it.hasNext()){
        it.next();

        day = QDateTime::fromTime_t(it.key());

        // half a day = 43200 s
        if(abs(cursorTime.secsTo(day)) < 43200){
            hours = it.value().value;
            break;
        }
    }

    return QString("%1 h").arg(hours,0,'f',1);
}

