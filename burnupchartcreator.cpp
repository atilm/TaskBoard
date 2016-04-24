#include "burnupchartcreator.h"
#include <QLabel>
#include <QVBoxLayout>

BurnUpChartCreator::BurnUpChartCreator(QObject *parent)
    : ProjectAnalyzer(parent)
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

void BurnUpChartCreator::buildControls()
{  
    projectChooser = new QComboBox();
    controlsLayout = new QVBoxLayout();
    chartView = new QCustomPlot;
    viewWidget = chartView;

    controlsLayout->addWidget(new QLabel(tr("Project:")));
    controlsLayout->addWidget(projectChooser);
    controlsLayout->addStretch();
    controlsWidget->setLayout(controlsLayout);
}

