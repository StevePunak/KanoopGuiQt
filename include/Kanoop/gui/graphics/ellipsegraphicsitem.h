#ifndef ELLIPSEGRAPHICSITEM_H
#define ELLIPSEGRAPHICSITEM_H
#include <QGraphicsEllipseItem>
#include <Kanoop/gui/libkanoopgui.h>

/**
 * @brief QGraphicsEllipseItem with an application-defined type integer and appearance helpers.
 *
 * EllipseGraphicsItem stores a type integer, returned by type(), for scene item-type filtering.
 */
class LIBKANOOPGUI_EXPORT EllipseGraphicsItem : public QGraphicsEllipseItem
{
public:
    /**
     * @brief Construct with a type integer and optional parent item.
     * @param type Application-defined graphics item type
     * @param parent Optional parent QGraphicsItem
     */
    EllipseGraphicsItem(int type, QGraphicsItem* parent = nullptr);

    /**
     * @brief Return the application-defined type integer.
     * @return Item type value
     */
    virtual int type() const override { return _type; }

    /**
     * @brief Set the pen width of the ellipse outline.
     * @param width Pen width in scene units, truncated to an integer by QPen::setWidth()
     */
    void setWidth(double width);

    /**
     * @brief Set the outline color.
     * @param color Color applied to the pen
     */
    void setColor(const QColor& color);

private:
    int _type;
};

#endif // ELLIPSEGRAPHICSITEM_H
