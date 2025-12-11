#include "widgets/checkbox.h"

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
