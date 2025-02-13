#ifndef GRAPHICSSCENE_H
#define GRAPHICSSCENE_H

#include <QGraphicsScene>
#include <Kanoop/utility/loggingbaseclass.h>
#include <Kanoop/gui/libkanoopgui.h>

class LIBKANOOPGUI_EXPORT GraphicsScene : public QGraphicsScene,
                                          public LoggingBaseClass
{
    Q_OBJECT
public:
    explicit GraphicsScene(QObject *parent = nullptr);

signals:

};

#endif // GRAPHICSSCENE_H
