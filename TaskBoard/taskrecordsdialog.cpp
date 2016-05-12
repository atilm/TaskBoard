#include <QDateTime>
#include <QMessageBox>
#include <QSqlRecord>

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

    connect(ui->addButton, SIGNAL(clicked(bool)),
            this, SLOT(handleAddButton()));
    connect(ui->removeButton, SIGNAL(clicked(bool)),
            this, SLOT(handleRemoveButton()));
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
    setTaskID(taskID);
    model->setFilter(QString("task=%1").arg(taskID));
    model->select();
    return QDialog::exec();
}

void TaskRecordsDialog::handleAddButton()
{
    QSqlRecord record = model->record();

    QDateTime now = QDateTime::currentDateTime();

    record.setValue(1, taskID);
    record.setValue(2, 0);
    record.setValue(3, now.toString("yyyy-MM-dd"));
    record.setValue(4, now.toString("hh:mm"));

    model->insertRecord(-1, record);
    model->select();
}

void TaskRecordsDialog::handleRemoveButton()
{
    QModelIndexList indexes = ui->tableView->selectionModel()->selectedIndexes();

    if(!indexes.isEmpty()){
        int answer = QMessageBox::question(this, "Delete", "Delete this record?");

        if(answer == QMessageBox::Yes){
            int rowCount = indexes.last().row() - indexes.first().row() + 1;
            model->removeRows(indexes.first().row(), rowCount);
            model->select();
        }
    }
}
