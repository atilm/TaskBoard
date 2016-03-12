#ifndef QLISTVIEWDELEGATE_H
#define QLISTVIEWDELEGATE_H

#include <QStyledItemDelegate>

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
    QRect boxRect(const QRect &rect) const;
};

#endif // QLISTVIEWDELEGATE_H
