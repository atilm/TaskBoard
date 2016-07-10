#include "edittaskdialog.h"
#include "ui_edittaskdialog.h"
#include <QDateTime>

EditTaskDialog::EditTaskDialog(EditProjectDialog *projectDialog,
                               TaskRecordsDialog *recordsDialog,
                               QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EditTaskDialog)
{
    ui->setupUi(this);
    this->projectDialog = projectDialog;
    this->recordsDialog = recordsDialog;

    setWindowTitle(tr("Edit Task"));

    initColorChooser();

    connect(ui->addProjectButton, SIGNAL(clicked(bool)),
            this, SLOT(handleAddProject()));
    connect(ui->editProjectButton, SIGNAL(clicked(bool)),
            this, SLOT(handleEditProject()));
    connect(ui->recordsButton, SIGNAL(clicked(bool)),
            this, SLOT(handleEditRecords()));
}

EditTaskDialog::~EditTaskDialog()
{
    delete ui;
    delete projectDialog;
    delete recordsDialog;
}

void EditTaskDialog::clear()
{
    ui->titleEdit->clear();
    ui->descriptionEdit->clear();
    ui->estimateEdit->clear();
    ui->projectComboBox->clear();
    ui->colorComboBox->setCurrentIndex(0);
}

void EditTaskDialog::setTaskModel(TaskModel *model)
{
    this->model = model;
    updateProjectList();
}

void EditTaskDialog::setTaskEntry(const TaskEntry &entry)
{
    currentTaskID = entry.id;
    ui->titleEdit->setPlainText(entry.title);
    ui->descriptionEdit->setPlainText(entry.description);
    ui->estimateEdit->setText(entry.estimateString());
    ui->colorComboBox->setCurrentIndex(entry.colorIndex);
    setProjectBoxToIndex(entry.projectIndex);
}

TaskEntry EditTaskDialog::getTaskEntry()
{
    TaskEntry entry;

    entry.title = ui->titleEdit->toPlainText();
    entry.description = ui->descriptionEdit->toPlainText();
    entry.projectShort = "DUMMY";
    entry.projectIndex = ui->projectComboBox->currentData().toInt();
    entry.setEstimate(ui->estimateEdit->text());
    entry.colorIndex = ui->colorComboBox->currentIndex();
    entry.created = QDateTime::currentDateTime();
    entry.closed = QDateTime();
    entry.state = TaskEntry::todo;

    return entry;
}

void EditTaskDialog::handleAddProject()
{
    projectDialog->clear();
    if(projectDialog->exec()){
        model->addProject(projectDialog->getProjectEntry());
        updateProjectList();
    }
}

void EditTaskDialog::handleEditProject()
{
    int projectIndex = ui->projectComboBox->currentData().toInt();
    ProjectEntry project = model->getProject(projectIndex);
    projectDialog->setProjectEntry(project);
    if(projectDialog->exec()){
        model->updateProject(projectDialog->getProjectEntry());
        updateProjectList();
        setProjectBoxToIndex(projectIndex);
    }
}

void EditTaskDialog::handleEditRecords()
{
    recordsDialog->exec(currentTaskID);
}

void EditTaskDialog::initColorChooser()
{
    ui->colorComboBox->addItems(colors.names);
}

void EditTaskDialog::updateProjectList()
{
    ui->projectComboBox->clear();

    QList<ProjectEntry> projects = model->getActiveProjects();

    foreach(ProjectEntry e, projects){
        ui->projectComboBox->addItem(e.title, e.id);
    }
}

void EditTaskDialog::setProjectBoxToIndex(int projectIndex)
{
    int itemCount = ui->projectComboBox->count();

    bool success = false;

    for(int i=0;i < itemCount; i++){
        if(ui->projectComboBox->itemData(i).toInt() == projectIndex){
            ui->projectComboBox->setCurrentIndex(i);
            success = true;
        }
    }

    // if project does not exist, because it is hidden:
    if(!success){
        ProjectEntry p = model->getProject(projectIndex);
        ui->projectComboBox->addItem(p.title, projectIndex);
        ui->projectComboBox->setCurrentIndex(ui->projectComboBox->count()-1);
    }
}
