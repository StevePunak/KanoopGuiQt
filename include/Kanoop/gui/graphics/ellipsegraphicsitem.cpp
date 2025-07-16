#include "ellipsegraphicsitem.h"

#include <QPen>

EllipseGraphicsItem::EllipseGraphicsItem(int type, QGraphicsItem* parent) :
    QGraphicsEllipseItem(parent),
    _type(type)
{
}

void EllipseGraphicsItem::setWidth(double width)
{
    QPen pen = QGraphicsEllipseItem::pen();
    pen.setWidth(width);
    setPen(pen);
}

void EllipseGraphicsItem::setColor(const QColor& color)
{
    QPen pen = QGraphicsEllipseItem::pen();
    pen.setColor(color);
    setPen(pen);
}
