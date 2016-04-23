#ifndef PROJECTANALYZER_H
#define PROJECTANALYZER_H

#include <QObject>
#include <QAction>

class ProjectAnalyzer : public QObject
{
    Q_OBJECT
public:
    explicit ProjectAnalyzer(QObject *parent = 0);

    virtual QString getActionText() const = 0;
    virtual void injectControlsContainer(QWidget *container);

signals:

public slots:

    virtual void buildControls() = 0;

protected:
    QWidget *controlsContainer;

    void resetControls();
};

#endif // PROJECTANALYZER_H
