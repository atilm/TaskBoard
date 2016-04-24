#ifndef BURNUPCHARTCREATOR_H
#define BURNUPCHARTCREATOR_H

#include <QComboBox>
#include <QVBoxLayout>

#include "projectanalyzer.h"

class BurnUpChartCreator : public ProjectAnalyzer
{
public:
    BurnUpChartCreator(QObject *parent = 0);
    virtual ~BurnUpChartCreator();

    QString getActionText() const;

public slots:

private:
    QVBoxLayout *controlsLayout;
    QComboBox *projectChooser;

    void buildControls();
};

#endif // BURNUPCHARTCREATOR_H
