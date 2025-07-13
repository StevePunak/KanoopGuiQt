#ifndef ELLIPSEGRAPHICSITEM_H
#define ELLIPSEGRAPHICSITEM_H
#include <QGraphicsEllipseItem>
#include <Kanoop/gui/libkanoopgui.h>

class LIBKANOOPGUI_EXPORT EllipseGraphicsItem : public QGraphicsEllipseItem
{
public:
    EllipseGraphicsItem(int type, QGraphicsItem* parent = nullptr);

    virtual int type() const override { return _type; }

    void setWidth(double width);
    void setColor(const QColor& color);

private:
    int _type;
};

#endif // ELLIPSEGRAPHICSITEM_H
