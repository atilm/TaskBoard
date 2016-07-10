#include "projectanalyzer.h"
#include <QLayout>

ProjectAnalyzer::ProjectAnalyzer(DatabaseManager *db, QObject *parent) : QObject(parent)
{
    this->db = db;
    controlsWidget = new QWidget();
    controlsWidget->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Preferred);
}

ProjectAnalyzer::~ProjectAnalyzer()
{
    delete controlsWidget;
}

void ProjectAnalyzer::injectControlsContainer(QHBoxLayout *container)
{
    controlsContainer = container;
}

void ProjectAnalyzer::showControls()
{
    QWidget *previousControls = controlsContainer->itemAt(0)->widget();
    QWidget *previousView = controlsContainer->itemAt(1)->widget();
    controlsContainer->removeWidget(previousControls);
    controlsContainer->removeWidget(previousView);
    previousControls->setVisible(false);
    previousView->setVisible(false);

    controlsContainer->insertWidget(0, viewWidget);
    controlsContainer->insertWidget(0, controlsWidget);
    controlsWidget->setVisible(true);
    viewWidget->setVisible(true);
}

