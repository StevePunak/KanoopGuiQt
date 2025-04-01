#ifndef RECTANGLEGRAPHICSITEM_H
#define RECTANGLEGRAPHICSITEM_H
#include <QGraphicsRectItem>

#include <Kanoop/utility/loggingbaseclass.h>
#include <Kanoop/gui/libkanoopgui.h>
#include <Kanoop/geometry/point.h>

class LIBKANOOPGUI_EXPORT RectangleGraphicsItem : public QGraphicsRectItem,
                                                  public LoggingBaseClass
{
public:
    explicit RectangleGraphicsItem(int type, QGraphicsItem* parent = nullptr);

    virtual int type() const override { return _type; }

    Point centerPoint() const;
    Point sceneCenterPoint() const;

protected:
    virtual QVariant itemChange(GraphicsItemChange change, const QVariant& value) override;

private:
    int _type;
};

#endif // RECTANGLEGRAPHICSITEM_H
