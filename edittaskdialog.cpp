#include "edittaskdialog.h"
#include "ui_edittaskdialog.h"
#include <QDateTime>

EditTaskDialog::EditTaskDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EditTaskDialog)
{
    ui->setupUi(this);

    initColorChooser();
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
    ui->projectComboBox->clear();
}

void EditTaskDialog::initProjets(const QStringList &projects)
{
    ui->projectComboBox->addItems(projects);
}

void EditTaskDialog::setTaskEntry(const TaskEntry &entry)
{
    ui->titleEdit->setText(entry.title);
    ui->descriptionEdit->setPlainText(entry.description);
    ui->estimateEdit->setText(entry.estimateString());
    ui->effortEdit->setText(entry.effortString());
    ui->colorComboBox->setCurrentIndex(entry.colorIndex);
}

TaskEntry EditTaskDialog::getTaskEntry()
{
    TaskEntry entry;

    entry.title = ui->titleEdit->text();
    entry.description = ui->descriptionEdit->toPlainText();
    entry.projectShort = "DUMMY";
    entry.projectIndex = 1;
    entry.estimated_minutes = 120;
    entry.colorIndex = ui->colorComboBox->currentIndex();
    entry.created = QDateTime::currentDateTime();
    entry.closed = QDateTime();
    entry.state = TaskEntry::todo;

    return entry;
}

void EditTaskDialog::initColorChooser()
{
    ui->colorComboBox->addItems(colors.names);
}
