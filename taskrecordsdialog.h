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
    ~TaskRecordsDialog();

    void setTaskID(int id);

    int exec(int taskID);

private:
    Ui::TaskRecordsDialog *ui;
    QSqlDatabase *db;
    QSqlTableModel *model;
    int taskID;
};

#endif // TASKRECORDSDIALOG_H
