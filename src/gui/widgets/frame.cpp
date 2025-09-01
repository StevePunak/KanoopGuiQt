#include "widgets/frame.h"

#include <utility/stylesheet.h>

Frame::Frame(QWidget *parent) :
    QFrame(parent)
{
}


void Frame::setForegroundColor(const QColor& color)
{
    _foregroundColor = color;
    StyleSheet<QFrame> ss;
    ss.setProperty(SP_Color, color);
    setStyleSheet(ss.toString());
}

void Frame::setBackgroundColor(const QColor& color)
{
    _backgroundColor = color;
    StyleSheet<QFrame> ss;
    ss.setProperty(SP_BackgroundColor, color);
    setStyleSheet(ss.toString());
}

