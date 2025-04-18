#include "rectanglegraphicsitem.h"

#include <Kanoop/geometry/rectangle.h>

RectangleGraphicsItem::RectangleGraphicsItem(int type, QGraphicsItem *parent) :
    QGraphicsRectItem(parent),
    _type(type)
{
}

Point RectangleGraphicsItem::centerPoint() const
{
    Point result = rect().center();
    return result;
}

Point RectangleGraphicsItem::sceneCenterPoint() const
{
    Rectangle r(Point(0, 0), rect().size());
    Point result = mapToScene(r.center());
    return result;
}

QVariant RectangleGraphicsItem::itemChange(GraphicsItemChange change, const QVariant &value)
{
    return QGraphicsRectItem::itemChange(change, value);
}
