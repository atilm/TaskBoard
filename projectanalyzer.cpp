#include "projectanalyzer.h"
#include <QLayout>

ProjectAnalyzer::ProjectAnalyzer(QObject *parent) : QObject(parent)
{

}

void ProjectAnalyzer::injectControlsContainer(QWidget *container)
{
    controlsContainer = container;
}

void ProjectAnalyzer::resetControls()
{
    delete controlsContainer->layout();
}


