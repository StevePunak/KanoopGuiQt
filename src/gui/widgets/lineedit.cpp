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
    StyleSheet<QLineEdit> ss;
    ss.setProperty(SP_Color, color);
    setStyleSheet(ss.toString());
}

void LineEdit::setBackgroundColor(const QColor& color)
{
    StyleSheet<QLineEdit> ss;
    ss.setProperty(SP_BackgroundColor, color);
    setStyleSheet(ss.toString());
}
