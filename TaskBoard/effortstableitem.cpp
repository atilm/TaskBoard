#include "effortstableitem.h"
#include <QColor>

EffortsTableItem::EffortsTableItem(const QString &text,
                                   double effort,
                                   Type type)
{
    this->text = text;
    this->effort = effort;
    this->type = type;
}

QVariant EffortsTableItem::getText(int column) const
{
    if(column == 0)
        return QVariant(text);
    else if(column == 1)
        return QVariant(QString("%1").arg(effort,0,'f',1));
    else
        return QVariant();
}

QVariant EffortsTableItem::getColor() const
{
    if(type == Project)
        return QVariant(QColor(153,204,255));
    else
        return QVariant(QColor(255,255,255));
}

