#include "effortstablecreator.h"


EffortsTableCreator::EffortsTableCreator(DatabaseManager *db,
                                         EffortsTableModel *model,
                                         TimePeriodChooser *timeChooser,
                                         QTableView *tableView, QObject *parent)
    :  ProjectAnalyzer(db, parent)
{
    this->model = model;
    this->timeChooser = timeChooser;
    this->tableView = tableView;

    model->injectDataBase(db);
    model->injectTimeChooser(timeChooser);

    tableView->setModel(model);

    buildControls();
}

EffortsTableCreator::~EffortsTableCreator()
{
    delete timeChooser;
    delete tableView;
}

QString EffortsTableCreator::getActionText() const
{
    return tr("Efforts Table");
}

void EffortsTableCreator::updateView()
{
    model->reload(timeChooser->beginDate(),
                  timeChooser->endDate());
}

void EffortsTableCreator::buildControls()
{
    timeChooser->toggleCurrentWeek(true);
    controlsLayout = new QVBoxLayout();
    controlsLayout->addWidget(timeChooser);
    controlsLayout->addStretch();
    controlsWidget->setLayout(controlsLayout);

    viewWidget = tableView;

    updateView();

    connect(timeChooser, SIGNAL(periodChanged()),
            this, SLOT(updateView()));
}

