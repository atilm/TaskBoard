#include "projectanalyzer.h"
#include <QLayout>

ProjectAnalyzer::ProjectAnalyzer(QObject *parent) : QObject(parent)
{
    controlsWidget = new QWidget();
    controlsWidget->setMaximumWidth(150);
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
    QWidget *previous = controlsContainer->itemAt(0)->widget();
    controlsContainer->removeWidget(previous);
    previous->setVisible(false);

    controlsContainer->insertWidget(0, controlsWidget);
    controlsWidget->setVisible(true);
}

