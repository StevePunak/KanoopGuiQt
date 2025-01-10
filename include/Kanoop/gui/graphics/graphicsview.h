#ifndef GRAPHICSVIEW_H
#define GRAPHICSVIEW_H

#include <QGraphicsView>

#include <Kanoop/utility/loggingbaseclass.h>

class GraphicsView : public QGraphicsView,
                     public LoggingBaseClass
{
    Q_OBJECT
public:
    explicit GraphicsView(QWidget *parent = nullptr);

signals:

};

#endif // GRAPHICSVIEW_H
