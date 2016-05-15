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
        if(histogramButton->isChecked()){
            calculateData();
            formatAxes();
            plotHistogram();
        }
        else{
            extractScatterData();
            formatScatterAxes();
            scatterPlot();
        }
    }

    chartView->replot();
}

void EstimationErrorChartCreator::buildControls()
{
    beginEdit = new QDateEdit();
    endEdit = new QDateEdit();
    binChooser = new QSpinBox();
    updateButton = new QPushButton();
    histogramButton = new QRadioButton();
    scatterPlotButton = new QRadioButton();

    endEdit->setDate(QDate::currentDate());
    beginEdit->setDate(QDate::currentDate().addMonths(-1));
    updateButton->setText(tr("Update"));

    binChooser->setMinimum(1);
    binChooser->setMaximum(24*60);
    binChooser->setValue(10);

    histogramButton->setText(tr("Histogram"));
    scatterPlotButton->setText(tr("Scatter Plot"));
    histogramButton->setChecked(true);

    connect(updateButton, SIGNAL(clicked(bool)),
            this, SLOT(updatePlot()));

    controlsLayout = new QVBoxLayout();
    chartView = new QCustomPlot();
    viewWidget = chartView;

    controlsLayout->addWidget(new QLabel(tr("From:")));
    controlsLayout->addWidget(beginEdit);
    controlsLayout->addWidget(new QLabel(tr("To:")));
    controlsLayout->addWidget(endEdit);
    controlsLayout->addWidget(new QLabel(tr("Estimation step [min]:")));
    controlsLayout->addWidget(binChooser);
    controlsLayout->addWidget(histogramButton);
    controlsLayout->addWidget(scatterPlotButton);
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
}

QVector<double> EstimationErrorChartCreator::extractDataFromErrors()
{
    QVector<double> data;

    foreach(DatabaseManager::EstimationError e, errors){
        data.append(e.estimationError());
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
    bars->setWidthType(QCPBars::wtPlotCoords);
    bars->setWidth(binChooser->value());

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
    chartView->yAxis->setAutoTicks(true);
    chartView->yAxis->setAutoTickLabels(true);
    chartView->yAxis->setAutoSubTicks(false);
    chartView->yAxis->setAutoTickStep(false);
    chartView->yAxis->setTickStep(1);
    chartView->yAxis->setSubTickCount(0);
}

void EstimationErrorChartCreator::extractScatterData()
{
    scatterX.clear();
    scatterY.clear();

    foreach(DatabaseManager::EstimationError e, errors){
        scatterX.append(e.estimation);
        scatterY.append(e.effort);
    }
}

void EstimationErrorChartCreator::formatScatterAxes()
{
    double maxEstimation = *std::max_element(scatterX.begin(), scatterX.end());
    double maxEffort = *std::max_element(scatterY.begin(), scatterY.end());
    double range = fmax(maxEstimation, maxEffort);

    chartView->xAxis->setTickLabelRotation(60);
    chartView->xAxis->setRange(0, range * 1.02);
    chartView->xAxis->setLabel(tr("Estimation [min]"));

    chartView->yAxis->setRange(0, range * 1.02);
    chartView->yAxis->setLabel(tr("Effort [min]"));

    chartView->xAxis->setAutoTicks(true);
    chartView->xAxis->setAutoTickLabels(true);
    chartView->xAxis->setAutoSubTicks(false);
    chartView->xAxis->setAutoTickStep(false);
    chartView->xAxis->setTickStep(binChooser->value());
    chartView->xAxis->setSubTickCount(0);

    chartView->yAxis->setAutoTicks(true);
    chartView->yAxis->setAutoTickLabels(true);
    chartView->yAxis->setAutoSubTicks(false);
    chartView->yAxis->setAutoTickStep(false);
    chartView->yAxis->setTickStep(binChooser->value());
    chartView->yAxis->setSubTickCount(0);
}

void EstimationErrorChartCreator::scatterPlot()
{
    QCPGraph *scatter = chartView->addGraph();
    scatter->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssDisc, 4));
    scatter->setLineStyle(QCPGraph::lsNone);
    scatter->setData(scatterX, scatterY);

    QPen pen;
    pen.setStyle(Qt::SolidLine);
    pen.setWidth(1);
    pen.setColor(Qt::gray);

    scatterX.insert(0, 0);
    scatterY.insert(0, 0);

    QCPGraph *identity = chartView->addGraph();
    identity->setPen(pen);
    identity->setData(scatterX, scatterX);
}

