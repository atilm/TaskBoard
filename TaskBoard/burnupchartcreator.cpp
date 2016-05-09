#include "burnupchartcreator.h"
#include <QLabel>
#include <QVBoxLayout>
#include "databasemanager.h"

BurnUpChartCreator::BurnUpChartCreator(DatabaseManager *db, QObject *parent)
    : ProjectAnalyzer(db, parent)
{
    buildControls();
}

BurnUpChartCreator::~BurnUpChartCreator()
{
    delete projectChooser;
}

QString BurnUpChartCreator::getActionText() const
{
    return tr("Burn-up chart");
}

void BurnUpChartCreator::updateView()
{
    updateProjectChooser();

    updatePlot();
}

void BurnUpChartCreator::showControls()
{
    updateProjectChooser();

    updatePlot();

    ProjectAnalyzer::showControls();
}

void BurnUpChartCreator::updatePlot()
{
    chartView->clearPlottables();

    updateEfforts();

    if(!efforts.isEmpty()){
        plotEfforts();
        formatAxes();
    }

    chartView->replot();
}

void BurnUpChartCreator::buildControls()
{  
    projectChooser = new QComboBox();
    controlsLayout = new QVBoxLayout();
    showAllDaysButton = new QRadioButton(tr("All days"));
    showAllDaysButton->setChecked(true);
    showRecordDaysButton = new QRadioButton(tr("Only days with records"));
    chartView = new QCustomPlot;
    viewWidget = chartView;

    connect(projectChooser, SIGNAL(currentIndexChanged(int)),
            this, SLOT(updatePlot()));
    connect(showAllDaysButton, SIGNAL(toggled(bool)),
            this, SLOT(updatePlot()));
    connect(showRecordDaysButton, SIGNAL(toggled(bool)),
            this, SLOT(updatePlot()));

    controlsLayout->addWidget(new QLabel(tr("Project:")));
    controlsLayout->addWidget(projectChooser);
    controlsLayout->addWidget(showAllDaysButton);
    controlsLayout->addWidget(showRecordDaysButton);
    controlsLayout->addStretch();
    controlsWidget->setLayout(controlsLayout);
}

void BurnUpChartCreator::updateProjectChooser()
{
    projectChooser->clear();
    projectChooser->addItems(db->listOfProjects());
}

void BurnUpChartCreator::updateEfforts()
{
    readEfforts();
    buildPlottableData();
}

void BurnUpChartCreator::readEfforts()
{
    if(projectChooser->count() == 0){
        qDebug() << "no projects";
        return;
    }

    if(projectChooser->currentIndex() < 0){
        qDebug() << "fixing negative index";
        projectChooser->setCurrentIndex(0);
    }

    efforts.clear();
    efforts = db->getDayEffortsOfProject(projectChooser->currentIndex()+1);
}

void BurnUpChartCreator::buildPlottableData()
{
    if(efforts.empty())
        return;

    dates.clear();
    accumulatedHours.clear();

    if(showAllDaysButton->isChecked())
        buildAllDates();
    else
        buildRecordDates();

    accumulatedHours.append(efforts.first().effortMinutes / 60.0);
    for(int i=1; i<efforts.size(); i++){
        accumulatedHours.append(accumulatedHours[i-1] + efforts[i].effortMinutes / 60.0);
    }
}

void BurnUpChartCreator::buildAllDates()
{
    foreach(DatabaseManager::DayEffort effort, efforts)
        dates.append(effort.date.toTime_t());
}

void BurnUpChartCreator::buildRecordDates()
{
    for(int d=0; d<efforts.count(); d++)
        dates.append(d+1);
}

void BurnUpChartCreator::formatAxes()
{
    chartView->xAxis->setTickLabelRotation(60);

    double xRangePadding = 0.1 * (dates.last() - dates.first());

    chartView->xAxis->setRange(dates.first() - xRangePadding,
                               dates.last()  + xRangePadding);
    chartView->yAxis->setRange(0, accumulatedHours.last() + 1);
    chartView->yAxis->setLabel(tr("Hours"));
}

void BurnUpChartCreator::plotEfforts()
{
    setXTicks();
    setYTicks();

    chartView->addGraph();
    chartView->graph()->setData(dates, accumulatedHours);
    chartView->graph()->setLineStyle(QCPGraph::lsLine);
    chartView->graph()->setScatterStyle(QCPScatterStyle::ssDisc);
}

void BurnUpChartCreator::setXTicks()
{
    xTicks.clear();
    xTickLabels.clear();

    if(showAllDaysButton->isChecked())
        buildAllDayTicks();
    else
        buildRecordDayTicks();

    chartView->xAxis->setAutoTicks(false);
    chartView->xAxis->setAutoTickLabels(false);
    chartView->xAxis->setTickVector(xTicks);
    chartView->xAxis->setTickVectorLabels(xTickLabels);
    chartView->xAxis->setSubTickCount(0);
}

void BurnUpChartCreator::buildAllDayTicks()
{
    QDateTime date = efforts.first().date;

    int days = date.daysTo(efforts.last().date);

    int step = getXTickSize(days);

    for(int d=0; d<=days; d += step){
        xTicks.append(date.toTime_t());
        xTickLabels.append(date.toString("yyyy-MM-dd"));
        date = date.addDays(step);
    }
}

void BurnUpChartCreator::buildRecordDayTicks()
{
    int step = getXTickSize(efforts.count());

    for(int i=0; i<efforts.count(); i += step){
        xTicks.append(i+1);
        xTickLabels.append(efforts[i].date.toString("yyyy-MM-dd"));
    }
}

int BurnUpChartCreator::getXTickSize(int days)
{
    int magnitude = days / 10;
    return (magnitude != 0) ? magnitude : 1;
}

void BurnUpChartCreator::setYTicks()
{

}

