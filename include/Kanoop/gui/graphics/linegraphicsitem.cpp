#include "linegraphicsitem.h"

#include <QPen>

LineGraphicsItem::LineGraphicsItem(int type, QGraphicsItem* parent) :
    QGraphicsLineItem(parent),
    LoggingBaseClass("line"),
    _type(type)
{
}

void LineGraphicsItem::setWidth(double width)
{
    QPen pen = LineGraphicsItem::pen();
    pen.setWidth(width);
    setPen(pen);
}

void LineGraphicsItem::setColor(const QColor& color)
{
    QPen pen = LineGraphicsItem::pen();
    pen.setColor(color);
    setPen(pen);
}
