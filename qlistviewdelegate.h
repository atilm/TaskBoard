#ifndef QLISTVIEWDELEGATE_H
#define QLISTVIEWDELEGATE_H

#include <QStyledItemDelegate>
#include <QList>
#include "colorcontainer.h"
#include "taskentry.h"

class QListViewDelegate : public QStyledItemDelegate
{
public:
    QListViewDelegate(QObject *parent = 0);
    virtual ~QListViewDelegate();

    virtual QSize sizeHint(const QStyleOptionViewItem &option,
                           const QModelIndex &index) const;
    virtual void paint(QPainter *painter,
                       const QStyleOptionViewItem &option,
                       const QModelIndex &index) const;

private:
    ColorContainer colors;

    QRect boxRect(const QRect &rect) const;
    void drawBackgroundBox(QPainter *painter, const QStyleOptionViewItem &option,
                           const QRect &rect, const TaskEntry &task) const;
    QRect drawProjectLabel(QPainter *painter, const QRect &rect, const TaskEntry &task) const;
    void drawEffortInfo(QPainter *painter, const QRect &rect, const TaskEntry &task) const;
    void drawDoneDate(QPainter *painter, const QRect &rect, const TaskEntry &task) const;
    void drawTaskTitle(QPainter *painter, const QRect &rect, const TaskEntry &task) const;
    QString verboseClosedString(const QString &dateString) const;
};

#endif // QLISTVIEWDELEGATE_H
