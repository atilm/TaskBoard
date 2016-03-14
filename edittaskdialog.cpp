#include "edittaskdialog.h"
#include "ui_edittaskdialog.h"
#include <QDateTime>

EditTaskDialog::EditTaskDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EditTaskDialog)
{
    ui->setupUi(this);
}

EditTaskDialog::~EditTaskDialog()
{
    delete ui;
}

void EditTaskDialog::clear()
{
    ui->titleEdit->clear();
    ui->descriptionEdit->clear();
    ui->estimateEdit->clear();
    ui->effortEdit->clear();
}

void EditTaskDialog::setTaskEntry(const TaskEntry &entry)
{
    ui->titleEdit->setText(entry.title);
    ui->descriptionEdit->setPlainText(entry.description);
    ui->estimateEdit->setText(entry.estimateString());
    ui->effortEdit->setText(entry.effortString());
}

TaskEntry EditTaskDialog::getTaskEntry()
{
    TaskEntry entry;

    entry.title = ui->titleEdit->text();
    entry.description = ui->descriptionEdit->toPlainText();
    entry.projectShort = "DUMMY";
    entry.projectIndex = 1;
    entry.estimated_minutes = 120;
    entry.colorIndex = 0;
    entry.created = QDateTime::currentDateTime();
    entry.closed = QDateTime();
    entry.state = TaskEntry::todo;

    return entry;
}
