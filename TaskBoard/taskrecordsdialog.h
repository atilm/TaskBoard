#ifndef TASKRECORDSDIALOG_H
#define TASKRECORDSDIALOG_H

#include <QDialog>
#include <QSqlDatabase>
#include <QSqlTableModel>

namespace Ui {
class TaskRecordsDialog;
}

class TaskRecordsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit TaskRecordsDialog(QSqlDatabase *db,
                               QWidget *parent = 0);
    virtual ~TaskRecordsDialog();

    void setTaskID(int id);

    int exec(int taskID);

private slots:
    void handleAddButton();
    void handleRemoveButton();

private:
    Ui::TaskRecordsDialog *ui;
    QSqlDatabase *db;
    QSqlTableModel *model;
    int taskID;
};

#endif // TASKRECORDSDIALOG_H
