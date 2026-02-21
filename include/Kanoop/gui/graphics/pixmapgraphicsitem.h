#ifndef PIXMAPGRAPHICSITEM_H
#define PIXMAPGRAPHICSITEM_H
#include <QGraphicsPixmapItem>

#include <Kanoop/gui/libkanoopgui.h>

/**
 * @brief QGraphicsPixmapItem with an application-defined type integer.
 *
 * PixmapGraphicsItem stores a type integer (returned by type()) that allows
 * qgraphicsitem_cast<> and scene item-type filtering to work with custom types.
 */
class LIBKANOOPGUI_EXPORT PixmapGraphicsItem : public QGraphicsPixmapItem
{
public:
    /**
     * @brief Construct with a type integer and optional parent item.
     * @param type Application-defined graphics item type
     * @param parent Optional parent QGraphicsItem
     */
    explicit PixmapGraphicsItem(int type, QGraphicsItem* parent = nullptr);

    /**
     * @brief Return the application-defined type integer.
     * @return Item type value
     */
    virtual int type() const override { return _type; }

protected:
    /** @brief Handle graphics item change notifications. */
    virtual QVariant itemChange(GraphicsItemChange change, const QVariant& value) override;

private:
    int _type;
};

#endif // PIXMAPGRAPHICSITEM_H
