#ifndef PUSHBUTTON_H
#define PUSHBUTTON_H

#include <QPushButton>
#include <Kanoop/gui/libkanoopgui.h>

class LIBKANOOPGUI_EXPORT PushButton : public QPushButton
{
    Q_OBJECT

    Q_PROPERTY(QColor foregroundColor READ foregroundColor WRITE setForegroundColor) // clazy:exclude=qproperty-without-notify
    Q_PROPERTY(QColor backgroundColor READ backgroundColor WRITE setBackgroundColor) // clazy:exclude=qproperty-without-notify

public:
    explicit PushButton(QWidget *parent = nullptr);
    explicit PushButton(const QString &text, QWidget *parent = nullptr);
    PushButton(const QIcon& icon, const QString &text, QWidget *parent = nullptr);

    // Font helpers
    void setFontPointSize(int size);
    void setFontPixelSize(int size);
    void setBold(bool bold);
    void setItalic(bool italic);

    // Color getters
    QColor foregroundColor() const { return _foregroundColor; }
    QColor backgroundColor() const { return _backgroundColor; }

    // Border
    void setBorderRadius(int radius);

    // Hover/pressed/disabled state colors
    void setHoverForegroundColor(const QColor& color);
    void setHoverBackgroundColor(const QColor& color);
    void setPressedForegroundColor(const QColor& color);
    void setPressedBackgroundColor(const QColor& color);
    void setDisabledForegroundColor(const QColor& color);
    void setDisabledBackgroundColor(const QColor& color);

public slots:
    void setForegroundColor(const QColor& color);
    void setBackgroundColor(const QColor& color);
    void setDefaultForegroundColor();
    void setDefaultBackgroundColor();

private:
    void commonInit();
    void makeStyleSheet();

    QColor _foregroundColor;
    QColor _backgroundColor;
    QColor _hoverForegroundColor;    // invalid QColor() = not set
    QColor _hoverBackgroundColor;
    QColor _pressedForegroundColor;
    QColor _pressedBackgroundColor;
    QColor _disabledForegroundColor;
    QColor _disabledBackgroundColor;
    int    _borderRadius = 0;
};

#endif // PUSHBUTTON_H
