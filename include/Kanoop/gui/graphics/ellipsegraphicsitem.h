#ifndef ELLIPSEGRAPHICSITEM_H
#define ELLIPSEGRAPHICSITEM_H
#include <QGraphicsEllipseItem>
#include <Kanoop/utility/loggingbaseclass.h>

class EllipseGraphicsItem : public QGraphicsEllipseItem,
                            public LoggingBaseClass
{
public:
    EllipseGraphicsItem(int type, QGraphicsItem* parent = nullptr);

    int type() const { return _type; }

    void setWidth(double width);
    void setColor(const QColor& color);

private:
    int _type;
};

#endif // ELLIPSEGRAPHICSITEM_H
