#ifndef PIXMAPGRAPHICSITEM_H
#define PIXMAPGRAPHICSITEM_H
#include <QGraphicsPixmapItem>

#include <Kanoop/utility/loggingbaseclass.h>

class PixmapGraphicsItem : public QGraphicsPixmapItem,
                           public LoggingBaseClass
{
public:
    explicit PixmapGraphicsItem(int type, QGraphicsItem* parent = nullptr);

    int type() const { return _type; }

protected:
    virtual QVariant itemChange(GraphicsItemChange change, const QVariant& value) override;

private:
    int _type;
};

#endif // PIXMAPGRAPHICSITEM_H
