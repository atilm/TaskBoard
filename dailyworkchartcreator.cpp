#include "dailyworkchartcreator.h"
#include <QLabel>

DailyWorkChartCreator::DailyWorkChartCreator(QObject *parent)
    : ProjectAnalyzer(parent)
{
    buildControls();
}

DailyWorkChartCreator::~DailyWorkChartCreator()
{
    delete beginEdit;
    delete endEdit;
}

QString DailyWorkChartCreator::getActionText() const
{
    return tr("Daily work");
}

void DailyWorkChartCreator::buildControls()
{
    beginEdit = new QDateEdit();
    endEdit = new QDateEdit();
    controlsLayout = new QVBoxLayout();

    controlsLayout->addWidget(new QLabel(tr("From:")));
    controlsLayout->addWidget(beginEdit);
    controlsLayout->addWidget(new QLabel(tr("To:")));
    controlsLayout->addWidget(endEdit);
    controlsLayout->addStretch();
    controlsWidget->setLayout(controlsLayout);
}

