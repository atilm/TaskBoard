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
    formatAxes();
    plotEfforts();

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

    qDebug() << "chooser items: " << projectChooser->count();
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

    dates.append(efforts.first().date.toTime_t());
    accumulatedHours.append(efforts.first().effortMinutes / 60.0);

    for(int i=1; i<efforts.size(); i++){
        dates.append(efforts[i].date.toTime_t());
        accumulatedHours.append(accumulatedHours[i-1] + efforts[i].effortMinutes / 60.0);
    }
}

void BurnUpChartCreator::formatAxes()
{
    chartView->xAxis->setTickLabelType(QCPAxis::ltDateTime);
    chartView->xAxis->setDateTimeFormat("yyyy-MM-dd");

    double secondsPerDay = 86400;

    chartView->xAxis->setRange(efforts.first().date.toTime_t() - secondsPerDay,
                               efforts.last().date.toTime_t() + secondsPerDay);
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

    if(showAllDaysButton->isChecked())
        buildAllDayTicks();
    else
        buildRecordDayTicks();

    chartView->xAxis->setAutoTicks(false);
    chartView->xAxis->setTickVector(xTicks);
    chartView->xAxis->setSubTickCount(0);
}

void BurnUpChartCreator::buildAllDayTicks()
{
    QDateTime date = efforts.first().date;

    int days = date.daysTo(efforts.last().date);

    for(int d=0; d<=days; d++){
        xTicks.append(date.toTime_t());
        date = date.addDays(1);
    }
}

void BurnUpChartCreator::buildRecordDayTicks()
{
    foreach(DatabaseManager::DayEffort entry, efforts)
        xTicks.append(entry.date.toTime_t());
}

void BurnUpChartCreator::setYTicks()
{

}

