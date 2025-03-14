#ifndef GRAPHICSVIEW_H
#define GRAPHICSVIEW_H

#include <QGraphicsView>

#include <Kanoop/utility/loggingbaseclass.h>
#include <Kanoop/gui/libkanoopgui.h>

class LIBKANOOPGUI_EXPORT GraphicsView : public QGraphicsView,
                                         public LoggingBaseClass
{
    Q_OBJECT
public:
    explicit GraphicsView(QWidget *parent = nullptr);

    bool isZoomEnabled() const { return _zoomEnabled; }
    void setZoomEnabled(bool value) { _zoomEnabled = value; }

    void setScaleFactor(double factor);
    double currentScale() const;

private:
    virtual bool eventFilter(QObject *watched, QEvent *event) override;
    bool processWheelEvent(QWheelEvent* event);

    bool _zoomEnabled = true;
    double _zoomFactor = 1.001;

signals:
    void scaleChanged(double scale);

};

#endif // GRAPHICSVIEW_H
