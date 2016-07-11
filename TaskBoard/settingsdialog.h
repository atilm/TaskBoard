#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>
#include <QSqlDatabase>

#include "sqlitetablemodel.h"

namespace Ui {
class SettingsDialog;
}

class SettingsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SettingsDialog(QSqlDatabase *db,
                            QWidget *parent = 0);
    virtual ~SettingsDialog();

public slots:
    int exec();

private:
    Ui::SettingsDialog *ui;
    QSqlDatabase *db;
    SQLiteTableModel *model;
};

#endif // SETTINGSDIALOG_H
