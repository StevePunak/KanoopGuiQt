#include "widgets/pushbutton.h"

#include <utility/stylesheet.h>


PushButton::PushButton(QWidget* parent) :
    QPushButton(parent)
{
    commonInit();
}

PushButton::PushButton(const QString& text, QWidget* parent) :
    QPushButton(text, parent)
{
    commonInit();
}

PushButton::PushButton(const QIcon& icon, const QString& text, QWidget* parent) :
    QPushButton(icon, text, parent)
{
    commonInit();
}

void PushButton::commonInit()
{
    _backgroundColor = palette().color(QPalette::Window);
    _foregroundColor = palette().color(QPalette::Text);
    setBackgroundColor(_backgroundColor);
    setForegroundColor(_foregroundColor);
}

void PushButton::setFontPointSize(int size)
{
    QFont f = font();
    f.setPointSize(size);
    setFont(f);
}

void PushButton::setFontPixelSize(int size)
{
    QFont f = font();
    f.setPixelSize(size);
    setFont(f);
}

void PushButton::setBold(bool bold)
{
    QFont f = font();
    f.setBold(bold);
    setFont(f);
}

void PushButton::setItalic(bool italic)
{
    QFont f = font();
    f.setItalic(italic);
    setFont(f);
}

void PushButton::setBorderRadius(int radius)
{
    _borderRadius = radius;
    makeStyleSheet();
}

void PushButton::setHoverForegroundColor(const QColor& color)
{
    _hoverForegroundColor = color;
    makeStyleSheet();
}

void PushButton::setHoverBackgroundColor(const QColor& color)
{
    _hoverBackgroundColor = color;
    makeStyleSheet();
}

void PushButton::setPressedForegroundColor(const QColor& color)
{
    _pressedForegroundColor = color;
    makeStyleSheet();
}

void PushButton::setPressedBackgroundColor(const QColor& color)
{
    _pressedBackgroundColor = color;
    makeStyleSheet();
}

void PushButton::setDisabledForegroundColor(const QColor& color)
{
    _disabledForegroundColor = color;
    makeStyleSheet();
}

void PushButton::setDisabledBackgroundColor(const QColor& color)
{
    _disabledBackgroundColor = color;
    makeStyleSheet();
}

void PushButton::setForegroundColor(const QColor& color)
{
    _foregroundColor = color;
    makeStyleSheet();
}

void PushButton::setBackgroundColor(const QColor& color)
{
    _backgroundColor = color;
    makeStyleSheet();
}

void PushButton::setDefaultForegroundColor()
{
    setForegroundColor(palette().color(QPalette::Text));
}

void PushButton::setDefaultBackgroundColor()
{
    setBackgroundColor(palette().color(QPalette::Window));
}

void PushButton::makeStyleSheet()
{
    StyleSheet<QPushButton> base;
    base.setProperty(SP_Color, _foregroundColor);
    base.setProperty(SP_BackgroundColor, _backgroundColor);
    if(_borderRadius > 0)
        base.setPropertyPixels(SP_BorderRadius, _borderRadius);
    QString ss = base.toString();

    if(_hoverForegroundColor.isValid() || _hoverBackgroundColor.isValid()) {
        StyleSheet<QPushButton> hover;
        hover.setPseudoState(PS_Hover);
        if(_hoverForegroundColor.isValid())  hover.setProperty(SP_Color, _hoverForegroundColor);
        if(_hoverBackgroundColor.isValid())  hover.setProperty(SP_BackgroundColor, _hoverBackgroundColor);
        ss += " " + hover.toString();
    }

    if(_pressedForegroundColor.isValid() || _pressedBackgroundColor.isValid()) {
        StyleSheet<QPushButton> pressed;
        pressed.setPseudoState(PS_Pressed);
        if(_pressedForegroundColor.isValid())  pressed.setProperty(SP_Color, _pressedForegroundColor);
        if(_pressedBackgroundColor.isValid())  pressed.setProperty(SP_BackgroundColor, _pressedBackgroundColor);
        ss += " " + pressed.toString();
    }

    if(_disabledForegroundColor.isValid() || _disabledBackgroundColor.isValid()) {
        StyleSheet<QPushButton> disabled;
        disabled.setPseudoState(PS_Disabled);
        if(_disabledForegroundColor.isValid())  disabled.setProperty(SP_Color, _disabledForegroundColor);
        if(_disabledBackgroundColor.isValid())  disabled.setProperty(SP_BackgroundColor, _disabledBackgroundColor);
        ss += " " + disabled.toString();
    }

    setStyleSheet(ss);
}
