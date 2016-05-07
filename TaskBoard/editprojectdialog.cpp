#include "editprojectdialog.h"
#include "ui_editprojectdialog.h"

EditProjectDialog::EditProjectDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EditProjectDialog)
{
    ui->setupUi(this);

    setWindowTitle(tr("Edit Project"));
}

EditProjectDialog::~EditProjectDialog()
{
    delete ui;
}

void EditProjectDialog::clear()
{
    currentId = -1;
    ui->titleEdit->clear();
    ui->shortSignEdit->clear();
    ui->descriptionEdit->clear();
}

void EditProjectDialog::setProjectEntry(ProjectEntry project)
{
    currentId = project.id;
    ui->titleEdit->setText(project.title);
    ui->shortSignEdit->setText(project.shortSign);
    ui->descriptionEdit->setPlainText(project.description);
}

ProjectEntry EditProjectDialog::getProjectEntry() const
{
    ProjectEntry project;
    project.id = currentId;
    project.title = ui->titleEdit->text();
    project.shortSign = ui->shortSignEdit->text();
    project.description = ui->descriptionEdit->toPlainText();
    return project;
}
