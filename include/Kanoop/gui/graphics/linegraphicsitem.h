#ifndef LINEGRAPHICSITEM_H
#define LINEGRAPHICSITEM_H
#include <QGraphicsLineItem>
#include <Kanoop/gui/libkanoopgui.h>

class Line;
class LIBKANOOPGUI_EXPORT LineGraphicsItem : public QGraphicsLineItem
{
public:
    LineGraphicsItem(int type, QGraphicsItem* parent = nullptr);
    LineGraphicsItem(const Line& line, int type, QGraphicsItem* parent = nullptr);

    virtual int type() const override { return _type; }

    void setWidth(double width);
    void setColor(const QColor& color);

    virtual QRectF boundingRect() const override;
    virtual QPainterPath shape() const override;

    void setLength(double length);

private:
    int _type;
};

#endif // LINEGRAPHICSITEM_H
