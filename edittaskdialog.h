#ifndef EDITTASKDIALOG_H
#define EDITTASKDIALOG_H

#include <QDialog>
#include "colorcontainer.h"
#include "taskentry.h"
#include "taskmodel.h"
#include "editprojectdialog.h"

namespace Ui {
class EditTaskDialog;
}

class EditTaskDialog : public QDialog
{
    Q_OBJECT

public:
    explicit EditTaskDialog(EditProjectDialog *projectDialog,
                            QWidget *parent = 0);
    virtual ~EditTaskDialog();

    void clear();
    void setTaskModel(TaskModel *model);
    void setTaskEntry(const TaskEntry &entry);
    TaskEntry getTaskEntry();

protected slots:
    void handleAddProject();
    void handleEditProject();

private:
    Ui::EditTaskDialog *ui;
    TaskModel *model;
    EditProjectDialog *projectDialog;

    ColorContainer colors;

    void initColorChooser();
    void updateProjectList();
};

#endif // EDITTASKDIALOG_H
