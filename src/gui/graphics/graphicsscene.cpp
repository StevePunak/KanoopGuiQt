#include "graphics/graphicsscene.h"

GraphicsScene::GraphicsScene(QObject* parent) :
    QGraphicsScene(parent),
    LoggingBaseClass("gfx")
{
}

#include "Kanoop/gui/graphics/moc_graphicsscene.cpp"
