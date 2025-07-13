#ifndef PIXMAPGRAPHICSITEM_H
#define PIXMAPGRAPHICSITEM_H
#include <QGraphicsPixmapItem>

#include <Kanoop/gui/libkanoopgui.h>

class LIBKANOOPGUI_EXPORT PixmapGraphicsItem : public QGraphicsPixmapItem
{
public:
    explicit PixmapGraphicsItem(int type, QGraphicsItem* parent = nullptr);

    virtual int type() const override { return _type; }

protected:
    virtual QVariant itemChange(GraphicsItemChange change, const QVariant& value) override;

private:
    int _type;
};

#endif // PIXMAPGRAPHICSITEM_H
