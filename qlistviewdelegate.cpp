#include "qlistviewdelegate.h"
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
    painter->save();

    painter->setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);

    if(option.state & QStyle::State_Selected)
        painter->fillRect(option.rect, option.palette.highlight());

    QRect rect = boxRect(option.rect);

    painter->setBrush(QBrush(QColor(225,220,220)));
    painter->drawRoundedRect(rect, 5, 5);
    painter->drawText(rect.topLeft().x() + 5, rect.bottomLeft().y() - 5,
                      index.data().toString());

    painter->restore();
}

QRect QListViewDelegate::boxRect(const QRect &rect) const
{
    QRect r(rect);
    return r.adjusted(2, 2, -2, -2);
}

