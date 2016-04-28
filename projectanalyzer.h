#ifndef PROJECTANALYZER_H
#define PROJECTANALYZER_H

#include <QObject>
#include <QAction>
#include <QHBoxLayout>

#include "databasemanager.h"

class ProjectAnalyzer : public QObject
{
    Q_OBJECT
public:
    explicit ProjectAnalyzer(DatabaseManager *db, QObject *parent = 0);
    virtual ~ProjectAnalyzer();

    virtual QString getActionText() const = 0;
    virtual void updateView() = 0;
    virtual void injectControlsContainer(QHBoxLayout *container);

signals:

public slots:
    virtual void showControls();

protected:
    DatabaseManager *db;
    QWidget *controlsWidget;
    QWidget *viewWidget;
    QHBoxLayout *controlsContainer;

    virtual void buildControls() = 0;
};

#endif // PROJECTANALYZER_H
