#ifndef LINEGRAPHICSITEM_H
#define LINEGRAPHICSITEM_H
#include <QGraphicsLineItem>
#include <Kanoop/utility/loggingbaseclass.h>
#include <Kanoop/gui/libkanoopgui.h>

class LIBKANOOPGUI_EXPORT LineGraphicsItem : public QGraphicsLineItem,
                                             public LoggingBaseClass
{
public:
    LineGraphicsItem(int type, QGraphicsItem* parent = nullptr);

    virtual int type() const override { return _type; }

    void setWidth(double width);
    void setColor(const QColor& color);

private:
    int _type;
};

#endif // LINEGRAPHICSITEM_H
