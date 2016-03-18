#ifndef COLORCONTAINER_H
#define COLORCONTAINER_H

#include <QBrush>
#include <QColor>
#include <QList>
#include <QStringList>

class ColorContainer
{
public:
    ColorContainer();

    QBrush brush(int index) const;

    QList<QColor> brushColors;
    QList<QColor> penColors;
    QStringList names;
};

#endif // COLORCONTAINER_H
