#include "dailyworkchartcreator.h"
#include <QLabel>
#include <iostream>
using namespace std;

DailyWorkChartCreator::DailyWorkChartCreator(DatabaseManager *db, QObject *parent)
    : ProjectAnalyzer(db, parent)
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

void DailyWorkChartCreator::showControls()
{
    endEdit->setDate(QDate::currentDate());
    beginEdit->setDate(QDate::currentDate().addDays(-7));

    updatePlot();

    ProjectAnalyzer::showControls();
}

void DailyWorkChartCreator::buildControls()
{
    beginEdit = new QDateEdit();
    endEdit = new QDateEdit();
    controlsLayout = new QVBoxLayout();
    chartView = new QCustomPlot;
    viewWidget = chartView;

    controlsLayout->addWidget(new QLabel(tr("From:")));
    controlsLayout->addWidget(beginEdit);
    controlsLayout->addWidget(new QLabel(tr("To:")));
    controlsLayout->addWidget(endEdit);
    controlsLayout->addStretch();
    controlsWidget->setLayout(controlsLayout);
}

void DailyWorkChartCreator::updatePlot()
{
    chartView->xAxis->setTickLabelType(QCPAxis::ltDateTime);
    chartView->xAxis->setDateTimeFormat("yyyy-MM-dd");

    chartView->xAxis->setRange(beginEdit->dateTime().toTime_t(),
                               endEdit->dateTime().toTime_t());

    efforts = db->getProjectEfforts(beginEdit->date(), endEdit->date());

    QMapIterator <QString, QVector<double>> it(efforts);

    while(it.hasNext()){
        it.next();
        cout << it.key().toStdString();
        QVector<double> vec = it.value();

        foreach(double d, vec){
            cout << " " << d;
        }

        cout << endl;
    }

}

