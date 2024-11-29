#ifndef LINEGRAPHICSITEM_H
#define LINEGRAPHICSITEM_H
#include <QGraphicsLineItem>
#include <Kanoop/utility/loggingbaseclass.h>

class LineGraphicsItem : public QGraphicsLineItem,
                         public LoggingBaseClass
{
public:
    LineGraphicsItem(int type, QGraphicsItem* parent = nullptr);

    int type() const { return _type; }

    void setWidth(double width);
    void setColor(const QColor& color);

private:
    int _type;
};

#endif // LINEGRAPHICSITEM_H
