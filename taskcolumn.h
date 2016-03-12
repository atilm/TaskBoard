#ifndef TASKCOLUMN_H
#define TASKCOLUMN_H

#include <QWidget>
#include <QAbstractListModel>

namespace Ui {
class TaskColumn;
}

class TaskColumn : public QWidget
{
    Q_OBJECT

public:
    explicit TaskColumn(QWidget *parent = 0);
    virtual ~TaskColumn();

    virtual void setTitle(const QString &title);
    virtual void setModel(QAbstractListModel *model);

private:
    Ui::TaskColumn *ui;
};

#endif // TASKCOLUMN_H
