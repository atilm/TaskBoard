#include "qlistviewdelegate.h"
#include "taskmodel.h"
#include <QDebug>
#include <QPainter>

QListViewDelegate::QListViewDelegate(QObject *parent)
    : QStyledItemDelegate(parent)
{

}

QListViewDelegate::~QListViewDelegate()
{

}

QSize QListViewDelegate::sizeHint(const QStyleOptionViewItem &option,
                                  const QModelIndex &index) const
{
    return QSize(200, 25);
}

void QListViewDelegate::paint(QPainter *painter,
                              const QStyleOptionViewItem &option,
                              const QModelIndex &index) const
{
    const TaskModel *model = static_cast<const TaskModel*>(index.model());
    TaskEntry task = model->getTask(index);
    QString displayString = QString("%1 %2").arg(task.projectShort).arg(task.title);

    painter->save();

    painter->setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);

    painter->setPen(QColor(254,210,47));

    if(option.state & QStyle::State_Selected)
        painter->setPen(Qt::red);

    QRect rect = boxRect(option.rect);

    painter->setBrush(QBrush(QColor(255,255,224)));
    painter->drawRoundedRect(rect, 5, 5);
    painter->setPen(Qt::black);
    painter->drawText(rect.topLeft().x() + 5, rect.bottomLeft().y() - 5,
                      displayString);

    painter->restore();
}

QRect QListViewDelegate::boxRect(const QRect &rect) const
{
    QRect r(rect);
    return r.adjusted(2, 2, -2, -2);
}

