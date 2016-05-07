#include "taskrecordsdialog.h"
#include "ui_taskrecordsdialog.h"

TaskRecordsDialog::TaskRecordsDialog(QSqlDatabase *db,
                                     QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TaskRecordsDialog)
{
    ui->setupUi(this);
    setWindowTitle(tr("Edit records"));

    this->db = db;

    model = new QSqlTableModel(0, *db);
    model->setTable("records");
    model->setEditStrategy(QSqlTableModel::OnFieldChange);

    ui->tableView->setModel(model);
    ui->tableView->hideColumn(0);
    ui->tableView->hideColumn(1);
}

TaskRecordsDialog::~TaskRecordsDialog()
{
    delete ui;
    delete model;
}

void TaskRecordsDialog::setTaskID(int id)
{
    taskID = id;
}

int TaskRecordsDialog::exec(int taskID)
{
    model->setFilter(QString("task=%1").arg(taskID));
    model->select();
    return QDialog::exec();
}
