#ifndef TASKCOLUMN_H
#define TASKCOLUMN_H

#include <QMenu>
#include <QWidget>
#include <QAbstractListModel>
#include "edittaskdialog.h"
#include "taskmodel.h"

namespace Ui {
class TaskColumn;
}

class TaskColumn : public QWidget
{
    Q_OBJECT

public:
    explicit TaskColumn(EditTaskDialog *editDialog,
                        QWidget *parent = 0);
    virtual ~TaskColumn();

    virtual void setTitle(const QString &title);
    virtual void setModel(TaskModel *model);
    virtual void setColumnState(TaskEntry::State state);

private:
    Ui::TaskColumn *ui;
    TaskEntry::State columnState;
    TaskModel *model;
    EditTaskDialog *editDialog;

protected slots:
    void handleAddClicked();
    void handleRemoveCurrent();
    void handleTaskDoubleClicked(QModelIndex index);
    void handleListViewContextMenuRequested(QPoint point);
};

#endif // TASKCOLUMN_H
