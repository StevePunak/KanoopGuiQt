#ifndef GRAPHICSVIEW_H
#define GRAPHICSVIEW_H

#include <QGraphicsView>

#include <Kanoop/utility/loggingbaseclass.h>
#include <Kanoop/gui/libkanoopgui.h>
#include <Kanoop/geometry/point.h>

class Size;

/**
 * @brief QGraphicsView subclass with built-in mouse pan and wheel zoom support.
 *
 * GraphicsView installs itself as an event filter and handles wheel events for
 * zooming and mouse button press/move/release events for panning.  Both features
 * can be enabled or disabled at runtime.  scaleChanged() is emitted whenever the
 * view transform changes, and resized() is emitted on resize events.
 */
class LIBKANOOPGUI_EXPORT GraphicsView : public QGraphicsView,
                                         public LoggingBaseClass
{
    Q_OBJECT
public:
    /**
     * @brief Construct with an optional parent widget.
     * @param parent Optional QWidget parent
     */
    explicit GraphicsView(QWidget *parent = nullptr);

    /**
     * @brief Return whether mouse-drag panning is enabled.
     * @return true if pan is enabled
     */
    bool panEnabled() const { return _panEnabled; }

    /**
     * @brief Enable or disable mouse-drag panning.
     * @param value true to enable pan
     */
    void setPanEnabled(bool value) { _panEnabled = value; }

    /**
     * @brief Return whether mouse-wheel zooming is enabled.
     * @return true if zoom is enabled
     */
    bool isZoomEnabled() const { return _zoomEnabled; }

    /**
     * @brief Enable or disable mouse-wheel zooming.
     * @param value true to enable zoom
     */
    void setZoomEnabled(bool value) { _zoomEnabled = value; }

    /**
     * @brief Set the view scale to an absolute factor.
     * @param factor Scale factor (1.0 = no zoom)
     */
    void setScaleFactor(double factor);

    /**
     * @brief Return the current horizontal scale of the view transform.
     * @return Current scale factor
     */
    double currentScale() const;

protected:
    /** @brief Emit resized() with old and new viewport sizes. */
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
    /**
     * @brief Emitted when the view scale changes.
     * @param scale New scale factor
     */
    void scaleChanged(double scale);

    /**
     * @brief Emitted when the viewport is resized.
     * @param newSize New viewport size
     * @param oldSize Previous viewport size
     */
    void resized(const Size& newSize, const Size& oldSize);
};

#endif // GRAPHICSVIEW_H
