#include "graphics/graphicsview.h"

#include <QMouseEvent>

#include <Kanoop/geometry/size.h>

GraphicsView::GraphicsView(QWidget *parent) :
    QGraphicsView(parent),
    LoggingBaseClass("gv")
{
    viewport()->installEventFilter(this);
}

void GraphicsView::setScaleFactor(double factor)
{
    // Record the position and center on it
    Point lastMouseScenePos = mapToScene(_lastMousePos.toPoint());
    centerOn(lastMouseScenePos);

    // Scale the view
    GraphicsView::scale(factor, factor);

    // Calculate the new center point
    Point deltaFromCenter = _lastMousePos - Point(viewport()->width() / 2, viewport()->height() / 2);
    Point newCenter = mapFromScene(lastMouseScenePos) - deltaFromCenter;

    // Center on new origin and emit a signal
    centerOn(mapToScene(newCenter.toPoint()));

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

    case QEvent::MouseButtonPress:
        result = processMouseButtonPressEvent(static_cast<QMouseEvent*>(event));
        break;

    case QEvent::MouseButtonRelease:
        result = processMouseButtonReleaseEvent(static_cast<QMouseEvent*>(event));
        break;

    case QEvent::MouseMove:
        result = processMouseMoveEvent(static_cast<QMouseEvent*>(event));
        break;

    default:
        break;
    }
    return result;
}

void GraphicsView::resizeEvent(QResizeEvent* event)
{
    QGraphicsView::resizeEvent(event);
    emit resized(event->size());
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

bool GraphicsView::processMouseButtonPressEvent(QMouseEvent* event)
{
    if(event->button() != Qt::MiddleButton || _panEnabled == false) {
        return false;
    }

    _state = Pan;
    _panOrigin = event->pos();
    _panStartCenter = viewport()->rect().center();
    setCursor(Qt::ClosedHandCursor);
    return true;
}

bool GraphicsView::processMouseButtonReleaseEvent(QMouseEvent* event)
{
    if(event->button() != Qt::MiddleButton || _panEnabled == false) {
        return false;
    }

    _state = Normal;
    setCursor(Qt::ArrowCursor);
    return true;
}

bool GraphicsView::processMouseMoveEvent(QMouseEvent* event)
{
    Point delta = Point(event->pos()) - _lastMousePos;
    _lastMousePos = event->pos();
    if(_state != Pan) {
        return false;
    }

    Point center = viewport()->rect().center();
    Point newCenter = center - delta;
    centerOn(mapToScene(newCenter.toPoint()));

    return true;
}



