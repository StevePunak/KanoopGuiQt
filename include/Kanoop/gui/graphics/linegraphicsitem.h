#ifndef LINEGRAPHICSITEM_H
#define LINEGRAPHICSITEM_H
#include <QGraphicsLineItem>
#include <Kanoop/gui/libkanoopgui.h>

class Line;

/**
 * @brief QGraphicsLineItem with an application-defined type integer and appearance helpers.
 *
 * LineGraphicsItem stores a type integer (returned by type()) that allows
 * qgraphicsitem_cast<> and scene item-type filtering to work with custom types.
 * It also provides helpers for changing pen width, color, and line length.
 */
class LIBKANOOPGUI_EXPORT LineGraphicsItem : public QGraphicsLineItem
{
public:
    /**
     * @brief Construct with a type integer and optional parent item.
     * @param type Application-defined graphics item type
     * @param parent Optional parent QGraphicsItem
     */
    LineGraphicsItem(int type, QGraphicsItem* parent = nullptr);

    /**
     * @brief Construct from a Line geometry object, with a type integer and optional parent.
     * @param line Line geometry to display
     * @param type Application-defined graphics item type
     * @param parent Optional parent QGraphicsItem
     */
    LineGraphicsItem(const Line& line, int type, QGraphicsItem* parent = nullptr);

    /**
     * @brief Return the application-defined type integer.
     * @return Item type value
     */
    virtual int type() const override { return _type; }

    /**
     * @brief Set the pen width.
     * @param width Pen width in scene units
     */
    void setWidth(double width);

    /**
     * @brief Set the pen color.
     * @param color Color to apply to the pen
     */
    void setColor(const QColor& color);

    /**
     * @brief Return the bounding rectangle (expanded to account for pen width).
     * @return Bounding rect in item coordinates
     */
    virtual QRectF boundingRect() const override;

    /**
     * @brief Return the shape used for hit testing.
     * @return Painter path representing the clickable area
     */
    virtual QPainterPath shape() const override;

    /**
     * @brief Set the length of the line, extending from its current start point.
     * @param length New length in scene units
     */
    void setLength(double length);

private:
    int _type;
};

#endif // LINEGRAPHICSITEM_H
