#ifndef EFFORTSTABLEITEM_H
#define EFFORTSTABLEITEM_H

#include <QString>
#include <QVariant>

class EffortsTableItem
{
public:

    enum Type{
        Project,
        Task
    };

    EffortsTableItem(const QString &text,
                     double effort,
                     Type type = Task);

    QVariant getText(int column) const;
    QVariant getColor() const;

    QString text;
    double effort;
    Type type;
};

#endif // EFFORTSTABLEITEM_H
