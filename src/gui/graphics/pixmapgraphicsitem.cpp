#include "pixmapgraphicsitem.h"

PixmapGraphicsItem::PixmapGraphicsItem(int type, QGraphicsItem* parent) :
    QGraphicsPixmapItem(parent),
    _type(type)
{
}

QVariant PixmapGraphicsItem::itemChange(GraphicsItemChange change, const QVariant& value)
{
    return QGraphicsPixmapItem::itemChange(change, value);
}
