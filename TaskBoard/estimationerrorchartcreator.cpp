#include "estimationerrorchartcreator.h"
#include "histogram.h"

EstimationErrorChartCreator::EstimationErrorChartCreator(DatabaseManager *db, QObject *parent)
    : ProjectAnalyzer(db, parent)
{
    buildControls();
}

EstimationErrorChartCreator::~EstimationErrorChartCreator()
{
    delete beginEdit;
    delete endEdit;
    delete updateButton;
}

QString EstimationErrorChartCreator::getActionText() const
{
    return tr("Estimation errors");
}

void EstimationErrorChartCreator::updateView()
{
    updatePlot();
}

void EstimationErrorChartCreator::showControls()
{
    updatePlot();

    ProjectAnalyzer::showControls();
}

void EstimationErrorChartCreator::updatePlot()
{
    chartView->clearPlottables();

    errors = db->getEstimationErrors(beginEdit->date(), endEdit->date());

    if(!errors.isEmpty()){
        calculateData();
        formatAxes();
        plotHistogram();
    }

    chartView->replot();
}

void EstimationErrorChartCreator::buildControls()
{
    beginEdit = new QDateEdit();
    endEdit = new QDateEdit();
    binChooser = new QSpinBox();
    updateButton = new QPushButton();
    minutesButton = new QRadioButton();
    percentButton = new QRadioButton();

    endEdit->setDate(QDate::currentDate());
    beginEdit->setDate(QDate::currentDate().addMonths(-1));
    updateButton->setText(tr("Update"));

    binChooser->setMinimum(1);
    binChooser->setMaximum(24*60);

    minutesButton->setText(tr("Minutes"));
    percentButton->setText(tr("Percent"));
    minutesButton->setChecked(true);

    connect(updateButton, SIGNAL(clicked(bool)),
            this, SLOT(updatePlot()));

    controlsLayout = new QVBoxLayout();
    chartView = new QCustomPlot();
    viewWidget = chartView;

    controlsLayout->addWidget(new QLabel(tr("From:")));
    controlsLayout->addWidget(beginEdit);
    controlsLayout->addWidget(new QLabel(tr("To:")));
    controlsLayout->addWidget(endEdit);
    controlsLayout->addWidget(new QLabel(tr("Histogram bin width:")));
    controlsLayout->addWidget(binChooser);
    controlsLayout->addWidget(minutesButton);
    controlsLayout->addWidget(percentButton);
    controlsLayout->addWidget(updateButton);
    controlsLayout->addStretch();
    controlsWidget->setLayout(controlsLayout);
}

void EstimationErrorChartCreator::calculateData()
{
    Histogram hist;
    hist.calculate(extractDataFromErrors(), binChooser->value());
    xTicks = hist.getTicks();
    barHeights = hist.getBars();

    foreach(double t, xTicks){
        qDebug() << t;
    }

    foreach(double h, barHeights){
        qDebug() << h;
    }
}

QVector<double> EstimationErrorChartCreator::extractDataFromErrors()
{
    QVector<double> data;

    foreach(DatabaseManager::EstimationError e, errors){
        data.append(e.estimationError);
    }

    return data;
}

void EstimationErrorChartCreator::formatAxes()
{
    chartView->xAxis->setTickLabelRotation(60);
    chartView->xAxis->setRange(xTicks.first() - binChooser->value(),
                               xTicks.last()  + binChooser->value());
    chartView->xAxis->setLabel(tr("Estimation error [min]"));

    double maxHeight = *std::max_element(barHeights.begin(), barHeights.end());
    chartView->yAxis->setRange(0, maxHeight * 1.02);
    chartView->yAxis->setLabel(tr("Count"));
}

void EstimationErrorChartCreator::plotHistogram()
{
    setXTicks();
    setYTicks();

    QCPBars *bars = new QCPBars(chartView->xAxis, chartView->yAxis);

    bars->setData(xTicks, barHeights);
    chartView->addPlottable(bars);
}

void EstimationErrorChartCreator::setXTicks()
{
    chartView->xAxis->setAutoTicks(false);
    chartView->xAxis->setTickVector(xTicks);
    chartView->xAxis->setSubTickCount(0);
}

void EstimationErrorChartCreator::setYTicks()
{

}

