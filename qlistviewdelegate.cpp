#include "qlistviewdelegate.h"
#include "taskmodel.h"
#include <QDebug>
#include <QFontMetrics>
#include <QPainter>

QListViewDelegate::QListViewDelegate(QObject *parent)
    : QStyledItemDelegate(parent)
{
    initializeColors();
}

QListViewDelegate::~QListViewDelegate()
{

}

QSize QListViewDelegate::sizeHint(const QStyleOptionViewItem &option,
                                  const QModelIndex &index) const
{
    QFontMetrics fm(option.font);

    return QSize(option.rect.width(), 2*fm.height()+15);
}

void QListViewDelegate::paint(QPainter *painter,
                              const QStyleOptionViewItem &option,
                              const QModelIndex &index) const
{
    const TaskModel *model = static_cast<const TaskModel*>(index.model());
    TaskEntry task = model->getTask(index);

    painter->save();
    painter->setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);

    QRect rect = boxRect(option.rect);

    drawBackgroundBox(painter, option, rect, task);
    drawProjectLabel(painter, rect, task);
    drawEffortInfo(painter, rect, task);
    drawTaskTitle(painter, rect, task);

    painter->restore();
}

void QListViewDelegate::initializeColors()
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
}

QRect QListViewDelegate::boxRect(const QRect &rect) const
{
    QRect r(rect);
    return r.adjusted(2, 2, -2, -2);
}

void QListViewDelegate::drawBackgroundBox(QPainter *painter, const QStyleOptionViewItem &option,
                                          const QRect &rect, const TaskEntry &task) const
{
    painter->save();
    if(option.state & QStyle::State_Selected)
        painter->setPen(Qt::red);
    else
        painter->setPen(penColors[task.colorIndex]);

    painter->setBrush(QBrush(brushColors[task.colorIndex]));
    painter->drawRoundedRect(rect, 5, 5);
    painter->restore();
}

void QListViewDelegate::drawProjectLabel(QPainter *painter, const QRect &rect, const TaskEntry &task) const
{
    painter->save();

    QFont font = painter->font();
    font.setBold(true);
    painter->setFont(font);

    QFontMetrics fm(font);

    QRect textRect = rect;
    textRect.moveTopLeft(rect.topLeft() + QPoint(5,5));
    textRect.setWidth(fm.width(task.projectShort));
    textRect.setHeight(fm.height());

    painter->drawText(textRect, task.projectShort);

    painter->restore();
}

void QListViewDelegate::drawEffortInfo(QPainter *painter, const QRect &rect, const TaskEntry &task) const
{
    QString s = QString("%1 / %2").arg(task.effortString()).arg(task.estimateString());
    QFontMetrics fm(painter->font());

    QRect textRect = rect;
    textRect.setWidth(fm.width(s));
    textRect.setHeight(fm.height());
    textRect.moveTopRight(rect.topRight() + QPoint(-5, 5));

    painter->drawText(textRect, s);
}

void QListViewDelegate::drawTaskTitle(QPainter *painter, const QRect &rect, const TaskEntry &task) const
{
    QFontMetrics fm(painter->font());

    QRect textRect = rect;
    textRect.setWidth(rect.width() - 10);
    textRect.setHeight(fm.height());
    textRect.moveTopLeft(rect.topLeft() + QPoint(5, 5 + fm.height() + 5));

    painter->drawText(textRect, task.title);
}

