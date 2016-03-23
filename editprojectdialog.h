#ifndef EDITPROJECTDIALOG_H
#define EDITPROJECTDIALOG_H

#include <QDialog>
#include "projectentry.h"

namespace Ui {
class EditProjectDialog;
}

class EditProjectDialog : public QDialog
{
    Q_OBJECT

public:
    explicit EditProjectDialog(QWidget *parent = 0);
    ~EditProjectDialog();

    void clear();
    void setProjectEntry(ProjectEntry project);
    ProjectEntry getProjectEntry() const;

private:
    Ui::EditProjectDialog *ui;
};

#endif // EDITPROJECTDIALOG_H
