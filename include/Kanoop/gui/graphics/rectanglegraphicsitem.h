#ifndef RECTANGLEGRAPHICSITEM_H
#define RECTANGLEGRAPHICSITEM_H
#include <QGraphicsRectItem>

#include <Kanoop/utility/loggingbaseclass.h>

class RectangleGraphicsItem : public QGraphicsRectItem,
                              public LoggingBaseClass
{
public:
    explicit RectangleGraphicsItem(int type, QGraphicsItem* parent = nullptr);

    int type() const { return _type; }

protected:
    virtual QVariant itemChange(GraphicsItemChange change, const QVariant& value) override;

private:
    int _type;
};

#endif // RECTANGLEGRAPHICSITEM_H
