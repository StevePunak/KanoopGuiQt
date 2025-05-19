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
    QFont font = PushButton::font();
    font.setPointSize(size);
    setFont(font);
}

void PushButton::setFontPixelSize(int size)
{
    QFont font = PushButton::font();
    font.setPixelSize(size);
    setFont(font);
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

void PushButton::makeStyleSheet()
{
    StyleSheet<QPushButton> ss;
    ss.setProperty(SP_Color, _foregroundColor);
    ss.setProperty(SP_BackgroundColor, _backgroundColor);
    setStyleSheet(ss.toString());
}
