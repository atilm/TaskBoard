#ifndef STATISTICSWINDOW_H
#define STATISTICSWINDOW_H

#include <QDialog>
#include <QToolBar>
#include <QList>

#include "projectanalyzer.h"

namespace Ui {
class StatisticsWindow;
}

class StatisticsWindow : public QDialog
{
    Q_OBJECT

public:
    explicit StatisticsWindow(QWidget *parent = 0);
    ~StatisticsWindow();

    void addAnalyzer(ProjectAnalyzer *analyzer);
    void show();

private:
    Ui::StatisticsWindow *ui;
    QToolBar *toolBar;
    QList<ProjectAnalyzer*> analyzers;

    void setup();
};

#endif // STATISTICSWINDOW_H
