#include "widgets/lineedit.h"

#include <utility/stylesheet.h>


LineEdit::LineEdit(QWidget *parent) :
    QLineEdit(parent)
{
}

LineEdit::LineEdit(const QString& text, QWidget* parent) :
    QLineEdit(text, parent)
{
}

void LineEdit::setForegroundColor(const QColor& color)
{
    _foregroundColor = color;
    _foregroundExplicitlySet = true;
    applyStylesheet();
}

void LineEdit::setBackgroundColor(const QColor& color)
{
    _backgroundColor = color;
    _backgroundExplicitlySet = true;
    applyStylesheet();
}

void LineEdit::setDefaultForegroundColor()
{
    _foregroundColor = QColor();
    _foregroundExplicitlySet = false;
    applyStylesheet();
}

void LineEdit::setDefaultBackgroundColor()
{
    _backgroundColor = QColor();
    _backgroundExplicitlySet = false;
    applyStylesheet();
}

void LineEdit::applyStylesheet()
{
    StyleSheet<QLineEdit> ss;
    if(_foregroundExplicitlySet) {
        ss.setProperty(SP_Color, _foregroundColor);
    }
    if(_backgroundExplicitlySet) {
        ss.setProperty(SP_BackgroundColor, _backgroundColor);
    }
    setStyleSheet(ss.toString());
}

#include "Kanoop/gui/widgets/moc_lineedit.cpp"
