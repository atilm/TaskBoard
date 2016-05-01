#include "qlistviewdelegate.h"
#include "taskmodel.h"
#include "taskcolumn.h"
#include <QDebug>
#include <QFontMetrics>
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
    QRect projectRect = drawProjectLabel(painter, rect, task);
    drawDoneDate(painter, projectRect, task);
    drawEffortInfo(painter, rect, task);
    drawTaskTitle(painter, rect, task);

    painter->restore();
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

    if(task.id == TaskColumn::getCurrentTaskID())
        painter->setPen(QPen(Qt::red, Qt::DashLine));
    else
        painter->setPen(colors.penColors[task.colorIndex]);

    painter->setBrush(colors.brush(task.colorIndex));
    painter->drawRoundedRect(rect, 5, 5);
    painter->restore();
}

QRect QListViewDelegate::drawProjectLabel(QPainter *painter, const QRect &rect, const TaskEntry &task) const
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

    return textRect;
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

void QListViewDelegate::drawDoneDate(QPainter *painter, const QRect &rect, const TaskEntry &task) const
{
    if(task.closedString().isEmpty())
        return;

    QString s = verboseClosedString(task.closedString());
    QFontMetrics fm(painter->font());

    QRect textRect = rect;
    textRect.moveTopLeft(textRect.topLeft() + QPoint(textRect.width() + 10, 0));
    textRect.setWidth(fm.width(s));
    textRect.setHeight(fm.height());

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

QString QListViewDelegate::verboseClosedString(const QString &dateString) const
{
    QDate today = QDate::currentDate();
    QDate date = QDate::fromString(dateString, "yyyy-MM-dd");

    int days = date.daysTo(today);

    switch(days){
    case 0:
        return "Today";
    case 1:
        return "1 day ago";
    default:
        return QString("%1 days ago").arg(days);
    }
}

