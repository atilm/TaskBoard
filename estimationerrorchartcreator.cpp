#include "estimationerrorchartcreator.h"

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

}

void EstimationErrorChartCreator::buildControls()
{
    beginEdit = new QDateEdit();
    endEdit = new QDateEdit();
    updateButton = new QPushButton();

    endEdit->setDate(QDate::currentDate());
    beginEdit->setDate(QDate::currentDate().addYears(-1));
    updateButton->setText(tr("Update"));

    connect(updateButton, SIGNAL(clicked(bool)),
            this, SLOT(updatePlot()));

    controlsLayout = new QVBoxLayout();
    chartView = new QCustomPlot();
    viewWidget = chartView;

    controlsLayout->addWidget(new QLabel(tr("From:")));
    controlsLayout->addWidget(beginEdit);
    controlsLayout->addWidget(new QLabel(tr("To:")));
    controlsLayout->addWidget(endEdit);
    controlsLayout->addWidget(updateButton);
    controlsLayout->addStretch();
    controlsWidget->setLayout(controlsLayout);
}

