#ifndef RECTANGLEGRAPHICSITEM_H
#define RECTANGLEGRAPHICSITEM_H
#include <QGraphicsRectItem>

#include <Kanoop/gui/libkanoopgui.h>
#include <Kanoop/geometry/point.h>

class Rectangle;

/**
 * @brief QGraphicsRectItem with an application-defined type integer and center-point helpers.
 *
 * RectangleGraphicsItem stores a type integer, returned by type(), for scene item-type filtering.
 * It also provides centerPoint() and sceneCenterPoint() convenience accessors.
 */
class LIBKANOOPGUI_EXPORT RectangleGraphicsItem : public QGraphicsRectItem
{
public:
    /**
     * @brief Construct with a type integer and optional parent item.
     * @param type Application-defined graphics item type
     * @param parent Optional parent QGraphicsItem
     */
    explicit RectangleGraphicsItem(int type, QGraphicsItem* parent = nullptr);

    /**
     * @brief Construct from a Rectangle geometry object.
     * @param rect Rectangle geometry to display
     * @param type Application-defined graphics item type
     * @param parent Optional parent QGraphicsItem
     */
    explicit RectangleGraphicsItem(const Rectangle& rect, int type, QGraphicsItem* parent = nullptr);

    /**
     * @brief Return the application-defined type integer.
     * @return Item type value
     */
    virtual int type() const override { return _type; }

    /**
     * @brief Return the center of the item's bounding rect in item coordinates.
     * @return Center point in item coordinates
     */
    Point centerPoint() const;

    /**
     * @brief Map the center of a rect of the item's size, placed at the item origin, into
     *        scene coordinates.
     * @return Center point in scene coordinates.  It differs from mapToScene(centerPoint())
     *         by rect().topLeft().
     */
    Point sceneCenterPoint() const;

protected:
    virtual QVariant itemChange(GraphicsItemChange change, const QVariant& value) override;

private:
    int _type;
};

#endif // RECTANGLEGRAPHICSITEM_H
