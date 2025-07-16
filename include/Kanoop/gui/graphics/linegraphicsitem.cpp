#include "linegraphicsitem.h"

#include <QPen>

#include <Kanoop/geometry/rectangle.h>

LineGraphicsItem::LineGraphicsItem(int type, QGraphicsItem* parent) :
    QGraphicsLineItem(parent),
    _type(type)
{
}

LineGraphicsItem::LineGraphicsItem(const Line& line, int type, QGraphicsItem* parent) :
    QGraphicsLineItem(line.toQLineF(), parent),
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

QRectF LineGraphicsItem::boundingRect() const
{
    static const double Extra = 2;
    Line line = LineGraphicsItem::line();
    Rectangle rectangle(line.p1().x() - Extra, line.p1().y(), (Extra * 2) + 1, line.p2().y());
    return rectangle;
}

QPainterPath LineGraphicsItem::shape() const
{
    QPainterPath result;
    result.addRect(boundingRect());
    return result;
}

void LineGraphicsItem::setLength(double length)
{
    Line line = LineGraphicsItem::line();
    line.setP2(Point(line.p1().x(), length - 1));
    setLine(line.toQLineF());
}
