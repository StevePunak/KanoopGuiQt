#ifndef GRAPHICSVIEW_H
#define GRAPHICSVIEW_H

#include <QGraphicsView>

#include <Kanoop/utility/loggingbaseclass.h>
#include <Kanoop/gui/libkanoopgui.h>
#include <Kanoop/geometry/point.h>

class Size;
class LIBKANOOPGUI_EXPORT GraphicsView : public QGraphicsView,
                                         public LoggingBaseClass
{
    Q_OBJECT
public:
    explicit GraphicsView(QWidget *parent = nullptr);

    bool panEnabled() const { return _panEnabled; }
    void setPanEnabled(bool value) { _panEnabled = value; }

    bool isZoomEnabled() const { return _zoomEnabled; }
    void setZoomEnabled(bool value) { _zoomEnabled = value; }

    void setScaleFactor(double factor);
    double currentScale() const;

protected:
    virtual void resizeEvent(QResizeEvent* event) override;

private:
    virtual bool eventFilter(QObject *watched, QEvent *event) override;
    bool processWheelEvent(QWheelEvent* event);
    bool processMouseButtonPressEvent(QMouseEvent* event);
    bool processMouseButtonReleaseEvent(QMouseEvent* event);
    bool processMouseMoveEvent(QMouseEvent* event);

    enum State { Normal, Pan, Zoom };

    bool _zoomEnabled = true;
    double _zoomFactor = 1.001;

    bool _panEnabled = true;
    Point _panOrigin;
    Point _panStartCenter;
    Point _lastMousePos;

    State _state = Normal;

signals:
    void scaleChanged(double scale);
    void resized(const Size& size);
};

#endif // GRAPHICSVIEW_H
