#include "widgets/frame.h"

#include <utility/stylesheet.h>

Frame::Frame(QWidget *parent) :
    QFrame(parent)
{
}


void Frame::setForegroundColor(const QColor& color)
{
    _foregroundColor = color;
    _foregroundExplicitlySet = true;
    applyStylesheet();
}

void Frame::setBackgroundColor(const QColor& color)
{
    _backgroundColor = color;
    _backgroundExplicitlySet = true;
    applyStylesheet();
}

void Frame::setDefaultForegroundColor()
{
    _foregroundColor = QColor();
    _foregroundExplicitlySet = false;
    applyStylesheet();
}

void Frame::setDefaultBackgroundColor()
{
    _backgroundColor = QColor();
    _backgroundExplicitlySet = false;
    applyStylesheet();
}

void Frame::applyStylesheet()
{
    StyleSheet<QFrame> ss;
    if(_foregroundExplicitlySet) {
        ss.setProperty(SP_Color, _foregroundColor);
    }
    if(_backgroundExplicitlySet) {
        ss.setProperty(SP_BackgroundColor, _backgroundColor);
    }
    setStyleSheet(ss.toString());
}


#include "Kanoop/gui/widgets/moc_frame.cpp"
