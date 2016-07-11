#include "settingsdialog.h"
#include "ui_settingsdialog.h"

SettingsDialog::SettingsDialog(QSqlDatabase *db,
                               QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SettingsDialog)
{
    ui->setupUi(this);
    this->db = db;

    model = new SQLiteTableModel(0, *db);
    model->setTable("projects");
    model->setEditStrategy(QSqlTableModel::OnFieldChange);
    model->setCheckable(4);

    ui->projectsTableView->setModel(model);
    ui->projectsTableView->hideColumn(0);
    ui->projectsTableView->hideColumn(1);
    ui->projectsTableView->hideColumn(3);

    setWindowTitle(tr("Settings"));
}

SettingsDialog::~SettingsDialog()
{
    delete ui;
}

int SettingsDialog::exec()
{
    model->select();
    return QDialog::exec();
}
