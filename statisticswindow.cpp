#include "statisticswindow.h"
#include "ui_statisticswindow.h"

StatisticsWindow::StatisticsWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::StatisticsWindow)
{
    ui->setupUi(this);

    setup();
}

StatisticsWindow::~StatisticsWindow()
{
    delete ui;
    delete toolBar;
}

void StatisticsWindow::addAnalyzer(ProjectAnalyzer *analyzer)
{
    analyzer->injectControlsContainer(ui->controlsContainer);

    QAction *action = toolBar->addAction(analyzer->getActionText());
    connect(action, SIGNAL(triggered(bool)), analyzer, SLOT(buildControls()));

    analyzers.push_back(analyzer);
}

void StatisticsWindow::setup()
{
    setWindowTitle(tr("Statistics"));

    toolBar = new QToolBar();

    QVBoxLayout *verticalLayout = static_cast<QVBoxLayout*>(layout());
    verticalLayout->insertWidget(0, toolBar);
}
