#include "colorcontainer.h"

ColorContainer::ColorContainer()
{
    brushColors.append(QColor(255, 255, 224));
    brushColors.append(QColor(220, 255, 194));
    brushColors.append(QColor(195, 221, 255));
    brushColors.append(QColor(255, 204, 214));
    brushColors.append(QColor(255, 231, 182));
    brushColors.append(QColor(226, 210, 242));
    brushColors.append(QColor(255, 232, 255));
    brushColors.append(QColor(219, 255, 242));

    penColors.append(QColor(254, 210, 47));
    penColors.append(QColor(125, 187, 0));
    penColors.append(QColor(74, 153, 255));
    penColors.append(QColor(255, 128, 153));
    penColors.append(QColor(255, 177, 59));
    penColors.append(QColor(198, 156, 243));
    penColors.append(QColor(226, 170, 226));
    penColors.append(QColor(112, 221, 180));

    names.append("Yellow");
    names.append("Green");
    names.append("Blue");
    names.append("Red");
    names.append("Orange");
    names.append("Purple");
    names.append("Magenta");
    names.append("Cyan");
}

QBrush ColorContainer::brush(int index) const
{
    return QBrush(brushColors[index]);
}

