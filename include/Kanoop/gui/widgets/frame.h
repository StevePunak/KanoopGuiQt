#ifndef FRAME_H
#define FRAME_H

#include <QFrame>

#include <Kanoop/utility/loggingbaseclass.h>
#include <Kanoop/gui/libkanoopgui.h>

class LIBKANOOPGUI_EXPORT Frame : public QFrame,
                                  public LoggingBaseClass
{
    Q_OBJECT
    Q_PROPERTY(QColor foregroundColor READ foregroundColor WRITE setForegroundColor)    // clazy:exclude=qproperty-without-notify
    Q_PROPERTY(QColor backgroundColor READ backgroundColor WRITE setBackgroundColor)    // clazy:exclude=qproperty-without-notify
public:
    explicit Frame(QWidget *parent = nullptr);

    QColor foregroundColor() const { return _foregroundColor; }
    void setForegroundColor(const QColor& color);

    QColor backgroundColor() const { return _backgroundColor; }
    void setBackgroundColor(const QColor& color);

private:
    QColor _foregroundColor;
    QColor _backgroundColor;
};

#endif // FRAME_H
