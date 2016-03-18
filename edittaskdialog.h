#ifndef EDITTASKDIALOG_H
#define EDITTASKDIALOG_H

#include <QDialog>
#include "colorcontainer.h"
#include "taskentry.h"

namespace Ui {
class EditTaskDialog;
}

class EditTaskDialog : public QDialog
{
    Q_OBJECT

public:
    explicit EditTaskDialog(QWidget *parent = 0);
    virtual ~EditTaskDialog();

    void clear();
    void initProjets(const QStringList &projects);
    void setTaskEntry(const TaskEntry &entry);
    TaskEntry getTaskEntry();

private:
    Ui::EditTaskDialog *ui;

    ColorContainer colors;

    void initColorChooser();
};

#endif // EDITTASKDIALOG_H
