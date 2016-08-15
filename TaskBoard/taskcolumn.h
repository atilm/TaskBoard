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

    static int getCurrentTaskID();

    virtual void setTitle(const QString &title);
    virtual void setModel(TaskModel *model);
    virtual void setColumnState(TaskEntry::State state);

signals:
    void currentTaskIDChanged(int current, int previous);

private:
    Ui::TaskColumn *ui;
    static int currentTaskID;
    TaskEntry::State columnState;
    TaskModel *model;
    EditTaskDialog *editDialog;

    bool eventFilter(QObject *, QEvent *event);

protected slots:
    void handleAddClicked();
    void handleRemoveCurrent();
    void handleTaskClicked(QModelIndex index);
    void handleTaskDoubleClicked(QModelIndex index);
    void handleListViewContextMenuRequested(QPoint point);
};

#endif // TASKCOLUMN_H
