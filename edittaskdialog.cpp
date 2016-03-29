#include "edittaskdialog.h"
#include "ui_edittaskdialog.h"
#include <QDateTime>

EditTaskDialog::EditTaskDialog(EditProjectDialog *projectDialog,
                               QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EditTaskDialog)
{
    ui->setupUi(this);
    this->projectDialog = projectDialog;

    setWindowTitle(tr("Edit Task"));

    initColorChooser();

    connect(ui->addProjectButton, SIGNAL(clicked(bool)),
            this, SLOT(handleAddProject()));
    connect(ui->editProjectButton, SIGNAL(clicked(bool)),
            this, SLOT(handleEditProject()));
}

EditTaskDialog::~EditTaskDialog()
{
    delete ui;
    delete projectDialog;
}

void EditTaskDialog::clear()
{
    ui->titleEdit->clear();
    ui->descriptionEdit->clear();
    ui->estimateEdit->clear();
    ui->effortEdit->clear();
    ui->projectComboBox->clear();
    ui->colorComboBox->setCurrentIndex(0);
}

void EditTaskDialog::setTaskModel(TaskModel *model)
{
    this->model = model;
    ui->projectComboBox->addItems(model->projectList());
}

void EditTaskDialog::setTaskEntry(const TaskEntry &entry)
{
    ui->titleEdit->setPlainText(entry.title);
    ui->descriptionEdit->setPlainText(entry.description);
    ui->estimateEdit->setText(entry.estimateString());
    ui->effortEdit->setText(entry.effortString());
    ui->colorComboBox->setCurrentIndex(entry.colorIndex);
}

TaskEntry EditTaskDialog::getTaskEntry()
{
    TaskEntry entry;

    entry.title = ui->titleEdit->toPlainText();
    entry.description = ui->descriptionEdit->toPlainText();
    entry.projectShort = "DUMMY";
    entry.projectIndex = 1;
    entry.setEstimate(ui->estimateEdit->text());
    entry.colorIndex = ui->colorComboBox->currentIndex();
    entry.created = QDateTime::currentDateTime();
    entry.closed = QDateTime();
    entry.state = TaskEntry::todo;

    return entry;
}

void EditTaskDialog::handleAddProject()
{
    projectDialog->exec();
}

void EditTaskDialog::handleEditProject()
{
    int projectIndex = ui->projectComboBox->currentIndex() + 1;
    ProjectEntry project = model->getProject(projectIndex);
    projectDialog->setProjectEntry(project);
    if(projectDialog->exec()){
        model->updateProject(projectDialog->getProjectEntry());
    }
}

void EditTaskDialog::initColorChooser()
{
    ui->colorComboBox->addItems(colors.names);
}
