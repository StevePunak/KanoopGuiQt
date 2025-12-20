#include "widgets/label.h"

#include <stylesheets.h>

#include <utility/stylesheet.h>

Label::Label(QWidget *parent, Qt::WindowFlags f) :
    QLabel(parent, f)
{
    commonInit();
}

Label::Label(const QString &text, QWidget *parent, Qt::WindowFlags f) :
    QLabel(text, parent, f)
{
    commonInit();
}

void Label::commonInit()
{
    _backgroundColor = palette().color(QPalette::Window);
    _foregroundColor = palette().color(QPalette::Text);
    setBackgroundColor(_backgroundColor);
    setForegroundColor(_foregroundColor);
}

void Label::setFontPointSize(int size)
{
    QFont font = Label::font();
    font.setPointSize(size);
    setFont(font);
}

void Label::setFontPixelSize(int size)
{
    QFont font = Label::font();
    font.setPixelSize(size);
    setFont(font);
}

void Label::setForegroundColor(const QColor &color)
{
    _foregroundColor = color;
    Label::applyStylesheet();
}

void Label::setBackgroundColor(const QColor &color)
{
    _backgroundColor = color;
    Label::applyStylesheet();
}

void Label::setDefaultForegroundColor()
{
    _foregroundColor = palette().color(QPalette::Text);
    applyStylesheet();
}

void Label::setDefaultBackgroundColor()
{
    _backgroundColor = palette().color(QPalette::Window);
    applyStylesheet();
}

void Label::applyStylesheet()
{
    StyleSheet<QLabel> ss;
    ss.setProperty(SP_Color, _foregroundColor);
    ss.setProperty(SP_BackgroundColor, _backgroundColor);
    setStyleSheet(ss.toString());
}

