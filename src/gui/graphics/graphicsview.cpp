#include "graphics/graphicsview.h"

#include <QMouseEvent>

GraphicsView::GraphicsView(QWidget *parent) :
    QGraphicsView(parent),
    LoggingBaseClass("gv")
{
    viewport()->installEventFilter(this);
}

void GraphicsView::setScaleFactor(double factor)
{
    GraphicsView::scale(factor, factor);
    emit scaleChanged(currentScale());
}

double GraphicsView::currentScale() const
{
    double scale = transform().m11();
    return scale;
}

bool GraphicsView::eventFilter(QObject *watched, QEvent *event)
{
    Q_UNUSED(watched)
    bool result = false;
    switch(event->type()) {
    case QEvent::Wheel:
        result = processWheelEvent(static_cast<QWheelEvent*>(event));
        break;
    default:
        break;
    }
    return result;
}

bool GraphicsView::processWheelEvent(QWheelEvent *event)
{
    bool result = false;
    if(_zoomEnabled && event->modifiers() == Qt::ControlModifier && event->angleDelta().x() == 0) {
        double angle = event->angleDelta().y();
        double factor = std::pow(_zoomFactor, angle);
        setScaleFactor(factor);
        result = true;
    }
    return result;
}


