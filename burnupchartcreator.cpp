#include "burnupchartcreator.h"
#include <QLabel>
#include <QVBoxLayout>

BurnUpChartCreator::BurnUpChartCreator(QObject *parent)
    : ProjectAnalyzer(parent)
{
    projectChooser = new QComboBox();
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
    resetControls();

    controlsLayout = new QVBoxLayout();
    controlsLayout->addWidget(new QLabel(tr("Project:")));
    controlsLayout->addWidget(projectChooser);
    controlsLayout->addStretch();
    controlsContainer->setLayout(controlsLayout);
}

