#include "rectanglegraphicsitem.h"

RectangleGraphicsItem::RectangleGraphicsItem(int type, QGraphicsItem *parent) :
    QGraphicsRectItem(parent),
    _type(type)
{
}

QVariant RectangleGraphicsItem::itemChange(GraphicsItemChange change, const QVariant &value)
{
    return QGraphicsRectItem::itemChange(change, value);
}
