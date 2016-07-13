#ifndef EFFORTSTABLECREATOR_H
#define EFFORTSTABLECREATOR_H

#include "databasemanager.h"
#include "effortstablemodel.h"
#include "projectanalyzer.h"
#include "timeperiodchooser.h"

#include <QTableView>

class EffortsTableCreator : public ProjectAnalyzer
{
    Q_OBJECT
public:
    EffortsTableCreator(DatabaseManager *db,
                        EffortsTableModel *model,
                        TimePeriodChooser *timeChooser,
                        QTableView *tableView,
                        QObject *parent = NULL);
    virtual ~EffortsTableCreator();

    virtual QString getActionText() const;

public slots:
    virtual void updateView();

protected:
    EffortsTableModel *model;
    TimePeriodChooser *timeChooser;
    QTableView *tableView;
    QVBoxLayout *controlsLayout;

    virtual void buildControls();
};

#endif // EFFORTSTABLECREATOR_H
