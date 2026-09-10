#include "widgets/checkbox.h"

#include <QKeyEvent>
#include <QMouseEvent>


CheckBox::CheckBox(QWidget *parent) :
    QCheckBox(parent)
{
}

CheckBox::CheckBox(const QString& text, QWidget* parent) :
    QCheckBox(text, parent)
{
}

void CheckBox::mousePressEvent(QMouseEvent* event)
{
    if(_readOnly) {
        event->accept();
    }
    else {
        QCheckBox::mousePressEvent(event);
    }
}

void CheckBox::keyPressEvent(QKeyEvent* event)
{
    if(_readOnly) {
        event->ignore();
    }
    else {
        QCheckBox::keyPressEvent(event);
    }
}

void CheckBox::keyReleaseEvent(QKeyEvent* event)
{
    if(_readOnly) {
        event->ignore();
    }
    else {
        QCheckBox::keyReleaseEvent(event);
    }
}

#include "Kanoop/gui/widgets/moc_checkbox.cpp"
